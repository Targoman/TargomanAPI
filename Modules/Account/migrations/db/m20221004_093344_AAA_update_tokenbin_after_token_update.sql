/* Migration File: m20221004_093344_AAA_update_tokenbin_after_token_update.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP TRIGGER IF EXISTS `trg_tblAPIToken_after_update`;
DELIMITER //
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

    IF NEW.aptToken != OLD.aptToken THEN
        UPDATE tblTokenBin
           SET tkbTokenMD5 = MD5(NEW.aptToken)
         WHERE tkbTokenMD5 = MD5(OLD.aptToken)
        ;
    END IF;

END//
DELIMITER ;
