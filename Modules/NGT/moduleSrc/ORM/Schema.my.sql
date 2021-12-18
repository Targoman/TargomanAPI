
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `NGT` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `NGT`;
DROP TABLE IF EXISTS `tblActionLogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblActionLogs` (
  `atlID` int NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` bigint unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblClasses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblClasses` (
  `clsID` smallint unsigned NOT NULL AUTO_INCREMENT,
  `clsName` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clsTranslationPPW` smallint unsigned NOT NULL DEFAULT '60',
  `clsAuditPPW` smallint unsigned NOT NULL DEFAULT '10',
  `clsStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_clsVersion` int unsigned NOT NULL DEFAULT '0',
  `clsCreatedBy_usrID` bigint unsigned NOT NULL DEFAULT '1',
  `clsCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `clsUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`clsID`),
  KEY `clsTranslationPPW` (`clsTranslationPPW`),
  KEY `clsAuditPPW` (`clsAuditPPW`),
  KEY `clsStatus` (`clsStatus`),
  KEY `clsCreatedBy_usrID` (`clsCreatedBy_usrID`),
  KEY `clsCreationDateTime` (`clsCreationDateTime`),
  KEY `clsUpdatedBy_usrID` (`clsUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblFiles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblFiles` (
  `filMD5` char(32) COLLATE utf8mb4_general_ci NOT NULL,
  `filType` char(3) COLLATE utf8mb4_general_ci NOT NULL,
  `filName` varchar(200) COLLATE utf8mb4_general_ci NOT NULL,
  `filPageCount` int unsigned DEFAULT NULL,
  `filTotalWordCount` int unsigned DEFAULT NULL,
  `fil_clsID` smallint unsigned DEFAULT NULL,
  `fil_hrdID` char(3) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `filLang` char(2) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `filDOI` varchar(100) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `filTitle` varchar(200) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `filAbstract` varchar(5000) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `filKeywords` json DEFAULT NULL,
  `fil_pubID` mediumint unsigned DEFAULT NULL,
  `filPublishDate` date DEFAULT NULL,
  `filAuthors` json DEFAULT NULL,
  `filCitations` smallint DEFAULT NULL,
  `filStorageURL` varchar(200) COLLATE utf8mb4_general_ci NOT NULL,
  `filStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT 'N' COMMENT 'N: New, P: Processed, R: Removed, B: Blocked',
  `_filVersion` int unsigned NOT NULL DEFAULT '0',
  `filCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `filCreatedBy_usrID` bigint unsigned NOT NULL,
  `filUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
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
  CONSTRAINT `FK_tblFiles_tblClasses` FOREIGN KEY (`fil_clsID`) REFERENCES `tblClasses` (`clsID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblFiles_tblHardness` FOREIGN KEY (`fil_hrdID`) REFERENCES `tblHardness` (`hrdid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblFiles_tblPublisher` FOREIGN KEY (`fil_pubID`) REFERENCES `tblPublisher` (`pubid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblHardness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblHardness` (
  `hrdID` char(3) COLLATE utf8mb4_general_ci NOT NULL,
  `hrdName` varchar(50) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '',
  `hrdMultiplier` double NOT NULL DEFAULT '1',
  `hrdCreatedBy_usrID` bigint unsigned NOT NULL DEFAULT '1',
  `hrdCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `hrdUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `hrdStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_hrdVersion` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`hrdID`),
  KEY `hrdStatus` (`hrdStatus`),
  KEY `hrdCreatedBy_usrID` (`hrdCreatedBy_usrID`),
  KEY `hrdCreationDateTime` (`hrdCreationDateTime`),
  KEY `hrdUpdatedBy_usrID` (`hrdUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblNGTAccounting`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblNGTAccounting` (
  `accID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `acc_nprjID` int unsigned NOT NULL,
  `accInfo` json DEFAULT NULL,
  `accCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `accCreatedBy_usrID` bigint unsigned NOT NULL,
  PRIMARY KEY (`accID`),
  KEY `accCreationDateTime` (`accCreationDateTime`),
  KEY `accCreatedBy_usrID` (`accCreatedBy_usrID`),
  KEY `FK_tblAccounting_tblNGTProject` (`acc_nprjID`),
  CONSTRAINT `FK_tblAccounting_tblNGTProject` FOREIGN KEY (`acc_nprjID`) REFERENCES `tblNGTProject` (`nprjid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblNGTProject`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblNGTProject` (
  `nprjID` int unsigned NOT NULL DEFAULT '0',
  `nprj_filMD5` char(32) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '',
  `nprjAffiliate_usrID` bigint unsigned NOT NULL,
  `nprjAffiliateInternalProjID` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `nprjCustomerEmail` varchar(100) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `nprjCustomerMobile` char(11) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `nprjHurriness` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'I: Immediate, Q: Quick, N: Normal',
  `nprjQuality` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'E: Economy, P: Plus, G: Gold, S: SeoOptimized',
  `nprjExpressedClass` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `nprjDestLang` char(2) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'fa',
  `nprjSelectedPages` json DEFAULT NULL,
  `nprj_olm` tinyint unsigned DEFAULT NULL,
  `nprjIsPrivate` tinyint NOT NULL DEFAULT '0',
  `nprj_parType` tinyint unsigned DEFAULT NULL,
  `nprjMustReview` tinyint DEFAULT '0',
  `nprjSameAs_nprjID` int unsigned DEFAULT NULL,
  `nprjLastPostEditor_usrID` bigint unsigned DEFAULT NULL,
  `nprjLastAuditor_usrID` bigint unsigned DEFAULT NULL,
  `nprjLastReviewer_usrID` bigint unsigned DEFAULT NULL,
  `nprjLastEffective_actID` bigint unsigned NOT NULL,
  `nprjLastTimeEffective_actID` bigint unsigned NOT NULL,
  `nprjStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_nprjVersion` int unsigned NOT NULL DEFAULT '0',
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
  CONSTRAINT `FK_tblNGTProject_tblNGTProject` FOREIGN KEY (`nprjSameAs_nprjID`) REFERENCES `tblNGTProject` (`nprjID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblOutputLMs` FOREIGN KEY (`nprj_olm`) REFERENCES `tblOutputLMs` (`olmid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblPartialTypes` FOREIGN KEY (`nprj_parType`) REFERENCES `tblPartialTypes` (`parid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblProjectActions` FOREIGN KEY (`nprjLastEffective_actID`) REFERENCES `tblProjectActions` (`actid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblNGTProject_tblProjectActions_2` FOREIGN KEY (`nprjLastTimeEffective_actID`) REFERENCES `tblProjectActions` (`actid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblProject_tblFiles` FOREIGN KEY (`nprj_filMD5`) REFERENCES `tblFiles` (`filMD5`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblNGTProjectExtraInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblNGTProjectExtraInfo` (
  `npei_nprjID` int unsigned NOT NULL,
  `npeiTM` json DEFAULT NULL,
  `npeiDictionary` json DEFAULT NULL,
  PRIMARY KEY (`npei_nprjID`),
  CONSTRAINT `FK_tblNGTProjectExtraInfo_tblNGTProject` FOREIGN KEY (`npei_nprjID`) REFERENCES `tblNGTProject` (`nprjID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblOutputLMs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblOutputLMs` (
  `olmID` tinyint unsigned NOT NULL AUTO_INCREMENT,
  `olmName` varchar(50) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '0',
  `olmMultiplier` double NOT NULL DEFAULT '1',
  `olmCreatedBy_usrID` bigint unsigned NOT NULL DEFAULT '1',
  `olmCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `olmUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `olmStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_olmVersion` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`olmID`),
  KEY `olmStatus` (`olmStatus`),
  KEY `olmCreatedBy_usrID` (`olmCreatedBy_usrID`),
  KEY `olmCreationDateTime` (`olmCreationDateTime`),
  KEY `olmUpdatedBy_usrID` (`olmUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblPartialTypes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblPartialTypes` (
  `parID` tinyint unsigned NOT NULL AUTO_INCREMENT,
  `parName` varchar(50) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '0',
  `parMultiplier` double NOT NULL DEFAULT '1',
  `parCreatedBy_usrID` bigint unsigned NOT NULL DEFAULT '1',
  `parCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `parUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `parStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_parVersion` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`parID`),
  KEY `parStatus` (`parStatus`),
  KEY `parCreatedBy_usrID` (`parCreatedBy_usrID`),
  KEY `parCreationDateTime` (`parCreationDateTime`),
  KEY `parUpdatedBy_usrID` (`parUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblPostEditors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblPostEditors` (
  `poe_usrID` int unsigned NOT NULL,
  `poeAffiliate_usrID` bigint DEFAULT NULL,
  `poeAverageSpeed` int unsigned DEFAULT NULL,
  `poeAverageScore` tinyint unsigned NOT NULL DEFAULT '1',
  `poeSex` char(1) COLLATE utf8mb4_general_ci DEFAULT NULL COMMENT 'M: Male, F: Female',
  `poeCreatedBy_usrID` bigint unsigned NOT NULL,
  `poeCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `poeUpdatedBy_usrID` bigint unsigned NOT NULL DEFAULT '0',
  `poeStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, D:Disabled, R:Removed, P: MustUpdateProfile',
  `_poeVersion` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`poe_usrID`),
  KEY `poeStatus` (`poeStatus`),
  KEY `poeAverageScore` (`poeAverageScore`),
  KEY `poeAverageSpeed` (`poeAverageSpeed`),
  KEY `poeAffiliate_usrID` (`poeAffiliate_usrID`),
  KEY `poeSex` (`poeSex`),
  KEY `poeCreatedBy_usrID` (`poeCreatedBy_usrID`),
  KEY `poeCreationDateTime` (`poeCreationDateTime`),
  KEY `poeUpdatedBy_usrID` (`poeUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblProjectActionStates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblProjectActionStates` (
  `pasCode` char(1) COLLATE utf8mb4_general_ci NOT NULL,
  `pasPriority` tinyint unsigned NOT NULL,
  `pasDescription` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `pasUIClass` varchar(50) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'pop',
  PRIMARY KEY (`pasCode`),
  KEY `pasPriority` (`pasPriority`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblProjectActions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblProjectActions` (
  `actID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `act_nprjID` int unsigned NOT NULL DEFAULT '0',
  `actActor_usrID` bigint unsigned NOT NULL,
  `act_pasCode` char(1) COLLATE utf8mb4_general_ci NOT NULL COMMENT 'C: NewReq, N: NewNote, P: Pending4Translators,  S: Assigned2Translator, R: Rejected, W: Waiting4Auditing, A: AcceptedByAuditor, T: TranslationReady, D: Downloaded, Q: ReviewRequired, G: SourceDownloaded, B: BigProject, V: NewPrivateNote',
  `actDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `actDescription` json DEFAULT NULL,
  PRIMARY KEY (`actID`),
  KEY `actActor_usrID` (`actActor_usrID`),
  KEY `act_pasCode` (`act_pasCode`),
  KEY `actDateTime` (`actDateTime`),
  KEY `FK_tblProjectActions_tblNGTProject` (`act_nprjID`),
  CONSTRAINT `FK_tblProjectActions_tblNGTProject` FOREIGN KEY (`act_nprjID`) REFERENCES `tblNGTProject` (`nprjID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblProjectActions_tblProjectActionStates` FOREIGN KEY (`act_pasCode`) REFERENCES `tblProjectActionStates` (`pascode`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblPublisher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblPublisher` (
  `pubID` mediumint unsigned NOT NULL AUTO_INCREMENT,
  `pubParentName` varchar(100) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `pubName` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `pubVolNo` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `pubPicURL` int DEFAULT NULL,
  `pubCreatedBy_usrID` bigint unsigned NOT NULL DEFAULT '1',
  `pubCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `pubUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`pubID`),
  UNIQUE KEY `pubParentName_pubName_pubVolNo` (`pubParentName`,`pubName`,`pubVolNo`),
  KEY `pubCreatedBy_usrID` (`pubCreatedBy_usrID`),
  KEY `pubCreationDateTime` (`pubCreationDateTime`),
  KEY `pubUpdatedBy_usrID` (`pubUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblSettings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblSettings` (
  `setKey` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `setValue` json DEFAULT NULL,
  `setUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`setKey`),
  KEY `setUpdatedBy_usrID` (`setUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblTranslatedFile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblTranslatedFile` (
  `tflID` int unsigned NOT NULL AUTO_INCREMENT,
  `tfl_nprjID` int unsigned NOT NULL,
  `tflTitle` varchar(200) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '',
  `tflAbstract` varchar(1000) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '',
  `tflKeywords` varchar(500) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '',
  `tflPageCount` int unsigned DEFAULT NULL,
  `tflWordCount` int unsigned DEFAULT NULL,
  PRIMARY KEY (`tflID`),
  KEY `FK_tblTranslatedFile_tblProject` (`tfl_nprjID`),
  CONSTRAINT `FK_tblTranslatedFile_tblProject` FOREIGN KEY (`tfl_nprjID`) REFERENCES `tblNGTProject` (`nprjID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

