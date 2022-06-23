/* Migration File: m20220608_150136_AAA_dbdiffof_dev_AAA.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

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
           tblVoucher.vchDesc = JSON_MERGE_PATCH(
               JSON_OBJECT(
                   "walletID", iWalletID
               ),
               -- do not merge this 2 JSON_OBJECTs together. there is a bug in mysql
               IF (JSON_VALID(iDesc),
                   JSON_OBJECT("desc", CONVERT(iDesc, JSON)),
                   JSON_OBJECT("desc", iDesc)
               )
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
