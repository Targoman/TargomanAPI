/* Migration File: m20220627_150459_AAA_dbdiffof_dev_AAA.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblPaymentGatewayTypes` (
	`pgtID` INT NOT NULL AUTO_INCREMENT,
	`pgtName` VARCHAR(50) NOT NULL,
	PRIMARY KEY (`pgtID`)
)
COLLATE='utf8mb4_general_ci'
/*!*/;

ALTER TABLE `tblPaymentGatewayTypes`
	ADD COLUMN `pgtMinRequestAmount` INT NOT NULL AFTER `pgtName`,
	ADD COLUMN `pgtMaxRequestAmount` INT NULL AFTER `pgtMinRequestAmount`,
	ADD COLUMN `pgtMaxPerDayAmount` INT NULL AFTER `pgtMaxRequestAmount`
/*!*/;

ALTER TABLE `tblPaymentGatewayTypes`
	ADD COLUMN `pgtType` CHAR(1) NOT NULL AFTER `pgtID`
/*!*/;

ALTER TABLE `tblPaymentGatewayTypes`
	CHANGE COLUMN `pgtID` `pgtID` INT(10) NOT NULL FIRST,
	DROP PRIMARY KEY
/*!*/;

ALTER TABLE `tblPaymentGatewayTypes`
	DROP COLUMN `pgtID`,
	ADD PRIMARY KEY (`pgtType`)
/*!*/;

ALTER TABLE `tblPaymentGatewayTypes`
	DROP COLUMN `pgtMaxPerDayAmount`
/*!*/;

ALTER TABLE `tblPaymentGateways`
	DROP COLUMN `pgwMinRequestAmount`,
	DROP COLUMN `pgwMaxRequestAmount`
/*!*/;

ALTER TABLE `tblPaymentGatewayTypes`
    ADD COLUMN `pgtStatus` CHAR(1) NOT NULL DEFAULT 'A' AFTER `pgtMaxRequestAmount`;

ALTER TABLE `tblPaymentGatewayTypes`
    CHANGE COLUMN `pgtMinRequestAmount` `pgtMinRequestAmount` FLOAT NOT NULL DEFAULT 0 AFTER `pgtName`,
    CHANGE COLUMN `pgtMaxRequestAmount` `pgtMaxRequestAmount` FLOAT NULL DEFAULT NULL AFTER `pgtMinRequestAmount`;

INSERT IGNORE INTO `tblPaymentGatewayTypes` (`pgtType`, `pgtName`, `pgtMinRequestAmount`, `pgtMaxRequestAmount`) VALUES
    ('-', '_DeveloperTest', 1, 100000),
    ('O', 'COD', 1, NULL),
    ('I', 'IranBank', 1, 50000000),
    ('M', 'IranIntermediateGateway', 1, 50000000),
    ('D', 'InternationalDebitCart', 1, NULL),
    ('C', 'InternationalCreditCart', 1, NULL),
    ('B', 'CryptoCurrency', 1, NULL)
;

ALTER TABLE `tblPaymentGateways`
    ADD CONSTRAINT `FK_tblPaymentGateways_tblPaymentGatewayTypes` FOREIGN KEY (`pgwType`) REFERENCES `tblPaymentGatewayTypes` (`pgtType`) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE `tblVoucher`
    ADD COLUMN `vch_rootVchID` BIGINT NULL DEFAULT NULL AFTER `vchProcessResult`,
    ADD COLUMN `vchSettled` BIT NULL DEFAULT NULL AFTER `vch_rootVchID`
/*!*/;

ALTER TABLE `tblVoucher`
    CHANGE COLUMN `vchType` `vchType` CHAR(1) NOT NULL COMMENT 'W:Withdrawal, E: Expense, I: Income, C:Credit, T:TransferTo, F: TransferFrom, Z: Prize' COLLATE 'utf8mb4_general_ci' AFTER `vch_usrID`,
    CHANGE COLUMN `vch_rootVchID` `vch_rootVchID` BIGINT(19) NULL DEFAULT NULL AFTER `vchDesc`,
    CHANGE COLUMN `vchSettled` `vchSettled` BIT(1) NULL DEFAULT NULL AFTER `vchTotalAmount`
/*!*/;

ALTER TABLE `tblVoucher`
    ADD COLUMN `vchTotalPayed` BIGINT(19) NULL AFTER `vchTotalAmount`
/*!*/;

ALTER TABLE `tblUserWallets`
    ADD COLUMN `walFreezedAmount` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `walNotTransferableAmount`
/*!*/;

ALTER TABLE `tblUserWallets`
    CHANGE COLUMN `walFreezedAmount` `walSumFreezed` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `walSumDebit`
/*!*/;

