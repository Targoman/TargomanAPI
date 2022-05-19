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

ALTER TABLE `tblVoucher`
    ADD COLUMN `vchProcessResult` JSON NULL AFTER `vchTotalAmount`;

ALTER TABLE `tblOfflinePaymentClaims`
    CHANGE COLUMN `ofpc_vchID` `ofpc_vchID` BIGINT(20) UNSIGNED NULL AFTER `ofpcID`;

RENAME TABLE `tblWalletBalances` TO `tblWalletsBalanceHistory`;

ALTER TABLE `tblWalletsTransactions`
    CHANGE COLUMN `wltStatus` `wltStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, A: Succeded, R: Removed' COLLATE 'utf8mb4_general_ci' AFTER `wltAmount`;

ALTER TABLE `tblUserWallets`
    CHANGE COLUMN `walLastBalance` `walBalance` BIGINT(19) NOT NULL DEFAULT '0' AFTER `walMaxTransferPerDay`;

ALTER TABLE `tblUserWallets`
    CHANGE COLUMN `walSumExpenses` `walSumExpense` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `walSumIncome`;

ALTER TABLE `tblWalletsTransactions`
    CHANGE COLUMN `wltDateTime` `wltDateTime` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) AFTER `wltStatus`;

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
           tblUserWallets.walBalance,
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

        WHEN 'Z' THEN -- Prize
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
    SET oMD5 = {{dbprefix}}Common.fnCreateRandomMD5();

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
    DECLARE vVoucherType CHAR(1);

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
               tblVoucher.vch_usrID,
               tblVoucher.vchType
          INTO
--               vPaymentID,
--               vExpenseVoucherID,
               vPaymentAmount,
               vTotalAmount,
               vUserID,
               vVoucherType
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
               tblVoucher.vch_usrID,
               tblVoucher.vchType
          INTO
--               vPaymentID,
--               vExpenseVoucherID,
               vPaymentAmount,
               vTotalAmount,
               vUserID,
               vVoucherType
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

    IF (vVoucherType = 'C') THEN
        -- bypass creating 'E' wallet transaction
        SET oRemainingAfterWallet = 0;
    ELSE
        SET oRemainingAfterWallet = vTotalAmount;

        SELECT vTotalAmount - SUM(ABS(tblWalletsTransactions.wltAmount))
          INTO oRemainingAfterWallet
          FROM tblWalletsTransactions
         WHERE tblWalletsTransactions.wlt_vchID = iVoucherID
           AND tblWalletsTransactions.wlt_vchType = 'E' -- Expense
           AND tblWalletsTransactions.wltStatus = 'A'
      GROUP BY tblWalletsTransactions.wlt_vchID
        ;
    END IF;

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
           SET tblWalletsTransactions.wltStatus = 'A' -- 'Y'?
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
    END IF;

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

DROP PROCEDURE IF EXISTS `spWalletTransaction_Create`;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransaction_Create`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    OUT `oAmount` INT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vTransactionType CHAR(1);
    DECLARE vMultiplier TINYINT;
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vLastID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'spWalletTransaction_Create.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iWalletID", iWalletID,
                   "iVoucherID", iVoucherID
               )
        ;

