/* Migration File: m20220306_094432_init.sql */

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `Common` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `Common`;
DROP TABLE IF EXISTS `tblAlertTemplates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAlertTemplates` (
  `altlID` int unsigned NOT NULL AUTO_INCREMENT,
  `altCode` varchar(50) NOT NULL,
  `altMedia` char(1) NOT NULL DEFAULT 'E' COMMENT 'E:Email, S:SMS, P:Push, A:All',
  `altLanguage` varchar(2) NOT NULL,
  `altTitleTemplate` varchar(500) NOT NULL,
  `altBodyTemplate` text NOT NULL,
  PRIMARY KEY (`altlID`),
  UNIQUE KEY `ptlPurpose_ptlLanguage` (`altCode`,`altLanguage`,`altMedia`),
  KEY `ptlLanguage` (`altLanguage`),
  KEY `ptlPurpose` (`altCode`),
  KEY `altMedia` (`altMedia`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblAlerts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAlerts` (
  `alrID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `alrType` char(1) NOT NULL DEFAULT 'I' COMMENT 'I: Informational, C: Critical, W:Warning',
  `alr_usrID` bigint unsigned DEFAULT NULL,
  `alrEmail` varchar(50) DEFAULT NULL,
  `alrMobile` varchar(50) DEFAULT NULL,
  `alrReplacedContactInfo` varchar(50) DEFAULT NULL,
  `alr_altCode` varchar(50) NOT NULL,
  `alrReplacements` text NOT NULL,
  `alrCreateDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `alrSentDate` timestamp NULL DEFAULT NULL,
  `alrStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N:New, S: Sent',
  PRIMARY KEY (`alrID`),
  KEY `altType` (`alrType`),
  KEY `altStatus` (`alrStatus`),
  KEY `FK_tbl_COMMON_Alert_tbl_AAA_User` (`alr_usrID`),
  KEY `alr_altCode` (`alr_altCode`),
  KEY `alrCreateDate` (`alrCreateDate`),
  KEY `alrSentDate` (`alrSentDate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblDBG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblDBG` (
  `dbgID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `dbgFrom` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `dbgTimestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `dbgInfo` varchar(500) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`dbgID`),
  KEY `dbgTimestamp` (`dbgTimestamp`),
  KEY `dbgFrom` (`dbgFrom`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblDownloadRequests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblDownloadRequests` (
  `dwrID` int unsigned NOT NULL AUTO_INCREMENT,
  `dwr_ssnID` bigint unsigned NOT NULL,
  `dwrUUID` varchar(50) NOT NULL,
  `dwrCreationTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `dwrFilePath` varchar(200) NOT NULL,
  `dwrStatus` char(1) NOT NULL DEFAULT 'A',
  PRIMARY KEY (`dwrID`),
  KEY `dwrUUID` (`dwrUUID`),
  KEY `dwrStatus` (`dwrStatus`),
  KEY `dwrSSID` (`dwr_ssnID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 DROP FUNCTION IF EXISTS `fnCreateRandomMD5` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `fnCreateRandomMD5`() RETURNS char(32) CHARSET utf8mb4
    NO SQL
BEGIN
  RETURN MD5(CONCAT(RAND(), UUID()));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spLogDebug` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE PROCEDURE `spLogDebug`(
    IN `iFrom` VARCHAR(50),
    IN `iInfo` VARCHAR(500)
)
BEGIN
    INSERT
      INTO Common.tblDBG
       SET tblDBG.dbgFrom = iFrom
         , tblDBG.dbgInfo = iInfo
    ;
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
