/* Migration File: m20220525_170624_AAA_dbdiffof_dev_AAA.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

/************************************************************\
| binlog.000078 --start-position=24048                       |
\************************************************************/

CREATE TABLE `tblCurrencyExchangeRate` (
	`exrID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
	`exrName` VARCHAR(128) NOT NULL,
	`exrSymbol` VARCHAR(32) NOT NULL,
	`exrRate` FLOAT UNSIGNED NULL,
	`exrStatus` CHAR(1) NOT NULL DEFAULT 'A' COMMENT 'A:Active, D:Deactive, R:Removed',
	`_InvalidatedAt` INT NULL,
	`exrCreationDateTime` DATETIME NULL,
	`exrCreatedBy_usrID` BIGINT UNSIGNED NULL DEFAULT NULL,
	`exrUpdatedBy_usrID` BIGINT UNSIGNED NULL DEFAULT NULL,
	PRIMARY KEY (`exrID`)
)
COLLATE='utf8mb4_general_ci'
/*!*/;

ALTER TABLE `tblCurrencyExchangeRate`
	CHANGE COLUMN `_InvalidatedAt` `_InvalidatedAt` INT(10) UNSIGNED NULL DEFAULT 0 AFTER `exrStatus`,
	CHANGE COLUMN `exrCreationDateTime` `exrCreationDateTime` DATETIME NULL DEFAULT CURRENT_TIMESTAMP AFTER `_InvalidatedAt`
/*!*/;

ALTER TABLE `tblCurrencyExchangeRate`
	CHANGE COLUMN `exrCreatedBy_usrID` `exrCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL AFTER `exrCreationDateTime`,
	CHANGE COLUMN `exrUpdatedBy_usrID` `exrUpdatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL AFTER `exrCreatedBy_usrID`
/*!*/;

ALTER TABLE `tblCurrencyExchangeRate`
	ADD CONSTRAINT `FK_tblCurrencyExchangeRate_tblUser` FOREIGN KEY (`exrCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE NO ACTION ON DELETE NO ACTION,
	ADD CONSTRAINT `FK_tblCurrencyExchangeRate_tblUser_2` FOREIGN KEY (`exrUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE NO ACTION ON DELETE NO ACTION
/*!*/;

ALTER TABLE `tblCurrencyExchangeRate`
	ADD COLUMN `exrIsDefault` BIT NOT NULL AFTER `exrRate`
/*!*/;

ALTER TABLE `tblCurrencyExchangeRate`
	CHANGE COLUMN `exrRate` `exrRate` FLOAT UNSIGNED NULL DEFAULT NULL AFTER `exrSymbol`,
	CHANGE COLUMN `exrIsDefault` `exrIsDefault` BIT(1) NOT NULL DEFAULT 0 AFTER `exrRate`,
	CHANGE COLUMN `exrCreatedBy_usrID` `exrCreatedBy_usrID` BIGINT(20) UNSIGNED NULL AFTER `exrCreationDateTime`,
	CHANGE COLUMN `exrUpdatedBy_usrID` `exrUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL AFTER `exrCreatedBy_usrID`
/*!*/;

DROP PROCEDURE IF EXISTS `spCurrencyExchangeRate_SetAsDefault`
/*!*/;

DELIMITER ;;

CREATE PROCEDURE `spCurrencyExchangeRate_SetAsDefault`(
	IN `iUserID` BIGINT UNSIGNED,
	IN `iExrID` INT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vExrIsDefault INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'spCurrencyExchangeRate_SetAsDefault.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iUserID,
                   "iExrID", iExrID
               )
        ;

--        ROLLBACK;
        RESIGNAL;
    END;

    SELECT tblCurrencyExchangeRate.exrIsDefault
      INTO vExrIsDefault
      FROM tblCurrencyExchangeRate
     WHERE tblCurrencyExchangeRate.exrID = iExrID
    ;

    IF ISNULL(vExrIsDefault) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:CurrencyExchangeRate not found';
    END IF;

    IF (vExrIsDefault = 1) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:CurrencyExchangeRate already is default';
    END IF;

    UPDATE tblCurrencyExchangeRate
       SET tblCurrencyExchangeRate.exrIsDefault = 1
         , tblCurrencyExchangeRate.exrUpdatedBy_usrID = iUserID
     WHERE tblCurrencyExchangeRate.exrID = iExrID
    ;

END
/*!*/;;

DELIMITER ;

ALTER TABLE `tblCurrencyExchangeRate`
	CHANGE COLUMN `exrRate` `exrRate` FLOAT UNSIGNED NOT NULL AFTER `exrSymbol`
/*!*/;
