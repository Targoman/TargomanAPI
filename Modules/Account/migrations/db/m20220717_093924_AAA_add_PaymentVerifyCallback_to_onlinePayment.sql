/* Migration File: m20220717_093924_AAA_add_PaymentVerifyCallback_to_onlinePayment.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblOnlinePayments`
    ADD COLUMN `onpCallbackUrl` VARCHAR(2048) NULL AFTER `onpAmount`
;

UPDATE tblOnlinePayments
    SET onpCallbackUrl = 'NOT-SET'
    WHERE onpCallbackUrl IS NULL
;

ALTER TABLE `tblOnlinePayments`
    CHANGE COLUMN `onpCallbackUrl` `onpCallbackUrl` VARCHAR(2048) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `onpAmount`
;

DROP PROCEDURE IF EXISTS `spOnlinePayment_Create`;
DELIMITER ;;
CREATE PROCEDURE `spOnlinePayment_Create`(
    IN `iVoucherID` BIGINT UNSIGNED,
    IN `iGatewayID` INT,
    IN `iAmount` BIGINT UNSIGNED,
    IN `iCallbackUrl` TEXT,
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
         , onpCallbackUrl = iCallbackUrl
         , onpTarget_walID = iTargetWalID
    ;

END;;
DELIMITER ;
