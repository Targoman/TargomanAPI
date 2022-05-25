/* Migration File: m20220525_170624_AAA_dbdiffof_dev_AAA.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

/************************************************************\
| binlog.000078 --start-position=24048                       |
\************************************************************/

CREATE TABLE `tblCurrency` (
    `curID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `curName` VARCHAR(128) NOT NULL,
    `curSymbol` VARCHAR(32) NOT NULL,
    `curRate` FLOAT UNSIGNED NULL,
    `curStatus` CHAR(1) NOT NULL DEFAULT 'A' COMMENT 'A:Active, D:Deactive, R:Removed',
	`_InvalidatedAt` INT NULL,
    `curCreationDateTime` DATETIME NULL,
    `curCreatedBy_usrID` BIGINT UNSIGNED NULL DEFAULT NULL,
    `curUpdatedBy_usrID` BIGINT UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`curID`)
)
COLLATE='utf8mb4_general_ci'
/*!*/;

ALTER TABLE `tblCurrency`
    CHANGE COLUMN `_InvalidatedAt` `_InvalidatedAt` INT(10) UNSIGNED NULL DEFAULT 0 AFTER `curStatus`,
    CHANGE COLUMN `curCreationDateTime` `curCreationDateTime` DATETIME NULL DEFAULT CURRENT_TIMESTAMP AFTER `_InvalidatedAt`
/*!*/;

ALTER TABLE `tblCurrency`
    CHANGE COLUMN `curCreatedBy_usrID` `curCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL AFTER `curCreationDateTime`,
    CHANGE COLUMN `curUpdatedBy_usrID` `curUpdatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL AFTER `curCreatedBy_usrID`
/*!*/;

ALTER TABLE `tblCurrency`
    ADD CONSTRAINT `FK_tblCurrency_tblUser` FOREIGN KEY (`curCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE NO ACTION ON DELETE NO ACTION,
    ADD CONSTRAINT `FK_tblCurrency_tblUser_2` FOREIGN KEY (`vUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE NO ACTION ON DELETE NO ACTION
/*!*/;

ALTER TABLE `tblCurrency`
    ADD COLUMN `curIsDefault` BIT NOT NULL AFTER `curRate`
/*!*/;

ALTER TABLE `tblCurrency`
    CHANGE COLUMN `curRate` `curRate` FLOAT UNSIGNED NULL DEFAULT NULL AFTER `curSymbol`,
    CHANGE COLUMN `curIsDefault` `curIsDefault` BIT(1) NOT NULL DEFAULT 0 AFTER `curRate`,
    CHANGE COLUMN `curCreatedBy_usrID` `curCreatedBy_usrID` BIGINT(20) UNSIGNED NULL AFTER `curCreationDateTime`,
    CHANGE COLUMN `curUpdatedBy_usrID` `curUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL AFTER `curCreatedBy_usrID`
/*!*/;

DROP PROCEDURE IF EXISTS `spCurrency_SetAsDefault`
/*!*/;

DELIMITER ;;

CREATE PROCEDURE `spCurrency_SetAsDefault`(
	IN `iUserID` BIGINT UNSIGNED,
    IN `iCurID` INT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vCurIsDefault INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'spCurrency_SetAsDefault.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iUserID,
                   "iCurID", iCurID
               )
        ;

--        ROLLBACK;
        RESIGNAL;
    END;

    SELECT tblCurrency.curIsDefault
      INTO vCurIsDefault
      FROM tblCurrency
     WHERE tblCurrency.curID = iCurID
    ;

    IF ISNULL(vCurIsDefault) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Currency not found';
    END IF;

    IF (vCurIsDefault = 1) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Currency already is default';
    END IF;

    UPDATE tblCurrency
       SET tblCurrency.curIsDefault = 1
         , tblCurrency.curUpdatedBy_usrID = iUserID
     WHERE tblCurrency.curID = iCurID
    ;

END
/*!*/;;

DELIMITER ;

ALTER TABLE `tblCurrency`
    CHANGE COLUMN `curRate` `curRate` FLOAT UNSIGNED NOT NULL AFTER `curSymbol`
/*!*/;

INSERT INTO tblCurrency(`curID`, `curName`, `curSymbol`, `curRate`, `curIsDefault`) VALUES
    (1, 'ریال ایران', 'IRR', 1.0, 1),
    (2, 'تومان ایران', 'IRT', 0.1, 0)
;

ALTER TABLE `tblCurrency`
    ADD INDEX `curStatus__InvalidatedAt` (`curStatus`, `_InvalidatedAt`)
/*!*/;

ALTER TABLE `tblCurrency`
    ADD INDEX `curIsDefault` (`curIsDefault`)
/*!*/;

ALTER TABLE `tblPaymentGateways`
    ADD COLUMN `pgw_curID` INT UNSIGNED NULL DEFAULT NULL AFTER `pgwMetaInfo`
/*!*/;

ALTER TABLE `tblPaymentGateways`
    ADD CONSTRAINT `FK_tblPaymentGateways_tblCurrency` FOREIGN KEY (`pgw_curID`) REFERENCES `tblCurrency` (`curID`) ON UPDATE NO ACTION ON DELETE NO ACTION
/*!*/;

UPDATE `tblPaymentGateways`
    SET `pgw_curID` = 1
    WHERE `pgw_curID` IS NULL
;

ALTER TABLE `tblPaymentGateways`
    CHANGE COLUMN `pgw_curID` `pgw_curID` INT(10) UNSIGNED NOT NULL AFTER `pgwMetaInfo`;
/*!*/;
