/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping database structure for NGT
DROP DATABASE IF EXISTS `NGT`;
CREATE DATABASE IF NOT EXISTS `NGT` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `NGT`;

-- Dumping structure for table NGT.tblActionLogs
DROP TABLE IF EXISTS `tblActionLogs`;
CREATE TABLE IF NOT EXISTS `tblActionLogs` (
  `atlID` int(11) NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` int(10) unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblActionLogs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActionLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActionLogs` ENABLE KEYS */;

-- Dumping structure for table NGT.tblClasses
DROP TABLE IF EXISTS `tblClasses`;
CREATE TABLE IF NOT EXISTS `tblClasses` (
  `clsID` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `clsName` varchar(50) DEFAULT NULL,
  `clsTranslationPPW` smallint(5) unsigned NOT NULL DEFAULT '60',
  `clsAuditPPW` smallint(5) unsigned NOT NULL DEFAULT '10',
  `clsCreatedBy_usrID` int(10) unsigned NOT NULL DEFAULT '1',
  `clsCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `clsUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `clsStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`clsID`),
  KEY `clsTranslationPPW` (`clsTranslationPPW`),
  KEY `clsAuditPPW` (`clsAuditPPW`),
  KEY `clsStatus` (`clsStatus`),
  KEY `clsCreatedBy_usrID` (`clsCreatedBy_usrID`),
  KEY `clsCreationDateTime` (`clsCreationDateTime`),
  KEY `clsUpdatedBy_usrID` (`clsUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblClasses: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblClasses` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblClasses` ENABLE KEYS */;

-- Dumping structure for table NGT.tblFiles
DROP TABLE IF EXISTS `tblFiles`;
CREATE TABLE IF NOT EXISTS `tblFiles` (
  `filMD5` char(32) NOT NULL,
  `filType` char(3) NOT NULL,
  `filName` varchar(200) NOT NULL,
  `filPageCount` int(10) unsigned DEFAULT NULL,
  `filTotalWordCount` int(10) unsigned DEFAULT NULL,
  `fil_clsID` smallint(5) unsigned DEFAULT NULL,
  `fil_hrdID` char(3) DEFAULT NULL,
  `filLang` char(2) DEFAULT NULL,
  `filDOI` varchar(100) DEFAULT NULL,
  `filTitle` varchar(200) DEFAULT NULL,
  `filAbstract` varchar(5000) DEFAULT NULL,
  `filKeywords` json DEFAULT NULL,
  `fil_pubID` mediumint(8) unsigned DEFAULT NULL,
  `filPublishDate` date DEFAULT NULL,
  `filAuthors` json DEFAULT NULL,
  `filCitations` smallint(6) DEFAULT NULL,
  `filStorageURL` varchar(200) NOT NULL,
  `filCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `filCreatedBy_usrID` int(10) unsigned NOT NULL,
  `filUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `filStatus` char(1) DEFAULT 'N' COMMENT 'N: New, P: Processed, R: Removed, B: Blocked',
  PRIMARY KEY (`filMD5`),
  KEY `filType` (`filType`),
  KEY `filTotalWordCount` (`filTotalWordCount`),
  KEY `filPageCount` (`filPageCount`),
  KEY `filCreationDateTime` (`filCreationDateTime`),
  KEY `filCreatedBy_usrID` (`filCreatedBy_usrID`),
  KEY `filUpdatedBy_usrID` (`filUpdatedBy_usrID`),
  KEY `filStatus` (`filStatus`),
  KEY `FK_tblFiles_tblClasses` (`fil_clsID`),
  KEY `FK_tblFiles_tblHardness` (`fil_hrdID`),
  KEY `filLang` (`filLang`),
  KEY `filDOI` (`filDOI`),
  KEY `FK_tblFiles_tblPublisher` (`fil_pubID`),
  CONSTRAINT `FK_tblFiles_tblClasses` FOREIGN KEY (`fil_clsID`) REFERENCES `tblClasses` (`clsid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblFiles_tblHardness` FOREIGN KEY (`fil_hrdID`) REFERENCES `tblHardness` (`hrdid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblFiles_tblPublisher` FOREIGN KEY (`fil_pubID`) REFERENCES `tblPublisher` (`pubid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblFiles: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblFiles` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblFiles` ENABLE KEYS */;

-- Dumping structure for table NGT.tblHardness
DROP TABLE IF EXISTS `tblHardness`;
CREATE TABLE IF NOT EXISTS `tblHardness` (
  `hrdID` char(3) NOT NULL,
  `hrdName` varchar(50) NOT NULL DEFAULT '',
  `hrdMultiplier` double NOT NULL DEFAULT '1',
  `hrdCreatedBy_usrID` int(10) unsigned NOT NULL DEFAULT '1',
  `hrdCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `hrdUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `hrdStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`hrdID`),
  KEY `hrdStatus` (`hrdStatus`),
  KEY `hrdCreatedBy_usrID` (`hrdCreatedBy_usrID`),
  KEY `hrdCreationDateTime` (`hrdCreationDateTime`),
  KEY `hrdUpdatedBy_usrID` (`hrdUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblHardness: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblHardness` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblHardness` ENABLE KEYS */;

-- Dumping structure for table NGT.tblNGTAccounting
DROP TABLE IF EXISTS `tblNGTAccounting`;
CREATE TABLE IF NOT EXISTS `tblNGTAccounting` (
  `accID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `acc_nprjID` int(10) unsigned NOT NULL,
  `accInfo` json DEFAULT NULL,
  `accCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `accCreatedBy_usrID` int(10) unsigned NOT NULL,
  PRIMARY KEY (`accID`),
  KEY `accCreationDateTime` (`accCreationDateTime`),
  KEY `accCreatedBy_usrID` (`accCreatedBy_usrID`),
  KEY `FK_tblAccounting_tblNGTProject` (`acc_nprjID`),
  CONSTRAINT `FK_tblAccounting_tblNGTProject` FOREIGN KEY (`acc_nprjID`) REFERENCES `tblNGTProject` (`nprjid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblNGTAccounting: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblNGTAccounting` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblNGTAccounting` ENABLE KEYS */;

-- Dumping structure for table NGT.tblNGTProject
DROP TABLE IF EXISTS `tblNGTProject`;
CREATE TABLE IF NOT EXISTS `tblNGTProject` (
  `nprjID` int(10) unsigned NOT NULL DEFAULT '0',
  `nprj_filMD5` char(32) NOT NULL DEFAULT '',
  `nprjAffiliate_usrID` int(10) unsigned NOT NULL,
  `nprjAffiliateInternalProjID` varchar(50) DEFAULT NULL,
  `nprjCustomerEmail` varchar(100) DEFAULT NULL,
  `nprjCustomerMobile` char(11) DEFAULT NULL,
  `nprjHurriness` char(1) NOT NULL DEFAULT 'N' COMMENT 'I: Immediate, Q: Quick, N: Normal',
  `nprjQuality` char(1) NOT NULL DEFAULT 'N' COMMENT 'E: Economy, P: Plus, G: Gold, S: SeoOptimized',
  `nprjExpressedClass` varchar(50) DEFAULT NULL,
  `nprjDestLang` char(2) NOT NULL DEFAULT 'fa',
  `nprjSelectedPages` json DEFAULT NULL,
  `nprj_olm` tinyint(3) unsigned DEFAULT NULL,
  `nprjIsPrivate` tinyint(4) NOT NULL DEFAULT '0',
  `nprj_parType` tinyint(4) unsigned DEFAULT NULL,
  `nprjMustReview` tinyint(4) DEFAULT '0',
  `nprjSameAs_nprjID` int(10) unsigned DEFAULT NULL,
  `nprjLastPostEditor_usrID` int(10) unsigned DEFAULT NULL,
  `nprjLastAuditor_usrID` int(10) unsigned DEFAULT NULL,
  `nprjLastReviewer_usrID` int(10) unsigned DEFAULT NULL,
  `nprjLastEffective_actID` bigint(20) unsigned NOT NULL,
  `nprjLastTimeEffective_actID` bigint(20) unsigned NOT NULL,
  `nprjStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`nprjID`),
  KEY `prjAffiliate_usrID` (`nprjAffiliate_usrID`),
  KEY `FK_tblProject_tblFiles` (`nprj_filMD5`),
  KEY `prjAffiliateInternalProjID` (`nprjAffiliateInternalProjID`),
  KEY `prjCustomerEmail` (`nprjCustomerEmail`),
  KEY `prjCustomerMobile` (`nprjCustomerMobile`),
  KEY `prjHurriness` (`nprjHurriness`),
  KEY `prjQuality` (`nprjQuality`),
  KEY `prjDestLang` (`nprjDestLang`),
  KEY `nprj_olm` (`nprj_olm`),
  KEY `nprjIsPrivate` (`nprjIsPrivate`),
  KEY `nprj_parType` (`nprj_parType`),
  KEY `nprjMustReview` (`nprjMustReview`),
  KEY `nprjStatus` (`nprjStatus`),
  KEY `FK_tblNGTProject_tblNGTProject` (`nprjSameAs_nprjID`),
  KEY `FK_tblNGTProject_tblProjectActions` (`nprjLastEffective_actID`),
  KEY `FK_tblNGTProject_tblProjectActions_2` (`nprjLastTimeEffective_actID`),
  CONSTRAINT `FK_tblNGTProject_tblNGTProject` FOREIGN KEY (`nprjSameAs_nprjID`) REFERENCES `tblNGTProject` (`nprjid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblOutputLMs` FOREIGN KEY (`nprj_olm`) REFERENCES `tblOutputLMs` (`olmid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblPartialTypes` FOREIGN KEY (`nprj_parType`) REFERENCES `tblPartialTypes` (`parid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblProjectActions` FOREIGN KEY (`nprjLastEffective_actID`) REFERENCES `tblProjectActions` (`actid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblProjectActions_2` FOREIGN KEY (`nprjLastTimeEffective_actID`) REFERENCES `tblProjectActions` (`actid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblProject_tblFiles` FOREIGN KEY (`nprj_filMD5`) REFERENCES `tblFiles` (`filmd5`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblNGTProject: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblNGTProject` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblNGTProject` ENABLE KEYS */;

-- Dumping structure for table NGT.tblNGTProjectExtraInfo
DROP TABLE IF EXISTS `tblNGTProjectExtraInfo`;
CREATE TABLE IF NOT EXISTS `tblNGTProjectExtraInfo` (
  `npei_nprjID` int(10) unsigned NOT NULL,
  `npeiTM` json DEFAULT NULL,
  `npeiDictionary` json DEFAULT NULL,
  PRIMARY KEY (`npei_nprjID`),
  CONSTRAINT `FK_tblNGTProjectExtraInfo_tblNGTProject` FOREIGN KEY (`npei_nprjID`) REFERENCES `tblNGTProject` (`nprjid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblNGTProjectExtraInfo: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblNGTProjectExtraInfo` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblNGTProjectExtraInfo` ENABLE KEYS */;

-- Dumping structure for table NGT.tblOutputLMs
DROP TABLE IF EXISTS `tblOutputLMs`;
CREATE TABLE IF NOT EXISTS `tblOutputLMs` (
  `olmID` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `olmName` varchar(50) NOT NULL DEFAULT '0',
  `olmMultiplier` double NOT NULL DEFAULT '1',
  `olmCreatedBy_usrID` int(10) unsigned NOT NULL DEFAULT '1',
  `olmCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `olmUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `olmStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`olmID`),
  KEY `olmStatus` (`olmStatus`),
  KEY `olmCreatedBy_usrID` (`olmCreatedBy_usrID`),
  KEY `olmCreationDateTime` (`olmCreationDateTime`),
  KEY `olmUpdatedBy_usrID` (`olmUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblOutputLMs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblOutputLMs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblOutputLMs` ENABLE KEYS */;

-- Dumping structure for table NGT.tblPartialTypes
DROP TABLE IF EXISTS `tblPartialTypes`;
CREATE TABLE IF NOT EXISTS `tblPartialTypes` (
  `parID` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `parName` varchar(50) NOT NULL DEFAULT '0',
  `parMultiplier` double NOT NULL DEFAULT '1',
  `parCreatedBy_usrID` int(10) unsigned NOT NULL DEFAULT '1',
  `parCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `parUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `parStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`parID`),
  KEY `parStatus` (`parStatus`),
  KEY `parCreatedBy_usrID` (`parCreatedBy_usrID`),
  KEY `parCreationDateTime` (`parCreationDateTime`),
  KEY `parUpdatedBy_usrID` (`parUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblPartialTypes: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblPartialTypes` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblPartialTypes` ENABLE KEYS */;

-- Dumping structure for table NGT.tblPostEditors
DROP TABLE IF EXISTS `tblPostEditors`;
CREATE TABLE IF NOT EXISTS `tblPostEditors` (
  `poe_usrID` int(10) unsigned NOT NULL,
  `poeAffiliate_usrID` bigint(20) DEFAULT NULL,
  `poeAverageSpeed` int(10) unsigned DEFAULT NULL,
  `poeAverageScore` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `poeSex` char(1) DEFAULT NULL COMMENT 'M: Male, F: Female',
  `poeCreatedBy_usrID` int(10) unsigned NOT NULL,
  `poeCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `poeUpdatedBy_usrID` int(10) unsigned NOT NULL,
  `poeStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, D:Disabled, R:Removed, P: MustUpdateProfile',
  PRIMARY KEY (`poe_usrID`),
  KEY `poeStatus` (`poeStatus`),
  KEY `poeAverageScore` (`poeAverageScore`),
  KEY `poeAverageSpeed` (`poeAverageSpeed`),
  KEY `poeAffiliate_usrID` (`poeAffiliate_usrID`),
  KEY `poeSex` (`poeSex`),
  KEY `poeCreatedBy_usrID` (`poeCreatedBy_usrID`),
  KEY `poeCreationDateTime` (`poeCreationDateTime`),
  KEY `poeUpdatedBy_usrID` (`poeUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblPostEditors: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblPostEditors` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblPostEditors` ENABLE KEYS */;

-- Dumping structure for table NGT.tblProjectActions
DROP TABLE IF EXISTS `tblProjectActions`;
CREATE TABLE IF NOT EXISTS `tblProjectActions` (
  `actID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `act_nprjID` int(10) unsigned NOT NULL DEFAULT '0',
  `actActor_usrID` int(10) unsigned NOT NULL,
  `act_pasCode` char(1) NOT NULL COMMENT 'C: NewReq, N: NewNote, P: Pending4Translators,  S: Assigned2Translator, R: Rejected, W: Waiting4Auditing, A: AcceptedByAuditor, T: TranslationReady, D: Downloaded, Q: ReviewRequired, G: SourceDownloaded, B: BigProject, V: NewPrivateNote',
  `actDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `actDescription` json DEFAULT NULL,
  PRIMARY KEY (`actID`),
  KEY `actActor_usrID` (`actActor_usrID`),
  KEY `act_pasCode` (`act_pasCode`),
  KEY `actDateTime` (`actDateTime`),
  KEY `FK_tblProjectActions_tblNGTProject` (`act_nprjID`),
  CONSTRAINT `FK_tblProjectActions_tblNGTProject` FOREIGN KEY (`act_nprjID`) REFERENCES `tblNGTProject` (`nprjid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblProjectActions_tblProjectActionStates` FOREIGN KEY (`act_pasCode`) REFERENCES `tblProjectActionStates` (`pascode`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblProjectActions: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblProjectActions` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblProjectActions` ENABLE KEYS */;

-- Dumping structure for table NGT.tblProjectActionStates
DROP TABLE IF EXISTS `tblProjectActionStates`;
CREATE TABLE IF NOT EXISTS `tblProjectActionStates` (
  `pasCode` char(1) NOT NULL,
  `pasPriority` tinyint(4) unsigned NOT NULL,
  `pasDescription` varchar(50) NOT NULL,
  `pasUIClass` varchar(50) NOT NULL DEFAULT 'pop',
  PRIMARY KEY (`pasCode`),
  KEY `pasPriority` (`pasPriority`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblProjectActionStates: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblProjectActionStates` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblProjectActionStates` ENABLE KEYS */;

-- Dumping structure for table NGT.tblPublisher
DROP TABLE IF EXISTS `tblPublisher`;
CREATE TABLE IF NOT EXISTS `tblPublisher` (
  `pubID` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `pubParentName` varchar(100) DEFAULT NULL,
  `pubName` varchar(100) NOT NULL,
  `pubVolNo` varchar(50) NOT NULL,
  `pubPicURL` int(11) DEFAULT NULL,
  `pubCreatedBy_usrID` int(10) unsigned NOT NULL DEFAULT '1',
  `pubCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `pubUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`pubID`),
  UNIQUE KEY `pubParentName_pubName_pubVolNo` (`pubParentName`,`pubName`,`pubVolNo`),
  KEY `pubCreatedBy_usrID` (`pubCreatedBy_usrID`),
  KEY `pubCreationDateTime` (`pubCreationDateTime`),
  KEY `pubUpdatedBy_usrID` (`pubUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblPublisher: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblPublisher` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblPublisher` ENABLE KEYS */;

-- Dumping structure for table NGT.tblSettings
DROP TABLE IF EXISTS `tblSettings`;
CREATE TABLE IF NOT EXISTS `tblSettings` (
  `setKey` varchar(50) NOT NULL,
  `setValue` json DEFAULT NULL,
  `setUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`setKey`),
  KEY `setUpdatedBy_usrID` (`setUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblSettings: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblSettings` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblSettings` ENABLE KEYS */;

-- Dumping structure for table NGT.tblTranslatedFile
DROP TABLE IF EXISTS `tblTranslatedFile`;
CREATE TABLE IF NOT EXISTS `tblTranslatedFile` (
  `tflID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `tfl_nprjID` int(10) unsigned NOT NULL,
  `tflTitle` varchar(200) NOT NULL DEFAULT '',
  `tflAbstract` varchar(1000) NOT NULL DEFAULT '',
  `tflKeywords` varchar(500) NOT NULL DEFAULT '',
  `tflPageCount` int(10) unsigned DEFAULT NULL,
  `tflWordCount` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`tflID`),
  KEY `FK_tblTranslatedFile_tblProject` (`tfl_nprjID`),
  CONSTRAINT `FK_tblTranslatedFile_tblProject` FOREIGN KEY (`tfl_nprjID`) REFERENCES `tblNGTProject` (`nprjID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table NGT.tblTranslatedFile: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblTranslatedFile` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblTranslatedFile` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
