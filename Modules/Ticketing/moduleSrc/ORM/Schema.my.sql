
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
DROP TABLE IF EXISTS `tblActionLogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblActionLogs` (
  `atlID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` bigint unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblTicketAttachments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblTicketAttachments` (
  `tatID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `tat_tktID` bigint unsigned DEFAULT NULL,
  `tat_uplID` bigint unsigned DEFAULT NULL,
  `tatCreationDateTime` datetime NOT NULL,
  `tatCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`tatID`),
  KEY `FK_tblTicketAttachments_tblTickets` (`tat_tktID`),
  KEY `FK_tblTicketAttachments_tblUploads` (`tat_uplID`),
  CONSTRAINT `FK_tblTicketAttachments_tblTickets` FOREIGN KEY (`tat_tktID`) REFERENCES `tblTickets` (`tktID`) ON DELETE CASCADE,
  CONSTRAINT `FK_tblTicketAttachments_tblUploads` FOREIGN KEY (`tat_uplID`) REFERENCES `tblUploadFiles` (`uflID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblTicketRead`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblTicketRead` (
  `tkr_tktID` bigint unsigned NOT NULL,
  `tkrBy_usrID` bigint unsigned NOT NULL,
  `tkrDateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`tkr_tktID`,`tkrBy_usrID`),
  KEY `bcrDateTime` (`tkrDateTime`),
  CONSTRAINT `FK__tblTickets` FOREIGN KEY (`tkr_tktID`) REFERENCES `tblTickets` (`tktid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblTickets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblTickets` (
  `tktID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `tktTarget_usrID` bigint unsigned DEFAULT NULL,
  `tkt_svcID` int unsigned DEFAULT NULL,
  `tktBase_tktID` bigint unsigned DEFAULT NULL,
  `tktInReply_tktID` bigint unsigned DEFAULT NULL,
  `tktType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'M' COMMENT 'B: Bug, C: Critical, S: Suggestion, M: Message, R: Reply, G: Broadcast',
  `tktScore` tinyint unsigned DEFAULT NULL,
  `tktTitle` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `tktBodyMarkdown` text CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `tktAttachments` json DEFAULT NULL,
  `tktStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: New, R: Removed, C: Closed',
  `tktCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `tktCreatedBy_usrID` bigint unsigned NOT NULL,
  `tktUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`tktID`),
  KEY `tktTime` (`tktCreationDateTime`),
  KEY `tktType` (`tktType`),
  KEY `tktStatus` (`tktStatus`),
  KEY `tktCreatedBy_usrID` (`tktCreatedBy_usrID`),
  KEY `tktUpdatedBy_usrID` (`tktUpdatedBy_usrID`),
  KEY `tktTarget_usrID` (`tktTarget_usrID`),
  KEY `tkt_svcID` (`tkt_svcID`),
  KEY `FK_tblTickets_tblTickets` (`tktInReply_tktID`),
  KEY `FK_tblTickets_tblTickets_2` (`tktBase_tktID`),
  CONSTRAINT `FK_tblTickets_tblTickets` FOREIGN KEY (`tktInReply_tktID`) REFERENCES `tblTickets` (`tktID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblTickets_tblTickets_2` FOREIGN KEY (`tktBase_tktID`) REFERENCES `tblTickets` (`tktID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblUploadFiles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblUploadFiles` (
  `uflID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `uflURL` varchar(1024) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `uflFileName` varchar(256) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `uflSize` bigint unsigned NOT NULL,
  `uflMimeType` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `uflTempFullFileName` varchar(256) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `uflUploadLastErrorMessage` varchar(1024) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `uflStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'Q' COMMENT 'Q:Queued, U:Uploaded, R:Removed',
  `uflCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `uflCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `uflUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`uflID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblUploadGateways`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblUploadGateways` (
  `ugwID` int unsigned NOT NULL AUTO_INCREMENT,
  `ugwBucket` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ugwEndpointUrl` varchar(512) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ugwSecretKey` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ugwAccessKey` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ugwMetaInfo` json DEFAULT NULL,
  `ugwAllowedFileTypes` varchar(512) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `ugwAllowedMimeTypes` varchar(1024) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `ugwAllowedMinFileSize` bigint DEFAULT NULL,
  `ugwAllowedMaxFileSize` bigint DEFAULT NULL,
  `ugwMaxFilesCount` bigint unsigned DEFAULT NULL,
  `ugwMaxFilesSize` bigint unsigned DEFAULT NULL,
  `ugwCreatedFilesCount` bigint unsigned NOT NULL DEFAULT '0',
  `ugwCreatedFilesSize` bigint unsigned NOT NULL DEFAULT '0',
  `ugwDeletedFilesCount` bigint unsigned NOT NULL DEFAULT '0',
  `ugwDeletedFilesSize` bigint unsigned NOT NULL DEFAULT '0',
  `ugwLastActionTime` datetime DEFAULT NULL,
  `ugwStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A:Active, D:Disabled, R:Removed',
  `ugwCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ugwCreatedBy_usrID` bigint unsigned NOT NULL,
  `ugwUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`ugwID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblUploadStorages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblUploadStorages` (
  `ustID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `ust_uflID` bigint unsigned NOT NULL,
  `ust_ugwID` int unsigned NOT NULL,
  `ustStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'Q',
  `ustCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ustCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `ustUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`ustID`),
  KEY `FK_tblUploadStorages_tblUploadGateways` (`ust_ugwID`),
  KEY `FK_tblUploadStorages_tblUploadFiles` (`ust_uflID`),
  CONSTRAINT `FK_tblUploadStorages_tblUploadFiles` FOREIGN KEY (`ust_uflID`) REFERENCES `tblUploadFiles` (`uflID`),
  CONSTRAINT `FK_tblUploadStorages_tblUploadGateways` FOREIGN KEY (`ust_ugwID`) REFERENCES `tblUploadGateways` (`ugwID`)
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