--	      ROLLBACK;
        RESIGNAL;
    END;

    SELECT tblVoucher.vch_usrID,
           tblVoucher.vchType,
           tblVoucher.vchTotalAmount
      INTO vUserID,
           vTransactionType,
           oAmount
      FROM tblVoucher
     WHERE tblVoucher.vchID = iVoucherID
       AND tblVoucher.vchStatus = 'N'
    ;

    IF ISNULL(vUserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid voucher ID';
    END IF;

    CALL spWalletTransaction_Validate(vUserID, iWalletID, vTransactionType, oAmount, vMultiplier);

    IF oAmount > 0 THEN
        INSERT INTO tblWalletsTransactions
           SET tblWalletsTransactions.wlt_walID = iWalletID,
               tblWalletsTransactions.wlt_vchID = iVoucherID,
               tblWalletsTransactions.wlt_vchType = vTransactionType,
               tblWalletsTransactions.wltAmount = CAST(oAmount AS SIGNED) * vMultiplier
        ;
    END IF;

    SET vLastID = LAST_INSERT_ID();
    IF (vLastID IS NULL) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = vLastID
    ;

END;;
DELIMITER ;

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
    DECLARE vMultiplier TINYINT;
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

    CALL spWalletTransaction_Validate(iFromUserID, iFromWalID, 'T', iAmount, vMultiplier);

    START TRANSACTION;

    -- 1:T
    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iFromUserID,
           tblVoucher.vchType = 'T',
           tblVoucher.vchDesc = JSON_OBJECT(
               "targetID", vTargetUserID,
               "targetLogin", iToUserLogin,
               "targetName", vToNameFamily
           ),
           tblVoucher.vchTotalAmount = CAST(iAmount AS SIGNED) * vMultiplier,
           tblVoucher.vchStatus = 'F';

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iFromWalID,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wlt_vchType = 'T',
           tblWalletsTransactions.wltAmount = CAST(iAmount AS SIGNED) * vMultiplier;

    -- Do not merge this with previous query. Creation of a transatction must be separaet from it's acceptance
    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();

    -- 2:F
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

DROP PROCEDURE IF EXISTS `spWithdrawal_Request`;
DELIMITER ;;
CREATE PROCEDURE `spWithdrawal_Request`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iForUsrID` BIGINT UNSIGNED,
    IN `iByUserID` BIGINT UNSIGNED,
    IN `iAmount` INT UNSIGNED,
    IN `iDesc` VARCHAR(500),
    OUT `oVoucherID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vMultiplier TINYINT;
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iByUserID,
               tblActionLogs.atlType = 'Withdraw.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iWalletID", iWalletID,
                   "iForUsrID", iForUsrID,
                   "iByUserID", iByUserID,
                   "iAmount", iAmount,
                   "iDesc", iDesc
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    CALL spWalletTransaction_Validate(iForUsrID, iWalletID, 'W', iAmount, vMultiplier);

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iForUsrID,
           tblVoucher.vchType = 'W',
           tblVoucher.vchTotalAmount = CAST(iAmount AS SIGNED) * vMultiplier,
           tblVoucher.vchDesc = JSON_OBJECT(
               "walletID", iWalletID,
               "desc", iDesc
           )
    ;

    SET oVoucherID = LAST_INSERT_ID();

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletPayment_Reserve`;
DELIMITER ;;
CREATE PROCEDURE `spWalletPayment_Reserve`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iUserID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vIsAllowed BOOLEAN;
    DECLARE vWalletStatus CHAR(1);
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vAmount BIGINT;
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'reserveWallet.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iWalletID", iWalletID,
                   "iVoucherID", iVoucherID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;


    SELECT tblVoucher.vchTotalAmount
      INTO vAmount
      FROM tblVoucher
     WHERE tblVoucher.vchID = iVoucherID
    ;

    IF ISNULL(vWalletStatus) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:voucher not found';
    END IF;

    SELECT tblUserWallets.walID,
           tblUserWallets.walBalance - vAmount < tblUserWallets.walMinBalance,
           tblUserWallets.walStatus
      INTO iWalletID,
           vIsAllowed,
           vWalletStatus
      FROM tblUserWallets
     WHERE tblUserWallets.wal_usrID = iUserID
       AND (tblUserWallets.walID = iWalletID
        OR (iWalletID = 0
       AND tblUserWallets.walDefault = 1
           )
           )
    ;

    IF ISNULL(vWalletStatus) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet not found';
    ELSEIF vWalletStatus != 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Selected wallet can not be used';
    ELSEIF vIsAllowed = FALSE THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Not enough credit in your wallet';
    END IF;

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iWalletID,
           tblWalletsTransactions.wlt_vchID = iVoucherID,
           tblWalletsTransactions.wltAmount = vAmount
    ;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spBalanceInfo_Prepare`;
DELIMITER ;;
CREATE PROCEDURE `spBalanceInfo_Prepare`(
    IN `iWalID` BIGINT UNSIGNED,
    IN `iWltID` BIGINT UNSIGNED,
    IN `iWltDateTime` DATETIME(6),
    OUT `oLastTransactionDate` DATETIME(6),
    OUT `oLastBalance` BIGINT,
    OUT `oLastSumIncome` BIGINT,
    OUT `oLastSumExpense` BIGINT,
    OUT `oLastSumCredit` BIGINT,
    OUT `oLastSumDebit` BIGINT,
    OUT `oMultipleInsert` TINYINT
)
BEGIN
    SELECT tblWalletsTransactions.wltDateTime
      INTO oLastTransactionDate
      FROM tblWalletsTransactions
INNER JOIN tblWalletsBalanceHistory
        ON tblWalletsBalanceHistory.wbl_wltID = tblWalletsTransactions.wltID
     WHERE tblWalletsTransactions.wlt_walID = iWalID
       AND tblWalletsTransactions.wltID < iWltID
  ORDER BY tblWalletsTransactions.wltDateTime DESC
     LIMIT 1
    ;

    CALL {{dbprefix}}Common.spLogDebug('spBalanceInfo_Prepare',
        JSON_OBJECT(
            'oLastTransactionDate', oLastTransactionDate,
            'iWalID', iWalID,
            'iWltID', iWltID,
            'iWltDateTime', iWltDateTime,
            'compare', oLastTransactionDate >= iWltDateTime
        ))
    ;

    IF NOT ISNULL(oLastTransactionDate) AND oLastTransactionDate >= iWltDateTime THEN
        DELETE tblWalletsBalanceHistory
          FROM tblWalletsBalanceHistory
    INNER JOIN tblWalletsTransactions
            ON tblWalletsTransactions.wltID = tblWalletsBalanceHistory.wbl_wltID
         WHERE tblWalletsTransactions.wltDateTime >= iWltDateTime
        ;

        SET oMultipleInsert = TRUE;
    END IF;

    SET oLastTransactionDate = NULL;

    SELECT tblWalletsTransactions.wltDateTime
         , tblWalletsBalanceHistory.wblBalance
         , tblWalletsBalanceHistory.wblSumIncome
         , tblWalletsBalanceHistory.wblSumExpense
         , tblWalletsBalanceHistory.wblSumCredit
         , tblWalletsBalanceHistory.wblSumDebit
      INTO oLastTransactionDate
         , oLastBalance
         , oLastSumIncome
         , oLastSumExpense
         , oLastSumCredit
         , oLastSumDebit
      FROM tblWalletsBalanceHistory
INNER JOIN tblWalletsTransactions
        ON tblWalletsTransactions.wltID = tblWalletsBalanceHistory.wbl_wltID
     WHERE tblWalletsTransactions.wlt_walID = iWalID
  ORDER BY tblWalletsTransactions.wltDateTime DESC,
           tblWalletsTransactions.wltID DESC
     LIMIT 1;

END;;
DELIMITER ;

DROP TRIGGER IF EXISTS `trg_tblWalletsTransactions_after_insert`;
DELIMITER ;;
CREATE TRIGGER `trg_tblWalletsTransactions_after_insert` AFTER INSERT ON `tblWalletsTransactions` FOR EACH ROW
Proc: BEGIN
    DECLARE vLastTransactionDate DATETIME(6);
    DECLARE vLastBalance BIGINT;
    DECLARE vLastSumIncome BIGINT;
    DECLARE vLastSumExpense BIGINT;
    DECLARE vLastSumCredit BIGINT;
    DECLARE vLastSumDebit BIGINT;
    DECLARE vMultipleInsert BOOL;

    IF NEW.wltStatus != 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Transaction status must be New on insertion';
    END IF;

    CALL spBalanceInfo_Prepare(
          NEW.wlt_walID
        , NEW.wltID
        , NEW.wltDateTime
        , vLastTransactionDate
        , vLastBalance
        , vLastSumIncome
        , vLastSumExpense
        , vLastSumCredit
        , vLastSumDebit
        , vMultipleInsert
    );

    SET vLastBalance = IFNULL(vLastBalance, 0);
    SET vLastSumIncome = IFNULL(vLastSumIncome, 0);
    SET vLastSumExpense = IFNULL(vLastSumExpense, 0);
    SET vLastSumCredit = IFNULL(vLastSumCredit, 0);
    SET vLastSumDebit = IFNULL(vLastSumDebit, 0);

    CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_insert:before action',
        JSON_OBJECT(
            'wlt_walID', NEW.wlt_walID,
            'wltID', NEW.wltID,
            'wltDateTime', NEW.wltDateTime,
            'wlt_vchType', NEW.wlt_vchType,
            'wltAmount', NEW.wltAmount,
            'vLastTransactionDate', vLastTransactionDate,
            'vLastBalance', vLastBalance,
            'vLastSumIncome', vLastSumIncome,
            'vLastSumExpense', vLastSumExpense,
            'vLastSumCredit', vLastSumCredit,
            'vLastSumDebit', vLastSumDebit,
            'vMultipleInsert', vMultipleInsert
        ))
    ;

    IF ISNULL(vMultipleInsert) THEN
        INSERT
          INTO tblWalletsBalanceHistory (
               wbl_wltID
             , wblBalance
             , wblSumIncome
             , wblSumExpense
             , wblSumCredit
             , wblSumDebit
               )
        VALUES (
               NEW.wltID
             , vLastBalance
             , vLastSumIncome
             , vLastSumExpense
             , vLastSumCredit
             , vLastSumDebit
               )
        ;

        SET @uvAffectedRows = ROW_COUNT();
        CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_insert:after action(1:INSERT)',
            JSON_OBJECT(
                '@uvAffectedRows', @uvAffectedRows,
                'wlt_walID', NEW.wlt_walID,
                'wltID', NEW.wltID,
                'wltDateTime', NEW.wltDateTime,
                'wlt_vchType', NEW.wlt_vchType,
                'wltAmount', NEW.wltAmount,
                'vLastTransactionDate', vLastTransactionDate,
                'vLastBalance', vLastBalance,
                'vLastSumIncome', vLastSumIncome,
                'vLastSumExpense', vLastSumExpense,
                'vLastSumCredit', vLastSumCredit,
                'vLastSumDebit', vLastSumDebit,
                'vMultipleInsert', vMultipleInsert
            ))
        ;
    ELSE
        SET @uvLastBalance = vLastBalance;
        SET @uvLastSumIncome = vLastSumIncome;
        SET @uvLastSumExpense = vLastSumExpense;
        SET @uvLastSumCredit = vLastSumCredit;
        SET @uvLastSumDebit = vLastSumDebit;

        INSERT
          INTO tblWalletsBalanceHistory (
               wbl_wltID
             , wblBalance
             , wblSumIncome
             , wblSumExpense
             , wblSumCredit
             , wblSumDebit
               )
        SELECT tblWalletsTransactions.wltID
             , (@uvLastBalance := @uvLastBalance + IF(tblWalletsTransactions.wltID = NEW.wltID, 0,
                   IF(tblWalletsTransactions.wltStatus = 'A', -- kz:why 'P'?
                       tblWalletsTransactions.wltAmount, 0)
               ))
             , (@uvLastSumIncome := @uvLastSumIncome + IF(tblWalletsTransactions.wltID = NEW.wltID, 0,
                   IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType IN ('I', 'Z'),
                       tblWalletsTransactions.wltAmount, 0)
               ))
             , (@uvLastSumExpense := @uvLastSumExpense + IF(tblWalletsTransactions.wltID = NEW.wltID, 0,
                   IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType = 'E',
                       ABS(tblWalletsTransactions.wltAmount), 0)
               ))
             , (@uvLastSumCredit := @uvLastSumCredit + IF(tblWalletsTransactions.wltID = NEW.wltID, 0,
                   IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType IN ('C', 'F'),
                       tblWalletsTransactions.wltAmount, 0)
               ))
             , (@uvLastSumDebit := @uvLastSumDebit + IF(tblWalletsTransactions.wltID = NEW.wltID, 0,
                   IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType IN ('W', 'T'),
                       ABS(tblWalletsTransactions.wltAmount), 0)
               ))
          FROM tblWalletsTransactions
