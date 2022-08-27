/* Migration File: m20220801_140000_Helpers_MT_init.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE IF NOT EXISTS `tblCorrectionRules` (
  `crlID` int unsigned NOT NULL AUTO_INCREMENT,
  `crlType` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'R' COMMENT 'R: Replace, T: Translation',
  `crlPattern` varchar(250) COLLATE utf8mb4_general_ci NOT NULL,
  `crlReplacement` varchar(500) COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`crlID`),
  UNIQUE KEY `crlPattern` (`crlPattern`),
  KEY `crlType` (`crlType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

INSERT IGNORE INTO tblCorrectionRules(crlType, crlPattern, crlReplacement)
    VALUES('R', '/(\\S)(''(?:s|ll|d|t))/', '$1 $2')
;

CREATE TABLE IF NOT EXISTS `tblDigestedTranslationLogs` (
  `dtlDateTime` datetime NOT NULL,
  `dtl_aptID` bigint unsigned NOT NULL DEFAULT '0',
  `dtl_actorID` bigint unsigned NOT NULL DEFAULT '0',
  `dtlEngine` char(3) COLLATE utf8mb4_general_ci NOT NULL,
  `dtlDir` char(5) COLLATE utf8mb4_general_ci NOT NULL,
  `dtlTotalWordsRequested` bigint unsigned NOT NULL,
  `dtlTotalTranslatedWords` bigint unsigned NOT NULL,
  `dtlAverageTranslationTime` double NOT NULL,
  `dtlMaxSpeed` double NOT NULL,
  `dtlAverageSourceWordCount` double NOT NULL,
  `dtlMaxSourceWordCount` int NOT NULL,
  `dtlMinSourceWordCount` int NOT NULL,
  `dtlOkCount` bigint unsigned NOT NULL,
  `dtlErrLongCount` bigint unsigned NOT NULL,
  `dtlErrNoResourceCount` bigint unsigned NOT NULL,
  `dtlErrOtherCount` bigint unsigned NOT NULL,
  PRIMARY KEY (`dtlDateTime`,`dtl_aptID`,`dtl_actorID`,`dtlEngine`,`dtlDir`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci ROW_FORMAT=COMPACT;

CREATE TABLE IF NOT EXISTS `tblMultiDic` (
  `dicID` int unsigned NOT NULL AUTO_INCREMENT,
  `dicSourceLang` char(2) COLLATE utf8mb4_general_ci NOT NULL,
  `dicWord` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `dicTranslation` json DEFAULT NULL,
  `dicStatus` char(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_dicVersion` int unsigned NOT NULL DEFAULT '0',
  `dicCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `dicCreatedBy_actorID` bigint NOT NULL,
  `dicUpdatedBy_actorID` bigint DEFAULT NULL,
  PRIMARY KEY (`dicID`),
  UNIQUE KEY `dicWord` (`dicWord`),
  KEY `dicLang` (`dicSourceLang`),
  KEY `dicCreatedBy_actorID` (`dicCreatedBy_actorID`),
  KEY `dicCreationDateTime` (`dicCreationDateTime`),
  KEY `dicUpdatedBy_actorID` (`dicUpdatedBy_actorID`),
  KEY `dicStatus` (`dicStatus`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblTokenStats` (
  `tks_tokID` bigint unsigned NOT NULL,
  `tksEngine` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `tksDir` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `tksTodayWords` int unsigned NOT NULL DEFAULT '0',
  `tksThisMonthWords` int unsigned NOT NULL DEFAULT '0',
  `tksTotalWords` bigint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`tks_tokID`,`tksEngine`,`tksDir`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblTranslatedPhrases` (
  `tps_tlsID` bigint unsigned NOT NULL,
  `tpsSourceString` longtext COLLATE utf8mb4_general_ci NOT NULL,
  `tpsTargetString` longtext COLLATE utf8mb4_general_ci,
  PRIMARY KEY (`tps_tlsID`),
  CONSTRAINT `FK_tblTranslatedPhrases_tblTranslationLogs` FOREIGN KEY (`tps_tlsID`) REFERENCES `tblTranslationLogs` (`tlsid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblTranslationLogs` (
  `tlsID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `tls_actorID` bigint unsigned NOT NULL,
  `tls_aptID` int unsigned DEFAULT NULL,
  `tlsRequestTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tlsDir` char(5) COLLATE utf8mb4_general_ci NOT NULL COMMENT 'Translation like en2fa or fa2en',
  `tlsEngine` char(3) COLLATE utf8mb4_general_ci NOT NULL COMMENT 'SMT, NMT, ENT, DIC',
  `tlsClass` varchar(10) COLLATE utf8mb4_general_ci NOT NULL,
  `tlsServer` bigint DEFAULT NULL,
  `tlsSourceWordsCount` int NOT NULL,
  `tlsTranslationTime` float NOT NULL,
  `tlsErrNo` tinyint NOT NULL DEFAULT '0',
  PRIMARY KEY (`tlsID`),
  KEY `tlsTime` (`tlsRequestTime`),
  KEY `tls_tfsID` (`tlsDir`),
  KEY `tlsErrNo` (`tlsErrNo`),
  KEY `tlsTranslationTime` (`tlsTranslationTime`),
  KEY `tlsEngine` (`tlsEngine`),
  KEY `tls_aptID` (`tls_aptID`),
  KEY `tlsServer` (`tlsServer`),
  KEY `tlsClass` (`tlsClass`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

DELIMITER //
CREATE TRIGGER `tblMultiDic_after_update` AFTER UPDATE ON `tblMultiDic` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";

  IF NEW.dicSourceLang     != OLD.dicSourceLang     THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicSourceLang", OLD.dicSourceLang)); END IF;
  IF NEW.dicWord           != OLD.dicWord           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicWord", OLD.dicWord)); END IF;
  IF NEW.dicTranslation    != OLD.dicTranslation    THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicTranslation", OLD.dicTranslation)); END IF;

  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.dicUpdatedBy_actorID,
         tblActionLogs.atlType = "tblMultiDic-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
