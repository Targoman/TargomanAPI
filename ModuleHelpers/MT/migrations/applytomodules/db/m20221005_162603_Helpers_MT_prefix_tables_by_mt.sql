/* Migration File: m20221005_162603_Helpers_MT_prefix_tables_by_mt.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

RENAME TABLE `tblCorrectionRules`           TO `tblMTCorrectionRules`;
RENAME TABLE `tblDigestedTranslationLogs`   TO `tblMTDigestedTranslationLogs`;
RENAME TABLE `tblMultiDic`                  TO `tblMTMultiDic`;
RENAME TABLE `tblTokenStats`                TO `tblMTTokenStats`;
RENAME TABLE `tblTranslationLogs`           TO `tblMTTranslationLogs`;
RENAME TABLE `tblTranslatedPhrases`         TO `tblMTTranslatedPhrases`;

DROP TRIGGER IF EXISTS `tblMultiDic_after_update`;
DELIMITER //
CREATE TRIGGER `trg_tblMTMultiDic_after_update` AFTER UPDATE ON `tblMTMultiDic` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";

  IF NEW.dicSourceLang     != OLD.dicSourceLang     THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicSourceLang", OLD.dicSourceLang)); END IF;
  IF NEW.dicWord           != OLD.dicWord           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicWord", OLD.dicWord)); END IF;
  IF NEW.dicTranslation    != OLD.dicTranslation    THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicTranslation", OLD.dicTranslation)); END IF;

  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.dicUpdatedBy_actorID,
         tblActionLogs.atlType = "tblMTMultiDic-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
