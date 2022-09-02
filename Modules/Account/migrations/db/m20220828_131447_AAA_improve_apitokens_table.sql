/* Migration File: m20220828_131447_AAA_improve_apitokens_table.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAPITokens`
    CHANGE COLUMN `aptToken` `aptToken` VARCHAR(750) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `aptID`;

CREATE TABLE `tblAPITokenServices` (
    `aptsvc_aptID` BIGINT(20) UNSIGNED NOT NULL,
    `aptsvc_svcID` INT(10) UNSIGNED NOT NULL,
    `aptsvcCreationDateTime` DATETIME NULL DEFAULT CURRENT_TIMESTAMP,
    `aptsvcCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`aptsvc_aptID`, `aptsvc_svcID`) USING BTREE,
    INDEX `FK_tblAPITokenServices_tblService` (`aptsvc_svcID`) USING BTREE,
    INDEX `FK_tblAPITokenServices_tblUser` (`aptsvcCreatedBy_usrID`) USING BTREE,
    CONSTRAINT `FK_tblAPITokenServices_tblAPITokens` FOREIGN KEY (`aptsvc_aptID`) REFERENCES `tblAPITokens` (`aptID`) ON UPDATE NO ACTION ON DELETE CASCADE,
    CONSTRAINT `FK_tblAPITokenServices_tblService` FOREIGN KEY (`aptsvc_svcID`) REFERENCES `tblService` (`svcID`) ON UPDATE NO ACTION ON DELETE NO ACTION,
    CONSTRAINT `FK_tblAPITokenServices_tblUser` FOREIGN KEY (`aptsvcCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE NO ACTION ON DELETE NO ACTION
) COLLATE='utf8mb4_general_ci' ENGINE=InnoDB;

INSERT IGNORE INTO tblAPITokenServices(aptsvc_aptID, aptsvc_svcID, aptsvcCreationDateTime, aptsvcCreatedBy_usrID)
    SELECT aptID, apt_svcID, aptCreationDateTime, aptCreatedBy_usrID
    FROM tblAPITokens
    WHERE apt_svcID IS NOT NULL;

ALTER TABLE `tblAPITokens`
    DROP INDEX `FK_tblAPIToken_tblRoles`,
    DROP FOREIGN KEY `FK_tblAPITokens_tblServices`;

ALTER TABLE `tblAPITokens`
    DROP COLUMN `apt_svcID`;

ALTER TABLE `tblAPITokens`
    ADD COLUMN `aptName` VARCHAR(250) NOT NULL AFTER `aptToken`;

ALTER TABLE `tblAPITokens`
    ADD UNIQUE INDEX `aptName_apt_usrID__InvalidatedAt` (`aptName`, `apt_usrID`, `_InvalidatedAt`);

DROP TRIGGER IF EXISTS `trg_tblAPIToken_after_update`;
DELIMITER ;;
CREATE TRIGGER `trg_tblAPIToken_after_update` AFTER UPDATE ON `tblAPITokens` FOR EACH ROW BEGIN
    DECLARE Changes JSON DEFAULT JSON_OBJECT("aptID", OLD.aptID);

    IF NEW.aptToken           != OLD.aptToken           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptToken", OLD.aptToken)); END IF;
    IF NEW.apt_usrID          != OLD.apt_usrID          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("apt_usrID", OLD.apt_usrID)); END IF;
    IF NEW.aptLang            != OLD.aptLang            THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptLang", OLD.aptLang)); END IF;
    IF NEW.aptValidateIP      != OLD.aptValidateIP      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptValidateIP", OLD.aptValidateIP)); END IF;
    IF NEW.aptExtraPrivileges != OLD.aptExtraPrivileges THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptExtraPrivileges", OLD.aptExtraPrivileges)); END IF;
    IF NEW.aptExpiryDate      != OLD.aptExpiryDate      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptExpiryDate", OLD.aptExpiryDate)); END IF;
    IF NEW.aptStatus          != OLD.aptStatus          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptStatus", OLD.aptStatus)); END IF;

    IF JSON_LENGTH(Changes) > 0 THEN
        INSERT INTO tblActionLogs
                SET tblActionLogs.atlBy_usrID = NEW.aptUpdatedBy_usrID,
                    tblActionLogs.atlType = "tblAPIToken",
                    tblActionLogs.atlDescription = Changes
        ;
    END IF;
END;;
DELIMITER ;

/*
ALTER TABLE `tblService`
    ADD COLUMN `svcAcceptableTokenType` CHAR(1) NOT NULL DEFAULT 'U' COMMENT 'U:User, A:API' AFTER `svcCancelVoucherItemEndPoint`;

ALTER TABLE `tblService`
    ADD COLUMN `svcOppositeTokenTypeServiceName` VARCHAR(50) NULL AFTER `svcName`;
*/
