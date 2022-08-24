/* Migration File: m20220802_145735_MTAPI_init.sql */

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping structure for table MT.tblActionLog
CREATE TABLE IF NOT EXISTS `tblActionLog` (
  `atlID` int NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` bigint unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping structure for table MT.tblCorrectionRules
CREATE TABLE IF NOT EXISTS `tblCorrectionRules` (
  `crlID` int unsigned NOT NULL AUTO_INCREMENT,
  `crlType` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'R' COMMENT 'R: Replace, T: Translation',
  `crlPattern` varchar(250) COLLATE utf8mb4_general_ci NOT NULL,
  `crlReplacement` varchar(500) COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`crlID`),
  UNIQUE KEY `crlPattern` (`crlPattern`),
  KEY `crlType` (`crlType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping structure for table MT.tblDigestedTranslationLogs
CREATE TABLE IF NOT EXISTS `tblDigestedTranslationLogs` (
  `dtlDateTime` datetime NOT NULL,
  `dtl_aptID` bigint unsigned NOT NULL DEFAULT '0',
  `dtl_usrID` bigint unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`dtlDateTime`,`dtl_aptID`,`dtlEngine`,`dtlDir`,`dtl_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci ROW_FORMAT=COMPACT;

-- Dumping structure for table MT.tblMultiDic
CREATE TABLE IF NOT EXISTS `tblMultiDic` (
  `dicID` int unsigned NOT NULL AUTO_INCREMENT,
  `dicSourceLang` char(2) COLLATE utf8mb4_general_ci NOT NULL,
  `dicWord` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `dicTranslation` json DEFAULT NULL,
  `dicStatus` char(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_dicVersion` int unsigned NOT NULL DEFAULT '0',
  `dicInsertedBy_usrID` bigint NOT NULL,
  `dicInsertionDate` datetime DEFAULT CURRENT_TIMESTAMP,
  `dicUpdatedBy_urID` bigint DEFAULT NULL,
  PRIMARY KEY (`dicID`),
  UNIQUE KEY `dicWord` (`dicWord`),
  KEY `dicLang` (`dicSourceLang`),
  KEY `dicInsertedBy_usrID` (`dicInsertedBy_usrID`),
  KEY `dicInsertionDate` (`dicInsertionDate`),
  KEY `dicUpdatedBy_urID` (`dicUpdatedBy_urID`),
  KEY `dicStatus` (`dicStatus`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping structure for table MT.tblTokenStats
CREATE TABLE IF NOT EXISTS `tblTokenStats` (
  `tks_tokID` bigint unsigned NOT NULL,
  `tksEngine` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `tksDir` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `tksTodayWords` int unsigned NOT NULL DEFAULT '0',
  `tksThisMonthWords` int unsigned NOT NULL DEFAULT '0',
  `tksTotalWords` bigint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`tks_tokID`,`tksEngine`,`tksDir`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping structure for table MT.tblTranslatedPhrases
CREATE TABLE IF NOT EXISTS `tblTranslatedPhrases` (
  `tps_tlsID` bigint unsigned NOT NULL,
  `tpsSourceString` longtext COLLATE utf8mb4_general_ci NOT NULL,
  `tpsTargetString` longtext COLLATE utf8mb4_general_ci,
  PRIMARY KEY (`tps_tlsID`),
  CONSTRAINT `FK__tbl_SMT_TranslationLogs` FOREIGN KEY (`tps_tlsID`) REFERENCES `tblTranslationLogs` (`tlsid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping structure for table MT.tblTranslationLogs
CREATE TABLE IF NOT EXISTS `tblTranslationLogs` (
  `tlsID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `tls_usrID` bigint unsigned NOT NULL,
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

-- Dumping structure for trigger MT.tblMultiDic_after_update
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `tblMultiDic_after_update` AFTER UPDATE ON `tblMultiDic` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";

  IF NEW.dicSourceLang     != OLD.dicSourceLang     THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicSourceLang", OLD.dicSourceLang)); END IF;
  IF NEW.dicWord           != OLD.dicWord           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicWord", OLD.dicWord)); END IF;
  IF NEW.dicTranslation    != OLD.dicTranslation    THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("dicTranslation", OLD.dicTranslation)); END IF;

  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.dicUpdatedBy_urID,
         tblActionLogs.atlType = "tblMultiDic-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
