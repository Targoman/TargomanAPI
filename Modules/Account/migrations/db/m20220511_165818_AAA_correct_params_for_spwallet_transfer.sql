/* Migration File: m20220511_165818_AAA_correct_params_for_spwallet_transfer.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

ALTER TABLE `tblOnlinePayments`
    ADD COLUMN `onpTarget_walID` BIGINT UNSIGNED NULL NULL AFTER `onpAmount`;

ALTER TABLE `tblOfflinePayments`
    ADD COLUMN `ofpTarget_walID` BIGINT UNSIGNED NULL AFTER `ofpAmount`;

ALTER TABLE `tblOfflinePaymentClaims`
    ADD COLUMN `ofpcTarget_walID` BIGINT UNSIGNED NULL AFTER `ofpcAmount`;

ALTER TABLE `tblOnlinePayments`
    CHANGE COLUMN `onpStatus` `onpStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N:New, P:Pending, Y:Payed, A:Succeded, E:Error, J:Rejected, R:Removed' COLLATE 'utf8mb4_general_ci' AFTER `onpResult`;

ALTER TABLE `tblOnlinePayments`
    CHANGE COLUMN `onpPGTrnID` `onpTrackNumber` VARCHAR(50) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `onp_pgwID`,
    DROP INDEX `onpBankTrnID`,
    ADD INDEX `onpTrackNumber` (`onpTrackNumber`) USING BTREE;

DROP PROCEDURE IF EXISTS `spWallet_Transfer`;
DELIMITER ;;
CREATE PROCEDURE `spWallet_Transfer`(
    IN `iFromUserID` BIGINT UNSIGNED,
    IN `iFromWalID` BIGINT UNSIGNED,
    IN `iToUserLogin` VARCHAR(50),
    IN `iAmount` INT UNSIGNED,
    IN `iPass` CHAR(32),
    IN `iSalt` VARCHAR(50)
)
BEGIN
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vValid TINYINT;
    DECLARE vTargetUserID BIGINT UNSIGNED;
    DECLARE vTargetWalletID BIGINT UNSIGNED;
    DECLARE vFromNameFamily VARCHAR(100);
    DECLARE vToNameFamily VARCHAR(100);
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;
        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iFromUserID,
               tblActionLogs.atlType = 'spWallet_Transfer.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iFromUserID", iFromUserID,
                   "iFromWalID", iFromWalID,
                   "iToUserLogin", iToUserLogin,
                   "iAmount", iAmount,
                   "iPass", iPass,
                   "iSalt", iSalt,
                   "vValid", vValid
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    SELECT IFNULL(CONCAT(tblUser.usrName, ' ', tblUser.usrFamily), IFNULL(tblUser.usrEmail, tblUser.usrMobile)),
           tblUser.usrID,
           tblUserWallets.walID
      INTO vToNameFamily,
           vTargetUserID,
           vTargetWalletID
      FROM tblUser
      JOIN tblUserWallets
        ON tblUserWallets.wal_usrID = tblUser.usrID
     WHERE tblUser.usrStatus = 'A'
       AND (tblUser.usrEmail = iToUserLogin OR tblUser.usrMobile = iToUserLogin)
       AND tblUserWallets.walDefault = 1
    ;

    IF ISNULL(vToNameFamily) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Target user not found or is inactive';
    END IF;

    SELECT fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass),
           IFNULL(CONCAT(tblUser.usrName, ' ', tblUser.usrFamily), IFNULL(tblUser.usrEmail, tblUser.usrMobile))
      INTO vValid,
           vFromNameFamily
      FROM tblUser
     WHERE tblUser.usrID = iFromUserID
       AND tblUser.usrStatus = 'A'
    ;

    IF ISNULL(vValid) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid UserID';
    END IF;

    IF vValid = FALSE THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Incorrect password';
    END IF;

    CALL spWalletTransaction_Validate(iFromUserID, iFromWalID, 'T', iAmount, vValid);

    START TRANSACTION;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iFromUserID,
           tblVoucher.vchType = 'T',
           tblVoucher.vchDesc = JSON_OBJECT(
               "targetID", vTargetUserID,
               "targetLogin", iToUserLogin,
               "targetName", vToNameFamily
           ),
           tblVoucher.vchTotalAmount = iAmount,
           tblVoucher.vchStatus = 'F';

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iFromWalID,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wlt_vchType = 'T',
           tblWalletsTransactions.wltAmount = CAST(iAmount AS SIGNED) * -1;

    -- Do not merge this with previous query. Creation of a transatction must be separaet from it's acceptance
    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = vTargetUserID,
           tblVoucher.vchType = 'F',
           tblVoucher.vchDesc = JSON_OBJECT(
               "fromID", iFromUserID,
               "fromName", vFromNameFamily
           ),
           tblVoucher.vchTotalAmount = iAmount,
           tblVoucher.vchStatus = 'F';

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = vTargetWalletID,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wlt_vchType = 'F',
           tblWalletsTransactions.wltAmount = iAmount;

    -- Do not merge this with previous query. Creation of a transatction must be separaet from it's acceptance
    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();

    COMMIT;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletTransaction_Validate`;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransaction_Validate`(
    IN `iUserID` BIGINT UNSIGNED,
    INOUT `ioWalletID` BIGINT UNSIGNED,
    IN `iTransactionType` CHAR(1),
    INOUT `ioAmount` INT UNSIGNED,
    OUT `oMultiplier` TINYINT
)
BEGIN
    DECLARE vLastBalance BIGINT;
    DECLARE vMinBalance BIGINT;
    DECLARE vNotTransferable BIGINT;
    DECLARE vMaxTransferPerDay BIGINT;
    DECLARE vTodayTransfers BIGINT;
    DECLARE vUserID BIGINT;
    DECLARE vIBAN VARCHAR(50);

    SELECT tblUserWallets.walID,
           tblUserWallets.walMinBalance,
           tblUserWallets.walLastBalance,
           tblUserWallets.walNotTransferableAmount,
           tblUserWallets.walMaxTransferPerDay,
           tblUser.usrID,
           tblUserExtraInfo.ueiIBAN
      INTO ioWalletID,
           vMinBalance,
           vLastBalance,
           vNotTransferable,
           vMaxTransferPerDay,
           vUserID,
           vIBAN
      FROM tblUserWallets
 LEFT JOIN tblUser
        ON tblUser.usrID = tblUserWallets.wal_usrID
 LEFT JOIN tblUserExtraInfo
        ON tblUserExtraInfo.uei_usrID = tblUserWallets.wal_usrID
     WHERE tblUserWallets.wal_usrID = iUserID
       AND (tblUserWallets.walID = ioWalletID
        OR (ioWalletID = 0
       AND tblUserWallets.walDefault = 1
           )
           )
    ;

    IF ISNULL(vUserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:User not found';
    END IF;

    IF ISNULL(ioWalletID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet not found or is not yours';
    END IF;

    CASE iTransactionType
        WHEN 'E' THEN -- Expense
            SET ioAmount = LEAST(ioAmount, GREATEST(0, vLastBalance - vMinBalance)), oMultiplier = -1;

        WHEN 'T' THEN -- TransferTo
            IF vLastBalance - vNotTransferable - vMinBalance - CAST(ioAmount AS SIGNED) < 0 THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '401:Not enough credit in wallet to transfer';
            END IF;

            SELECT COALESCE(SUM(ABS(tblWalletsTransactions.wltAmount)), 0)
              INTO vTodayTransfers
              FROM tblWalletsTransactions
             WHERE tblWalletsTransactions.wlt_walID = ioWalletID
               AND DATE(tblWalletsTransactions.wltDateTime) = DATE(NOW())
               AND tblWalletsTransactions.wlt_vchType = 'T'
               AND tblWalletsTransactions.wltStatus = 'A'
            ;

            IF vMaxTransferPerDay > 0 AND vMaxTransferPerDay < CAST(ioAmount AS SIGNED) + vTodayTransfers THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '401:Amount is greater than your per day allowed transfer';
            END IF;
            SET oMultiplier = -1;

        WHEN 'F' THEN -- TransferFrom
            SET oMultiplier = 1;

        WHEN 'I' THEN -- Income
            SET oMultiplier = 1;

        WHEN 'C' THEN -- Credit
            SET oMultiplier = 1;

        WHEN 'W' THEN -- Withdrawal
            IF ISNULL(vIBAN) THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '401:IBAN not defined';
            END IF;

            IF vLastBalance - vNotTransferable - vMinBalance - CAST(ioAmount AS SIGNED) < 0 THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '401:Not enough credit in wallet to withdraw';
            END IF;

            SET oMultiplier = -1;

        ELSE
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:Invalid wallet transaction type';
    END CASE;

END;;
DELIMITER ;

DROP TRIGGER IF EXISTS `trg_tblWalletsTransactions_after_update`;
DELIMITER ;;
CREATE TRIGGER `trg_tblWalletsTransactions_after_update` AFTER UPDATE ON `tblWalletsTransactions` FOR EACH ROW
Proc: BEGIN
    DECLARE vLastTransactionDate DATETIME;
    DECLARE vLastBalance BIGINT;
    DECLARE vMultipleInsert BOOL;

    IF NEW.wltStatus = 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Wallet status can not be reset to new';
    END IF;

-- TODO IMPORTANT recheck this trigger

    SELECT tblUserWallets.walLastBalance
      INTO vLastBalance
      FROM tblUserWallets
     WHERE tblUserWallets.walID = NEW.wlt_walID;

/*
    CALL sp_SYSTEM_deleteOldBalances(
          NEW.wlt_walID
        , NEW.wltID
        , NEW.wltDateTime
        , vLastTransactionDate
        , vLastBalance
        , LastIncome
        , LastExpense
        , vMultipleInsert
    );
*/
    IF (IFNULL(vLastBalance,0) + NEW.wltAmount < 0) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet balance can not be negative';
    END IF;

    IF ISNULL(vMultipleInsert) THEN
        UPDATE tblWalletBalances
           SET tblWalletBalances.wblBalance = IFNULL(vLastBalance,0) + IF(NEW.wltStatus IN ('Y', 'A'), NEW.wltAmount, 0)
         WHERE tblWalletBalances.wbl_wltID  = NEW.wltID
        ;

        SET vLastBalance = IFNULL(vLastBalance,0) + IF(NEW.wltStatus IN ('Y', 'A'), NEW.wltAmount, 0);
    ELSE
        SET @vLastBalance = IFNULL(vLastBalance,0);
        DELETE
          FROM tblWalletBalances
         WHERE tblWalletBalances.wbl_wltID = NEW.wltID
        ;
        INSERT INTO tblWalletBalances (
                    wbl_wltID
                  , wblBalance
                    )
             SELECT tblWalletsTransactions.wltID
                  , (@vLastBalance := @vLastBalance +
                    IF(tblWalletsTransactions.wltStatus IN ('Y', 'A'), tblWalletsTransactions.wltAmount, 0 )
                    )
               FROM tblWalletsTransactions
               JOIN (SELECT @vLastBalance := IFNULL(vLastBalance,0)) r
              WHERE tblWalletsTransactions.wltDateTime > vLastTransactionDate
           ORDER BY tblWalletsTransactions.wltDateTime ASC
                  , tblWalletsTransactions.wltID ASC
        ;
        SET vLastBalance = @vLastBalance;
    END IF;

    UPDATE tblUserWallets
       SET tblUserWallets.walLastBalance = IFNULL(vLastBalance,0)
     WHERE tblUserWallets.walID = NEW.wlt_walID;

    -- TODO Update Last Income and LastExpenses
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spOnlinePayment_Create`;
DELIMITER ;;
CREATE PROCEDURE `spOnlinePayment_Create`(
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iGatewayID` INT,
    IN `iAmount` BIGINT UNSIGNED,
    IN `iTargetWalID` BIGINT UNSIGNED,
    OUT `oMD5` VARCHAR(50)
)
BEGIN
    SET oMD5 = dev_Common.fnCreateRandomMD5();

    INSERT INTO tblOnlinePayments
       SET onpMD5 = oMD5
         , onp_vchID = iVoucherID
         , onp_pgwID = iGatewayID
         , onpAmount = iAmount
         , onpTarget_walID = iTargetWalID
    ;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletTransactionOnPayment_Create`;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransactionOnPayment_Create`(
    IN `iPaymentID` BIGINT UNSIGNED,
    IN `iPaymentType` CHAR(1),
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iTargetWalletID` BIGINT UNSIGNED,
    OUT `oRemainingAfterWallet` BIGINT
)
BEGIN
    DECLARE vErr VARCHAR(500);
--    DECLARE vPaymentID BIGINT UNSIGNED;
--    DECLARE vExpenseVoucherID BIGINT UNSIGNED;
--    DECLARE vCreditVoucherID BIGINT UNSIGNED;
    DECLARE vPaymentAmount BIGINT UNSIGNED;
    DECLARE vTotalAmount BIGINT UNSIGNED;
    DECLARE vUserID BIGINT UNSIGNED;
--    DECLARE vUserDefaultWallet BIGINT UNSIGNED;
    DECLARE vLastID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'VirtWLT.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iVoucherID", iVoucherID,
                   "iPaymentID", iPaymentID,
                   "iPaymentType", iPaymentType,
                   "oRemainingAfterWallet", oRemainingAfterWallet
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    START TRANSACTION;

    IF iPaymentType = 'N' THEN -- Online
        SELECT
--               tblOnlinePayments.onpID,
--               tblOnlinePayments.onp_vchID,
               tblOnlinePayments.onpAmount,
               tblVoucher.vchTotalAmount,
               tblVoucher.vch_usrID
          INTO
--               vPaymentID,
--               vExpenseVoucherID,
               vPaymentAmount,
               vTotalAmount,
               vUserID
          FROM tblOnlinePayments
    INNER JOIN tblVoucher
            ON tblVoucher.vchID = tblOnlinePayments.onp_vchID
         WHERE tblOnlinePayments.onp_vchID = iVoucherID
           AND tblOnlinePayments.onpStatus = 'Y' -- Payed
           AND tblOnlinePayments.onpID = iPaymentID
        ;
    ELSEIF iPaymentType = 'F' THEN -- Offline
        SELECT
--               tblOfflinePayments.ofpID,
--               tblOfflinePayments.ofp_vchID,
               tblOfflinePayments.ofpAmount,
               tblVoucher.vchTotalAmount,
               tblVoucher.vch_usrID
          INTO
--               vPaymentID,
--               vExpenseVoucherID,
               vPaymentAmount,
               vTotalAmount,
               vUserID
          FROM tblOfflinePayments
    INNER JOIN tblVoucher
            ON tblVoucher.vchID = tblOfflinePayments.ofp_vchID
         WHERE tblOfflinePayments.ofp_vchID = iVoucherID
           AND tblOfflinePayments.ofpStatus = 'Y' -- Payed
           AND tblOfflinePayments.ofpID = iPaymentID
        ;
    ELSE
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Invalid payment type';
    END IF;

    IF ISNULL(vUserID) THEN -- vExpenseVoucherID
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Payment not found or is not payed yet';
    END IF;

    IF (iTargetWalletID = 0) THEN
        SELECT tblUserWallets.walID
          INTO iTargetWalletID
          FROM tblUserWallets
         WHERE tblUserWallets.wal_usrID = vUserID
           AND tblUserWallets.walDefault = true
        ;

        IF (IFNULL(iTargetWalletID, 0) = 0) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:Default wallet not found';
        END IF;
    END IF;

    IF NOT EXISTS(
        SELECT walID
          FROM tblUserWallets
         WHERE tblUserWallets.wal_usrID = vUserID
           AND walID = iTargetWalletID
    ) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Wallet not found or is not yours';
    END IF;

    /**
     * delete creating 'C' voucher by discussion with mehran @ 05, 13 2022
    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = vUserID,
           tblVoucher.vchType = 'C',
           tblVoucher.vchDesc = JSON_OBJECT(
               "type", IF(iPaymentType = 'N', 'Online', 'Offline'),
               "paymentID", vPaymentID
           ),
           tblVoucher.vchTotalAmount = vPaymentAmount,
           tblVoucher.vchStatus = 'F'
    ;

    SET vLastID = LAST_INSERT_ID();
    IF (vLastID IS NULL) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;
    */

    SET oRemainingAfterWallet = vTotalAmount;

    SELECT vTotalAmount - SUM(ABS(tblWalletsTransactions.wltAmount))
      INTO oRemainingAfterWallet
      FROM tblWalletsTransactions
     WHERE tblWalletsTransactions.wlt_vchID = iVoucherID
       AND tblWalletsTransactions.wlt_vchType = 'E' -- Expense
       AND tblWalletsTransactions.wltStatus = 'A'
  GROUP BY tblWalletsTransactions.wlt_vchID
    ;

    -- 1: add CREDIT
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iTargetWalletID,
           tblWalletsTransactions.wlt_vchID = iVoucherID, -- vLastID,
           tblWalletsTransactions.wlt_vchType = 'C', -- Credit
           tblWalletsTransactions.wltAmount = vPaymentAmount
    ;

    SET vLastID = LAST_INSERT_ID();
    IF (vLastID IS NULL) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = vLastID
    ;

    -- 2: add EXPENSE
    IF (IFNULL(oRemainingAfterWallet, 0) < 0) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Remaining After Wallet can not be negative';
    END IF;

    IF (oRemainingAfterWallet > 0) THEN
        INSERT INTO tblWalletsTransactions
           SET tblWalletsTransactions.wlt_walID = iTargetWalletID,
               tblWalletsTransactions.wlt_vchID = iVoucherID,
               tblWalletsTransactions.wlt_vchType = 'E', -- Expense
               tblWalletsTransactions.wltAmount = (-1) * LEAST(vPaymentAmount, oRemainingAfterWallet)
        ;

        SET vLastID = LAST_INSERT_ID();
        IF (vLastID IS NULL) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:LastID IS NULL';
        END IF;

        SET oRemainingAfterWallet = oRemainingAfterWallet - LEAST(vPaymentAmount, oRemainingAfterWallet);

        UPDATE tblWalletsTransactions
           SET tblWalletsTransactions.wltStatus = 'A' -- why 'Y' ?
         WHERE tblWalletsTransactions.wltID = vLastID
        ;
    END IF;

    COMMIT;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spVoucher_GetRemaining`;
DELIMITER ;;
CREATE PROCEDURE `spVoucher_GetRemaining`(
    IN `iVoucherID` BIGINT UNSIGNED,
    OUT `oRemainingAmount` BIGINT UNSIGNED
)
BEGIN
    DECLARE vTotalAmount BIGINT UNSIGNED;
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET -- tblActionLogs.atlBy_usrID = iFromUserID,
               tblActionLogs.atlType = 'spVoucher_GetRemaining.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iVoucherID", iVoucherID,
                   "vTotalAmount", vTotalAmount,
                   "oRemainingAmount", oRemainingAmount
               )
        ;

--        ROLLBACK;
        RESIGNAL;
    END;

    SELECT vchTotalAmount
      INTO vTotalAmount
      FROM tblVoucher
     WHERE vchID = iVoucherID
    ;

    IF ISNULL(vTotalAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Voucher not found';
    END IF

    SELECT vTotalAmount - SUM(ABS(tblWalletsTransactions.wltAmount))
      INTO oRemainingAmount
      FROM tblWalletsTransactions
     WHERE tblWalletsTransactions.wlt_vchID = iVoucherID
       AND tblWalletsTransactions.wlt_vchType = 'E' -- Expense
       AND tblWalletsTransactions.wltStatus = 'A'
  GROUP BY tblWalletsTransactions.wlt_vchID
    ;

END;;
DELIMITER ;