-- kz:why?          JOIN (SELECT @uvLastBalance := IFNULL(vLastBalance, 0)) rB
-- kz:why?          JOIN (SELECT @uvLastSumIncome := IFNULL(vLastSumIncome, 0)) rI
-- kz:why?          JOIN (SELECT @uvLastSumExpense := IFNULL(vLastSumExpense, 0)) rE
-- kz:why?          JOIN (SELECT @uvLastSumCredit := IFNULL(vLastSumCredit, 0)) rC
-- kz:why?          JOIN (SELECT @uvLastSumDebit := IFNULL(vLastSumDebit, 0)) rC
         WHERE tblWalletsTransactions.wlt_walID = NEW.wlt_walID
           AND tblWalletsTransactions.wltDateTime >= vLastTransactionDate
      ORDER BY tblWalletsTransactions.wltDateTime ASC
             , tblWalletsTransactions.wltID ASC
        ;

        SET @uvAffectedRows = ROW_COUNT();

        SET vLastBalance = @uvLastBalance;
        SET vLastSumIncome = @uvLastSumIncome;
        SET vLastSumExpense = @uvLastSumExpense;
        SET vLastSumCredit = @uvLastSumCredit;
        SET vLastSumDebit = @uvLastSumDebit;

        CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_insert:after action(2:INSERT)',
            JSON_OBJECT(
                '@uvAffectedRows', @uvAffectedRows,
                'wlt_walID', NEW.wlt_walID,
                'wltID', NEW.wltID,
                'wltDateTime', NEW.wltDateTime,
                'wlt_vchType', NEW.wlt_vchType,
                'wltAmount', NEW.wltAmount,
                'vLastTransactionDate', vLastTransactionDate,
                'vLastBalance', vLastBalance,
                'vLastSumIncome', vLastSumIncome,
                'vLastSumExpense', vLastSumExpense,
                'vLastSumCredit', vLastSumCredit,
                'vLastSumDebit', vLastSumDebit,
                'vMultipleInsert', vMultipleInsert
            ))
        ;
    END IF;

