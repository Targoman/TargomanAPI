
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
DROP TABLE IF EXISTS `tblAPITokenValidIPs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblAPITokenValidIPs` (
  `tviID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `tvi_aptID` bigint unsigned NOT NULL,
  `tviIP` bigint NOT NULL,
  `tviIPReadable` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci GENERATED ALWAYS AS (inet_ntoa(`tviIP`)) VIRTUAL,
  `tviStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `tviCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `tviCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `tviUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`tviID`),
  UNIQUE KEY `tvi_aptID_tviIP__InvalidatedAt` (`tvi_aptID`,`tviIP`,`_InvalidatedAt`),
  KEY `tviStatus` (`tviStatus`),
  KEY `tviIP` (`tviIP`),
  KEY `tviCreatedBy_usrID` (`tviCreatedBy_usrID`),
  KEY `tviCreationDateTime` (`tviCreationDateTime`),
  KEY `FK_tblAPITokenValidIPs_tblUser_2` (`tviUpdatedBy_usrID`),
  CONSTRAINT `FK_tblAPITokenValidIPs_tblAPIToken` FOREIGN KEY (`tvi_aptID`) REFERENCES `tblAPITokens` (`aptID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokenValidIPs_tblUser` FOREIGN KEY (`tviCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokenValidIPs_tblUser_2` FOREIGN KEY (`tviUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblAPITokenValidIPs_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblAPITokenValidIPs_after_update` AFTER UPDATE ON `tblAPITokenValidIPs` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT JSON_OBJECT("tviID", OLD.tviID);
  
  IF NEW.tvi_aptID           != OLD.tvi_aptID           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("tvi_aptID", OLD.tvi_aptID)); END IF;
  IF NEW.tviIP               != OLD.tviIP               THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("tviIP", OLD.tviIP)); END IF;
  IF NEW.tviStatus           != OLD.tviStatus           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("tviStatus", OLD.tviStatus)); END IF;
  
  IF JSON_LENGTH(Changes) > 0 THEN   
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = NEW.tviUpdatedBy_usrID,
           tblActionLogs.atlType = "tblAPITokenValidIPs",
           tblActionLogs.atlDescription = Changes;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblAPITokens`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblAPITokens` (
  `aptID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `aptToken` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `apt_usrID` bigint unsigned NOT NULL,
  `apt_svcID` int unsigned NOT NULL,
  `aptLang` char(2) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'en',
  `aptValidateIP` bit(1) NOT NULL DEFAULT b'0',
  `aptExtraPriviledges` json DEFAULT NULL,
  `aptExpiryDate` date DEFAULT NULL,
  `aptLastActivity` datetime DEFAULT NULL,
  `aptAccessCount` bigint unsigned NOT NULL DEFAULT '0',
  `aptStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, P: Pending, C: CreditFinished, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `aptCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `aptCreatedBy_usrID` bigint unsigned NOT NULL,
  `aptUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`aptID`),
  UNIQUE KEY `aptToken__InvalidatedAt` (`aptToken`,`_InvalidatedAt`),
  KEY `aptStatus` (`aptStatus`),
  KEY `aptValidateIP` (`aptValidateIP`),
  KEY `aptExpiryDate` (`aptExpiryDate`),
  KEY `aptLastActivity` (`aptLastActivity`),
  KEY `aptAccessCount` (`aptAccessCount`),
  KEY `FK_tblAPIToken_tblUser` (`apt_usrID`),
  KEY `FK_tblAPIToken_tblRoles` (`apt_svcID`),
  KEY `aptCreateDate` (`aptCreationDateTime`),
  KEY `FK_tblAPIToken_tblUser_2` (`aptCreatedBy_usrID`),
  KEY `FK_tblAPIToken_tblUser_3` (`aptUpdatedBy_usrID`),
  KEY `aptLang` (`aptLang`),
  CONSTRAINT `FK_tblAPITokens_tblServices` FOREIGN KEY (`apt_svcID`) REFERENCES `tblService` (`svcid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokens_tblUser` FOREIGN KEY (`apt_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokens_tblUser_2` FOREIGN KEY (`aptCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokens_tblUser_3` FOREIGN KEY (`aptUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblAPIToken_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblAPIToken_after_update` AFTER UPDATE ON `tblAPITokens` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT JSON_OBJECT("aptID", OLD.aptID);
  
  IF NEW.aptToken            != OLD.aptToken            THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptToken", OLD.aptToken)); END IF;
  IF NEW.apt_usrID           != OLD.apt_usrID           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("apt_usrID", OLD.apt_usrID)); END IF;
  IF NEW.apt_svcID           != OLD.apt_svcID           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("apt_svcID", OLD.apt_svcID)); END IF;
  IF NEW.aptLang             != OLD.aptLang             THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptLang", OLD.aptLang)); END IF;
  IF NEW.aptValidateIP       != OLD.aptValidateIP       THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptValidateIP", OLD.aptValidateIP)); END IF;
  IF NEW.aptExtraPriviledges != OLD.aptExtraPriviledges THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptExtraPriviledges", OLD.aptExtraPriviledges)); END IF;
  IF NEW.aptExpiryDate       != OLD.aptExpiryDate       THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptExpiryDate", OLD.aptExpiryDate)); END IF;
  IF NEW.aptStatus           != OLD.aptStatus           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptStatus", OLD.aptStatus)); END IF;
  
  IF JSON_LENGTH(Changes) > 0 THEN   
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = NEW.aptUpdatedBy_usrID,
           tblActionLogs.atlType = "tblAPIToken",
           tblActionLogs.atlDescription = Changes;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
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
  KEY `FK_tbl_AAA_ActionLog_tbl_AAA_User` (`atlBy_usrID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`),
  CONSTRAINT `FK_tblActionLogs_tblUser` FOREIGN KEY (`atlBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=68944 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblActionLogs_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblActionLogs_before_update` BEFORE UPDATE ON `tblActionLogs` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblActionLogs';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblActionLogs_before_delete */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblActionLogs_before_delete` BEFORE DELETE ON `tblActionLogs` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:DELETE is not allowed on tblActionLogs';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblActiveSessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblActiveSessions` (
  `ssnKey` char(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ssn_usrID` bigint unsigned NOT NULL,
  `ssnIP` bigint unsigned NOT NULL,
  `ssnIPReadable` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci GENERATED ALWAYS AS (inet_ntoa(`ssnIP`)) VIRTUAL,
  `ssnInfo` json DEFAULT NULL,
  `ssnFingerPrint` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `ssnLastActivity` datetime DEFAULT CURRENT_TIMESTAMP,
  `ssnRemember` bit(1) NOT NULL DEFAULT b'0',
  `ssnStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, G: LoggedOut, F: ForciblyLoggedOut, E: Expired',
  `ssnCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ssnUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`ssnKey`),
  KEY `ssnStatus` (`ssnStatus`),
  KEY `ssnCreationDateTime` (`ssnCreationDateTime`),
  KEY `FK_tbl_AAA_ActiveSessions_tbl_AAA_User` (`ssn_usrID`),
  KEY `FK_tbl_AAA_ActiveSessions_tbl_AAA_User_1` (`ssnUpdatedBy_usrID`),
  KEY `ssnLastActivity` (`ssnLastActivity`),
  KEY `ssnRemember` (`ssnRemember`),
  KEY `ssnIP` (`ssnIP`),
  KEY `ssnFingerPrint` (`ssnFingerPrint`),
  CONSTRAINT `FK_tblActiveSessions_tblUser` FOREIGN KEY (`ssn_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblActiveSessions_tblUser_2` FOREIGN KEY (`ssnUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblActiveSessions_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblActiveSessions_after_update` AFTER UPDATE ON `tblActiveSessions` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT JSON_OBJECT("ssnKey", OLD.ssnKey);
  
  IF NEW.ssnStatus           != OLD.ssnStatus           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("ssnStatus", OLD.ssnStatus)); END IF;
  
  IF JSON_LENGTH(Changes) > 0 THEN   
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = NEW.ssn_usrID,
           tblActionLogs.atlType = "tblActiveSessions",
           tblActionLogs.atlDescription = Changes;
  END IF;
         
  IF NEW.ssnStatus != 'A' THEN
    UPDATE tblUser
       SET tblUser.usrActiveSessions = tblUser.usrActiveSessions - 1
     WHERE tblUser.usrID = NEW.ssn_usrID;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblApprovalRequest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblApprovalRequest` (
  `aprID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `apr_usrID` bigint unsigned NOT NULL,
  `aprRequestedFor` char(1) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT 'E' COMMENT 'E: Email, M: Mobile',
  `aprApprovalKey` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT 'Email Address or Mobile Number',
  `aprApprovalCode` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `aprRequestDate` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `aprSentDate` datetime DEFAULT NULL,
  `aprApplyDate` datetime DEFAULT NULL,
  `aprStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2:SecondTry, E: Expired',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`aprID`),
  UNIQUE KEY `aprApprovalKey_aprApprovalCode__InvalidatedAt` (`aprApprovalCode`,`aprApprovalKey`,`_InvalidatedAt`) USING BTREE,
  KEY `aprRequestedFor` (`aprRequestedFor`),
  KEY `FK_tblApprovalRequest_tblUser` (`apr_usrID`),
  KEY `aprRequestDate` (`aprRequestDate`),
  KEY `aprApplyDate` (`aprApplyDate`),
  KEY `aprStatus` (`aprStatus`) USING BTREE,
  CONSTRAINT `FK_tblApprovalRequest_tblUser` FOREIGN KEY (`apr_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=909 DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblBlockingRules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblBlockingRules` (
  `blrID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `blr_ipbIP` bigint unsigned DEFAULT NULL,
  `blr_ipIPReadable` varchar(50) GENERATED ALWAYS AS (inet_ntoa(`blr_ipbIP`)) VIRTUAL,
  `blrStartingTime` timestamp NULL DEFAULT NULL,
  `blrEndingTime` timestamp NULL DEFAULT NULL,
  `blrCause` varchar(500) DEFAULT NULL,
  `blrStatus` char(1) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `blrCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `blrCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `blrUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`blrID`),
  KEY `blaStatus` (`blrStatus`),
  KEY `blaStartingTime` (`blrStartingTime`),
  KEY `blaEndingTime` (`blrEndingTime`),
  KEY `FK_tblBlockingsApplied_tblIPBin` (`blr_ipbIP`),
  KEY `blrInsertedBy_usrID` (`blrCreatedBy_usrID`),
  KEY `blrInsertionDate` (`blrCreationDateTime`),
  KEY `FK_tbl_AAA_BlockingRules_tblUser_1` (`blrUpdatedBy_usrID`),
  CONSTRAINT `FK_tblBlockingRules_tblUser` FOREIGN KEY (`blrCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblBlockingRules_tblUser_2` FOREIGN KEY (`blrUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tblBlockingsApplied_tblIPBin` FOREIGN KEY (`blr_ipbIP`) REFERENCES `tblIPBin` (`ipbip`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblBlockingRules_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblBlockingRules_after_update` AFTER UPDATE ON `tblBlockingRules` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT JSON_OBJECT("blrID", OLD.blrID);
  
  IF NEW.blr_ipbIP           != OLD.blr_ipbIP           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("blr_ipbIP", OLD.blr_ipbIP)); END IF;
  IF NEW.blrStartingTime     != OLD.blrStartingTime     THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("blrStartingTime", OLD.blrStartingTime)); END IF;
  IF NEW.blrEndingTime       != OLD.blrEndingTime       THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("blrEndingTime", OLD.blrEndingTime)); END IF;
  IF NEW.blrCause            != OLD.blrCause            THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("blrCause", OLD.blrCause)); END IF;
  IF NEW.blrStatus           != OLD.blrStatus       THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("blrStatus", OLD.blrStatus)); END IF;
  
  IF JSON_LENGTH(Changes) > 0 THEN   
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = NEW.blrUpdatedBy_usrID,
           tblActionLogs.atlType = "tblBlockingRules",
           tblActionLogs.atlDescription = Changes;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblForgotPassRequest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblForgotPassRequest` (
  `fprUUID` char(36) NOT NULL,
  `fpr_usrID` bigint unsigned NOT NULL,
  `fprRequestedVia` char(1) NOT NULL COMMENT 'E: Email, M: Mobile',
  `fprRequestDate` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `fprApplyDate` datetime DEFAULT NULL,
  `fprStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, S: Sent, A: Applied, R: Removed',
  PRIMARY KEY (`fprUUID`),
  KEY `fprStatus` (`fprStatus`),
  KEY `FK__tbl_AAA_User` (`fpr_usrID`),
  KEY `fprRequestDate` (`fprRequestDate`),
  KEY `fprApplyDate` (`fprApplyDate`),
  KEY `fprRequestedVia` (`fprRequestedVia`),
  CONSTRAINT `FK_tblForgotPassRequest_tblUser` FOREIGN KEY (`fpr_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblIPBin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblIPBin` (
  `ipbIP` bigint unsigned NOT NULL,
  `ipbReadable` varchar(50) GENERATED ALWAYS AS (inet_ntoa(`ipbIP`)) VIRTUAL,
  `ipbFirstAccess` datetime DEFAULT CURRENT_TIMESTAMP,
  `ipbAccessCount` bigint unsigned NOT NULL DEFAULT '0',
  `ipbLastAccess` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `ipbBlockedBy_usrID` bigint unsigned DEFAULT NULL,
  `ipbBlockingTime` datetime DEFAULT NULL,
  `ipbStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, B: Blocked, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `ipbIP__InvalidatedAt` (`ipbIP`,`_InvalidatedAt`),
  KEY `ipbAccesCount` (`ipbAccessCount`),
  KEY `ipbLastAccess` (`ipbLastAccess`),
  KEY `FK_tbl_AAA_IPBin_tbl_AAA_User` (`ipbBlockedBy_usrID`),
  KEY `ipbInsertionDateTime` (`ipbFirstAccess`),
  KEY `ipbStatus` (`ipbStatus`),
  KEY `ipbBlockingTime` (`ipbBlockingTime`),
  CONSTRAINT `FK_tblIPBin_tblUser` FOREIGN KEY (`ipbBlockedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblIPBin_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblIPBin_before_update` BEFORE UPDATE ON `tblIPBin` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblIPBin';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblIPStats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblIPStats` (
  `ips_ipbIP` bigint unsigned NOT NULL,
  `ipsTimeStamp` double unsigned NOT NULL,
  `ipsInsertionDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  UNIQUE KEY `ips_ipbID_ipsInsertionDate` (`ips_ipbIP`,`ipsTimeStamp`),
  KEY `ipsTimeStamp` (`ipsTimeStamp`),
  CONSTRAINT `FK_tblIPStats_tblIPBin` FOREIGN KEY (`ips_ipbIP`) REFERENCES `tblIPBin` (`ipbIP`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblIPStats_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblIPStats_before_update` BEFORE UPDATE ON `tblIPStats` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblIPStats';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblOfflinePayments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblOfflinePayments` (
  `ofpID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `ofp_vchID` bigint unsigned NOT NULL,
  `ofpBank` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ofpReceiptCode` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ofpReceiptDate` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `ofpAmount` int unsigned NOT NULL,
  `ofpNotes` varchar(500) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `ofpStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: New, A: Succeded, E: Error, R: Removed',
  `ofpCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ofpCreatedBy_usrID` bigint unsigned NOT NULL,
  `ofpUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`ofpID`),
  KEY `ofpBank` (`ofpBank`),
  KEY `ofpReceiptCode` (`ofpReceiptCode`),
  KEY `ofpCreationDateTime` (`ofpCreationDateTime`),
  KEY `ofpStatus` (`ofpStatus`),
  KEY `ofpReceiptDate` (`ofpReceiptDate`),
  KEY `FK_tblOfflinePayments_tblUser` (`ofpCreatedBy_usrID`),
  KEY `FK_tblOfflinePayments_tblUser_2` (`ofpUpdatedBy_usrID`),
  KEY `FK_tblOfflinePayments_tblInvoice` (`ofp_vchID`),
  FULLTEXT KEY `ofpNotes` (`ofpNotes`),
  CONSTRAINT `FK_tblOfflinePayments_tblInvoice` FOREIGN KEY (`ofp_vchID`) REFERENCES `tblVoucher` (`vchID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblOfflinePayments_tblUser` FOREIGN KEY (`ofpCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblOfflinePayments_tblUser_2` FOREIGN KEY (`ofpUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblOfflinePayments_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblOfflinePayments_before_update` BEFORE UPDATE ON `tblOfflinePayments` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblOfflinePayments';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblOnlinePayments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblOnlinePayments` (
  `onpID` bigint NOT NULL AUTO_INCREMENT,
  `onpMD5` char(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `onp_vchID` bigint unsigned NOT NULL,
  `onp_pgwID` int unsigned NOT NULL,
  `onpPGTrnID` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `onpAmount` bigint NOT NULL,
  `onpResult` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `onpStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'P' COMMENT 'N: New, P: Pending, Y: Payed, A: Succeded, E: Error, R: Removed',
  `onpCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `onpLastUpdateDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`onpID`),
  UNIQUE KEY `onpMD5` (`onpMD5`),
  KEY `onpRequestBy` (`onp_vchID`),
  KEY `onpBankTrnID` (`onpPGTrnID`),
  KEY `onpStatus` (`onpStatus`),
  KEY `onpCreationDateTime` (`onpCreationDateTime`),
  KEY `onpAmount` (`onpAmount`),
  KEY `onpLastUpdateDateTime` (`onpLastUpdateDateTime`),
  KEY `FK_tblOnlinePayments_tblPaymentGateways` (`onp_pgwID`),
  CONSTRAINT `FK_tblBankPaymentOrder_tblInvoice` FOREIGN KEY (`onp_vchID`) REFERENCES `tblVoucher` (`vchID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblOnlinePayments_tblPaymentGateways` FOREIGN KEY (`onp_pgwID`) REFERENCES `tblPaymentGateways` (`pgwID`)
) ENGINE=InnoDB AUTO_INCREMENT=131 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblOnlinePayments_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblOnlinePayments_before_update` BEFORE UPDATE ON `tblOnlinePayments` FOR EACH ROW BEGIN
--	SIGNAL SQLSTATE '45000'
--    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblOnlinePayments';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblPaymentGateways`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblPaymentGateways` (
  `pgwID` int unsigned NOT NULL AUTO_INCREMENT,
  `pgwName` varchar(64) COLLATE utf8mb4_general_ci NOT NULL,
  `pgwType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `pgwDriver` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `pgwMetaInfo` json DEFAULT NULL,
  `pgwAllowedDomainName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `pgwTransactionFeeValue` int unsigned DEFAULT NULL,
  `pgwTransactionFeeType` char(50) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '$',
  `pgwMinRequestAmount` int unsigned NOT NULL,
  `pgwMaxRequestAmount` int unsigned DEFAULT NULL,
  `pgwMaxPerDayAmount` int unsigned DEFAULT NULL,
  `pgwLastPaymentDateTime` datetime DEFAULT NULL,
  `pgwSumTodayPaidAmount` bigint unsigned NOT NULL DEFAULT '0',
  `pgwSumRequestCount` int unsigned NOT NULL DEFAULT '0',
  `pgwSumRequestAmount` int unsigned NOT NULL DEFAULT '0',
  `pgwSumFailedCount` int unsigned NOT NULL DEFAULT '0',
  `pgwSumOkCount` int unsigned NOT NULL DEFAULT '0',
  `pgwSumPaidAmount` int unsigned NOT NULL DEFAULT '0',
  `pgwStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A',
  `pgwCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `pgwCreatedBy_usrID` bigint unsigned NOT NULL DEFAULT '0',
  `pgwUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`pgwID`)
) ENGINE=InnoDB AUTO_INCREMENT=101 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblRoles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblRoles` (
  `rolID` int unsigned NOT NULL AUTO_INCREMENT,
  `rolName` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `rolParent_rolID` int unsigned DEFAULT NULL,
  `rolPrivileges` json DEFAULT NULL,
  `rolSignupAllowedIPs` varchar(500) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `rolStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, B: Blocked, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `rolCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `rolCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `rolUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`rolID`),
  UNIQUE KEY `rolName__InvalidatedAt` (`rolName`,`_InvalidatedAt`),
  KEY `rolCreationDateTime` (`rolCreationDateTime`),
  KEY `rolStatus` (`rolStatus`),
  KEY `FK_tblRoles_tblRoles` (`rolParent_rolID`),
  KEY `FK_tblRoles_tblUser` (`rolCreatedBy_usrID`),
  KEY `FK_tblRoles_tblUser_2` (`rolUpdatedBy_usrID`),
  CONSTRAINT `FK_tblRoles_tblRoles` FOREIGN KEY (`rolParent_rolID`) REFERENCES `tblRoles` (`rolID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblRoles_tblUser` FOREIGN KEY (`rolCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblRoles_tblUser_2` FOREIGN KEY (`rolUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1105 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblService`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblService` (
  `svcID` int unsigned NOT NULL AUTO_INCREMENT,
  `svcName` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `svc_rolID` int unsigned NOT NULL,
  `svcProcessVoucherItemEndPoint` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `svcCancelVoucherItemEndPoint` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `svcStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R:Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `svcCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `svcCreatedBy_usrID` bigint unsigned NOT NULL,
  `svcUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`svcID`),
  UNIQUE KEY `svcName__InvalidatedAt` (`svcName`,`_InvalidatedAt`),
  KEY `svcCreationDateTime` (`svcCreationDateTime`),
  KEY `svcStatus` (`svcStatus`),
  KEY `FK_tblServices_tblUser` (`svcCreatedBy_usrID`),
  KEY `FK_tblServices_tblUser_2` (`svcUpdatedBy_usrID`),
  KEY `FK_tblServices_tblRoles` (`svc_rolID`),
  CONSTRAINT `FK_tblService_tblUser` FOREIGN KEY (`svcCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblService_tblUser_2` FOREIGN KEY (`svcUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblServices_tblRoles` FOREIGN KEY (`svc_rolID`) REFERENCES `tblRoles` (`rolID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblUser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblUser` (
  `usrID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `usrEmail` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `usrName` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `usrFamily` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `usrGender` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'F: Female, M:Male, N: Not expressed',
  `usrMobile` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `usrApprovalState` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: Not Approved, M: JustMobile, E:JustEmail, A: All',
  `usrPass` char(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'NOT_SET',
  `usr_rolID` int unsigned NOT NULL,
  `usrSpecialPrivs` json DEFAULT NULL,
  `usrLanguage` char(2) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'fa',
  `usrMaxSessions` tinyint NOT NULL DEFAULT '-1',
  `usrActiveSessions` mediumint NOT NULL DEFAULT '0',
  `usrLastLogin` datetime DEFAULT NULL,
  `usrStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'V' COMMENT 'A: Active, R: Removed, B: Banned, C: Must Change Pass, V:Must validate',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `usrCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `usrCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `usrUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`usrID`),
  UNIQUE KEY `usrEmail__InvalidatedAt` (`usrEmail`,`_InvalidatedAt`),
  UNIQUE KEY `usrMobile__InvalidatedAt` (`usrMobile`,`_InvalidatedAt`),
  KEY `usrLastLogin` (`usrLastLogin`),
  KEY `usrStatus` (`usrStatus`),
  KEY `usrFullName` (`usrName`),
  KEY `usrFamilyName` (`usrFamily`),
  KEY `usrActiveSessions` (`usrActiveSessions`),
  KEY `usrCreationDateTime` (`usrCreationDateTime`),
  KEY `FK_tbl_AAA_User_tbl_AAA_User` (`usrCreatedBy_usrID`),
  KEY `FK_tbl_AAA_User_tbl_AAA_User_1` (`usrUpdatedBy_usrID`),
  KEY `usrMaxConcurrentSessions` (`usrMaxSessions`),
  KEY `usrApprovalState` (`usrApprovalState`),
  KEY `usrLanguage` (`usrLanguage`),
  KEY `usrSex` (`usrGender`),
  KEY `FK_tblUser_tblRoles` (`usr_rolID`),
  CONSTRAINT `FK_tblUser_tblRoles` FOREIGN KEY (`usr_rolID`) REFERENCES `tblRoles` (`rolID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUser_tblUser` FOREIGN KEY (`usrCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE,
  CONSTRAINT `FK_tblUser_tblUser_2` FOREIGN KEY (`usrUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2172 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblUser_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblUser_after_update` AFTER UPDATE ON `tblUser` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT JSON_OBJECT("usrID", OLD.usrID);
  
  IF NEW.usrName          != OLD.usrName          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrName", OLD.usrName)); END IF;
  IF NEW.usrFamily        != OLD.usrFamily        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrFamily", OLD.usrFamily)); END IF;
  IF NEW.usrEmail         != OLD.usrEmail         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrEmail", OLD.usrEmail)); END IF;
  IF NEW.usrMobile        != OLD.usrMobile        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrMobile", OLD.usrMobile)); END IF;
  IF NEW.usrApprovalState != OLD.usrApprovalState THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrApprovalState", OLD.usrApprovalState)); END IF;
  IF NEW.usrPass          != OLD.usrPass          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrPass", OLD.usrPass)); END IF;
  IF NEW.usr_rolID        != OLD.usr_rolID        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usr_rolID", OLD.usr_rolID)); END IF;
  IF NEW.usrSpecialPrivs  != OLD.usrSpecialPrivs  THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrSpecialPrivs", OLD.usrSpecialPrivs)); END IF;
  IF NEW.usrLanguage      != OLD.usrLanguage      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrLanguage", OLD.usrLanguage)); END IF;
  IF NEW.usrMaxSessions   != OLD.usrMaxSessions   THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrMaxSessions", OLD.usrMaxSessions)); END IF;
  IF NEW.usrStatus        != OLD.usrStatus        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("usrStatus", OLD.usrStatus)); END IF;
  
  IF JSON_LENGTH(Changes) > 0 THEN   
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = NEW.usrUpdatedBy_usrID,
           tblActionLogs.atlType = "tblUser",
           tblActionLogs.atlDescription = Changes;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblUserExtraInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblUserExtraInfo` (
  `uei_usrID` bigint unsigned NOT NULL,
  `ueiBirthDate` date DEFAULT NULL,
  `ueiPhoto` longtext CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci,
  `ueiOAuthAccounts` json DEFAULT NULL,
  `ueiIBAN` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `ueiEther` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `ueiExtraInfo` json DEFAULT NULL,
  `ueiUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`uei_usrID`),
  KEY `FK_tbl_AAA_UserExtraInfo_tbl_AAA_User_1` (`ueiUpdatedBy_usrID`),
  KEY `ueiSheba` (`ueiIBAN`) USING BTREE,
  CONSTRAINT `FK_tblUserExtraInfo_tblUser` FOREIGN KEY (`uei_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUserExtraInfo_tblUser_2` FOREIGN KEY (`ueiUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblUserExtraInfo_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblUserExtraInfo_after_update` AFTER UPDATE ON `tblUserExtraInfo` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT JSON_OBJECT("uei_usrID", OLD.uei_usrID);
  
  IF NEW.ueiExtraInfo     != OLD.ueiExtraInfo     THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("ueiExtraInfo", OLD.ueiExtraInfo)); END IF;
  IF NEW.ueiPhoto         != OLD.ueiPhoto         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("ueiPhoto", OLD.ueiPhoto)); END IF;
  IF NEW.ueiOAuthAccounts != OLD.ueiOAuthAccounts THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("ueiOAuthAccounts", OLD.ueiOAuthAccounts)); END IF;
  
  IF JSON_LENGTH(Changes) > 0 THEN   
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = NEW.ueiUpdatedBy_usrID,
           tblActionLogs.atlType = "tblUserExtraInfo",
           tblActionLogs.atlDescription = Changes;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblUserWallets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblUserWallets` (
  `walID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `wal_usrID` bigint unsigned NOT NULL,
  `walName` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'default',
  `walDefault` bit(1) NOT NULL DEFAULT b'0',
  `walMinBalance` bigint unsigned NOT NULL DEFAULT '0',
  `walNotTransferableAmount` bigint unsigned NOT NULL DEFAULT '0',
  `walMaxTransferPerDay` bigint unsigned NOT NULL DEFAULT '10000000',
  `walLastBalance` bigint NOT NULL DEFAULT '0',
  `walSumIncome` bigint unsigned NOT NULL DEFAULT '0',
  `walSumExpenses` bigint unsigned NOT NULL DEFAULT '0',
  `walSumCredit` bigint unsigned NOT NULL DEFAULT '0',
  `walSumDebit` bigint unsigned NOT NULL DEFAULT '0',
  `walStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, D: Deactive, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `walCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `walCreatedBy_usrID` bigint unsigned NOT NULL,
  `walUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`walID`),
  UNIQUE KEY `wal_usrID_walName__InvalidatedAt` (`wal_usrID`,`walName`,`_InvalidatedAt`),
  KEY `walStatus` (`walStatus`),
  KEY `walCreationDateTime` (`walCreationDateTime`),
  KEY `FK_tblUserWallets_tblUser` (`walCreatedBy_usrID`),
  KEY `FK_tblUserWallets_tblUser_2` (`walUpdatedBy_usrID`),
  CONSTRAINT `FK_tblUserWallets_tblUser` FOREIGN KEY (`wal_usrID`) REFERENCES `tblUser` (`usrID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUserWallets_tblUser_2` FOREIGN KEY (`walCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUserWallets_tblUser_3` FOREIGN KEY (`walUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1900 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblVoucher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblVoucher` (
  `vchID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `vch_usrID` bigint unsigned NOT NULL,
  `vchDesc` json NOT NULL,
  `vchType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT 'W:Withdrawal, E: Expense, I: Income, C:Credit, T:TransferTo, F: TransferFrom, Z: Prize',
  `vchTotalAmount` bigint NOT NULL,
  `vchStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: New, C: Canceled, E: Error, F: Finished, R: Removed',
  `vchCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`vchID`),
  KEY `vchStatus` (`vchStatus`),
  KEY `vchPaymentType` (`vchType`),
  KEY `vchCreationDateTime` (`vchCreationDateTime`),
  KEY `FK_tblVoucher_tblUser` (`vch_usrID`),
  CONSTRAINT `FK_tblVoucher_tblUser` FOREIGN KEY (`vch_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=395 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblVoucher_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblVoucher_before_update` BEFORE UPDATE ON `tblVoucher` FOR EACH ROW BEGIN
	IF OLD.vchCreationDateTime != NEW.vchCreationDateTime
     OR OLD.vch_usrID != NEW.vch_usrID
     OR OLD.vchType != NEW.vchType
     OR OLD.vchTotalAmount != NEW.vchTotalAmount
  THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:tblVoucher update is not allowed except for status';
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblWalletBalances`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblWalletBalances` (
  `wbl_wltID` bigint unsigned NOT NULL,
  `wblBalance` bigint NOT NULL,
  `wblSumIncome` bigint unsigned NOT NULL DEFAULT '0',
  `wblSumExpense` bigint unsigned NOT NULL DEFAULT '0',
  `wblSumCredit` bigint unsigned NOT NULL DEFAULT '0',
  `wblSumDebit` bigint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`wbl_wltID`),
  CONSTRAINT `FK_tblWalletBalances_tblWalletsTransactions` FOREIGN KEY (`wbl_wltID`) REFERENCES `tblWalletsTransactions` (`wltid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_persian_ci ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblWalletsTransactions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblWalletsTransactions` (
  `wltID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `wlt_walID` bigint unsigned NOT NULL,
  `wlt_vchID` bigint unsigned NOT NULL,
  `wlt_vchType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT 'W:Withdrawal, E: Expense, I: Income, C:Credit, T:TransferTo, F: TransferFrom, Z: Prize',
  `wltAmount` bigint NOT NULL,
  `wltStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: New, P: Pending, Y: Payed, A: Succeded, E: Error, R: Removed',
  `wltDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`wltID`),
  KEY `walStatus` (`wltStatus`),
  KEY `walCreationDateTime` (`wltDateTime`),
  KEY `FK_tblWalletsTransactions_tblUserWallets` (`wlt_walID`),
  KEY `FK_tblWalletsTransactions_tblInvoice` (`wlt_vchID`),
  KEY `wltType` (`wlt_vchType`),
  CONSTRAINT `FK_tblWalletsTransactions_tblInvoice` FOREIGN KEY (`wlt_vchID`) REFERENCES `tblVoucher` (`vchID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblWalletsTransactions_tblUserWallets` FOREIGN KEY (`wlt_walID`) REFERENCES `tblUserWallets` (`walID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=167 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci ROW_FORMAT=COMPRESSED;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblWalletsTransactions_after_insert */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblWalletsTransactions_after_insert` AFTER INSERT ON `tblWalletsTransactions` FOR EACH ROW Proc: BEGIN
    DECLARE LastTransactionDate DATETIME;
    DECLARE LastBalance BIGINT;
    DECLARE LastIncome BIGINT;
    DECLARE LastExpense BIGINT;
    DECLARE MultipleInsert BOOL;

    IF NEW.wltStatus != 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Transaction status must be New on insertion';
    END IF;

    CALL sp_SYSTEM_prepareBalanceInfo(
          NEW.wlt_walID
        , NEW.wltID
        , NEW.wltDateTime
        , LastTransactionDate
        , LastBalance
        , LastIncome
        , LastExpense
        , MultipleInsert
    );

    IF ISNULL(MultipleInsert) THEN
        INSERT INTO tblWalletBalances (
                    wbl_wltID
                  , wblBalance
                  , wblSumIncome
                  , wblSumExpense
                    )
             VALUES (
                    NEW.wltID
                  , IFNULL(LastBalance, 0)
                  , IFNULL(LastIncome, 0)
                  , IFNULL(LastExpense, 0)
                    )
        ;
    ELSE
        SET @LastBalance = IFNULL(LastBalance, 0);
        INSERT INTO tblWalletBalances (
                    wbl_wltID
                  , wblBalance
                    )
             SELECT tblWalletsTransactions.wltID
                  , (@LastBalance := @LastBalance + 
                    IF (tblWalletsTransactions.wltID = NEW.wltID
                        , 0 
                        , IF(tblWalletsTransactions.wltStatus = 'P'
                            , tblWalletsTransactions.wltAmount
                            , 0
                          )
                    )
                    )
               FROM tblWalletsTransactions
               JOIN (SELECT @LastBalance := IFNULL(LastBalance, 0)) r
              WHERE tblWalletsTransactions.wltDateTime > LastTransactionDate 
           ORDER BY tblWalletsTransactions.wltDateTime ASC
                  , tblWalletsTransactions.wltID ASC
        ;
        SET LastBalance = @LastBalance;
    END IF;

    UPDATE tblUserWallets
       SET tblUserWallets.walLastBalance = IFNULL(LastBalance, 0) - IF(NEW.wltAmount < 0, NEW.wltAmount, 0)
     WHERE tblUserWallets.walID = NEW.wlt_walID;
  /**/
  -- TODO Update Last Income and LastExpenses
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblWalletsTransactions_before_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblWalletsTransactions_before_update` BEFORE UPDATE ON `tblWalletsTransactions` FOR EACH ROW BEGIN
	IF OLD.wlt_walID != NEW.wlt_walID
     OR OLD.wlt_vchID != NEW.wlt_vchID
     OR OLD.wlt_vchType != NEW.wlt_vchType
     OR OLD.wltAmount != NEW.wltAmount
  THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:tblWallet update is not allowed except for status';
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_tblWalletsTransactions_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `trg_tblWalletsTransactions_after_update` AFTER UPDATE ON `tblWalletsTransactions` FOR EACH ROW Proc: BEGIN
    DECLARE LastTransactionDate DATETIME;
    DECLARE LastBalance BIGINT;
    DECLARE MultipleInsert BOOL;

    IF NEW.wltStatus = 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Wallet status can not be reset to new';
    END IF;
/*
    CALL sp_SYSTEM_deleteOldBalances(
          NEW.wlt_walID
        , NEW.wltID
        , NEW.wltDateTime
        , LastTransactionDate
        , LastBalance
        , LastIncome
        , LastExpense
        , MultipleInsert
    );
*/
    IF (IFNULL(LastBalance,0) + NEW.wltAmount < 0) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Wallet balance can not be negative';
    END IF;

    IF ISNULL(MultipleInsert) THEN
        UPDATE tblWalletBalances 
           SET tblWalletBalances.wblBalance = IFNULL(LastBalance,0) + IF(NEW.wltStatus IN ('Y', 'A'), NEW.wltAmount, 0)
         WHERE tblWalletBalances.wbl_wltID  = NEW.wltID
        ;
        
        SET LastBalance = IFNULL(LastBalance,0) + IF(NEW.wltStatus IN ('Y', 'A'), NEW.wltAmount, 0);
    ELSE
        SET @LastBalance = IFNULL(LastBalance,0);
        DELETE
          FROM tblWalletBalances
         WHERE tblWalletBalances.wbl_wltID = NEW.wltID
        ;
        INSERT INTO tblWalletBalances (
                    wbl_wltID
                  , wblBalance
                    )
             SELECT tblWalletsTransactions.wltID
                  , (@LastBalance := @LastBalance + 
                    IF(tblWalletsTransactions.wltStatus IN ('Y', 'A'), tblWalletsTransactions.wltAmount, 0 )
                    )
               FROM tblWalletsTransactions
               JOIN (SELECT @LastBalance := IFNULL(LastBalance,0)) r
              WHERE tblWalletsTransactions.wltDateTime > LastTransactionDate 
           ORDER BY tblWalletsTransactions.wltDateTime ASC
                  , tblWalletsTransactions.wltID ASC
        ;  
        SET LastBalance = @LastBalance;
    END IF;
    
    UPDATE tblUserWallets
       SET tblUserWallets.walLastBalance = IFNULL(LastBalance,0)
     WHERE tblUserWallets.walID = NEW.wlt_walID;
  /**/
  -- TODO Update Last Income and LastExpenses
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50106 SET @save_time_zone= @@TIME_ZONE */ ;
/*!50106 DROP EVENT IF EXISTS `evExpireSession` */;
DELIMITER ;;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;;
/*!50003 SET character_set_client  = utf8mb4 */ ;;
/*!50003 SET character_set_results = utf8mb4 */ ;;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;;
/*!50003 SET @saved_time_zone      = @@time_zone */ ;;
/*!50003 SET time_zone             = 'SYSTEM' */ ;;
/*!50106 CREATE*/ /*!50117 DEFINER=`root`@`%`*/ /*!50106 EVENT `evExpireSession` ON SCHEDULE EVERY 15 MINUTE STARTS '2020-04-14 15:18:12' ON COMPLETION NOT PRESERVE ENABLE DO BEGIN

END */ ;;
/*!50003 SET time_zone             = @saved_time_zone */ ;;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;;
/*!50003 SET character_set_client  = @saved_cs_client */ ;;
/*!50003 SET character_set_results = @saved_cs_results */ ;;
/*!50003 SET collation_connection  = @saved_col_connection */ ;;
DELIMITER ;
/*!50106 SET TIME_ZONE= @save_time_zone */ ;
/*!50003 DROP FUNCTION IF EXISTS `fnGetAllPrivs` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `fnGetAllPrivs`(
	`iRolID` INT UNSIGNED,
	`iSpecificPrivs` VARCHAR(5000)



) RETURNS varchar(5000) CHARSET utf8mb4
    READS SQL DATA
BEGIN
	DECLARE Privs VARCHAR(5000);
  SELECT CONCAT_WS('','[',
            (SELECT GROUP_CONCAT(fnSanitizePrivs(T1.rolPrivileges) ORDER BY T1.rolID ASC)
               FROM (WITH RECURSIVE CTE (rolID, rolPrivileges, rolParent_rolID) AS (
                SELECT   tblRoles.rolID,
                         tblRoles.rolPrivileges,
                         tblRoles.rolParent_rolID
                  FROM   tblRoles
                 WHERE   tblRoles.rolID = iRolID
                 UNION ALL
                SELECT   Parent.rolID,
                         Parent.rolPrivileges,
                         Parent.rolParent_rolID
                  FROM   tblRoles Parent
                  INNER JOIN CTE
                     ON Parent.rolID = CTE.rolParent_rolID 
              )
              SELECT * FROM CTE) T1
            ),
            IF(ISNULL(iSpecificPrivs),'',CONCAT(',',fnSanitizePrivs(iSpecificPrivs))),
        ']')INTO Privs;
            
	RETURN Privs;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `fnPasswordsAreEqual` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `fnPasswordsAreEqual`(
	`iSaltedPass` VARCHAR(50),
	`iSalt` VARCHAR(50),
	`iHashedPass` VARCHAR(50)

) RETURNS tinyint(1)
    NO SQL
    DETERMINISTIC
BEGIN
  RETURN MD5(CONCAT(iSalt, LOWER(iHashedPass))) = LOWER(iSaltedPass);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `fnSanitizePrivs` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `fnSanitizePrivs`(
	`iPrivs` VARCHAR(5000)
) RETURNS varchar(5000) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
  RETURN REPLACE(REPLACE(iPrivs, '\n', ''),'\r','');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `fnSanitizeToken` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `fnSanitizeToken`(
	`iToken` VARCHAR(50)
) RETURNS varchar(100) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
  RETURN LOWER(REPLACE(REPLACE(REPLACE(REPLACE(iToken, '-',''), ' ', ''), '_', ''),',',''));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_approvalRequest` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_approvalRequest`(
	IN `iBy` CHAR(1),
	IN `iUserID` BIGINT UNSIGNED,
	IN `iKey` VARCHAR(128),
	IN `iPass` VARCHAR(100),
	IN `iSalt` VARCHAR(100)
)
BEGIN
    DECLARE vApprovalCode VARCHAR(50);
    DECLARE vUserName VARCHAR(100);
    DECLARE vUserFamily VARCHAR(100);

    SELECT 1
         , tblUser.usrName
         , tblUser.usrFamily
      INTO vApprovalCode
         , vUserName
         , vUserFamily
      FROM tblUser
     WHERE tblUser.usrID = iUserID
           ;

    IF (NOT ISNULL(iPass)) THEN 
        IF ISNULL(vApprovalCode) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User Not Found';
        END IF;

        SELECT IF(fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass), 'Ok', 'Err')
             , tblUser.usrName
             , tblUser.usrFamily
          INTO vApprovalCode
             , vUserName
             , vUserFamily
          FROM tblUser
         WHERE tblUser.usrID = iUserID
        ;

        IF ISNULL(vApprovalCode) THEN 
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid userID';
        END IF;

        IF vApprovalCode = 'Err' THEN 
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid password';
        END IF;
    END IF;
  
    -- find user id if not provided
    IF ISNULL(vApprovalCode) THEN
        IF (NOT ISNULL(iUserID)) THEN 
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid userID';
        END IF;

          SELECT tblApprovalRequest.apr_usrID
               , tblUser.usrName
               , tblUser.usrFamily
            INTO vApprovalCode
               , vUserName
               , vUserFamily
            FROM tblApprovalRequest
       LEFT JOIN tblUser
              ON tblUser.usrID = tblApprovalRequest.apr_usrID
           WHERE tblApprovalRequest.aprApprovalKey = iKey
             AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2', 'E')
--               N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
        ORDER BY aprRequestDate DESC
           LIMIT 1
                 ;

        IF ISNULL(vApprovalCode) THEN 
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Could not find user by key';
        END IF;
        
        SET iUserID = CAST(vApprovalCode AS UNSIGNED);
    END IF;

    -- check if active approval request exists, expire it
    UPDATE tblApprovalRequest
       SET tblApprovalRequest.aprStatus = 'E'
     WHERE tblApprovalRequest.aprApprovalKey = iKey
       AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2')
--         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
           ;

    IF iBy = 'E' THEN
        SET vApprovalCode = Common.fnCreateRandomMD5();
    ELSE 
        SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;
    END IF;
  
    INSERT
      INTO tblApprovalRequest
       SET tblApprovalRequest.apr_usrID = iUserID,
           tblApprovalRequest.aprRequestedFor = iBy,
--           aprIsForLogin = 0,
           tblApprovalRequest.aprApprovalKey = iKey,
           tblApprovalRequest.aprApprovalCode = vApprovalCode
    ;

    INSERT
      INTO Common.tblAlerts
       SET Common.tblAlerts.alr_usrID = iUserID,
           Common.tblAlerts.alr_altCode = IF(iBy = 'E', 'approveEmail', 'approveMobile'),
           Common.tblAlerts.alrReplacedContactInfo = iKey,
           Common.tblAlerts.alrReplacements = JSON_OBJECT(
              'usrName', vUserName,
              'usrFamily', vUserFamily,
              'ApprovalCode', vApprovalCode
           );
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_forgotPassRequest` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_forgotPassRequest`(
	IN `iLogin` VARCHAR(50),
	IN `iVia` CHAR(1)

)
BEGIN
 DECLARE UserID INT UNSIGNED;
 DECLARE UserName VARCHAR(50);
 DECLARE UserFamily VARCHAR(50);
 DECLARE LinkUUID CHAR(32);
  
  SELECT tblUser.usrID,
         tblUser.usrName,
         tblUser.usrFamily
    INTO UserID,
         UserName,
         UserFamily
    FROM tblUser
      LEFT JOIN tblForgotPassRequest 
        ON tblForgotPassRequest.fpr_usrID = tblUser.usrID
   WHERE (tblUser.usrEmail = iLogin OR tblUser.usrMobile = iLogin)
     AND (ISNULL(tblForgotPassRequest.fprStatus)
          OR tblForgotPassRequest.fprStatus != 'N' 
          OR TIME_TO_SEC(TIMEDIFF(NOW(), tblForgotPassRequest.fprRequestDate)) > 60
         )
   LIMIT 1;
   
  IF (UserID IS NOT NULL) THEN 
    SET LinkUUID = Common.fnCreateRandomMD5();
    
    INSERT INTO tblForgotPassRequest 
       SET tblForgotPassRequest.fpr_usrID = UserID,
           tblForgotPassRequest.fprRequestedVia = iVia,
           tblForgotPassRequest.fprUUID = LinkUUID;	

    INSERT INTO Common.tblAlerts
       SET Common.tblAlerts.alr_usrID = UserID,
           Common.tblAlerts.alr_altCode = 'passReset',
           Common.tblAlerts.alrReplacements = JSON_OBJECT(
    			 	'usrName',UserName,
		    		'usrFamily',UserFamily,
            'via', iVia,
				    'UUID',LinkUUID
          ); 		 
  END IF;       
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_newOnlinePayment` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_newOnlinePayment`(
	IN `iVoucherID` BIGINT UNSIGNED,
	IN `iGatewayID` INT,
	IN `iAmount` BIGINT UNSIGNED,
	OUT `oMD5` VARCHAR(50)
)
BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:DEPRECATED. use sp_CREATE_onlinePayment instead';
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_onlinePayment` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_onlinePayment`(
	IN `iVoucherID` BIGINT UNSIGNED,
	IN `iGatewayID` INT,
	IN `iAmount` BIGINT UNSIGNED,
	OUT `oMD5` VARCHAR(50)
)
BEGIN
  SET oMD5 = Common.fnCreateRandomMD5();
  
  INSERT
    INTO tblOnlinePayments
     SET onpMD5 = oMD5
       , onp_vchID = iVoucherID
       , onp_pgwID = iGatewayID
       , onpAmount = iAmount
  ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_requestMobileVerifyCode` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_requestMobileVerifyCode`(
	IN `iMobile` VARCHAR(50),
	IN `iSignupIfNotExists` TINYINT,
	IN `iSignupRole` VARCHAR(50)
)
BEGIN
    DECLARE UserID BIGINT UNSIGNED;
    DECLARE ApprovalCode VARCHAR(50);

    IF ISNULL(iMobile) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid mobile';
    END IF;

    SELECT usrID
      INTO UserID
      FROM tblUser
     WHERE usrMobile = iMobile
    ;

    IF ISNULL(UserID) THEN
        IF iSignupIfNotExists = 1 THEN
            INSERT
              INTO tblUser
               SET tblUser.usrMobile = iMobile,
                   tblUser.usr_rolID = (
            SELECT tblRoles.rolID
              FROM tblRoles
             WHERE LOWER(tblRoles.rolName) = LOWER(iSignupRole)
--               AND (tblRoles.rolSignupAllowedIPs IS NULL
--                OR tblRoles.rolSignupAllowedIPs LIKE CONCAT("%,',iIP,',%"))
                   )
            ;
                   
            SET UserID = LAST_INSERT_ID();
        ELSE
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User Not Found';
        END IF;
    END IF;

    SET ApprovalCode = FLOOR(RAND() * 90000) + 10000;
  
    INSERT
      INTO tblApprovalRequest
       SET apr_usrID = UserID,
           aprRequestedFor = 'M',
--           aprIsForLogin = 1,
           aprApprovalKey = iMobile,
           aprApprovalCode = ApprovalCode
    ;

    INSERT
      INTO Common.tblAlerts
       SET alr_usrID = UserID,
           alrMobile = iMobile,
           alr_altCode = 'approveMobile',
           alrReplacedContactInfo = iMobile,
           alrReplacements = JSON_OBJECT(
               'ApprovalCode', ApprovalCode
           )
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_signup` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_signup`(
	IN `iBy` CHAR(1),
	IN `iLogin` VARCHAR(50),
	IN `iPass` CHAR(32),
	IN `iRole` VARCHAR(50),
	IN `iIP` VARCHAR(50),
	IN `iName` VARCHAR(100),
	IN `iFamily` VARCHAR(100),
	IN `iSpecialPrivs` VARCHAR(5000),
	IN `iMaxSessions` INT,
	IN `iCreatorUserID` BIGINT UNSIGNED,
	OUT `oUserID` BIGINT UNSIGNED
)
BEGIN
    DECLARE RoleID BIGINT UNSIGNED;
    DECLARE InnerRolID BIGINT;
    DECLARE SessionGUID CHAR(32);
    DECLARE Err VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;

        ROLLBACK;  

        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = 1,
               tblActionLogs.atlType = 'Signup.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", Err,
                   "iBy", iBy,
                   "iLogin", iLogin,
                   "iPass", iPass,
                   "iRole", iRole,
                   "iIP", iIP,
                   "iName", iName,
                   "iFamily", iFamily,
                   "iSpecialPrivs", iSpecialPrivs,
                   "iMaxSessions", iMaxSessions,
                   "iCreatorUserID", iCreatorUserID
               )
        ;

        RESIGNAL;  
    END;
    
    DECLARE EXIT HANDLER FOR 1062
    BEGIN
        ROLLBACK;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '409:Already registered.';
    END;

    CALL Common.sp_AddDebugLog('AAA', 'signup');

    SELECT tblRoles.rolID
      INTO RoleID
      FROM tblRoles
     WHERE tblRoles.rolName = iRole
       AND (tblRoles.rolSignupAllowedIPs IS NULL
        OR tblRoles.rolSignupAllowedIPs LIKE CONCAT("%,',iIP,',%"))
    ;

    IF ISNULL(RoleID) THEN
        SIGNAL SQLSTATE '45403'
           SET MESSAGE_TEXT = "403:Role not found or is not allowed to signup from this IP";
    END IF;

    START TRANSACTION;

    UPDATE tblUser
       SET _InvalidatedAt = UNIX_TIMESTAMP()
     WHERE _InvalidatedAt = 0
       AND usrStatus = 'R'
       AND (
           (IFNULL(IF(iBy = 'E', iLogin, NULL), '') <> '' AND IFNULL(usrEmail, '') = IF(iBy = 'E', iLogin, NULL))
        OR (IFNULL(IF(iBy = 'M', iLogin, NULL), '') <> '' AND IFNULL(usrMobile, '') = IF(iBy = 'M', iLogin, NULL))
           )
    ;

    IF (IFNULL(iPass, '') = '') THEN
        SET iPass = 'NOT_SET';
    ELSE
        SET iPass = LOWER(iPass);
    END IF;

    INSERT INTO tblUser
       SET tblUser.usrName = iName,
           tblUser.usrFamily = iFamily,
           tblUser.usrEmail = IF(iBy = 'E', iLogin, NULL),
           tblUser.usrMobile = IF(iBy = 'M', iLogin, NULL),     
           tblUser.usrPass = iPass,
           tblUser.usr_rolID = RoleID,
           tblUser.usrSpecialPrivs = iSpecialPrivs,
           tblUser.usrMaxSessions = iMaxSessions,
           tblUser.usrCreatedBy_usrID = IFNULL(iCreatorUserID, 1)
    ;

    SET oUserID = LAST_INSERT_ID();

    CALL sp_CREATE_approvalRequest(iBy, oUserID, iLogin, NULL, NULL);

    INSERT
      INTO tblUserWallets
       SET tblUserWallets.wal_usrID = oUserID,
           tblUserWallets.walName = 'Default',
           tblUserWallets.walDefault = 1,
           tblUserWallets.walCreatedBy_usrID = IFNULL(iCreatorUserID, 1)
    ;

    INSERT
      INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = oUserID,
           tblActionLogs.atlType = 'UserCreated'
    ;

    COMMIT;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_transfer` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_transfer`(
	IN `iFromUserID` BIGINT UNSIGNED,
	IN `iFromWalID` INT,
	IN `iToUserLogin` INT,
	IN `iAmount` INT,
	IN `iPass` INT,
	IN `iSalt` INT
)
BEGIN
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE Valid TINYINT;
  DECLARE TargetUserID BIGINT UNSIGNED;
  DECLARE TargetWalletID BIGINT UNSIGNED;
  DECLARE FromNameFamily VARCHAR(100);
  DECLARE ToNameFamily VARCHAR(100);
  DECLARE Err VARCHAR(500);
  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = iFromUserID,
         tblActionLogs.atlType = 'Transfer.Error',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", Err,
              "iFromUserID", iFromUserID,
              "iFromWalID", iFromWalID,
              "iToUserLogin", iToUserLogin,
              "iAmount", iAmount,
              "iPass", iPass,
              "iSalt", iSalt
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;
  
  SELECT IFNULL(CONCAT(tblUser.usrName, ' ', tblUser.usrFamily), IFNULL(tblUser.usrEmail, tblUser.usrMobile)),
         tblUser.usrID,
         tblUserWallets.walID
    INTO ToNameFamily,
         TargetUserID,
         TargetWalletID
    FROM tblUser
      JOIN tblUserWallets
        ON tblUserWallets.wal_usrID = tblUser.usrID
   WHERE tblUser.usrStatus = 'A'
     AND (tblUser.usrEmail = iToUserLogin OR tblUser.usrMobile = iToUserLogin)
     AND tblUserWallets.walDefault = TRUE;
     
  IF ISNULL(ToNameFamily) THEN 
 		SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '401: Target user not found or is inactive';
  END IF;

  SELECT fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass),
         IFNULL(CONCAT(tblUser.usrName, ' ', tblUser.usrFamily), IFNULL(tblUser.usrEmail, tblUser.usrMobile))
    INTO Valid,
         FromNameFamily
    FROM tblUser
   WHERE tblUser.usrID = iFromUserID
     AND tblUser.usrStatus = 'A';
   
  IF ISNULL(Valid) THEN
 		SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '401:Invalid UserID';
  END IF;
  
  IF Valid = FALSE THEN
 		SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '401:Incorrect password';
  END IF;
  
  CALL sp_SYSTEM_validateWalletTransaction(iFromUserID, iFromWalID, 'T', iAmount, Valid);

  START TRANSACTION;
    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iFromUserID,
           tblVoucher.vchType = 'T',
           tblVoucher.vchDesc = JSON_OBJECT(
              "targetID", TargetUserID,
              "targetLogin", iToUserLogin,
              "targetName", ToNameFamily
           ),
           tblVoucher.vchTotalAmount = Amount,
           tblVoucher.vchStatus = 'F';
           
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = iFromWalID,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wltType = 'T',
           tblWalletsTransactions.wltAmount = iAmount * -1;

    -- Do not merge this with previous query. Creation of a transatction must be separaet from it's acceptance           
    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();  
    
    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = TargetUserID,
           tblVoucher.vchType = 'F',
           tblVoucher.vchDesc = JSON_OBJECT(
              "fromID", iFromUserID,
              "fromName", FromNameFamily
           ),
           tblVoucher.vchTotalAmount = Amount,
           tblVoucher.vchStatus = 'F';

    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = TargetWalletID,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wltType = 'F',
           tblWalletsTransactions.wltAmount = iAmount;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();  
  COMMIT;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_walletTransaction` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_walletTransaction`(
	IN `iWalletID` BIGINT UNSIGNED,
	IN `iVoucherID` BIGINT UNSIGNED,
	OUT `oAmount` INT UNSIGNED
)
BEGIN
  DECLARE TransactionType CHAR(1);
  DECLARE Multiplier TINYINT;
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE Err VARCHAR(500);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;

    INSERT
      INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = UserID,
           tblActionLogs.atlType = 'WLT.Error',
           tblActionLogs.atlDescription = JSON_OBJECT(
               "err", Err,
               "iWalletID", iWalletID,
               "iVoucherID",iVoucherID
           );

--	  ROLLBACK;  
	  RESIGNAL;  
  END;

    SELECT tblVoucher.vch_usrID,
           tblVoucher.vchType, 
           tblVoucher.vchTotalAmount
      INTO UserID, 
           TransactionType,
           oAmount
      FROM tblVoucher
     WHERE tblVoucher.vchID = iVoucherID
       AND tblVoucher.vchStatus = 'N'
  ;

  IF ISNULL(UserID) THEN 
    SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '401:Invalid voucher ID';
  END IF;

  CALL sp_SYSTEM_validateWalletTransaction(UserID, iWalletID, TransactionType, oAmount, Multiplier);
  
  IF oAmount > 0 THEN 
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = WalletID,
           tblWalletsTransactions.wlt_vchID = iVoucherID,
           tblWalletsTransactions.wltType = TrasnactionType,
           tblWalletsTransactions.wltAmount = oAmount * Multiplier;
  END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_walletTransactionOnPayment` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_walletTransactionOnPayment`(
	IN `iVoucherID` BIGINT UNSIGNED,
	IN `iPaymentType` CHAR(1)
)
BEGIN
  DECLARE PaymentID BIGINT UNSIGNED;
  DECLARE ExpenseVoucherID BIGINT UNSIGNED;
  DECLARE CreditVoucherID BIGINT UNSIGNED;
  DECLARE Amount BIGINT UNSIGNED;
  DECLARE TotalAmount BIGINT UNSIGNED;
  DECLARE RemainingAfterWallet BIGINT UNSIGNED;
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE UserDefaultWallet BIGINT UNSIGNED;
  DECLARE Err VARCHAR(500);
  DECLARE LastID BIGINT UNSIGNED;

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = UserID,
         tblActionLogs.atlType = 'VirtWLT.Error',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", Err,
              "iVoucherID", iVoucherID,
              "iPaymentType", iPaymentType
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;

  START TRANSACTION;
    IF iPaymentType = 'N' THEN /*Online*/
      SELECT tblOnlinePayments.onpID,
             tblOnlinePayments.onp_vchID,
             tblOnlinePayments.onpAmount,
             tblVoucher.vchTotalAmount,
             tblVoucher.vch_usrID
        INTO PaymentID,
             ExpenseVoucherID,
             Amount,
             TotalAmount,
             UserID
        FROM tblOnlinePayments
        JOIN tblVoucher
          ON tblVoucher.vchID = tblOnlinePayments.onp_vchID
       WHERE tblOnlinePayments.onp_vchID = iVoucherID
         AND tblOnlinePayments.onpStatus = 'Y'; /*Payed*/
    ELSEIF iPaymentType = 'F' THEN /*Offline*/
      SELECT tblOnlinePayments.onpID,
             tblOfflinePayments.ofp_vchID,
             tblOfflinePayments.ofpAmount,
             tblVoucher.vchTotalAmount,
             tblVoucher.vch_usrID
        INTO PaymentID,
             ExpenseVoucherID,
             Amount,
             TotalAmount,
             UserID     
        FROM tblOfflinePayments
        JOIN tblVoucher
          ON tblVoucher.vchID = tblOnlinePayments.onp_vchID
       WHERE tblOfflinePayments.ofp_invID = iVoucherID
         AND tblOfflinePayments.onpStatus = 'Y'; /*Payed*/
    ELSE
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Invalid payment type';
    END IF;

    IF ISNULL(ExpenseVoucherID) THEN 
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Payment not found or is not yet payed';
    END IF;

    SELECT tblUserWallets.walID INTO UserDefaultWallet
      FROM tblUserWallets
     WHERE tblUserWallets.wal_usrID = UserID
       AND tblUserWallets.walDefault = TRUE;

    IF ISNULL(UserDefaultWallet) THEN 
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '500:Default wallet not found';
    END IF;

    SELECT TotalAmount - tblWalletsTransactions.wltAmount
      INTO RemainingAfterWallet
      FROM tblWalletsTransactions
	  WHERE tblWalletsTransactions.wlt_vchID = iVoucherID
       AND tblWalletsTransactions.wltStatus = 'A';

    IF (IFNULL(RemainingAfterWallet, 0) < 0) THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:Remaining After Wallet can not be negative';
    END IF;

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = UserID,
           tblVoucher.vchType = 'C',
           tblVoucher.vchDesc = JSON_OBJECT(
              "type", IF(iPaymentType = 'N', 'Online', 'Offline'),
              "paymentID", PaymentID
           ),
           tblVoucher.vchTotalAmount = Amount,
           tblVoucher.vchStatus = 'F';
    
	 SET LastID = LAST_INSERT_ID();
	 IF (LastID IS NULL) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;
    
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = UserDefaultWallet,
           tblWalletsTransactions.wlt_vchID = LastID,
           tblWalletsTransactions.wlt_vchType = 'C', /*Credit*/
           tblWalletsTransactions.wltAmount = Amount;

	 SET LastID = LAST_INSERT_ID();
	 IF (LastID IS NULL) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LastID;

  IF (RemainingAfterWallet > 0) THEN
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = UserDefaultWallet,
           tblWalletsTransactions.wlt_vchID = iVoucherID,
           tblWalletsTransactions.wlt_vchType = 'E', /*Expense*/
           tblWalletsTransactions.wltAmount = RemainingAfterWallet;

	 SET LastID = LAST_INSERT_ID();
	 IF (LastID IS NULL) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '500:LastID IS NULL';
    END IF;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'Y'
     WHERE tblWalletsTransactions.wltID = LastID;
  END IF;

  COMMIT;  
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CREATE_withdrawalRequest` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_CREATE_withdrawalRequest`(
	IN `iWalletID` BIGINT UNSIGNED,
	IN `iForUsrID` BIGINT UNSIGNED,
	IN `iByUserID` BIGINT UNSIGNED,
	IN `iAmount` INT UNSIGNED,
	IN `iDesc` VARCHAR(500),
	OUT `oVoucherID` BIGINT UNSIGNED
)
BEGIN
  DECLARE Multiplier TINYINT;
  DECLARE Err VARCHAR(500);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = iByUserID,
         tblActionLogs.atlType = 'Withdraw.Error',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", Err,
              "iWalletID", iWalletID,
              "iForUsrID", iForUsrID,
              "iByUserID", iByUserID,
              "iAmount",iAmount,
              "iDesc",iDesc
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;
  
  CALL sp_SYSTEM_validateWalletTransaction(iForUsrID, iWalletID, 'W', iAmount, Multiplier);

  INSERT INTO tblVoucher
     SET tblVoucher.vch_usrID = iForUsrID,
         tblVoucher.vchType = 'W',
         tblVoucher.vchTotalAmount = iAmount,
         tblVoucher.vchDesc = JSON_OBJECT(
          "desc", iDesc
         );
         
  SET oVoucherID = LAST_INSERT_ID();
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_SYSTEM_prepareBalanceInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_SYSTEM_prepareBalanceInfo`(
	IN `iWalID` BIGINT UNSIGNED,
	IN `iWltID` BIGINT UNSIGNED,
	IN `iWltDateTime` DATETIME,
	OUT `oLastTransactionDate` DATETIME,
	OUT `oLastBalance` BIGINT,
	OUT `oLastIncome` BIGINT,
	OUT `oLastExpense` BIGINT,
	OUT `oMultipleInsert` TINYINT
)
BEGIN

  SELECT tblWalletsTransactions.wltDateTime INTO oLastTransactionDate
    FROM tblWalletsTransactions
    JOIN tblWalletBalances
      ON tblWalletBalances.wbl_wltID = tblWalletsTransactions.wltID
   WHERE tblWalletsTransactions.wlt_walID = iWalID
     AND tblWalletsTransactions.wltID < iWltID
   ORDER BY tblWalletsTransactions.wltDateTime DESC
   LIMIT 1;
   
  CALL Common.sp_AddDebugLog('afterInsertTransaction', JSON_OBJECT('oLastTransactionDate', oLastTransactionDate, 
                                                              'iWalID',iWalID,
                                                              'iWltID',iWltID,
                                                              'iWltDateTime',iWltDateTime,
                                                              'compare', oLastTransactionDate >= iWltDateTime
                                                              ));
   
  IF NOT ISNULL(oLastTransactionDate) AND oLastTransactionDate >= iWltDateTime THEN
    DELETE tblWalletBalances
      FROM tblWalletBalances
      JOIN tblWalletsTransactions
        ON tblWalletsTransactions.wltID = tblWalletBalances.wbl_wltID
     WHERE tblWalletsTransactions.wltDateTime >= iWltDateTime;   
    SET oMultipleInsert = TRUE;
  END IF;
  
  SET oLastTransactionDate = NULL;
  
  SELECT tblWalletBalances.wblBalance,
         tblWalletsTransactions.wltDateTime 
    INTO oLastBalance,
         oLastTransactionDate
    FROM tblWalletBalances
    JOIN tblWalletsTransactions
      ON tblWalletsTransactions.wltID = tblWalletBalances.wbl_wltID
   WHERE tblWalletsTransactions.wlt_walID = iWalID
   ORDER BY tblWalletsTransactions.wltDateTime DESC, tblWalletsTransactions.wltID DESC
   LIMIT 1;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_SYSTEM_validateIPAccess` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_SYSTEM_validateIPAccess`(
	IN `iIP` BIGINT


)
    READS SQL DATA
Proc: BEGIN
	DECLARE IPStatus CHAR DEFAULT NULL;
	DECLARE LastAccess BIGINT UNSIGNED DEFAULT NULL;
	DECLARE Diff DOUBLE;
	
	SELECT tblIPBin.ipbStatus,
			 (UNIX_TIMESTAMP(CURTIME(4)) - MAX(tblIPStats.ipsTimeStamp))
		INTO IPStatus,
		     Diff
		FROM tblIPBin
			JOIN tblIPStats
				ON tblIPStats.ips_ipbIP = tblIPBin.ipbIP
		WHERE tblIPBin.ipbIP = iIP;
	
	IF IPStatus IS NULL THEN
		LEAVE Proc;
	END IF;
	
	IF (IPStatus != 'A') THEN  
		SIGNAL SQLSTATE '45403'
	      SET MESSAGE_TEXT = "403:IP %IP% has been blocked. Contact system admin.";
   ELSEIF (Diff < 0.01) THEN
		SIGNAL SQLSTATE '45403'
	      SET MESSAGE_TEXT = "403:Fast request from %IP% has been banned";
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_SYSTEM_validateWalletTransaction` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_SYSTEM_validateWalletTransaction`(
	IN `iUserID` BIGINT UNSIGNED,
	INOUT `ioWalletID` BIGINT UNSIGNED,
	IN `iTransactionType` CHAR(1),
	INOUT `ioAmount` INT UNSIGNED,
	OUT `oMultiplier` TINYINT
)
BEGIN
  DECLARE LastBalance BIGINT;
  DECLARE MinBalance BIGINT;
  DECLARE NotTransferable BIGINT;
  DECLARE MaxTransferPerDay BIGINT;
  DECLARE TodayTransfers BIGINT;

  SELECT tblUserWallets.walID,
         tblUserWallets.walMinBalance,
         tblUserWallets.walLastBalance,
         tblUserWallets.walNotTransferableAmount
    INTO ioWalletID,
         MinBalance,
         LastBalance,
         NotTransferable
    FROM tblUserWallets
   WHERE tblUserWallets.wal_usrID = iUserID
     AND (tblUserWallets.walID = ioWalletID 
          OR (ioWalletID = 0 
              AND  tblUserWallets.walDefault = TRUE
              )
         );
         
  IF ISNULL(ioWalletID) THEN 
 		SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '401:Wallet not found or is not yours';
  END IF;
  
  CASE iTransactionType
    WHEN 'E' THEN 
      SET ioAmount = LEAST(ioAmount, GREATEST(0, LastBalance - MinBalance)), oMultiplier = -1;
    WHEN 'T' THEN 
      IF LastBalance - NotTransferable - MinBalance - ioAmount < 0 THEN
     		SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Not enough credit in wallet to transfer';
      END IF;
      
      SELECT COALESCE(SUM(tblWalletsTransactions.wltAmount),0) INTO TodayTransfers
        FROM tblWalletsTransactions
       WHERE tblWalletsTransactions.wlt_walID = ioWalletID
         AND DATE(tblWalletsTransactions.wltDateTime) = DATE(NOW())
         AND tblWalletsTransactions.wltType = 'T'
         AND tblWalletsTransactions.wltStatus = 'A';        
      
      IF MaxTransferable >0 AND MaxTransferPerDay < ioAmount + TodayTransfers THEN
     		SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Amount is greater than your per day allowed transfer';
      END IF;
      SET oMultiplier = -1;
    WHEN 'F' THEN 
      SET oMultiplier = 1;
    WHEN 'I' THEN 
      SET oMultiplier = 1;
    WHEN 'C' THEN 
      SET oMultiplier = 1;
    WHEN 'W' THEN 
      IF LastBalance - NotTransferable - MinBalance - ioAmount < 0 THEN
     		SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Not enough credit in wallet to withdraw';
      END IF;
      
      SET oMultiplier = -1;
    ELSE 
    	SIGNAL SQLSTATE '45000'
         SET MESSAGE_TEXT = '500:Invalid wallet transaction type';
  END CASE;
/**/
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_acceptApproval` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_acceptApproval`(
	IN `iBy` CHAR(1),
	IN `iKey` VARCHAR(128),
	IN `iCode` VARCHAR(50),
	IN `iLogin` TINYINT,
	IN `iLoginIP` VARCHAR(50),
	IN `iLoginInfo` JSON,
	IN `iLoginRemember` TINYINT,
	IN `iTTL` INT
)
BEGIN
    DECLARE vAprID BIGINT UNSIGNED;
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vNewKey VARCHAR(128);
    DECLARE vByType CHAR(1);
    DECLARE vAprStatus CHAR(1);
    DECLARE vIsExpired BOOL;
    DECLARE vSessionGUID VARCHAR(32);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    IF ISNULL(iKey) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid key';
    END IF;

    -- 24*60*60
    -- never expire?
    IF iTTL = 0 THEN
        SET iTTL = NULL;
    END IF;

      SELECT tblApprovalRequest.aprID
           , tblApprovalRequest.apr_usrID
           , tblApprovalRequest.aprApprovalKey
           , tblApprovalRequest.aprRequestedFor
           , tblApprovalRequest.aprStatus
           , (tblApprovalRequest.aprStatus = 'E'
          OR (iTTL IS NOT NULL
         AND tblApprovalRequest.aprSentDate IS NOT NULL 
         AND TIME_TO_SEC(TIMEDIFF(NOW(), tblApprovalRequest.aprSentDate)) > iTTL)
             )
        INTO vAprID
           , vUserID
           , vNewKey
           , vByType
           , vAprStatus
           , vIsExpired
        FROM tblApprovalRequest
   LEFT JOIN tblUser
          ON tblUser.usrID = tblApprovalRequest.apr_usrID
       WHERE tblApprovalRequest.aprApprovalKey = iKey
--         AND tblApprovalRequest.aprIsForLogin = 0
         AND tblApprovalRequest.aprApprovalCode = iCode
         AND tblApprovalRequest.aprStatus IN ('N', 'S', 'A', '1', '2', 'E')
--         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2:SecondTry, E: Expired
    ORDER BY aprRequestDate DESC
       LIMIT 1
             ;

    IF vAprStatus = 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code not sent to the client';
    END IF;

    IF vAprStatus = 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Already applied before';
    END IF;

    IF ISNULL(vUserID) THEN 
        IF (iBy = 'M') THEN
            UPDATE tblApprovalRequest
               SET aprStatus = IF(aprStatus = 'S', '1', 
                                   IF(aprStatus = '1', '2', 'E'))
             WHERE aprApprovalKey = iKey
--               AND aprIsForLogin = 0
               AND aprStatus IN ('S', '1', '2')
            ;
        END IF;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid user or code';
    END IF;

    IF vIsExpired THEN 
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code expired';
    END IF;

    START TRANSACTION;

    UPDATE tblApprovalRequest
       SET aprApplyDate = NOW(),
           aprStatus = 'A'
     WHERE aprID = vAprID;

    IF vByType = 'E' THEN
        UPDATE tblUser
           SET usrEmail = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','E'), 'E', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUserID
         WHERE usrID = vUserID
        ;
    ELSE
        UPDATE tblUser
           SET usrMobile = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','M'), 'M', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUserID
         WHERE usrID = vUserID
        ;
    END IF;

    IF iLogin = 1 THEN
        SET vSessionGUID = SUBSTRING(CommonFuncs.guid(NULL), 1, 32);

        INSERT
          INTO tblActiveSessions
           SET tblActiveSessions.ssnKey          = vSessionGUID,
               tblActiveSessions.ssn_usrID       = vUserID,
               tblActiveSessions.ssnIP           = INET_ATON(iLoginIP),
               tblActiveSessions.ssnRemember     = iLoginRemember,
               tblActiveSessions.ssnLastActivity = NOW(),
               tblActiveSessions.ssnInfo         = iLoginInfo;
    
        UPDATE tblUser
           SET tblUser.usrLastLogin = NOW(),
               tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
         WHERE tblUser.usrID = vUserID
        ;
    
        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'UserLoggedIn'
        ;
    END IF;

    COMMIT;

    SELECT tblUser.usrID,
           tblUser.usrName,
           tblUser.usrFamily,
           tblUser.usrEmail,
           tblUser.usr_rolID,
           tblUser.usrApprovalState,
           tblRoles.rolName,
           fnGetAllPrivs(tblUser.usr_rolID, tblUser.usrSpecialPrivs) AS privs,
           NOT ISNULL(tblUser.usrPass) AS hasPass,
           tblUser.usrStatus,
           vSessionGUID AS ssnKey
      FROM tblUser
      JOIN tblRoles
        ON tblRoles.rolID = tblUser.usr_rolID
     WHERE tblUser.usrID = vUserID
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_changePass` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_changePass`(
	IN `iUserID` BIGINT UNSIGNED,
	IN `iOldPass` CHAR(32),
	IN `iOldPassSalt` VARCHAR(50),
	IN `iNewPass` CHAR(32)
)
BEGIN
  DECLARE WasOK BOOL DEFAULT FALSE;

  SELECT 1 INTO WasOK
    FROM tblUser
   WHERE tblUser.usrID = iUserID
     AND fnPasswordsAreEqual(iOldPass, iOldPassSalt, tblUser.usrPass);
  
  IF NOT WasOK THEN
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Invalid User or Password';
  END IF;
  
  UPDATE tblUser
     SET tblUser.usrPass = iNewPass
   WHERE tblUser.usrID = iUserID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_changePassByUUID` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_changePassByUUID`(
	IN `iUUID` VARCHAR(50),
	IN `iNewPass` VARCHAR(50)
)
BEGIN
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE IsExpired BOOL;
  
  SELECT tblForgotPassRequest.fpr_usrID,
         TIMEDIFF(NOW(), tblForgotPassRequest.fprRequestDate) > "00:30:00"
    INTO UserID, IsExpired
    FROM tblForgotPassRequest
   WHERE tblForgotPassRequest.fprUUID = iUUID
     AND tblForgotPassRequest.fprStatus = 'S';
  
  IF ISNULL (IsExpired) OR IsExpired THEN 
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Invalid or Expired link';
  END IF;
  
  UPDATE tblForgotPassRequest
     SET tblForgotPassRequest.fprStatus = IF(tblForgotPassRequest.fprUUID = iUUID, 'A', 'E')
   WHERE tblForgotPassRequest.fpr_usrID = UserID
     AND tblForgotPassRequest.fprStatus IN ('S', 'N'); 
       
  UPDATE tblUser
     SET tblUser.usrPass = iNewPass
   WHERE tblUser.usrID = UserID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_login` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_login`(
	IN `iLogin` VARCHAR(100),
	IN `iIP` VARCHAR(50),
	IN `iPass` CHAR(32),
	IN `iSalt` VARCHAR(50),
	IN `iInfo` JSON,
	IN `iRemember` TINYINT,
	IN `iOAuthInfo` VARCHAR(10000)
)
BEGIN
    DECLARE vLoginStatus CHAR(1);
    DECLARE vUserID BIGINT UNSIGNED;
--    DECLARE InnerRolID BIGINT;
    DECLARE vSessionGUID CHAR(32);
    DECLARE vLastOAuthInfo JSON;
    DECLARE vUserApprovalState CHAR(1);
    DECLARE vUserEmail CHAR(50);
    DECLARE vMessage CHAR(128);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    START TRANSACTION;

    SELECT IF (tblUser.usrMaxSessions > 0 AND tblUser.usrMaxSessions - tblUser.usrActiveSessions <= 0,
              'O',
              IF (fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass),
                  tblUser.usrStatus,
                  'I'
              )
           ),
           tblUser.usrID,
           tblUserExtraInfo.ueiUpdatedBy_usrID,
           usrApprovalState,
           usrEmail
      INTO vLoginStatus,
           vUserID,
           vLastOAuthInfo,
           vUserApprovalState,
           vUserEmail
      FROM tblUser
 LEFT JOIN tblUserExtraInfo
        ON tblUserExtraInfo.uei_usrID = tblUser.usrID
     WHERE (
           tblUser.usrEmail  = iLogin 
        OR tblUser.usrMobile = iLogin
        OR (
           NOT ISNULL(iOAuthInfo) 
       AND JSON_EXTRACT(iOAuthInfo, "$.type") = 'Linkedin'
       AND tblUserExtraInfo.ueiOAuthAccounts->"$.Linkedin" = JSON_EXTRACT(iOAuthInfo, "$.id")
           )
           )
       AND tblUser.usrStatus IN ('A','V');

    IF NOT ISNULL(vUserApprovalState) AND vUserApprovalState NOT IN ('A', IF(vUserEmail = iLogin, 'E', 'M')) THEN
        SET vMessage = CONCAT('401:', IF(vUserEmail = iLogin, 'Email', 'Mobile'), ' not approved');

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = vMessage;
    END IF;
    
    IF ISNULL(vLoginStatus) THEN
      IF ISNULL(iOAuthInfo) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid User or Password';
      ELSE 
--        CALL sp_CREATE_signup(iBy, iLogin, iPass, iRole, iIP, iName, iFamily, iSpecialPrivs, iMaxSessions, oUserID);
-- TODO create wallet
        INSERT
          INTO tblUser
           SET tblUser.usrName   = JSON_EXTRACT(iOAuthInfo, "$.name"),
               tblUser.usrFamily = JSON_EXTRACT(iOAuthInfo, "$.family"),
               tblUser.usrEmail  = JSON_EXTRACT(iOAuthInfo, "$.email"),
               tblUser.usrApprovalState  = 'E';
               
        SET vUserID = LAST_INSERT_ID();

        INSERT
          INTO tblUserExtraInfo
           SET tblUserExtraInfo.uei_usrID = vUserID,
               tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
               tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"));

        SET vLoginStatus = 'H';
      END IF;
    END IF;

    CASE vLoginStatus
        WHEN 'O' THEN
            INSERT
              INTO tblActionLogs 
               SET tblActionLogs.atlType = 'OverSession',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '405:Max sessions used close old sessions';

        WHEN 'I' THEN
            INSERT
              INTO tblActionLogs 
               SET tblActionLogs.atlType = 'InvalidPass',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid user or Password';

        WHEN 'R' THEN
            INSERT
              INTO tblActionLogs 
               SET tblActionLogs.atlType = 'UserRemoved',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '405:User Removed. Ask administrator';

        WHEN 'B' THEN
            INSERT
              INTO tblActionLogs 
               SET tblActionLogs.atlType = 'UserBlocked',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '405:User Blocked. Ask administrator';

        WHEN 'V' THEN
            IF ISNULL(iOAuthInfo) THEN 
                INSERT
                  INTO tblActionLogs 
                   SET tblActionLogs.atlType = 'UserNotApprovedYet',
                       tblActionLogs.atlBy_usrID = vUserID;
                COMMIT;
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '405:You must approve either email or mobile';
            END IF;

        WHEN 'A' THEN
            SET @a = 1;
    END CASE;

    IF NOT ISNULL(iOAuthInfo) AND vLoginStatus != 'H' THEN 
        IF ISNULL(vLastOAuthInfo) THEN 
            INSERT
              INTO tblUserExtraInfo
               SET tblUserExtraInfo.uei_usrID = vUserID,
                   tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                   tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"))
                ON DUPLICATE KEY UPDATE 
                   tblUserExtraInfo.uei_usrID = vUserID,
                   tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                   tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"));
        ELSE
            UPDATE tblUserExtraInfo
               SET tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                   tblUserExtraInfo.ueiOAuthAccounts = JSON_MERGE(
                       astOAuthInfo,
                       JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"))
                   ),
                   tblUserExtraInfo.ueiUpdatedBy_usrID = vUserID
             WHERE tblUserExtraInfo.uei_usrID = vUserID;    
                
            INSERT
              INTO tblActionLogs
               SET tblActionLogs.atlBy_usrID = vUserID,
                   tblActionLogs.atlType = 'UserOAuthUpdated';
        END IF;
    END IF;

    SET vSessionGUID = SUBSTRING(CommonFuncs.guid(NULL), 1, 32);

    INSERT
      INTO tblActiveSessions
       SET tblActiveSessions.ssnKey          = vSessionGUID,
           tblActiveSessions.ssn_usrID       = vUserID,
           tblActiveSessions.ssnIP           = INET_ATON(iIP),
           tblActiveSessions.ssnRemember     = iRemember,
           tblActiveSessions.ssnLastActivity = NOW(),
           tblActiveSessions.ssnInfo         = iInfo;

    UPDATE tblUser
       SET tblUser.usrLastLogin = NOW(),
           tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
     WHERE tblUser.usrID = vUserID;

    INSERT
      INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = vUserID,
           tblActionLogs.atlType = 'UserLoggedIn';

    COMMIT;     

    SELECT tblUser.usrID,
           tblUser.usrName,
           tblUser.usrFamily,
           tblUser.usrEmail,
           tblUser.usr_rolID,
           tblUser.usrApprovalState,
           tblRoles.rolName,
           fnGetAllPrivs(tblUser.usr_rolID, tblUser.usrSpecialPrivs) AS privs,
           NOT ISNULL(tblUser.usrPass) AS hasPass,
           tblUser.usrStatus,
           vSessionGUID AS ssnKey
      FROM tblUser
      JOIN tblRoles
        ON tblRoles.rolID = tblUser.usr_rolID
     WHERE tblUser.usrID = vUserID; 
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_logout` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_logout`(
	IN `iByUserID` BIGINT UNSIGNED,
	IN `iSessionGUID` CHAR(32)
)
BEGIN
    DECLARE vUserID BIGINT UNSIGNED;
    
    SELECT tblActiveSessions.ssn_usrID 
      INTO vUserID
      FROM tblActiveSessions
     WHERE tblActiveSessions.ssnKey = iSessionGUID
       AND tblActiveSessions.ssnStatus = 'A'
    ;
    
    IF vUserID = iByUserID THEN
        UPDATE tblActiveSessions
           SET tblActiveSessions.ssnStatus = 'G'
         WHERE tblActiveSessions.ssnKey = iSessionGUID
        ;
        
        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'UserLogout'
        ;
    ELSE 
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '405:Logout by other users is not allowed yet';
    END IF;          
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_paymentGateway_FailedCounters` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_paymentGateway_FailedCounters`(
	IN `iPgwID` INT UNSIGNED,
	IN `iAmount` BIGINT UNSIGNED
)
BEGIN
	UPDATE tblPaymentGateways
	   SET pgwSumFailedCount = IFNULL(pgwSumFailedCount, 0) + 1
	 WHERE pgwID = iPgwID
	;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_paymentGateway_OkCounters` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_paymentGateway_OkCounters`(
	IN `iPgwID` INT UNSIGNED,
	IN `iAmount` BIGINT UNSIGNED
)
BEGIN
	UPDATE tblPaymentGateways
	   SET pgwSumTodayPaidAmount = IF(DATEDIFF(pgwLastPaymentDateTime, CURDATE()) = 0, pgwSumTodayPaidAmount + iAmount, iAmount)
	 WHERE pgwID = iPgwID
	;

	UPDATE tblPaymentGateways
	   SET pgwLastPaymentDateTime = CURRENT_TIMESTAMP()
	 WHERE pgwID = iPgwID
	;

	UPDATE tblPaymentGateways
	   SET pgwSumOkCount = IFNULL(pgwSumOkCount, 0) + 1
	     , pgwSumPaidAmount = IFNULL(pgwSumPaidAmount, 0) + iAmount
	 WHERE pgwID = iPgwID
	;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_paymentGateway_RequestCounters` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_paymentGateway_RequestCounters`(
	IN `iPgwID` INT UNSIGNED,
	IN `iAmount` BIGINT UNSIGNED
)
BEGIN
	UPDATE tblPaymentGateways
	   SET pgwSumRequestCount = IFNULL(pgwSumRequestCount, 0) + 1
	     , pgwSumRequestAmount = IFNULL(pgwSumRequestAmount, 0) + iAmount
	 WHERE pgwID = iPgwID
	;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_reserveWalletPayment` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_reserveWalletPayment`(
	IN `iWalletID` BIGINT UNSIGNED,
	IN `iUserID` BIGINT UNSIGNED,
	IN `iVoucherID` BIGINT UNSIGNED
)
BEGIN
  
  DECLARE IsAllowed BOOLEAN;
  DECLARE WalletStatus CHAR(1);
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE Amount BIGINT;
  DECLARE Err VARCHAR(500);
  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = UserID,
         tblActionLogs.atlType = 'reserveWallet.Error',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", Err,
              "iWalletID", iWalletID,
              "iVoucherID", iVoucherID
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;

  
  SELECT tblVoucher.vchTotalAmount INTO Amount
    FROM tblVoucher
   WHERE tblVoucher.vchID = iVoucherID;
   
  IF ISNULL(WalletStatus) THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '401:voucher not found';
  END IF;   
  
  SELECT tblUserWallets.walID,
         tblUserWallets.walLastBalance - Amount < tblUserWallets.walMinBalance,
         tblUserWallets.walStatus 
    INTO iWalletID, 
         IsAllowed, 
         WalletStatus
    FROM tblUserWallets
   WHERE tblUserWallets.wal_usrID = iUserID
     AND (tblUserWallets.walID = iWalletID 
          OR (iWalletID =0 AND tblUserWallets.walDefault = 1)
         );
          
  IF ISNULL(WalletStatus) THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '401:Wallet not found';
  ELSEIF WalletStatus != 'A' THEN
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '401:Selected wallet can not be used';
  ELSEIF IsAllowed = FALSE THEN
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '401:Not enough credit in your wallet';
  END IF;
  
  INSERT INTO tblWalletsTransactions
     SET tblWalletsTransactions.wlt_walID = iWalletID,
         tblWalletsTransactions.wlt_vchID = iVoucherID,
         tblWalletsTransactions.wltAmount = Amount;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_retrieveTokenInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_retrieveTokenInfo`(
	IN `iToken` VARCHAR(50),
	IN `iIP` VARCHAR(50)
)
BEGIN
	DECLARE UserID BIGINT DEFAULT NULL;
	DECLARE TokenID BIGINT DEFAULT NULL;
	DECLARE ValidateIP BOOL DEFAULT FALSE;
	DECLARE IP BIGINT UNSIGNED;
	DECLARE AccessCount BIGINT UNSIGNED;
	DECLARE ExpiryDate DATE;
	DECLARE Privs VARCHAR(5000);
	DECLARE RoleName VARCHAR(50);
	DECLARE PreferedLang CHAR(2);
	DECLARE LastStatus CHAR(1);
  DECLARE Err VARCHAR(500);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = UserID,
         tblActionLogs.atlType = 'Token.Error',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", Err,
              "iToken", iToken,
              "iIP", iIP
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;

		
	SET IP = INET_ATON(iIP);
	CALL sp_SYSTEM_validateIPAccess(IP);

	SELECT tblAPITokens.aptID,
			   tblAPITokens.apt_usrID,
			   tblAPITokens.aptValidateIP,
			   tblAPITokens.aptAccessCount,
			   fnGetAllPrivs(tblAPITokens.apt_rolID, tblAPITokens.aptExtraPriviledges),
			   tblAPITokens.aptExpiryDate,
			   LOWER(tblRoles.rolName),
			   tblAPITokens.aptLang,
			   tblAPITokens.aptStatus
	  INTO TokenID,
	     	 UserID,
			   ValidateIP,
			   AccessCount,
			   Privs,
			   ExpiryDate,
			   RoleName,
			   PreferedLang,
			   LastStatus		
	  FROM tblAPITokens
			JOIN tblUser
				ON tblUser.usrID = tblAPITokens.apt_usrID
			JOIN tblRoles
				ON tblRoles.rolID = tblAPITokens.apt_rolID
	 WHERE	fnSanitizeToken(tblAPITokens.aptToken)  = fnSanitizeToken(iToken)
	   AND  (ISNULL(tblAPITokens.aptExpiryDate) OR tblAPITokens.aptExpiryDate = "2000-01-01" OR tblAPITokens.aptExpiryDate > now())
	   AND	tblAPITokens.aptStatus IN ('A', 'C')
	   AND	tblUser.usrStatus = 'A';	
	
	IF ISNULL(UserID) THEN
		SIGNAL SQLSTATE '45000'
	      SET MESSAGE_TEXT = '403:User blocked or Token Expired';
	END IF;	
	
	IF LastStatus = 'C' THEN 
		SIGNAL SQLSTATE '45000'
	      SET MESSAGE_TEXT = '402:Your credit has been finished please recharge it.';
	END IF;
	
	IF (ValidateIP = 1) THEN
		SET ValidateIP = NULL;
		SELECT 1 INTO ValidateIP 
			FROM tblAPITokenValidIPs 
		 WHERE tblAPITokenValidIPs.tviIP = IP 
		   AND tblAPITokenValidIPs.tvi_aptID = TokenID
		   AND tblAPITokenValidIPs.tviStatus = 'A';
			  
		IF (ISNULL(ValidateIP)) THEN
			SIGNAL SQLSTATE '45000'
		      SET MESSAGE_TEXT = '403:Token usage on %IP% is restricted.';
		END IF;	
	END IF;

	START TRANSACTION;
		UPDATE tblAPITokens
			SET tblAPITokens.aptAccessCount = tblAPITokens.aptAccessCount + 1,
	 	      tblAPITokens.aptLastActivity = now(),
	 			  tblAPITokens.aptExpiryDate = IF ((NOT ISNULL(ExpiryDate) AND ExpiryDate <= "2010-01-01"),
	 			 		DATE_ADD(CURDATE(),INTERVAL MONTH(ExpiryDate) * 30 + DAY(ExpiryDate) DAY),
	 			 		tblAPITokens.aptExpiryDate)
	 		WHERE tblAPITokens.aptID = TokenID;
				
		INSERT IGNORE INTO tblIPBin
		 	 SET tblIPBin.ipbIP = IP;
		
		INSERT INTO tblIPStats(
				tblIPStats.ips_ipbIP,
				tblIPStats.ipsTimeStamp
			)VALUES(
				IP,
				UNIX_TIMESTAMP(CURTIME(4))
			);
			
		SELECT 
			iToken       AS `token`,
			TokenID      AS `tokID`,
			UserID       AS `usrID`,
			iIP          AS `ip`,
			RoleName     AS `usrRole`,
			AccessCount  AS `tokAccessCount`,
			Privs        AS `privs`,
			PreferedLang AS `intfLang`;
	COMMIT;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_sessionActivity` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_sessionActivity`(
	IN `iSSID` CHAR(32),
	IN `iIP` VARCHAR(50)
)
BEGIN
  DECLARE SessionStatus CHAR(1);
  DECLARE UserStatus CHAR(1);
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE Err VARCHAR(500);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = UserID,
         tblActionLogs.atlType = 'Session.act',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", Err,
              "iSSID", iSSID,
              "iIP", iIP
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;

  SELECT tblActiveSessions.ssnStatus,
         tblActiveSessions.ssn_usrID, 
         tblUser.usrStatus 
    INTO SessionStatus,
         UserID,
         UserStatus
    FROM tblActiveSessions
    JOIN tblUser
      ON tblUser.usrID = tblActiveSessions.ssn_usrID
   WHERE tblActiveSessions.ssnKey = iSSID;
   
  IF ISNULL(SessionStatus) THEN
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Invalid Session';
  ELSEIF SessionStatus = 'E' THEN
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Session expired';
  ELSEIF SessionStatus = 'F' THEN
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401: You were fired out contact admin';
  ELSEIF SessionStatus = 'G' THEN
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401: You were logged out';
  ELSEIF UserStatus = 'B' THEN
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '405:User Blocked. Ask administrator';
  ELSEIF UserStatus = 'B' THEN
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '405:User Removed. Ask administrator';
  ELSEIF UserStatus != 'A' THEN
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '501:Invalid Session State';
  END IF;
  
  -- CHECK for same IP was discarded
  UPDATE tblActiveSessions
     SET tblActiveSessions.ssnLastActivity = NOW()
   WHERE tblActiveSessions.ssnKey = iSSID;
    
  SELECT tblUser.usrID,
         tblUser.usrName,
         tblUser.usrFamily,
         tblUser.usrEmail,
         tblUser.usr_rolID,
         tblUser.usrApprovalState,
         tblRoles.rolName,
         fnGetAllPrivs(tblUser.usr_rolID, tblUser.usrSpecialPrivs) AS privs,
         NOT ISNULL(tblUser.usrPass) AS hasPass,
         tblUser.usrStatus,
         iSSID AS ssnKey
    FROM tblUser
    JOIN tblRoles
      ON tblRoles.rolID = tblUser.usr_rolID
   WHERE tblUser.usrID = UserID; 
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_setSessionExpired` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_setSessionExpired`(
	IN `iSessionGUID` INT,
	IN `Param2` INT
)
BEGIN
    UPDATE tblActiveSessions
       SET ssnStatus = 'E'
     WHERE ssnKey = iSessionGUID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_verifyLoginByMobileCode` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_verifyLoginByMobileCode`(
	IN `iMobile` VARCHAR(50),
	IN `iCode` INT UNSIGNED,
	IN `iIP` VARCHAR(50),
	IN `iInfo` JSON,
	IN `iRemember` TINYINT
)
BEGIN
    DECLARE AprID BIGINT UNSIGNED;
    DECLARE UserID BIGINT UNSIGNED;
    DECLARE IsExpired BOOL;
    DECLARE SessionGUID CHAR(32);

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:DEPRECATED. USE sp_UPDATE_acceptApproval INSTEAD.';

    IF ISNULL(iMobile) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid mobile';
    END IF;

    SELECT tblApprovalRequest.aprID
         , tblApprovalRequest.apr_usrID
         , (tblApprovalRequest.aprStatus = 'E' OR TIMEDIFF(NOW(), tblApprovalRequest.aprSentDate) > "00:30:00")
      INTO AprID
         , UserID
         , IsExpired
      FROM tblApprovalRequest
     WHERE tblApprovalRequest.aprApprovalKey = iMobile
       AND tblApprovalRequest.aprIsForLogin = 1
       AND tblApprovalRequest.aprApprovalCode = iCode
       AND tblApprovalRequest.aprStatus IN ('S', '1', '2', 'E')
    ;

    IF ISNULL(AprID) THEN
        UPDATE tblApprovalRequest
           SET tblApprovalRequest.aprStatus = IF(tblApprovalRequest.aprStatus = 'S', '1', 
                                                  IF(tblApprovalRequest.aprStatus = '1', '2', 'E'))
         WHERE tblApprovalRequest.aprApprovalKey = iMobile
           AND tblApprovalRequest.aprIsForLogin = 1
           AND tblApprovalRequest.aprStatus IN ('S', '1', '2')
        ;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid mobile or code';
    END IF;

    IF IsExpired THEN 
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code expired';
    END IF;

    START TRANSACTION;

    UPDATE tblApprovalRequest
       SET tblApprovalRequest.aprApplyDate = NOW(),
           tblApprovalRequest.aprStatus = 'A'
     WHERE tblApprovalRequest.aprID = AprID
    ;

    SET SessionGUID = SUBSTRING(CommonFuncs.guid(NULL), 1, 32);

    INSERT
      INTO tblActiveSessions
       SET tblActiveSessions.ssnKey          = SessionGUID,
           tblActiveSessions.ssn_usrID       = UserID,
           tblActiveSessions.ssnIP           = INET_ATON(iIP),
           tblActiveSessions.ssnRemember     = iRemember,
           tblActiveSessions.ssnLastActivity = NOW(),
           tblActiveSessions.ssnInfo         = iInfo;

    UPDATE tblUser
       SET /*tblUser.usrMobile = NewKey,*/
           tblUser.usrApprovalState = IF(tblUser.usrApprovalState IN ('N','M'), 'M', 'A'),
           tblUser.usrStatus = IF(tblUser.usrStatus IN('A','V'), 'A', tblUser.usrStatus),
           tblUser.usrUpdatedBy_usrID = UserID,

           tblUser.usrLastLogin = NOW(),
           tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
     WHERE tblUser.usrID = UserID
    ;

    INSERT
      INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = UserID,
           tblActionLogs.atlType = 'UserLoggedIn'
    ;

    COMMIT;

    SELECT tblUser.usrID,
           tblUser.usrName,
           tblUser.usrFamily,
           tblUser.usrEmail,
           tblUser.usr_rolID,
           tblUser.usrApprovalState,
           tblRoles.rolName,
           fnGetAllPrivs(tblUser.usr_rolID, tblUser.usrSpecialPrivs) AS privs,
           NOT ISNULL(tblUser.usrPass) AS hasPass,
           tblUser.usrStatus,
           SessionGUID AS ssnKey
      FROM tblUser
      JOIN tblRoles
        ON tblRoles.rolID = tblUser.usr_rolID
     WHERE tblUser.usrID = UserID
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_UPDATE_voucher_cancel` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_UPDATE_voucher_cancel`(
	IN `iUserID` BIGINT UNSIGNED,
	IN `iVoucherID` BIGINT UNSIGNED,
	IN `iSetAsError` BOOL
)
BEGIN
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iUserID,
               tblActionLogs.atlType = 'cancelVoucher.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iVoucherID", iVoucherID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    UPDATE tblVoucher
       SET vchStatus = IF(iSetAsError, 'E', 'C')
     WHERE vchID = iVoucherID
    ;

    -- ///TODO: cancel voucher and credit to wallet

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

