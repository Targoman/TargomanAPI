/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping database structure for Common
DROP DATABASE IF EXISTS `Common`;
CREATE DATABASE IF NOT EXISTS `Common` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `Common`;

-- Dumping structure for table Common.tblAlerts
DROP TABLE IF EXISTS `tblAlerts`;
CREATE TABLE IF NOT EXISTS `tblAlerts` (
  `alrID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `alrType` char(1) NOT NULL DEFAULT 'I' COMMENT 'I: Informational, C: Critical, W:Warning',
  `alr_usrID` bigint unsigned NOT NULL,
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

-- Dumping data for table Common.tblAlerts: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAlerts` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAlerts` ENABLE KEYS */;

-- Dumping structure for table Common.tblAlertTemplates
DROP TABLE IF EXISTS `tblAlertTemplates`;
CREATE TABLE IF NOT EXISTS `tblAlertTemplates` (
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

-- Dumping data for table Common.tblAlertTemplates: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAlertTemplates` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAlertTemplates` ENABLE KEYS */;

-- Dumping structure for table Common.tblDBG
DROP TABLE IF EXISTS `tblDBG`;
CREATE TABLE IF NOT EXISTS `tblDBG` (
  `dbgID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `dbgFrom` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `dbgTimestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `dbgInfo` varchar(500) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`dbgID`),
  KEY `dbgTimestamp` (`dbgTimestamp`),
  KEY `dbgFrom` (`dbgFrom`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Common.tblDBG: 0 rows
/*!40000 ALTER TABLE `tblDBG` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblDBG` ENABLE KEYS */;

-- Dumping structure for table Common.tblDownloadRequests
DROP TABLE IF EXISTS `tblDownloadRequests`;
CREATE TABLE IF NOT EXISTS `tblDownloadRequests` (
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

-- Dumping data for table Common.tblDownloadRequests: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblDownloadRequests` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblDownloadRequests` ENABLE KEYS */;

-- Dumping structure for procedure Common.sp_AddDebugLog
DROP PROCEDURE IF EXISTS `sp_AddDebugLog`;
DELIMITER //
CREATE PROCEDURE `sp_AddDebugLog`(
	IN `iFrom` VARCHAR(50),
	IN `iInfo` VARCHAR(500)

)
BEGIN

  INSERT INTO Common.tblDBG 
     SET tblDBG.dbgFrom = iFrom,
         tblDBG.dbgInfo = iInfo; 

END//
DELIMITER ;

-- Dumping structure for function Common.fnCreateRandomMD5
DROP FUNCTION IF EXISTS `fnCreateRandomMD5`;
DELIMITER //
CREATE FUNCTION `fnCreateRandomMD5`() RETURNS char(32) CHARSET utf8mb4
    NO SQL
BEGIN
  RETURN MD5(CONCAT(RAND(), UUID()));
END//
DELIMITER ;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
