/* Migration File: m20220526_123623_AAA_dbdiffof_dev_AAA.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

/************************************************************\
| binlog.000078 --start-position=48542                       |
\************************************************************/

DROP PROCEDURE IF EXISTS `spWallet_Transfer`
/*!*/;

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
           SET MESSAGE_TEXT = '404:Target user not found or is inactive';
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
           SET MESSAGE_TEXT = '404:Invalid UserID';
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

END
/*!*/;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spWalletTransaction_Validate`
/*!*/;

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
           SET MESSAGE_TEXT = '404:User not found';
    END IF;

    IF ISNULL(ioWalletID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '404:Wallet not found or is not yours';
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
                   SET MESSAGE_TEXT = '404:IBAN not defined';
            END IF;

            IF vLastBalance - vNotTransferable - vMinBalance - CAST(ioAmount AS SIGNED) < 0 THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '404:Not enough credit in wallet to withdraw';
            END IF;

            SET oMultiplier = -1;

        ELSE
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '500:Invalid wallet transaction type';
    END CASE;

END
/*!*/;;
DELIMITER ;