/*
this lines moved to after update
    UPDATE tblUserWallets
       SET tblUserWallets.walBalance = IFNULL(vLastBalance, 0) - IF(NEW.wltAmount < 0, NEW.wltAmount, 0)
     WHERE tblUserWallets.walID = NEW.wlt_walID;
/**/

END;;
DELIMITER ;

DROP TRIGGER IF EXISTS `trg_tblWalletsTransactions_after_update`;
DELIMITER ;;
CREATE TRIGGER `trg_tblWalletsTransactions_after_update` AFTER UPDATE ON `tblWalletsTransactions` FOR EACH ROW
Proc: BEGIN
    DECLARE vLastTransactionDate DATETIME(6);
    DECLARE vLastBalance BIGINT;
    DECLARE vLastSumIncome BIGINT;
    DECLARE vLastSumExpense BIGINT;
    DECLARE vLastSumCredit BIGINT;
    DECLARE vLastSumDebit BIGINT;
    DECLARE vMultipleInsert BOOL;

    IF NEW.wltStatus = 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Wallet status can not be reset to new';
    END IF;

    CALL spBalanceInfo_Prepare(
          NEW.wlt_walID
        , NEW.wltID
        , NEW.wltDateTime
        , vLastTransactionDate
        , vLastBalance
        , vLastSumIncome
        , vLastSumExpense
        , vLastSumCredit
        , vLastSumDebit
        , vMultipleInsert
    );

    SET vLastBalance = IFNULL(vLastBalance, 0);
    SET vLastSumIncome = IFNULL(vLastSumIncome, 0);
    SET vLastSumExpense = IFNULL(vLastSumExpense, 0);
    SET vLastSumCredit = IFNULL(vLastSumCredit, 0);
    SET vLastSumDebit = IFNULL(vLastSumDebit, 0);

    CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_update:before action',
        JSON_OBJECT(
            'wlt_walID', NEW.wlt_walID,
            'wltID', NEW.wltID,
            'wltDateTime', NEW.wltDateTime,
            'wlt_vchType', NEW.wlt_vchType,
            'wltAmount', NEW.wltAmount,
            'vLastTransactionDate', vLastTransactionDate,
            'vLastBalance', vLastBalance,
            'vLastSumIncome', vLastSumIncome,
            'vLastSumExpense', vLastSumExpense,
            'vLastSumCredit', vLastSumCredit,
            'vLastSumDebit', vLastSumDebit,
            'vMultipleInsert', vMultipleInsert
        ))
    ;

    IF (vLastBalance + NEW.wltAmount < 0) THEN
        SET @uvMessageText = CONCAT('401:Wallet balance can not be negative.',
            ' LastBalance(', CAST(vLastBalance AS CHAR), ')',
            ' wlt_vchType(', CAST(NEW.wlt_vchType AS CHAR), ')',
            ' wltAmount(', CAST(NEW.wltAmount AS CHAR), ')',
            ''
            );

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = @uvMessageText;
    END IF;

    IF ISNULL(vMultipleInsert) THEN
        SET vLastBalance = vLastBalance + NEW.wltAmount;
        SET vLastSumIncome = vLastSumIncome + IF(NEW.wlt_vchType IN ('I', 'Z'), NEW.wltAmount, 0);
        SET vLastSumExpense = vLastSumExpense + IF(NEW.wlt_vchType = 'E', ABS(NEW.wltAmount), 0);
        SET vLastSumCredit = vLastSumCredit + IF(NEW.wlt_vchType IN ('C', 'F'), NEW.wltAmount, 0);
        SET vLastSumDebit = vLastSumDebit + IF(NEW.wlt_vchType IN ('W', 'T'), ABS(NEW.wltAmount), 0);

        UPDATE tblWalletsBalanceHistory
           SET tblWalletsBalanceHistory.wblBalance = vLastBalance
             , tblWalletsBalanceHistory.wblSumIncome = vLastSumIncome
             , tblWalletsBalanceHistory.wblSumExpense = vLastSumExpense
             , tblWalletsBalanceHistory.wblSumCredit = vLastSumCredit
             , tblWalletsBalanceHistory.wblSumDebit = vLastSumDebit
         WHERE tblWalletsBalanceHistory.wbl_wltID  = NEW.wltID
        ;

        SET @uvAffectedRows = ROW_COUNT();
        CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_update:after action(1:UPDATE)',
            JSON_OBJECT(
                '@uvAffectedRows', @uvAffectedRows,
                'wlt_walID', NEW.wlt_walID,
                'wltID', NEW.wltID,
                'wltDateTime', NEW.wltDateTime,
                'wlt_vchType', NEW.wlt_vchType,
                'wltAmount', NEW.wltAmount,
                'vLastTransactionDate', vLastTransactionDate,
                'vLastBalance', vLastBalance,
                'vLastSumIncome', vLastSumIncome,
                'vLastSumExpense', vLastSumExpense,
                'vLastSumCredit', vLastSumCredit,
                'vLastSumDebit', vLastSumDebit,
                'vMultipleInsert', vMultipleInsert
            ))
        ;
    ELSE
        SET @uvLastBalance = vLastBalance;
        SET @uvLastSumIncome = vLastSumIncome;
        SET @uvLastSumExpense = vLastSumExpense;
        SET @uvLastSumCredit = vLastSumCredit;
        SET @uvLastSumDebit = vLastSumDebit;

        DELETE
          FROM tblWalletsBalanceHistory
         WHERE tblWalletsBalanceHistory.wbl_wltID = NEW.wltID
        ;

        SET @uvAffectedRows = ROW_COUNT();
        CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_update:after action (2:DELETE)',
            JSON_OBJECT(
                '@uvAffectedRows', @uvAffectedRows,
                'wlt_walID', NEW.wlt_walID,
                'wltID', NEW.wltID,
                'wltDateTime', NEW.wltDateTime,
                'wlt_vchType', NEW.wlt_vchType,
                'wltAmount', NEW.wltAmount,
                'vLastTransactionDate', vLastTransactionDate,
                'vLastBalance', vLastBalance,
                'vLastSumIncome', vLastSumIncome,
                'vLastSumExpense', vLastSumExpense,
                'vLastSumCredit', vLastSumCredit,
                'vLastSumDebit', vLastSumDebit,
                'vMultipleInsert', vMultipleInsert
            ))
        ;

        INSERT
          INTO tblWalletsBalanceHistory (
               wbl_wltID
             , wblBalance
             , wblSumIncome
             , wblSumExpense
             , wblSumCredit
             , wblSumDebit
               )
        SELECT tblWalletsTransactions.wltID
             , (@uvLastBalance := @uvLastBalance + IF(tblWalletsTransactions.wltStatus = 'A', -- kz:why 'P'?
                    tblWalletsTransactions.wltAmount, 0)
               )
             , (@uvLastSumIncome := @uvLastSumIncome + IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType IN ('I', 'Z'),
                    tblWalletsTransactions.wltAmount, 0)
               )
             , (@uvLastSumExpense := @uvLastSumExpense + IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType = 'E',
                    ABS(tblWalletsTransactions.wltAmount), 0)
               )
             , (@uvLastSumCredit := @uvLastSumCredit + IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType IN ('C', 'F'),
                    tblWalletsTransactions.wltAmount, 0)
               )
             , (@uvLastSumDebit := @uvLastSumDebit + IF(tblWalletsTransactions.wltStatus = 'A' AND tblWalletsTransactions.wlt_vchType IN ('W', 'T'),
                    ABS(tblWalletsTransactions.wltAmount), 0)
               )
          FROM tblWalletsTransactions
