/* Migration File: m20220507_141821_AAA_some_wallet_errors.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

DROP PROCEDURE IF EXISTS `spWallet_Transfer`;
DELIMITER ;;
CREATE PROCEDURE `spWallet_Transfer`(
    IN `iFromUserID` BIGINT UNSIGNED,
    IN `iFromWalID` INT,
    IN `iToUserLogin` VARCHAR(50),
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
