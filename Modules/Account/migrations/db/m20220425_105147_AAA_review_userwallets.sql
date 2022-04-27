/* Migration File: m20220425_105147_AAA_review_userwallets.sql */


ALTER TABLE `tblWalletsTransactions`
    DROP INDEX `wltType`,
    ADD INDEX `wlt_vchType` (`wlt_vchType`) USING BTREE;

DROP PROCEDURE IF EXISTS `spWalletTransaction_Create`;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransaction_Create`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    OUT `oAmount` INT UNSIGNED
)
BEGIN
    DECLARE vTransactionType CHAR(1);
    DECLARE vMultiplier TINYINT;
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vErr VARCHAR(500);

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
           SET tblWalletsTransactions.wlt_walID = WalletID,
               tblWalletsTransactions.wlt_vchID = iVoucherID,
               tblWalletsTransactions.wlt_vchType = TrasnactionType,
               tblWalletsTransactions.wltAmount = oAmount * vMultiplier
        ;
    END IF;

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
    DECLARE vIBAN VARCHAR(50);

    SELECT tblUserWallets.walID,
           tblUserWallets.walMinBalance,
           tblUserWallets.walLastBalance,
           tblUserWallets.walNotTransferableAmount,
           tblUserWallets.walMaxTransferPerDay,
           tblUserExtraInfo.ueiIBAN
      INTO ioWalletID,
           vMinBalance,
           vLastBalance,
           vNotTransferable,
           vMaxTransferPerDay,
           vIBAN
      FROM tblUserWallets
 LEFT JOIN tblUserExtraInfo
        ON tblUserExtraInfo.uei_usrID = tblUserWallets.wal_usrID
     WHERE tblUserWallets.wal_usrID = iUserID
       AND (tblUserWallets.walID = ioWalletID
        OR (ioWalletID = 0
       AND tblUserWallets.walDefault = 1
           )
           )
    ;

    IF ISNULL(ioWalletID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet not found or is not yours';
    END IF;

    CASE iTransactionType
        WHEN 'E' THEN -- Expense
            SET ioAmount = LEAST(ioAmount, GREATEST(0, vLastBalance - vMinBalance)), oMultiplier = -1;

        WHEN 'T' THEN -- TransferTo
            IF vLastBalance - vNotTransferable - vMinBalance - ioAmount < 0 THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '401:Not enough credit in wallet to transfer';
            END IF;

            SELECT COALESCE(SUM(tblWalletsTransactions.wltAmount),0) INTO vTodayTransfers
              FROM tblWalletsTransactions
             WHERE tblWalletsTransactions.wlt_walID = ioWalletID
               AND DATE(tblWalletsTransactions.wltDateTime) = DATE(NOW())
               AND tblWalletsTransactions.wlt_vchType = 'T'
               AND tblWalletsTransactions.wltStatus = 'A'
            ;

            IF MaxTransferable >0 AND vMaxTransferPerDay < ioAmount + vTodayTransfers THEN
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

            IF vLastBalance - vNotTransferable - vMinBalance - ioAmount < 0 THEN
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

DROP PROCEDURE IF EXISTS `spWallet_Transfer`;
DELIMITER ;;
CREATE PROCEDURE `spWallet_Transfer`(
    IN `iFromUserID` BIGINT UNSIGNED,
    IN `iFromWalID` INT,
    IN `iToUserLogin` INT,
    IN `iAmount` INT,
    IN `iPass` INT,
    IN `iSalt` INT
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
               tblActionLogs.atlType = 'Transfer.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iFromUserID", iFromUserID,
                   "iFromWalID", iFromWalID,
                   "iToUserLogin", iToUserLogin,
                   "iAmount", iAmount,
                   "iPass", iPass,
                   "iSalt", iSalt
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
           tblVoucher.vchTotalAmount = Amount,
           tblVoucher.vchStatus = 'F';

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iFromWalID,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wlt_vchType = 'T',
           tblWalletsTransactions.wltAmount = iAmount * -1;

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
           tblVoucher.vchTotalAmount = Amount,
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

DROP PROCEDURE IF EXISTS `spWallet_SetAsDefault`;
DELIMITER ;;
CREATE PROCEDURE `spWallet_SetAsDefault`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iWalID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vWalDefault INT;
    DECLARE vUserID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'UserWallets_setAsDefault.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iUserID,
                   "iWalID", iWalID
               )
        ;

--        ROLLBACK;
        RESIGNAL;
    END;

    SELECT tblUserWallets.wal_usrID,
           tblUserWallets.walDefault
      INTO vUserID,
           vWalDefault
      FROM tblUserWallets
     WHERE tblUserWallets.walID = iWalID
    ;

    IF ISNULL(vUserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet not found';
    END IF;

    IF (iUserID > 0 AND iUserID <> vUserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet is not yours';
    END IF;

    IF (vWalDefault = 1) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet already is default';
    END IF;

    UPDATE tblUserWallets
       SET tblUserWallets.walDefault = IF(tblUserWallets.walID = iWalID, 1, 0)
     WHERE tblUserWallets.wal_usrID = vUserID
    ;

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
           tblVoucher.vchTotalAmount = iAmount,
           tblVoucher.vchDesc = JSON_OBJECT(
               "desc", iDesc
           )
    ;

    SET oVoucherID = LAST_INSERT_ID();

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWithdrawal_Accept`;
DELIMITER ;;
CREATE PROCEDURE `spWithdrawal_Accept`(
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iOperator_usrID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vAmount BIGINT;
    DECLARE vWalletID BIGINT UNSIGNED;
    DECLARE vLastID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iOperator_usrID,
               tblActionLogs.atlType = 'Withdrawal_Accept.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iOperator_usrID,
                   "iVoucherID", iVoucherID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    -- find voucher
    SELECT vch_usrID,
           vchTotalAmount
      INTO vUserID,
           vAmount
      FROM tblVoucher
     WHERE tblVoucher.vchID = iVoucherID
    ;

    IF ISNULL(vAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Voucher not found';
    END IF;

    -- find default wallet
    SELECT walID
      INTO vWalletID
      FROM tblUserWallets
     WHERE tblUserWallets.wal_usrID = vUserID
       AND tblUserWallets.walDefault = 1
    ;

    IF ISNULL(vWalletID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Default wallet not found';
    END IF;

    START TRANSACTION;

    UPDATE tblVoucher
       SET tblVoucher.vchStatus = 'F' -- New -> Finished
--           tblVoucher.vchUpdatedBy_usrID = iOperator_usrID
     WHERE tblVoucher.vchID = iVoucherID
    ;

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = vWalletID,
           tblWalletsTransactions.wlt_vchID = iVoucherID,
           tblWalletsTransactions.wlt_vchType = 'W', -- Withdrawal
           tblWalletsTransactions.wltAmount = vAmount
    ;

    SET vLastID = LAST_INSERT_ID();
    IF ISNULL(vLastID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A' -- Succeded
     WHERE tblWalletsTransactions.wltID = LastID
    ;

    COMMIT;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletTransactionOnPayment_Create`;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransactionOnPayment_Create`(
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iPaymentType` CHAR(1)
)
BEGIN
    DECLARE vPaymentID BIGINT UNSIGNED;
    DECLARE vExpenseVoucherID BIGINT UNSIGNED;
    DECLARE vCreditVoucherID BIGINT UNSIGNED;
    DECLARE vAmount BIGINT UNSIGNED;
    DECLARE vTotalAmount BIGINT UNSIGNED;
    DECLARE vRemainingAfterWallet BIGINT UNSIGNED;
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vUserDefaultWallet BIGINT UNSIGNED;
    DECLARE vErr VARCHAR(500);
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
                   "iPaymentType", iPaymentType
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    START TRANSACTION;

    IF iPaymentType = 'N' THEN -- Online
        SELECT tblOnlinePayments.onpID,
               tblOnlinePayments.onp_vchID,
               tblOnlinePayments.onpAmount,
               tblVoucher.vchTotalAmount,
               tblVoucher.vch_usrID
          INTO vPaymentID,
               vExpenseVoucherID,
               vAmount,
               vTotalAmount,
               vUserID
          FROM tblOnlinePayments
          JOIN tblVoucher
            ON tblVoucher.vchID = tblOnlinePayments.onp_vchID
         WHERE tblOnlinePayments.onp_vchID = iVoucherID
           AND tblOnlinePayments.onpStatus = 'Y' -- Payed
        ;
    ELSEIF iPaymentType = 'F' THEN -- Offline
        SELECT tblOfflinePayments.ofpID,
               tblOfflinePayments.ofp_vchID,
               tblOfflinePayments.ofpAmount,
               tblVoucher.vchTotalAmount,
               tblVoucher.vch_usrID
          INTO vPaymentID,
               vExpenseVoucherID,
               vAmount,
               vTotalAmount,
               vUserID
          FROM tblOfflinePayments
          JOIN tblVoucher
            ON tblVoucher.vchID = tblOfflinePayments.ofp_vchID
         WHERE tblOfflinePayments.ofp_vchID = iVoucherID
           AND tblOfflinePayments.ofpStatus = 'Y' -- Payed
        ;
    ELSE
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Invalid payment type';
    END IF;

    IF ISNULL(vExpenseVoucherID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Payment not found or is not yet payed';
    END IF;

    SELECT tblUserWallets.walID INTO vUserDefaultWallet
      FROM tblUserWallets
     WHERE tblUserWallets.wal_usrID = vUserID
       AND tblUserWallets.walDefault = TRUE
    ;

    IF ISNULL(vUserDefaultWallet) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Default wallet not found';
    END IF;

    SELECT vTotalAmount - tblWalletsTransactions.wltAmount
      INTO vRemainingAfterWallet
      FROM tblWalletsTransactions
     WHERE tblWalletsTransactions.wlt_vchID = iVoucherID
       AND tblWalletsTransactions.wltStatus = 'A'
    ;

    IF (IFNULL(vRemainingAfterWallet, 0) < 0) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Remaining After Wallet can not be negative';
    END IF;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = vUserID,
           tblVoucher.vchType = 'C',
           tblVoucher.vchDesc = JSON_OBJECT(
               "type", IF(iPaymentType = 'N', 'Online', 'Offline'),
               "paymentID", vPaymentID
           ),
           tblVoucher.vchTotalAmount = vAmount,
           tblVoucher.vchStatus = 'F'
    ;

    SET vLastID = LAST_INSERT_ID();
    IF (vLastID IS NULL) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = vUserDefaultWallet,
           tblWalletsTransactions.wlt_vchID = vLastID,
           tblWalletsTransactions.wlt_vchType = 'C', -- Credit
           tblWalletsTransactions.wltAmount = vAmount
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

    IF (vRemainingAfterWallet > 0) THEN
        INSERT INTO tblWalletsTransactions
           SET tblWalletsTransactions.wlt_walID = vUserDefaultWallet,
               tblWalletsTransactions.wlt_vchID = iVoucherID,
               tblWalletsTransactions.wlt_vchType = 'E', -- Expense
               tblWalletsTransactions.wltAmount = vRemainingAfterWallet
        ;

        SET vLastID = LAST_INSERT_ID();
        IF (vLastID IS NULL) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:LastID IS NULL';
        END IF;

        UPDATE tblWalletsTransactions
           SET tblWalletsTransactions.wltStatus = 'Y'
         WHERE tblWalletsTransactions.wltID = vLastID
        ;
    END IF;

    COMMIT;

END;;
DELIMITER ;