-- kz:why?          JOIN (SELECT @uvLastBalance := IFNULL(vLastBalance, 0)) rB
-- kz:why?          JOIN (SELECT @uvLastSumIncome := IFNULL(vLastSumIncome, 0)) rI
-- kz:why?          JOIN (SELECT @uvLastSumExpense := IFNULL(vLastSumExpense, 0)) rE
-- kz:why?          JOIN (SELECT @uvLastSumCredit := IFNULL(vLastSumCredit, 0)) rC
-- kz:why?          JOIN (SELECT @uvLastSumDebit := IFNULL(vLastSumDebit, 0)) rC
         WHERE tblWalletsTransactions.wlt_walID = NEW.wlt_walID
           AND tblWalletsTransactions.wltDateTime >= vLastTransactionDate
      ORDER BY tblWalletsTransactions.wltDateTime ASC
             , tblWalletsTransactions.wltID ASC
        ;

        SET @uvAffectedRows = ROW_COUNT();

        SET vLastBalance = @uvLastBalance;
        SET vLastSumIncome = @uvLastSumIncome;
        SET vLastSumExpense = @uvLastSumExpense;
        SET vLastSumCredit = @uvLastSumCredit;
        SET vLastSumDebit = @uvLastSumDebit;

        CALL {{dbprefix}}Common.spLogDebug('trg_tblWalletsTransactions_after_update:after action (2:INSERT)',
            JSON_OBJECT(
                '@uvAffectedRows', @uvAffectedRows,
                'wlt_walID', NEW.wlt_walID,
                'wltID', NEW.wltID,
                'wltDateTime', NEW.wltDateTime,
                'wlt_vchType', NEW.wlt_vchType,
                'wltAmount', NEW.wltAmount,
                'vLastTransactionDate', vLastTransactionDate,
                'vLastBalance', vLastBalance,
                'vLastSumIncome', vLastSumIncome,
                'vLastSumExpense', vLastSumExpense,
                'vLastSumCredit', vLastSumCredit,
                'vLastSumDebit', vLastSumDebit,
                'vMultipleInsert', vMultipleInsert
            ))
        ;
    END IF;

    UPDATE tblUserWallets
       SET tblUserWallets.walBalance = vLastBalance
         , tblUserWallets.walSumIncome = vLastSumIncome
         , tblUserWallets.walSumExpense = vLastSumExpense
         , tblUserWallets.walSumCredit = vLastSumCredit
         , tblUserWallets.walSumDebit = vLastSumDebit
     WHERE tblUserWallets.walID = NEW.wlt_walID;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWallet_Increase`;
DELIMITER ;;
CREATE PROCEDURE `spWallet_Increase`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iForUsrID` BIGINT UNSIGNED,
    IN `iByUserID` BIGINT UNSIGNED,
    IN `iType` CHAR(1),
    IN `iAmount` INT UNSIGNED,
    IN `iDesc` VARCHAR(500),
    OUT `oVoucherID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vMultiplier TINYINT;
    DECLARE vLastID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iByUserID,
               tblActionLogs.atlType = 'spWallet_Increase.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iWalletID", iWalletID,
                   "iForUsrID", iForUsrID,
                   "iType", iType,
                   "iAmount", iAmount,
                   "iDesc", iDesc
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    CALL spWalletTransaction_Validate(iForUsrID, iWalletID, iType, iAmount, vMultiplier);

    START TRANSACTION;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iForUsrID,
           tblVoucher.vchType = iType,
           tblVoucher.vchTotalAmount = CAST(iAmount AS SIGNED) * vMultiplier,
           tblVoucher.vchDesc = JSON_OBJECT(
               "walletID", iWalletID,
               "desc", iDesc
           )
    ;

    SET oVoucherID = LAST_INSERT_ID();

    UPDATE tblVoucher
       SET tblVoucher.vchStatus = 'F' -- New -> Finished
     WHERE tblVoucher.vchID = oVoucherID
    ;

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iWalletID,
           tblWalletsTransactions.wlt_vchID = oVoucherID,
           tblWalletsTransactions.wlt_vchType = iType,
           tblWalletsTransactions.wltAmount = CAST(iAmount AS SIGNED) * vMultiplier
    ;

    SET vLastID = LAST_INSERT_ID();
    IF ISNULL(vLastID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:WLT.LastID IS NULL';
    END IF;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A' -- Succeded
     WHERE tblWalletsTransactions.wltID = vLastID
    ;

    COMMIT;

END;;
DELIMITER ;