ALTER TABLE `tblVoucher`
    CHANGE COLUMN `vch_rootVchID` `vch_rootVchID` BIGINT(19) UNSIGNED NULL DEFAULT NULL AFTER `vchDesc`
/*!*/;

DROP PROCEDURE IF EXISTS `spVoucher_Cancel`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spVoucher_Cancel`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iSetAsError` BOOL
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vVoucher_usrID      BIGINT UNSIGNED;
    DECLARE vVoucherType        CHAR(1);
    DECLARE vVoucherStatus      CHAR(1);
    DECLARE vWalletID BIGINT UNSIGNED;
    DECLARE vVoucherAmount BIGINT UNSIGNED;
    DECLARE vFinished INTEGER DEFAULT 0;

    -- declare cursor
    DEClARE curunFreeze CURSOR FOR
            SELECT tblUserWallets.walID,
                   SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END) AS TotalFreezed
              FROM tblVoucher
        INNER JOIN tblUserWallets
                ON tblUserWallets.walID = JSON_EXTRACT(tblVoucher.vchDesc, '$.WalletID')
             WHERE tblVoucher.vchType IN ('R', 'U') -- freeze, unfreeze
               AND tblVoucher.vch_rootVchID = iVoucherID
          GROUP BY tblUserWallets.walID
            ;

    -- declare NOT FOUND handler
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET vFinished = 1;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'cancelVoucher.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iVoucherID", iVoucherID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    SELECT vch_usrID,
           vchType,
           vchStatus
      INTO vVoucher_usrID,
           vVoucherType,
           vVoucherStatus
      FROM tblVoucher
     WHERE vchID = iVoucherID
    ;

    IF ISNULL(vVoucher_usrID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Voucher not found';
    END IF;

    -- user or operator?
    IF (iUserID > 0 AND iUserID <> vVoucher_usrID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '403:Voucher is not yours';
    END IF;

    -- Expense (Invoice)
    IF (vVoucherType <> 'E') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Only expense vouchers are allowed';
    END IF;

    -- New
    IF (vVoucherStatus <> 'N') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Only new vouchers are allowed';
    END IF;

    START TRANSACTION;

    OPEN curunFreeze;

    unFreeze: LOOP
        FETCH curunFreeze
         INTO vWalletID
            , vVoucherAmount
        ;

        IF vFinished = 1 THEN
            LEAVE unFreeze;
        END IF;

        -- unfreeze
        CALL spWallet_unFreeze(
            iUserID,
            vWalletID,
            iVoucherID,
            vVoucherAmount
        );

    END LOOP unFreeze;

    CLOSE curunFreeze;

    UPDATE tblVoucher
       SET vchStatus = IF(iSetAsError, 'E', 'C')
     WHERE vchID = iVoucherID
    ;

    COMMIT;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletTransactionOnPayment_Create`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransactionOnPayment_Create`(
    IN `iPaymentID` BIGINT UNSIGNED,
    IN `iPaymentType` CHAR(1),
    IN `iVoucherID` BIGINT UNSIGNED,
    INOUT `ioTargetWalletID` BIGINT UNSIGNED,
    IN `iAddExpense` TINYINT,
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

    IF (ioTargetWalletID = 0) THEN
        SELECT tblUserWallets.walID
          INTO ioTargetWalletID
          FROM tblUserWallets
         WHERE tblUserWallets.wal_usrID = vUserID
           AND tblUserWallets.walDefault = true
        ;

        IF (IFNULL(ioTargetWalletID, 0) = 0) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:Default wallet not found';
        END IF;
    END IF;

    IF NOT EXISTS(
        SELECT walID
          FROM tblUserWallets
         WHERE tblUserWallets.wal_usrID = vUserID
           AND walID = ioTargetWalletID
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
       SET tblWalletsTransactions.wlt_walID = ioTargetWalletID,
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
    IF (iAddExpense) THEN
        IF (IFNULL(oRemainingAfterWallet, 0) < 0) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:Remaining After Wallet can not be negative';
        END IF;

        IF (oRemainingAfterWallet > 0) THEN
            INSERT INTO tblWalletsTransactions
               SET tblWalletsTransactions.wlt_walID = ioTargetWalletID,
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
    END IF;

    COMMIT;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletTransaction_Create`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spWalletTransaction_Create`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    INOUT `ioAmount` BIGINT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vTransactionType CHAR(1);
    DECLARE vMultiplier TINYINT;
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vLastID BIGINT UNSIGNED;
    DECLARE vAmount BIGINT UNSIGNED;

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
           vAmount
      FROM tblVoucher
     WHERE tblVoucher.vchID = iVoucherID
       AND tblVoucher.vchStatus = 'N'
    ;

    IF ISNULL(vUserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid voucher ID';
    END IF;

    IF (vAmount = 0) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Voucher remaining amount is zero';
    ELSEIF (ioAmount = 0) THEN
        SET ioAmount = vAmount;
    ELSEIF (ioAmount > vAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Requested amount is greater than voucher amount';
    ELSEIF (ioAmount < vAmount) THEN
        SET vAmount = ioAmount;
    END IF;

    CALL spWalletTransaction_Validate(vUserID, iWalletID, vTransactionType, ioAmount, vMultiplier);

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iWalletID,
           tblWalletsTransactions.wlt_vchID = iVoucherID,
           tblWalletsTransactions.wlt_vchType = vTransactionType,
           tblWalletsTransactions.wltAmount = CAST(ioAmount AS SIGNED) * vMultiplier
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

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWallet_Freeze`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spWallet_Freeze`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    INOUT `ioAmount` BIGINT UNSIGNED
)
BEGIN
    -- ioAmount=0 for freeze all WalletAvailableAmount

    DECLARE vErr VARCHAR(500);
    DECLARE vWalletAvailableAmount BIGINT;
    DECLARE vWalletStatus CHAR(1);
--    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vRemainedVoucherAmount BIGINT;
    DECLARE vLastID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'WalletFreeze.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iWalletID", iWalletID,
                   "iVoucherID", iVoucherID,
                   "ioAmount", ioAmount,
                   "vWalletAvailableAmount", vWalletAvailableAmount
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    SELECT tblUserWallets.walID,
           tblUserWallets.walBalance
           - CAST(tblUserWallets.walFreezedAmount AS SIGNED)
           - CAST(tblUserWallets.walMinBalance AS SIGNED),
           tblUserWallets.walStatus
      INTO iWalletID,
           vWalletAvailableAmount,
           vWalletStatus
      FROM tblUserWallets
     WHERE tblUserWallets.wal_usrID = iUserID
       AND (tblUserWallets.walID = iWalletID
        OR (iWalletID = 0
       AND tblUserWallets.walDefault = 1
           )
           )
    ;

    SELECT tblVoucher.vchTotalAmount
           - IFNULL(tblVoucher.vchTotalPayed, 0)
           - IFNULL(tmpFreeze.TotalFreezed, 0)
      INTO vRemainedVoucherAmount
      FROM tblVoucher
 LEFT JOIN (
    SELECT tblVoucher.vch_rootVchID
--         , SUM(tblVoucher.vchTotalAmount) AS TotalFreezed
         , SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END) AS TotalFreezed
      FROM tblVoucher
--     WHERE tblVoucher.vchType = 'R' -- freeze
     WHERE tblVoucher.vchType IN ('R', 'U') -- freeze, unfreeze
       AND tblVoucher.vch_rootVchID IS NOT NULL
  GROUP BY tblVoucher.vch_rootVchID
           ) AS tmpFreeze
        ON tmpFreeze.vch_rootVchID = tblVoucher.vchID
     WHERE tblVoucher.vchID = iVoucherID
    ;

    IF ISNULL(vWalletStatus) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet not found';
    ELSEIF (vWalletStatus != 'A') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:The selected wallet can not be used';
    ELSEIF (ioAmount = 0) THEN
        SET ioAmount = LEAST(vWalletAvailableAmount, vRemainedVoucherAmount);
    ELSEIF (vWalletAvailableAmount < ioAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Not enough credit in your wallet';
    END IF;

    IF ISNULL(vRemainedVoucherAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:voucher not found';
    END IF;

    IF (ioAmount > vRemainedVoucherAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:amount is greater than voucher remaining amount';
    END IF;

    START TRANSACTION;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iUserID,
           tblVoucher.vchType = 'R', -- freeze
           tblVoucher.vchDesc = JSON_OBJECT(
               "Summary", "Freeze wallet",
               "WalletID", iWalletID
           ),
           tblVoucher.vch_rootVchID = iVoucherID,
           tblVoucher.vchTotalAmount = ioAmount,
           tblVoucher.vchStatus = 'F'
    ;

    SET vLastID = LAST_INSERT_ID();
    IF (vLastID IS NULL) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblUserWallets
       SET tblUserWallets.walFreezedAmount = tblUserWallets.walFreezedAmount + ioAmount
     WHERE tblUserWallets.walID = iWalletID
    ;

    COMMIT;
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWallet_Increase`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spWallet_Increase`(
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iForUsrID` BIGINT UNSIGNED,
    IN `iByUserID` BIGINT UNSIGNED,
    IN `iType` CHAR(1),
    IN `iAmount` INT UNSIGNED,
    IN `iDesc` VARCHAR(500),
    IN `iRootVoucherID` BIGINT UNSIGNED,
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

    IF (iRootVoucherID = 0) THEN
        SET iRootVoucherID = NULL;
    END IF;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iForUsrID,
           tblVoucher.vchType = iType,
           tblVoucher.vchTotalAmount = CAST(iAmount AS SIGNED) * vMultiplier,
           tblVoucher.vchDesc = JSON_MERGE_PATCH(
               JSON_OBJECT(
                   "walletID", iWalletID
               ),
               -- do not merge this 2 JSON_OBJECTs together. there is a bug in mysql
               IF (JSON_VALID(iDesc),
                   JSON_OBJECT("desc", CONVERT(iDesc, JSON)),
                   JSON_OBJECT("desc", iDesc)
               )
           ),
           tblVoucher.vch_rootVchID = iRootVoucherID
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

DROP PROCEDURE IF EXISTS `spWallet_unFreeze`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spWallet_unFreeze`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iWalletID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iAmount` BIGINT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vFreezedAmount BIGINT;
    DECLARE vLastID BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'WalletUnFreeze.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iWalletID", iWalletID,
                   "iVoucherID", iVoucherID,
                   "iAmount", iAmount,
                   "vFreezedAmount", vFreezedAmount
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    SELECT tblUserWallets.walFreezedAmount
      INTO vFreezedAmount
      FROM tblUserWallets
     WHERE tblUserWallets.walID = iWalletID
    ;

    IF ISNULL(vFreezedAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:wallet not found';
    END IF;

    IF (iAmount > vFreezedAmount) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:amount is greater than wallet freezed amount';
    END IF;

    START TRANSACTION;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iUserID,
           tblVoucher.vchType = 'U', -- unfreeze
           tblVoucher.vchDesc = JSON_OBJECT(
               "Summary", "unFreeze wallet",
               "WalletID", iWalletID
           ),
           tblVoucher.vch_rootVchID = iVoucherID,
           tblVoucher.vchTotalAmount = iAmount,
           tblVoucher.vchStatus = 'F'
    ;

    SET vLastID = LAST_INSERT_ID();
    IF (vLastID IS NULL) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblUserWallets
       SET tblUserWallets.walFreezedAmount = tblUserWallets.walFreezedAmount - iAmount
     WHERE tblUserWallets.walID = iWalletID
    ;

    COMMIT;
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWallet_unFreezeAndDoTransaction`
/*!*/;
DELIMITER ;;
CREATE PROCEDURE `spWallet_unFreezeAndDoTransaction`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iCheckTotalFreezed` BIGINT
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vWalletID BIGINT UNSIGNED;
    DECLARE vVoucherAmount BIGINT UNSIGNED;
    DECLARE vFinished INTEGER DEFAULT 0;

    -- declare cursor
    DEClARE curUnFreezeAndDoTransaction CURSOR FOR
            SELECT tblUserWallets.walID,
                   SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END) AS TotalFreezed
              FROM tblVoucher
        INNER JOIN tblUserWallets
                ON tblUserWallets.walID = JSON_EXTRACT(tblVoucher.vchDesc, '$.WalletID')
             WHERE tblVoucher.vchType IN ('R', 'U') -- freeze, unfreeze
               AND tblVoucher.vch_rootVchID = iVoucherID
          GROUP BY tblUserWallets.walID
            ;

    -- declare NOT FOUND handler
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET vFinished = 1;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'WalletFreeze.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iVoucherID", iVoucherID,
                   "iCheckTotalFreezed", iCheckTotalFreezed
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    START TRANSACTION;

    OPEN curUnFreezeAndDoTransaction;

    unFreezeAndDoTransaction: LOOP
        FETCH curUnFreezeAndDoTransaction
         INTO vWalletID
            , vVoucherAmount
        ;

        IF vFinished = 1 THEN
            LEAVE unFreezeAndDoTransaction;
        END IF;

        -- do action

        -- 1: unfreeze
        CALL spWallet_unFreeze(
            iUserID,
            vWalletID,
            iVoucherID,
            vVoucherAmount
        );

        -- 2: wallet Transaction
        CALL spWalletTransaction_Create(
            vWalletID,
            iVoucherID,
            vVoucherAmount
        );

        IF (vVoucherAmount > iCheckTotalFreezed) THEN
            CLOSE curUnFreezeAndDoTransaction;

            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:freezed and unfreeze amount does not match';
        END IF;

        SET iCheckTotalFreezed = iCheckTotalFreezed - CAST(vVoucherAmount AS SIGNED);

    END LOOP unFreezeAndDoTransaction;

    CLOSE curUnFreezeAndDoTransaction;

    IF (iCheckTotalFreezed <> 0) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:freezed and unfreeze amount does not match';
    END IF;

    COMMIT;
END;;
DELIMITER ;
