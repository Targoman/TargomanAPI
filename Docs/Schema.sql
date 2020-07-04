-- --------------------------------------------------------
-- Host:                         192.168.0.240
-- Server version:               8.0.11 - MySQL Community Server - GPL
-- Server OS:                    Linux
-- HeidiSQL Version:             11.0.0.5919
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Dumping database structure for AAA
DROP DATABASE IF EXISTS `AAA`;
CREATE DATABASE IF NOT EXISTS `AAA` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `AAA`;

-- Dumping structure for table AAA.tblActionLogs
DROP TABLE IF EXISTS `tblActionLogs`;
CREATE TABLE IF NOT EXISTS `tblActionLogs` (
  `atlID` int(11) NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` int(10) unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `FK_tbl_AAA_ActionLog_tbl_AAA_User` (`atlBy_usrID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`),
  CONSTRAINT `FK_tbl_AAA_ActionLog_tbl_AAA_User` FOREIGN KEY (`atlBy_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblActionLogs: ~1,631 rows (approximately)
/*!40000 ALTER TABLE `tblActionLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActionLogs` ENABLE KEYS */;

-- Dumping structure for table AAA.tblActiveSessions
DROP TABLE IF EXISTS `tblActiveSessions`;
CREATE TABLE IF NOT EXISTS `tblActiveSessions` (
  `ssnKey` char(32) NOT NULL,
  `ssn_usrID` int(10) unsigned NOT NULL,
  `ssnIP` bigint(20) unsigned NOT NULL,
  `ssnIPReadable` varchar(50) GENERATED ALWAYS AS (inet_ntoa(`ssnIP`)) VIRTUAL,
  `ssnCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ssnInfo` json DEFAULT NULL,
  `ssnFingerPrint` varchar(50) DEFAULT NULL,
  `ssnLastActivity` datetime DEFAULT CURRENT_TIMESTAMP,
  `ssnRemember` bit(1) NOT NULL DEFAULT b'0',
  `ssnUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `ssnStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, G: LoggedOut, F: ForciblyLoggedOut, E: Expired',
  PRIMARY KEY (`ssnKey`),
  KEY `ssnStatus` (`ssnStatus`),
  KEY `ssnCreationDateTime` (`ssnCreationDateTime`),
  KEY `FK_tbl_AAA_ActiveSessions_tbl_AAA_User` (`ssn_usrID`),
  KEY `FK_tbl_AAA_ActiveSessions_tbl_AAA_User_1` (`ssnUpdatedBy_usrID`),
  KEY `ssnLastActivity` (`ssnLastActivity`),
  KEY `ssnRemember` (`ssnRemember`),
  KEY `ssnIP` (`ssnIP`),
  KEY `ssnFingerPrint` (`ssnFingerPrint`),
  CONSTRAINT `FK_tbl_AAA_ActiveSessions_tbl_AAA_User` FOREIGN KEY (`ssn_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tbl_AAA_ActiveSessions_tbl_AAA_User_1` FOREIGN KEY (`ssnUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblActiveSessions: ~92 rows (approximately)
/*!40000 ALTER TABLE `tblActiveSessions` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActiveSessions` ENABLE KEYS */;

-- Dumping structure for table AAA.tblAPITokens
DROP TABLE IF EXISTS `tblAPITokens`;
CREATE TABLE IF NOT EXISTS `tblAPITokens` (
  `aptID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `aptToken` varchar(50) NOT NULL,
  `apt_usrID` int(10) unsigned NOT NULL,
  `apt_svcID` int(10) unsigned NOT NULL,
  `aptLang` char(2) NOT NULL DEFAULT 'en',
  `aptValidateIP` bit(1) NOT NULL DEFAULT b'0',
  `aptExtraPriviledges` json DEFAULT NULL,
  `aptExpiryDate` date DEFAULT NULL,
  `aptLastActivity` datetime DEFAULT NULL,
  `aptAccessCount` bigint(20) unsigned NOT NULL DEFAULT '0',
  `aptCreatedBy_usrID` int(10) unsigned NOT NULL,
  `aptCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `aptUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `aptStatus` char(1) DEFAULT 'A' COMMENT 'A: Active, P: Pending, C: CreditFinished, R: Removed',
  PRIMARY KEY (`aptID`),
  UNIQUE KEY `aptToken` (`aptToken`),
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
  CONSTRAINT `FK_tblAPIToken_tblUser` FOREIGN KEY (`apt_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPIToken_tblUser_2` FOREIGN KEY (`aptCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPIToken_tblUser_3` FOREIGN KEY (`aptUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokens_tblServices` FOREIGN KEY (`apt_svcID`) REFERENCES `tblServices` (`svcid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblAPITokens: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAPITokens` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAPITokens` ENABLE KEYS */;

-- Dumping structure for table AAA.tblAPITokenValidIPs
DROP TABLE IF EXISTS `tblAPITokenValidIPs`;
CREATE TABLE IF NOT EXISTS `tblAPITokenValidIPs` (
  `tviID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `tvi_aptID` bigint(20) unsigned NOT NULL,
  `tviIP` bigint(20) NOT NULL,
  `tviIPReadable` varchar(50) GENERATED ALWAYS AS (inet_ntoa(`tviIP`)) VIRTUAL,
  `tviCreatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `tviCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `tviUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `tviStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`tviID`),
  UNIQUE KEY `tvi_aptID_tviIP` (`tvi_aptID`,`tviIP`),
  KEY `tviStatus` (`tviStatus`),
  KEY `tviIP` (`tviIP`),
  KEY `tviCreatedBy_usrID` (`tviCreatedBy_usrID`),
  KEY `tviCreationDateTime` (`tviCreationDateTime`),
  KEY `FK_tblAPITokenValidIPs_tblUser_2` (`tviUpdatedBy_usrID`),
  CONSTRAINT `FK_tblAPITokenValidIPs_tblAPIToken` FOREIGN KEY (`tvi_aptID`) REFERENCES `tblAPITokens` (`aptid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokenValidIPs_tblUser` FOREIGN KEY (`tviCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblAPITokenValidIPs_tblUser_2` FOREIGN KEY (`tviUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblAPITokenValidIPs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAPITokenValidIPs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAPITokenValidIPs` ENABLE KEYS */;

-- Dumping structure for table AAA.tblApprovalRequest
DROP TABLE IF EXISTS `tblApprovalRequest`;
CREATE TABLE IF NOT EXISTS `tblApprovalRequest` (
  `aprID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `apr_usrID` int(11) unsigned NOT NULL,
  `aprRequestedFor` char(1) NOT NULL DEFAULT '',
  `aprApprovalCode` varchar(50) NOT NULL DEFAULT '',
  `aprApprovalValue` varchar(50) NOT NULL DEFAULT '',
  `aprRequestDate` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `aprApplyDate` datetime DEFAULT NULL,
  `aprStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2:SecondTry, E: Expired',
  PRIMARY KEY (`aprID`),
  UNIQUE KEY `aprApprovalValue_fprStatus` (`aprApprovalValue`,`aprStatus`,`aprApprovalCode`),
  KEY `fprStatus` (`aprStatus`),
  KEY `aprRequestedFor` (`aprRequestedFor`),
  KEY `FK_tblApprovalRequest_tblUser` (`apr_usrID`),
  KEY `aprRequestDate` (`aprRequestDate`),
  KEY `aprApplyDate` (`aprApplyDate`),
  CONSTRAINT `FK_tblApprovalRequest_tblUser` FOREIGN KEY (`apr_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=140 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Dumping data for table AAA.tblApprovalRequest: ~17 rows (approximately)
/*!40000 ALTER TABLE `tblApprovalRequest` DISABLE KEYS */;
INSERT INTO `tblApprovalRequest` (`aprID`, `apr_usrID`, `aprRequestedFor`, `aprApprovalCode`, `aprApprovalValue`, `aprRequestDate`, `aprApplyDate`, `aprStatus`) VALUES
	(50, 349, 'E', 'ec8c2cb2c4759a4dd127a93cd5a8bca5', 'asd@bsd.ir', '2020-02-10 13:32:12', NULL, 'N'),
	(51, 351, 'E', '8f7ea16a2a8526ecbfbd8b8e480c60d3', 'asd2@bsd.ir', '2020-02-10 13:54:21', NULL, 'N'),
	(53, 100, 'E', 'ac44bc6041ed1252d36f5a591d15542b', 'mehran@gmail.com', '2020-02-18 07:23:00', NULL, 'N'),
	(54, 100, 'E', 'a53692e228cc8d22496c5ef520835fbc', '09126174250', '2020-02-18 07:23:02', NULL, 'N'),
	(56, 100, 'E', '2db35df0127801cb2f7c8b44c6061d29', 'mehran@gmail.com', '2020-02-18 07:28:25', NULL, 'N'),
	(57, 100, 'E', '03c844f7b6325baba5a6099c8c05291d', '09126174250', '2020-02-18 07:28:25', NULL, 'N'),
	(58, 100, 'E', '70113602d23521c197a9b844ad3bf419', 'mehran@gmail.com', '2020-02-18 07:28:33', NULL, 'N'),
	(59, 100, 'E', '6a733130908d406516b1770ea08c1fad', '09126174250', '2020-02-18 07:28:33', NULL, 'N'),
	(60, 100, 'E', 'c6b2eb5d0c30dc56803fdafaa6d843ba', 'mehran@gmail.com', '2020-02-18 07:28:37', NULL, 'N'),
	(61, 100, 'E', 'ae1a193c9c4f366fe14713842eb3fa3f', '09126174250', '2020-02-18 07:28:38', NULL, 'N'),
	(96, 376, 'E', '3ced8b2b1516bd10fb399c5ac37fbb87', 'customer@ctp.ir', '2020-06-09 12:09:56', NULL, 'N'),
	(97, 377, 'E', '2e43e91b86ba57f73b3da4d3ffc5053b', 'sir.ri4n@gmail.com', '2020-06-10 07:03:01', NULL, 'S'),
	(98, 379, 'E', 'c163dd8ed41a818bebf62d0087386372', 'wasddasdasdasd@gmail.com', '2020-06-10 12:07:04', NULL, 'N'),
	(99, 380, 'E', '622612078bd6ee09c06c68397eadeebf', 'casdasdustomer@ctp.ir', '2020-06-10 12:42:01', '2020-06-10 12:42:45', 'A'),
	(100, 381, 'E', '31042051fac18c09ba689f9f3dc82e50', 'kvlsoj@gmail.com', '2020-06-13 06:59:15', NULL, 'N'),
	(138, 404, 'E', 'f7698f77f9b74c0eeb2a51da0adac408', 'unit_test@unittest.test', '2020-07-04 12:35:27', '2020-07-04 12:35:28', 'S'),
	(139, 404, 'M', '34492', '09121234567', '2020-07-04 12:35:30', '2020-07-04 12:35:31', 'A');
/*!40000 ALTER TABLE `tblApprovalRequest` ENABLE KEYS */;

-- Dumping structure for table AAA.tblBlockingRules
DROP TABLE IF EXISTS `tblBlockingRules`;
CREATE TABLE IF NOT EXISTS `tblBlockingRules` (
  `blrID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `blr_ipbIP` bigint(20) unsigned DEFAULT NULL,
  `blr_ipIPReadable` varchar(50) GENERATED ALWAYS AS (inet_ntoa(`blr_ipbIP`)) VIRTUAL,
  `blrStartingTime` timestamp NULL DEFAULT NULL,
  `blrEndingTime` timestamp NULL DEFAULT NULL,
  `blrCause` varchar(500) DEFAULT NULL,
  `blrCreatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `blrCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `blrUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `blrStatus` char(1) DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`blrID`),
  KEY `blaStatus` (`blrStatus`),
  KEY `blaStartingTime` (`blrStartingTime`),
  KEY `blaEndingTime` (`blrEndingTime`),
  KEY `FK_tblBlockingsApplied_tblIPBin` (`blr_ipbIP`),
  KEY `blrInsertedBy_usrID` (`blrCreatedBy_usrID`),
  KEY `blrInsertionDate` (`blrCreationDateTime`),
  KEY `FK_tbl_AAA_BlockingRules_tblUser_1` (`blrUpdatedBy_usrID`),
  CONSTRAINT `FK_tblBlockingsApplied_tblIPBin` FOREIGN KEY (`blr_ipbIP`) REFERENCES `tblIPBin` (`ipbip`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tbl_AAA_BlockingRules_tblUser` FOREIGN KEY (`blrCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tbl_AAA_BlockingRules_tblUser_1` FOREIGN KEY (`blrUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table AAA.tblBlockingRules: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblBlockingRules` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblBlockingRules` ENABLE KEYS */;

-- Dumping structure for table AAA.tblForgotPassRequest
DROP TABLE IF EXISTS `tblForgotPassRequest`;
CREATE TABLE IF NOT EXISTS `tblForgotPassRequest` (
  `fprUUID` char(36) NOT NULL,
  `fpr_usrID` int(11) unsigned NOT NULL,
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
  CONSTRAINT `FK_tbl_AAA_ForgotPassRequests_tbl_AAA_User` FOREIGN KEY (`fpr_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table AAA.tblForgotPassRequest: ~8 rows (approximately)
/*!40000 ALTER TABLE `tblForgotPassRequest` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblForgotPassRequest` ENABLE KEYS */;

-- Dumping structure for table AAA.tblIPBin
DROP TABLE IF EXISTS `tblIPBin`;
CREATE TABLE IF NOT EXISTS `tblIPBin` (
  `ipbIP` bigint(20) unsigned NOT NULL,
  `ipbReadable` varchar(50) GENERATED ALWAYS AS (inet_ntoa(`ipbIP`)) VIRTUAL,
  `ipbFirstAccess` datetime DEFAULT CURRENT_TIMESTAMP,
  `ipbAccessCount` bigint(20) unsigned NOT NULL DEFAULT '0',
  `ipbLastAccess` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `ipbBlockedBy_usrID` int(10) unsigned DEFAULT NULL,
  `ipbBlockingTime` datetime DEFAULT NULL,
  `ipbStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, B: Blocked, R: Removed',
  UNIQUE KEY `ipsIP` (`ipbIP`),
  KEY `ipbAccesCount` (`ipbAccessCount`),
  KEY `ipbLastAccess` (`ipbLastAccess`),
  KEY `FK_tbl_AAA_IPBin_tbl_AAA_User` (`ipbBlockedBy_usrID`),
  KEY `ipbInsertionDateTime` (`ipbFirstAccess`),
  KEY `ipbStatus` (`ipbStatus`),
  KEY `ipbBlockingTime` (`ipbBlockingTime`),
  CONSTRAINT `FK_tblIPBin_tblUser` FOREIGN KEY (`ipbBlockedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table AAA.tblIPBin: ~3 rows (approximately)
/*!40000 ALTER TABLE `tblIPBin` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblIPBin` ENABLE KEYS */;

-- Dumping structure for table AAA.tblIPStats
DROP TABLE IF EXISTS `tblIPStats`;
CREATE TABLE IF NOT EXISTS `tblIPStats` (
  `ips_ipbIP` bigint(20) unsigned NOT NULL,
  `ipsTimeStamp` double unsigned NOT NULL,
  `ipsInsertionDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  UNIQUE KEY `ips_ipbID_ipsInsertionDate` (`ips_ipbIP`,`ipsTimeStamp`),
  KEY `ipsTimeStamp` (`ipsTimeStamp`),
  CONSTRAINT `FK_tblIPStats_tblIPBin` FOREIGN KEY (`ips_ipbIP`) REFERENCES `tblIPBin` (`ipbip`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci ROW_FORMAT=COMPACT;

-- Dumping data for table AAA.tblIPStats: ~278 rows (approximately)
/*!40000 ALTER TABLE `tblIPStats` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblIPStats` ENABLE KEYS */;

-- Dumping structure for table AAA.tblOfflinePayments
DROP TABLE IF EXISTS `tblOfflinePayments`;
CREATE TABLE IF NOT EXISTS `tblOfflinePayments` (
  `ofpID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ofp_vchID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `ofpBank` varchar(50) NOT NULL,
  `ofpReceiptCode` varchar(50) NOT NULL,
  `ofpReceiptDate` varchar(50) NOT NULL,
  `ofpAmount` int(10) unsigned NOT NULL,
  `ofpNotes` varchar(500) NOT NULL DEFAULT '',
  `ofpCreatedBy_usrID` int(10) unsigned NOT NULL,
  `ofpCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ofpUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `ofpStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, A: Succeded, E: Error, R: Removed',
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
  CONSTRAINT `FK_tblOfflinePayments_tblUser` FOREIGN KEY (`ofpCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblOfflinePayments_tblUser_2` FOREIGN KEY (`ofpUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblOfflinePayments: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblOfflinePayments` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblOfflinePayments` ENABLE KEYS */;

-- Dumping structure for table AAA.tblOnlinePayments
DROP TABLE IF EXISTS `tblOnlinePayments`;
CREATE TABLE IF NOT EXISTS `tblOnlinePayments` (
  `onpID` bigint(20) NOT NULL AUTO_INCREMENT,
  `onpMD5` char(32) NOT NULL,
  `onpCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `onp_vchID` bigint(20) unsigned NOT NULL,
  `onpPaymentGateway` char(1) NOT NULL DEFAULT '',
  `onpPGTrnID` varchar(50) NOT NULL,
  `onpAmount` bigint(20) NOT NULL,
  `onpLastUpdateDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `onpStatus` char(1) NOT NULL DEFAULT 'P' COMMENT 'N: New, P: Pending, Y: Payed, A: Succeded, E: Error, R: Removed',
  `onpResult` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`onpID`),
  UNIQUE KEY `onpMD5` (`onpMD5`),
  KEY `onpRequestBy` (`onp_vchID`),
  KEY `onpBankTrnID` (`onpPGTrnID`),
  KEY `onpStatus` (`onpStatus`),
  KEY `onpCreationDateTime` (`onpCreationDateTime`),
  KEY `onpAmount` (`onpAmount`),
  KEY `onpBank` (`onpPaymentGateway`),
  KEY `onpLastUpdateDateTime` (`onpLastUpdateDateTime`),
  CONSTRAINT `FK_tblBankPaymentOrder_tblInvoice` FOREIGN KEY (`onp_vchID`) REFERENCES `tblVoucher` (`vchID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblOnlinePayments: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblOnlinePayments` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblOnlinePayments` ENABLE KEYS */;

-- Dumping structure for table AAA.tblRoles
DROP TABLE IF EXISTS `tblRoles`;
CREATE TABLE IF NOT EXISTS `tblRoles` (
  `rolID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rolName` varchar(50) NOT NULL,
  `rolParent_rolID` int(10) unsigned DEFAULT NULL,
  `rolPrivileges` json DEFAULT NULL,
  `rolSignupAllowedIPs` varchar(500) DEFAULT NULL,
  `rolCreatedBy_usrID` int(10) unsigned NOT NULL,
  `rolCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `rolUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `rolStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, B: Blocked, R: Removed',
  PRIMARY KEY (`rolID`),
  UNIQUE KEY `rolName` (`rolName`),
  KEY `rolCreationDateTime` (`rolCreationDateTime`),
  KEY `rolStatus` (`rolStatus`),
  KEY `FK_tblRoles_tblRoles` (`rolParent_rolID`),
  KEY `FK_tblRoles_tblUser` (`rolCreatedBy_usrID`),
  KEY `FK_tblRoles_tblUser_2` (`rolUpdatedBy_usrID`),
  CONSTRAINT `FK_tblRoles_tblRoles` FOREIGN KEY (`rolParent_rolID`) REFERENCES `tblRoles` (`rolid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblRoles_tblUser` FOREIGN KEY (`rolCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblRoles_tblUser_2` FOREIGN KEY (`rolUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=61 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblRoles: ~4 rows (approximately)
/*!40000 ALTER TABLE `tblRoles` DISABLE KEYS */;
INSERT INTO `tblRoles` (`rolID`, `rolName`, `rolParent_rolID`, `rolPrivileges`, `rolSignupAllowedIPs`, `rolCreatedBy_usrID`, `rolCreationDateTime`, `rolUpdatedBy_usrID`, `rolStatus`) VALUES
	(1, 'System', NULL, NULL, '0', 1, '2020-02-10 11:06:58', NULL, 'A'),
	(2, 'BaseUser', NULL, NULL, NULL, 1, '2020-02-10 11:08:26', NULL, 'A'),
	(3, 'Administrator', NULL, '{"ALL": 1}', NULL, 1, '2020-02-10 11:27:58', NULL, 'A'),
	(14, 'unitTest', NULL, NULL, NULL, 1, '2020-02-19 13:05:38', NULL, 'A');
/*!40000 ALTER TABLE `tblRoles` ENABLE KEYS */;

-- Dumping structure for table AAA.tblServices
DROP TABLE IF EXISTS `tblServices`;
CREATE TABLE IF NOT EXISTS `tblServices` (
  `svcID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `svcName` varchar(50) NOT NULL,
  `svc_rolID` int(10) unsigned NOT NULL,
  `svcCreatedBy_usrID` int(10) unsigned NOT NULL,
  `svcCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `svcUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `svcStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R:Removed',
  PRIMARY KEY (`svcID`),
  UNIQUE KEY `svcName` (`svcName`),
  KEY `svcCreationDateTime` (`svcCreationDateTime`),
  KEY `svcStatus` (`svcStatus`),
  KEY `FK_tblServices_tblUser` (`svcCreatedBy_usrID`),
  KEY `FK_tblServices_tblUser_2` (`svcUpdatedBy_usrID`),
  KEY `FK_tblServices_tblRoles` (`svc_rolID`),
  CONSTRAINT `FK_tblServices_tblRoles` FOREIGN KEY (`svc_rolID`) REFERENCES `tblRoles` (`rolid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblServices_tblUser` FOREIGN KEY (`svcCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblServices_tblUser_2` FOREIGN KEY (`svcUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblServices: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblServices` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblServices` ENABLE KEYS */;

-- Dumping structure for table AAA.tblUser
DROP TABLE IF EXISTS `tblUser`;
CREATE TABLE IF NOT EXISTS `tblUser` (
  `usrID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `usrEmail` varchar(100) DEFAULT NULL,
  `usrName` varchar(100) DEFAULT NULL,
  `usrFamily` varchar(100) DEFAULT NULL,
  `usrGender` char(1) NOT NULL DEFAULT 'N' COMMENT 'F: Female, M:Male, N: Not expressed',
  `usrMobile` varchar(100) DEFAULT NULL,
  `usrApprovalState` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: Not Approved, M: JustMobile, E:JustEmail, A: All',
  `usrPass` char(32) DEFAULT NULL,
  `usr_rolID` int(10) unsigned DEFAULT NULL,
  `usrSpecialPrivs` json DEFAULT NULL,
  `usrLanguage` char(2) NOT NULL DEFAULT 'fa',
  `usrMaxSessions` tinyint(4) NOT NULL DEFAULT '-1',
  `usrActiveSessions` mediumint(9) NOT NULL DEFAULT '0',
  `usrLastLogin` datetime DEFAULT NULL,
  `usrCreatedBy_usrID` int(10) unsigned NOT NULL,
  `usrCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `usrUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `usrStatus` char(1) NOT NULL DEFAULT 'V' COMMENT 'A: Active, R: Removed, B: Banned, C: Must Change Pass, V:Must validate',
  PRIMARY KEY (`usrID`),
  UNIQUE KEY `usrEmail` (`usrEmail`,`usrStatus`),
  UNIQUE KEY `usrMobile` (`usrMobile`,`usrStatus`),
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
  CONSTRAINT `FK_tblUser_tblRoles` FOREIGN KEY (`usr_rolID`) REFERENCES `tblRoles` (`rolid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUser_tblUser` FOREIGN KEY (`usrCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUser_tblUser_2` FOREIGN KEY (`usrUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=405 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblUser: ~10 rows (approximately)
/*!40000 ALTER TABLE `tblUser` DISABLE KEYS */;
INSERT INTO `tblUser` (`usrID`, `usrEmail`, `usrName`, `usrFamily`, `usrGender`, `usrMobile`, `usrApprovalState`, `usrPass`, `usr_rolID`, `usrSpecialPrivs`, `usrLanguage`, `usrMaxSessions`, `usrActiveSessions`, `usrLastLogin`, `usrCreatedBy_usrID`, `usrCreationDateTime`, `usrUpdatedBy_usrID`, `usrStatus`) VALUES
	(1, 'system@local.host', 'System', 'System', 'N', '', 'N', NULL, 1, NULL, 'fa', -1, 0, NULL, 1, '2019-03-20 17:45:54', NULL, 'B'),
	(100, 'mehran@gmail.com', 'مهران', 'ضیابری', 'M', '09126174250', 'A', '81dc9bdb52d04dc20036dbd8313ed055', 3, NULL, 'fa', -1, 215, '2020-06-16 11:58:39', 1, '2019-08-21 15:47:28', NULL, 'A'),
	(349, 'asd@bsd.ir', NULL, NULL, 'N', NULL, 'N', 'd41d8cd98f00b204e9800998ecf8427e', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-02-10 13:32:12', NULL, 'V'),
	(351, 'asd2@bsd.ir', NULL, NULL, 'N', NULL, 'N', 'e2fc714c4727ee9395f324cd2e7f331f', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-02-10 13:54:21', NULL, 'V'),
	(376, 'customer@ctp.ir', 'fgh', 'fghfg', 'N', NULL, 'N', 'f84187ce2bf4e04c3184c92dba10a1ee', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-06-09 12:09:56', NULL, 'V'),
	(377, 'sir.ri4n@gmail.com', 'arian', 'atefi', 'N', NULL, 'N', 'e10adc3949ba59abbe56e057f20f883e', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-06-10 07:03:01', NULL, 'V'),
	(379, 'wasddasdasdasd@gmail.com', 'foodo', '78i', 'N', NULL, 'N', 'e10adc3949ba59abbe56e057f20f883e', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-06-10 12:07:04', NULL, 'V'),
	(380, 'casdasdustomer@ctp.ir', 'ewwer', 'werwer', 'N', NULL, 'E', 'e10adc3949ba59abbe56e057f20f883e', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-06-10 12:42:01', 380, 'A'),
	(381, 'kvlsoj@gmail.com', 'sdasf', 'asfasfasfasf', 'N', NULL, 'N', 'e10adc3949ba59abbe56e057f20f883e', 2, NULL, 'fa', -1, 0, NULL, 1, '2020-06-13 06:59:15', NULL, 'V'),
	(404, 'unit_test@unittest.test', 'unit', 'test', 'N', '09121234567', 'A', 'df6d2338b2b8fce1ec2f6dda0a630eb0', 14, NULL, 'fa', -1, 1, '2020-07-04 12:35:29', 1, '2020-07-04 12:35:27', 404, 'A');
/*!40000 ALTER TABLE `tblUser` ENABLE KEYS */;

-- Dumping structure for table AAA.tblUserExtraInfo
DROP TABLE IF EXISTS `tblUserExtraInfo`;
CREATE TABLE IF NOT EXISTS `tblUserExtraInfo` (
  `uei_usrID` int(11) unsigned NOT NULL,
  `ueiSex` char(1) NOT NULL DEFAULT '',
  `ueiExtraInfo` json DEFAULT NULL,
  `ueiSheba` varchar(50) DEFAULT NULL,
  `ueiPhoto` longtext,
  `ueiOAuthAccounts` json DEFAULT NULL,
  `ueiUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`uei_usrID`),
  KEY `FK_tbl_AAA_UserExtraInfo_tbl_AAA_User_1` (`ueiUpdatedBy_usrID`),
  CONSTRAINT `FK_tbl_AAA_UserExtraInfo_tbl_AAA_User` FOREIGN KEY (`uei_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tbl_AAA_UserExtraInfo_tbl_AAA_User_1` FOREIGN KEY (`ueiUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblUserExtraInfo: ~1 rows (approximately)
/*!40000 ALTER TABLE `tblUserExtraInfo` DISABLE KEYS */;
INSERT INTO `tblUserExtraInfo` (`uei_usrID`, `ueiSex`, `ueiExtraInfo`, `ueiSheba`, `ueiPhoto`, `ueiOAuthAccounts`, `ueiUpdatedBy_usrID`) VALUES
	(100, '', NULL, NULL, 'data:image/jpeg;base64,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HuASp0hprpHR4AhSTLBSc0QxOgG5Ss/USWasQshMoZiExoOi8CNIVtZFwZE2lpgNpwDUHcaAWK3MVx7DUWMVma8ik4nQ8SRqyO6a17OF1sVIfFknQaq2vgW74upX0ZCUSa2xeDQjHKNRUYq5WoyTNv4FHH8EpSXD1ieslql8dBfbTsGNaOxsB+xlP//aAAwDAQACAAMAAAAQSS222y22223/ANttts+2kktttttttttthkttltskltttsttttv8A7bbErtt5ZJZJLbbbbbbDbbbbbZJLKbbbbbbbbbbZItsr/nIL+TJJbbbbbbbBbbbJJZRZbbbbDbbbbZS1PNKPLZ0bLLbbbbbbDZACZJLbZbbbbTbbbbZT5R9UxIbnJLCJJbbbbbbbbbJJbbbbbf8A22/6SyM36bzV/wDwIK0G+m3tttttttkktttttv8A5bdtJRb5WldT+JTtzFKol9JbbbbbbJJbbbbbflbb9paKfpkiOGUN/qqhGwtpbbbbbbZJJZbZLYX7ZdtbobdwCL79d3ucbRJypQ7bbbbLJJLLaJbDbZJv0Bg4kBMNTRNLtMn71i/xLJbbbZJLbbbbYbbZPtvIwViTcBJP9/WMKYlV+JJLbbbLbbbbbbD99r9TCQgJQ2D3fUdii2rNlX9JLbbbZbbbbbbY/ts2zpd5sGTP34saLpjGkRUNBJJbbbLbbbbbbf8Aba3iW6cAUHf4TSypIGtK10/ySW222WW2220G/dbxK/8AaxPoGWXPX5Hmj0NRVvlksltsklttttt++VXnErNE439S6rteI/efwrOEkklttkktttptn2z0Nu+gaX7e1xEMkAG+OpwA2+8sllskltthtsZwK3l0eWUn32aD70iX/wBPyB1lthZJLJJLbbDb8X8/nn0+NGiSk2A5aPou/umuzHjBLbJJJbbbDf3QlFS4t6xrx4sxFZxrsv8Abv0jvCyWyySS2222/wDZm4yX3AMoj3JJg/T2uVe/ynUVUktskkltthtspqB55ITH+8hhyJrPul1ivvazwikkkkksttsNtqG/JMgs70bAY31wonNm7TRl70jUklkklttthtstZu56Ag/jZOAyuos/s3zbsjoUAkltkltttsN1v0EpFoctuZKZ4PLkxDVmuETEUbUvluktttths20XFMICv35aZQynvF5tA/g7zBCazVtm1tttsNv5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', NULL, NULL);
/*!40000 ALTER TABLE `tblUserExtraInfo` ENABLE KEYS */;

-- Dumping structure for table AAA.tblUserWallets
DROP TABLE IF EXISTS `tblUserWallets`;
CREATE TABLE IF NOT EXISTS `tblUserWallets` (
  `walID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `wal_usrID` int(10) unsigned NOT NULL,
  `walName` varchar(100) NOT NULL DEFAULT 'default',
  `walDefault` bit(1) NOT NULL DEFAULT b'0',
  `walMinBalance` bigint(20) NOT NULL DEFAULT '0',
  `walNotTransferable` bigint(20) NOT NULL DEFAULT '0',
  `walMaxTransferPerDay` bigint(20) NOT NULL DEFAULT '10000000',
  `walLastBalance` bigint(20) NOT NULL DEFAULT '0',
  `walSumIncome` bigint(20) NOT NULL DEFAULT '0',
  `walSumExpenses` bigint(20) NOT NULL DEFAULT '0',
  `walSumCredit` bigint(20) NOT NULL DEFAULT '0',
  `walSumDebit` bigint(20) NOT NULL DEFAULT '0',
  `walCreatedBy_usrID` int(10) unsigned NOT NULL,
  `walCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `walUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `walStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, D: Deactive, R: Removed',
  PRIMARY KEY (`walID`),
  UNIQUE KEY `wal_usrID_walName` (`wal_usrID`,`walName`),
  KEY `walStatus` (`walStatus`),
  KEY `walCreationDateTime` (`walCreationDateTime`),
  KEY `FK_tblUserWallets_tblUser` (`walCreatedBy_usrID`),
  KEY `FK_tblUserWallets_tblUser_2` (`walUpdatedBy_usrID`),
  CONSTRAINT `FK__tblUser` FOREIGN KEY (`wal_usrID`) REFERENCES `tblUser` (`usrid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUserWallets_tblUser` FOREIGN KEY (`walCreatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblUserWallets_tblUser_2` FOREIGN KEY (`walUpdatedBy_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=58 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblUserWallets: ~9 rows (approximately)
/*!40000 ALTER TABLE `tblUserWallets` DISABLE KEYS */;
INSERT INTO `tblUserWallets` (`walID`, `wal_usrID`, `walName`, `walDefault`, `walMinBalance`, `walNotTransferable`, `walMaxTransferPerDay`, `walLastBalance`, `walSumIncome`, `walSumExpenses`, `walSumCredit`, `walSumDebit`, `walCreatedBy_usrID`, `walCreationDateTime`, `walUpdatedBy_usrID`, `walStatus`) VALUES
	(3, 100, 'default', b'1', 0, 0, 0, 0, 0, 0, 0, 0, 100, '2020-01-26 12:10:11', NULL, 'A'),
	(5, 349, 'Default', b'1', 0, 0, 0, 0, 0, 0, 0, 0, 1, '2020-02-10 13:32:12', NULL, 'A'),
	(6, 351, 'Default', b'1', 0, 0, 0, 0, 0, 0, 0, 0, 1, '2020-02-10 13:54:21', NULL, 'A'),
	(31, 376, 'Default', b'1', 0, 0, 10000000, 0, 0, 0, 0, 0, 1, '2020-06-09 12:09:56', NULL, 'A'),
	(32, 377, 'Default', b'1', 0, 0, 10000000, 0, 0, 0, 0, 0, 1, '2020-06-10 07:03:01', NULL, 'A'),
	(33, 379, 'Default', b'1', 0, 0, 10000000, 0, 0, 0, 0, 0, 1, '2020-06-10 12:07:04', NULL, 'A'),
	(34, 380, 'Default', b'1', 0, 0, 10000000, 0, 0, 0, 0, 0, 1, '2020-06-10 12:42:01', NULL, 'A'),
	(35, 381, 'Default', b'1', 0, 0, 10000000, 0, 0, 0, 0, 0, 1, '2020-06-13 06:59:15', NULL, 'A'),
	(57, 404, 'Default', b'1', 0, 0, 10000000, 0, 0, 0, 0, 0, 1, '2020-07-04 12:35:28', NULL, 'A');
/*!40000 ALTER TABLE `tblUserWallets` ENABLE KEYS */;

-- Dumping structure for table AAA.tblVoucher
DROP TABLE IF EXISTS `tblVoucher`;
CREATE TABLE IF NOT EXISTS `tblVoucher` (
  `vchID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `vchCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `vch_usrID` int(11) unsigned NOT NULL,
  `vchDesc` varchar(500) NOT NULL,
  `vchType` char(1) NOT NULL COMMENT 'W:Withdrawal, E: Expense, I: Income, C:Credit, T:TransferTo, F: TransferFrom, Z: Prize',
  `vchTotalAmount` bigint(20) NOT NULL,
  `vchStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, C: Canceled, F: Finished, R: Removed',
  PRIMARY KEY (`vchID`),
  KEY `vchStatus` (`vchStatus`),
  KEY `vchPaymentType` (`vchType`),
  KEY `vchCreationDateTime` (`vchCreationDateTime`),
  KEY `FK_tblVoucher_tblUser` (`vch_usrID`),
  CONSTRAINT `FK_tblVoucher_tblUser` FOREIGN KEY (`vch_usrID`) REFERENCES `tblUser` (`usrid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table AAA.tblVoucher: ~10 rows (approximately)
/*!40000 ALTER TABLE `tblVoucher` DISABLE KEYS */;
INSERT INTO `tblVoucher` (`vchID`, `vchCreationDateTime`, `vch_usrID`, `vchDesc`, `vchType`, `vchTotalAmount`, `vchStatus`) VALUES
	(4, '2020-02-18 09:28:11', 100, '{\n    "walletID": 3\n}\n', 'W', 123, 'N'),
	(5, '2020-02-18 09:28:18', 100, '{\n    "walletID": 3\n}\n', 'W', 123, 'N'),
	(6, '2020-02-19 09:44:31', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(7, '2020-02-19 09:46:57', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(8, '2020-02-19 09:47:00', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(9, '2020-02-19 09:47:06', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(10, '2020-02-19 09:47:21', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(11, '2020-02-19 09:47:23', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(12, '2020-02-19 09:47:25', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N'),
	(13, '2020-02-19 09:47:49', 100, '{\n    "walletID": 3\n}\n', 'W', 1231, 'N');
/*!40000 ALTER TABLE `tblVoucher` ENABLE KEYS */;

-- Dumping structure for table AAA.tblWalletBalances
DROP TABLE IF EXISTS `tblWalletBalances`;
CREATE TABLE IF NOT EXISTS `tblWalletBalances` (
  `wbl_wltID` bigint(20) unsigned NOT NULL,
  `wblBalance` bigint(20) NOT NULL,
  `wblSumIncome` bigint(20) NOT NULL,
  `wblSumExpense` bigint(20) NOT NULL,
  `wblSumCredit` bigint(20) NOT NULL,
  `wblSumDebit` bigint(20) NOT NULL,
  PRIMARY KEY (`wbl_wltID`),
  CONSTRAINT `FK_tblWalletBalances_tblWalletsTransactions` FOREIGN KEY (`wbl_wltID`) REFERENCES `tblWalletsTransactions` (`wltid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_persian_ci ROW_FORMAT=DYNAMIC;

-- Dumping data for table AAA.tblWalletBalances: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblWalletBalances` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblWalletBalances` ENABLE KEYS */;

-- Dumping structure for table AAA.tblWalletsTransactions
DROP TABLE IF EXISTS `tblWalletsTransactions`;
CREATE TABLE IF NOT EXISTS `tblWalletsTransactions` (
  `wltID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `wlt_walID` bigint(20) unsigned NOT NULL,
  `wlt_vchID` bigint(20) unsigned NOT NULL,
  `wltDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `wlt_vchType` char(1) NOT NULL COMMENT 'W:Withdrawal, E: Expense, I: Income, C:Credit, T:TransferTo, F: TransferFrom, Z: Prize',
  `wltAmount` bigint(20) NOT NULL,
  `wltStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, P: Pending, Y: Payed, A: Succeded, E: Error, R: Removed',
  PRIMARY KEY (`wltID`),
  KEY `walStatus` (`wltStatus`),
  KEY `walCreationDateTime` (`wltDateTime`),
  KEY `FK_tblWalletsTransactions_tblUserWallets` (`wlt_walID`),
  KEY `FK_tblWalletsTransactions_tblInvoice` (`wlt_vchID`),
  KEY `wltType` (`wlt_vchType`),
  CONSTRAINT `FK_tblWalletsTransactions_tblInvoice` FOREIGN KEY (`wlt_vchID`) REFERENCES `tblVoucher` (`vchID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblWalletsTransactions_tblUserWallets` FOREIGN KEY (`wlt_walID`) REFERENCES `tblUserWallets` (`walid`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=39 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci ROW_FORMAT=COMPRESSED;

-- Dumping data for table AAA.tblWalletsTransactions: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblWalletsTransactions` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblWalletsTransactions` ENABLE KEYS */;

-- Dumping structure for procedure AAA.sp_CREATE_approvalRequest
DROP PROCEDURE IF EXISTS `sp_CREATE_approvalRequest`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_approvalRequest`(
	IN `iWhat2Approve` CHAR(1),
	IN `iUserID` INT UNSIGNED,
	IN `iValue` VARCHAR(100),
	IN `iPass` VARCHAR(100),
	IN `iSalt` VARCHAR(100)





)
BEGIN
  DECLARE ApprovalCode VARCHAR(50);
  DECLARE UserName VARCHAR(100);
  DECLARE UserFamily VARCHAR(100);
  
  SELECT 1 INTO ApprovalCode
    FROM tblUser
   WHERE tblUser.usrID = iUserID;
      
  
  IF (NOT ISNULL(iPass)) THEN 
    IF ISNULL(ApprovalCode) THEN
       		SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = '401:User Not Found';
    END IF;
    SELECT IF(fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass), 'Ok', 'Err'), 
           tblUser.usrName,
           tblUser.usrFamily
      INTO ApprovalCode, UserName, UserFamily
      FROM tblUser
     WHERE tblUser.usrID = iUserID;
     IF ISNULL(ApprovalCode) THEN 
     		SIGNAL SQLSTATE '45000'
          SET MESSAGE_TEXT = '401:Invalid userID';
     END IF;
     IF ApprovalCode = 'Err' THEN 
     		SIGNAL SQLSTATE '45000'
          SET MESSAGE_TEXT = '401:Invalid password';
     END IF;
  END IF;
  
  IF iWhat2Approve = 'E' THEN
    SET ApprovalCode = Common.fnCreateRandomMD5();
  ELSE 
    SET ApprovalCode = FLOOR(RAND() * 90000) + 10000;
  END IF;
  
  INSERT INTO tblApprovalRequest
     SET tblApprovalRequest.apr_usrID = iUserID,
         tblApprovalRequest.aprRequestedFor = iWhat2Approve,
         tblApprovalRequest.aprApprovalCode = ApprovalCode,
         tblApprovalRequest.aprApprovalValue = iValue
         ;
  
  INSERT INTO Common.tblAlerts
     SET Common.tblAlerts.alr_usrID = iUserID,
         Common.tblAlerts.alr_altCode = IF(iWhat2Approve = 'E', 'approveEmail', 'approveMobile'),
         Common.tblAlerts.alrReplacedContactInfo = iValue,
         Common.tblAlerts.alrReplacements = JSON_OBJECT(
  			 	'usrName',UserName,
	    		'usrFamily',UserFamily,
			    'ApprovalCode',ApprovalCode
        ); 	  
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_forgotPassRequest
DROP PROCEDURE IF EXISTS `sp_CREATE_forgotPassRequest`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_forgotPassRequest`(
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_newOnlinePayment
DROP PROCEDURE IF EXISTS `sp_CREATE_newOnlinePayment`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_newOnlinePayment`(
	IN `iVoucherID` BIGINT UNSIGNED,
	IN `iGateway` CHAR(1),
	OUT `oMD5` VARCHAR(50)


)
BEGIN

  SET oMD5 = Common.fnCreateRandomMD5();
  
  INSERT INTO tblOnlinePayments
     SET tblOnlinePayments.opyMD5 = oMD5,
         tblOnlinePayments.onp_vchID = iVoucherID,
         tblOnlinePayments.opyPaymentGateway = iGateway;
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_signup
DROP PROCEDURE IF EXISTS `sp_CREATE_signup`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_signup`(
	IN `iBy` CHAR(1),
	IN `iLogin` VARCHAR(50),
	IN `iPass` CHAR(32),
	IN `iRole` VARCHAR(50),
	IN `iIP` VARCHAR(50),
	IN `iName` VARCHAR(100),
	IN `iFamily` VARCHAR(100),
	IN `iSpecialPrivs` VARCHAR(5000),
	IN `iMaxSessions` INT
,
	IN `iCreatorUserID` INT UNSIGNED,
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
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = UserID,
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
          );
	  ROLLBACK;  
	  RESIGNAL;  
  END;

	DECLARE EXIT HANDLER FOR 1062
	BEGIN
		ROLLBACK;
		SIGNAL SQLSTATE '45000'
      	SET MESSAGE_TEXT = '409:Already registered.';
	END;

  CALL Common.sp_AddDebugLog('AAA', 'signup');
  
  SELECT tblRoles.rolID INTO RoleID
    FROM tblRoles
   WHERE tblRoles.rolName = iRole
     AND (tblRoles.rolSignupAllowedIPs IS NULL OR tblRoles.rolSignupAllowedIPs LIKE CONCAT("%,',iIP,',%"));
    

  IF ISNULL(RoleID) THEN
		SIGNAL SQLSTATE '45403'
	      SET MESSAGE_TEXT = "403:Role not found or is not allowed to signup from this IP";
  END IF;
  
  START TRANSACTION;
  
  INSERT INTO tblUser
     SET tblUser.usrName = iName,
         tblUser.usrFamily = iFamily,
         tblUser.usrEmail = IF(iBy = 'E', iLogin, NULL),
         tblUser.usrMobile = IF(iBy = 'M', iLogin, NULL),     
         tblUser.usrPass = lower(iPass),
         tblUser.usr_rolID = RoleID,
         tblUser.usrSpecialPrivs = iSpecialPrivs,
         tblUser.usrMaxSessions = iMaxSessions,
         tblUser.usrCreatedBy_usrID = IFNULL(iCreatorUserID, 1)
         ;
       
  SET oUserID = LAST_INSERT_ID();
  
  CALL sp_CREATE_approvalRequest(iBy, oUserID, iLogin, NULL, NULL);
  
  INSERT INTO tblUserWallets
     SET tblUserWallets.wal_usrID = oUserID,
         tblUserWallets.walName = 'Default',
         tblUserWallets.walDefault = 1,
         tblUserWallets.walCreatedBy_usrID = IFNULL(iCreatorUserID, 1)
         ;

  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = oUserID,
         tblActionLogs.atlType = 'UserCreated';
  
  COMMIT;
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_transfer
DROP PROCEDURE IF EXISTS `sp_CREATE_transfer`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_transfer`(
	IN `iFromUserID` INT UNSIGNED,
	IN `iFromWalID` INT,
	IN `iToUserLogin` INT,
	IN `iAmount` INT,
	IN `iPass` INT,
	IN `iSalt` INT

)
BEGIN
  DECLARE UserID INT UNSIGNED;
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_walletTransaction
DROP PROCEDURE IF EXISTS `sp_CREATE_walletTransaction`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_walletTransaction`(
	IN `iWalletID` BIGINT UNSIGNED,
	IN `iVoucherID` BIGINT UNSIGNED,
	OUT `oAmount` INT UNSIGNED


)
BEGIN
  DECLARE TransactionType CHAR(1);
  DECLARE Multiplier TINYINT;
  DECLARE UserID INT UNSIGNED;
  DECLARE Err VARCHAR(500);
  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
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
     AND tblVoucher.vchStatus = 'N';
   
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_walletTransactionOnPayment
DROP PROCEDURE IF EXISTS `sp_CREATE_walletTransactionOnPayment`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_walletTransactionOnPayment`(
	IN `iVoucherID` BIGINT UNSIGNED,
	IN `iPaymentType` CHAR(1)
)
BEGIN
  DECLARE PaymentID BIGINT UNSIGNED;
  DECLARE ExpenseVoucherID BIGINT UNSIGNED;
  DECLARE CreditVoucherID BIGINT UNSIGNED;
  DECLARE Amount BIGINT UNSIGNED;
  DECLARE UserID INT UNSIGNED;
  DECLARE UserDefaultWallet BIGINT UNSIGNED;
  DECLARE Err VARCHAR(500);

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
             tblVoucher.vch_usrID
        INTO PaymentID,
             ExpenseVoucherID,
             Amount     
        FROM tblOnlinePayments
          JOIN tblVoucher
            ON tblVoucher.vchID = tblOnlinePayments.onp_vchID
       WHERE tblOnlinePayments.onp_invID = iVoucherID
         AND tblOnlinePayments.onpResult = 'P'; /*Payed*/
    ELSEIF iPaymentType = 'F' THEN /*Offline*/
      SELECT tblOnlinePayments.onpID,
             tblOfflinePayments.ofp_vchID,
             tblOfflinePayments.ofpAmount,
             tblVoucher.vch_usrID
        INTO PaymentID,
             ExpenseVoucherID,
             Amount,
             UserID     
        FROM tblOfflinePayments
          JOIN tblVoucher
            ON tblVoucher.vchID = tblOnlinePayments.onp_vchID
       WHERE tblOfflinePayments.ofp_invID = iVoucherID
         AND tblOfflinePayments.ofpResult = 'P'; /*Payed*/
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

    INSERT INTO tblVoucher
       SET tblVoucher.vch_usrID = iUserID,
           tblVoucher.vchType = 'C',
           tblVoucher.vchDesc = JSON_OBJECT(
              "type", IF(iPaymentType = 'N', 'Online', 'Offline'),
              "paymentID", PaymentID
           ),
           tblVoucher.vchTotalAmount = Amount,
           tblVoucher.vchStatus = 'F';
      
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = UserDefaultWallet,
           tblWalletsTransactions.wlt_vchID = LAST_INSERT_ID(),
           tblWalletsTransactions.wltType = 'C', /*Credit*/
           tblWalletsTransactions.wltAmount = Amount;
    
    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'A'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();
     
    INSERT INTO tblWalletsTransactions
       SET tblWalletsTransactions.wlt_walID = UserDefaultWallet,
           tblWalletsTransactions.wlt_vchID = iInvoiceID,
           tblWalletsTransactions.wltType = 'E', /*Expense*/
           tblWalletsTransactions.wltAmount = Amount;

    UPDATE tblWalletsTransactions
       SET tblWalletsTransactions.wltStatus = 'Y'
     WHERE tblWalletsTransactions.wltID = LAST_INSERT_ID();
           
  COMMIT;  
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_CREATE_withdrawalRequest
DROP PROCEDURE IF EXISTS `sp_CREATE_withdrawalRequest`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_withdrawalRequest`(
	IN `iWalletID` BIGINT UNSIGNED,
	IN `iForUsrID` INT UNSIGNED,
	IN `iByUserID` INT UNSIGNED,
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_SYSTEM_prepareBalanceInfo
DROP PROCEDURE IF EXISTS `sp_SYSTEM_prepareBalanceInfo`;
DELIMITER //
CREATE PROCEDURE `sp_SYSTEM_prepareBalanceInfo`(
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

END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_SYSTEM_validateIPAccess
DROP PROCEDURE IF EXISTS `sp_SYSTEM_validateIPAccess`;
DELIMITER //
CREATE PROCEDURE `sp_SYSTEM_validateIPAccess`(
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_SYSTEM_validateWalletTransaction
DROP PROCEDURE IF EXISTS `sp_SYSTEM_validateWalletTransaction`;
DELIMITER //
CREATE PROCEDURE `sp_SYSTEM_validateWalletTransaction`(
	IN `iUserID` INT UNSIGNED,
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
         tblUserWallets.walNotTransferable
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
         
  IF ISNULL(WalletID) THEN 
 		SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '401:Wallet not found or is not yours';
  END IF;
  
  CASE iTrasnactionType
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

END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_acceptApproval
DROP PROCEDURE IF EXISTS `sp_UPDATE_acceptApproval`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_acceptApproval`(
	IN `iUUID` VARCHAR(50),
	IN `iMobile` VARCHAR(50)
)
BEGIN
  DECLARE UserID INT UNSIGNED;
  DECLARE AprID BIGINT UNSIGNED;
  DECLARE NewValue VARCHAR(50);
  DECLARE ByType CHAR(1);
  DECLARE IsExpired BOOL;

  SELECT tblApprovalRequest.aprID,
         tblApprovalRequest.apr_usrID,
         tblApprovalRequest.aprApprovalValue,
         tblApprovalRequest.aprRequestedFor,
         (TIMEDIFF(NOW(), tblApprovalRequest.aprRequestDate) > "00:30:00" OR aprStatus = 'E')
    INTO AprID, UserID, NewValue, ByType, IsExpired
    FROM tblApprovalRequest
      JOIN tblUser
        ON tblUser.usrID = tblApprovalRequest.apr_usrID
   WHERE tblApprovalRequest.aprApprovalCode = iUUID
     AND (ISNULL(iMobile) OR iMobile = tblApprovalRequest.aprApprovalValue)
     AND tblApprovalRequest.aprStatus IN ('S', '1', '2', 'E');
       
   IF ISNULL(UserID) THEN 
      IF NOT ISNULL(iMobile) THEN
        UPDATE tblApprovalRequest
           SET tblApprovalRequest.aprStatus = IF(tblApprovalRequest.aprStatus = 'S', '1', 
                                                  IF(tblApprovalRequest.aprStatus = '1', '2', 'E'))
         WHERE iMobile = tblApprovalRequest.aprApprovalValue
           AND tblApprovalRequest.aprStatus IN ('S', '1', '2');
      END IF;
   		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Invalid user or code';
   END IF;
   
   IF IsExpired THEN 
   		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Code expired';
   END IF;
   
   
   START TRANSACTION;
     IF ByType = 'E' THEN
        UPDATE tblUser
           SET tblUser.usrEmail = NewValue,
               tblUser.usrApprovalState = IF(tblUser.usrApprovalState IN ('N','E'), 'E', 'A'),
               tblUser.usrStatus = IF(tblUser.usrStatus IN('A','V'), 'A', tblUser.usrStatus),
               tblUser.usrUpdatedBy_usrID = UserID
         WHERE tblUser.usrID = UserID;
     ELSE
        UPDATE tblUser
           SET tblUser.usrMobile = NewValue,
               tblUser.usrApprovalState = IF(tblUser.usrApprovalState IN ('N','M'), 'M', 'A'),
               tblUser.usrStatus = IF(tblUser.usrStatus IN('A','V'), 'A', tblUser.usrStatus),
               tblUser.usrUpdatedBy_usrID = UserID
         WHERE tblUser.usrID = UserID;
     END IF;
     
     UPDATE tblApprovalRequest
        SET tblApprovalRequest.aprApplyDate = NOW(),
            tblApprovalRequest.aprStatus = 'A'
      WHERE tblApprovalRequest.aprID = AprID;
    COMMIT;
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_changePass
DROP PROCEDURE IF EXISTS `sp_UPDATE_changePass`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_changePass`(
	IN `iUserID` INT UNSIGNED,
	IN `iOldPass` CHAR(32),
	IN `iOldPassSalt` VARCHAR(50),
	IN `iNewPass` CHAR(32)




)
BEGIN
  DECLARE WasOK BOOL DEFAULT FALSE;

  SELECT 1 INTO WasOK
    FROM tblUser
   WHERE tblUser.usrID  = iUserID
     AND fnPasswordsAreEqual(iOldPass, iOldPassSalt, tblUser.usrPass);
  
  IF NOT WasOK THEN
		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Invalid User or Password';
  END IF;
  
  UPDATE tblUser
     SET tblUser.usrPass = iNewPass
   WHERE tblUser.usrID = iUserID;
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_changePassByUUID
DROP PROCEDURE IF EXISTS `sp_UPDATE_changePassByUUID`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_changePassByUUID`(
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_login
DROP PROCEDURE IF EXISTS `sp_UPDATE_login`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_login`(
	IN `iLogin` VARCHAR(100),
	IN `iIP` VARCHAR(50),
	IN `iPass` CHAR(32),
	IN `iSalt` VARCHAR(50),
	IN `iInfo` JSON




















,
	IN `iRemember` TINYINT,
	IN `iOAuthInfo` VARCHAR(10000)








)
BEGIN
  DECLARE LoginStatus CHAR(1);
  DECLARE UserID BIGINT UNSIGNED;
  DECLARE InnerRolID BIGINT;
  DECLARE SessionGUID CHAR(32);
  DECLARE LastOAuthInfo JSON;
  
	DECLARE EXIT HANDLER FOR SQLEXCEPTION

  BEGIN
	  ROLLBACK;  
	  RESIGNAL;  
  END;

  START TRANSACTION;
    SELECT IF(tblUser.usrMaxSessions > 0 AND tblUser.usrMaxSessions - tblUser.usrActiveSessions <= 0, 
              'O', 
              IF (fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass),
                  tblUser.usrStatus,
                  'I')), 
           tblUser.usrID,
           tblUserExtraInfo.ueiUpdatedBy_usrID 
      INTO LoginStatus, UserID, LastOAuthInfo
      FROM tblUser
        LEFT JOIN tblUserExtraInfo
          ON tblUserExtraInfo.uei_usrID = tblUser.usrID
     WHERE (tblUser.usrEmail  = iLogin 
            OR tblUser.usrMobile = iLogin
            OR (NOT ISNULL(iOAuthInfo) 
                AND JSON_EXTRACT(iOAuthInfo, "$.type") = 'Linkedin'
                AND tblUserExtraInfo.ueiOAuthAccounts->"$.Linkedin" = JSON_EXTRACT(iOAuthInfo, "$.id")
               )
            )
       AND tblUser.usrStatus IN ('A','V');
    
    IF ISNULL(LoginStatus) THEN
      IF ISNULL(iOAuthInfo) THEN
    		SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid User or Password';
      ELSE 
--        CALL sp_CREATE_signup(iBy, iLogin, iPass, iRole, iIP, iName, iFamily, iSpecialPrivs, iMaxSessions, oUserID);
-- TODO create wallet
        INSERT INTO tblUser
           SET tblUser.usrName   = JSON_EXTRACT(iOAuthInfo, "$.name"),
               tblUser.usrFamily = JSON_EXTRACT(iOAuthInfo, "$.family"),
               tblUser.usrEmail  = JSON_EXTRACT(iOAuthInfo, "$.email"),
               tblUser.usrApprovalState  = 'E';
               
        SET UserID = LAST_INSERT_ID();
        INSERT INTO tblUserExtraInfo
           SET tblUserExtraInfo.uei_usrID = UserID,
               tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
               tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"));
        SET LoginStatus = 'H';
      END IF;
    END IF;

    CASE LoginStatus
      WHEN 'O' THEN
        INSERT INTO tblActionLogs 
           SET tblActionLogs.atlType = 'OverSession',
               tblActionLogs.atlBy_usrID = UserID;
        COMMIT;
     		SIGNAL SQLSTATE '45000'
          SET MESSAGE_TEXT = '405:Max sessions used close old sessions';
      WHEN 'I' THEN
        INSERT INTO tblActionLogs 
           SET tblActionLogs.atlType = 'InvalidPass',
               tblActionLogs.atlBy_usrID = UserID;
        COMMIT;
     		SIGNAL SQLSTATE '45000'
          SET MESSAGE_TEXT = '401:Invalid user or Password';
      WHEN 'R' THEN
        INSERT INTO tblActionLogs 
           SET tblActionLogs.atlType = 'UserRemoved',
               tblActionLogs.atlBy_usrID = UserID;
        COMMIT;
     		SIGNAL SQLSTATE '45000'
          SET MESSAGE_TEXT = '405:User Removed. Ask administrator';
      WHEN 'B' THEN
        INSERT INTO tblActionLogs 
           SET tblActionLogs.atlType = 'UserBlocked',
               tblActionLogs.atlBy_usrID = UserID;
        COMMIT;
     		SIGNAL SQLSTATE '45000'
          SET MESSAGE_TEXT = '405:User Blocked. Ask administrator';
      WHEN 'V' THEN
        IF ISNULL(iOAuthInfo) THEN 
          INSERT INTO tblActionLogs 
             SET tblActionLogs.atlType = 'UserNotApprovedYet',
                 tblActionLogs.atlBy_usrID = UserID;
          COMMIT;
       		SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = '405:You must approve either email or mobile';
        END IF;
      WHEN 'A' THEN
      	SET @a = 1;

    END CASE;
    
    IF NOT ISNULL(iOAuthInfo) AND LoginStatus != 'H' THEN 
       IF ISNULL(LastOAuthInfo) THEN 
         INSERT INTO tblUserExtraInfo
            SET tblUserExtraInfo.uei_usrID = UserID,
               tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
               tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"))
            ON DUPLICATE KEY UPDATE 
               tblUserExtraInfo.uei_usrID = UserID,
               tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
               tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"));
       ELSE
         UPDATE tblUserExtraInfo
            SET tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                tblUserExtraInfo.ueiOAuthAccounts = JSON_MERGE(
                      LastOAuthInfo,
                      JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"))
                      ),
                tblUserExtraInfo.ueiUpdatedBy_usrID = UserID
          WHERE tblUserExtraInfo.uei_usrID = UserID;    
              
         INSERT INTO tblActionLogs
            SET tblActionLogs.atlBy_usrID = UserID,
                tblActionLogs.atlType = 'UserOAuthUpdated';
       END IF;
    END IF;
    
    SET SessionGUID = SUBSTRING(CommonFuncs.guid(NULL),1,32);
    INSERT INTO tblActiveSessions
       SET tblActiveSessions.ssnKey    = SessionGUID,
		       tblActiveSessions.ssn_usrID = UserID,
           tblActiveSessions.ssnIP     = INET_ATON(iIP),
           tblActiveSessions.ssnRemember = iRemember,
           tblActiveSessions.ssnLastActivity = NOW(),
			     tblActiveSessions.ssnInfo   = iInfo;
    
    UPDATE tblUser
       SET tblUser.usrLastLogin = NOW(),
           tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
     WHERE tblUser.usrID = UserID;
     
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = UserID,
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
         SessionGUID AS ssnKey
    FROM tblUser
    JOIN tblRoles
      ON tblRoles.rolID = tblUser.usr_rolID
   WHERE tblUser.usrID = UserID; 

END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_logout
DROP PROCEDURE IF EXISTS `sp_UPDATE_logout`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_logout`(
	IN `iByUserID` INT UNSIGNED,
	IN `iSessionGUID` CHAR(32)


















)
BEGIN
  DECLARE UserID BIGINT UNSIGNED;
  
  SELECT tblActiveSessions.ssn_usrID 
    INTO UserID
    FROM tblActiveSessions
   WHERE tblActiveSessions.ssnKey = iSessionGUID
     AND tblActiveSessions.ssnStatus = 'A';
  
  IF UserID = iByUserID THEN
    UPDATE tblActiveSessions
    	 SET tblActiveSessions.ssnStatus = 'G'
     WHERE tblActiveSessions.ssnKey = iSessionGUID;
	  	 
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = UserID,
           tblActionLogs.atlType = 'UserLogout';
  ELSE 
 		SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '405:Logout by other users is not implemented yet';
  END IF;          
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_reserveWalletPayment
DROP PROCEDURE IF EXISTS `sp_UPDATE_reserveWalletPayment`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_reserveWalletPayment`(
	IN `iWalletID` BIGINT UNSIGNED,
	IN `iUserID` INT UNSIGNED,
	IN `iVoucherID` BIGINT UNSIGNED




)
BEGIN
  
  DECLARE IsAllowed BOOLEAN;
  DECLARE WalletStatus CHAR(1);
  DECLARE UserID INT UNSIGNED;
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_retrieveTokenInfo
DROP PROCEDURE IF EXISTS `sp_UPDATE_retrieveTokenInfo`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_retrieveTokenInfo`(
	IN `iToken` VARCHAR(50),
	IN `iIP` VARCHAR(50)
















)
BEGIN
	DECLARE UserID INT DEFAULT NULL;
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_sessionActivity
DROP PROCEDURE IF EXISTS `sp_UPDATE_sessionActivity`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_sessionActivity`(
	IN `iSSID` CHAR(32)

,
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
END//
DELIMITER ;

-- Dumping structure for procedure AAA.sp_UPDATE_setSessionExpired
DROP PROCEDURE IF EXISTS `sp_UPDATE_setSessionExpired`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_setSessionExpired`(
	IN `iSessionGUID` INT,
	IN `Param2` INT





)
BEGIN
	UPDATE tblActiveSessions
		SET tblActiveSessions.ssnStatus = 'E'
	 WHERE tblActiveSessions.ssnKey = iSessionGUID;
END//
DELIMITER ;

-- Dumping structure for function AAA.fnGetAllPrivs
DROP FUNCTION IF EXISTS `fnGetAllPrivs`;
DELIMITER //
CREATE FUNCTION `fnGetAllPrivs`(
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
END//
DELIMITER ;

-- Dumping structure for function AAA.fnPasswordsAreEqual
DROP FUNCTION IF EXISTS `fnPasswordsAreEqual`;
DELIMITER //
CREATE FUNCTION `fnPasswordsAreEqual`(
	`iSaltedPass` VARCHAR(50),
	`iSalt` VARCHAR(50),
	`iHashedPass` VARCHAR(50)

) RETURNS tinyint(1)
    NO SQL
    DETERMINISTIC
BEGIN
  RETURN MD5(CONCAT(iSalt, LOWER(iHashedPass))) = LOWER(iSaltedPass);
END//
DELIMITER ;

-- Dumping structure for function AAA.fnSanitizePrivs
DROP FUNCTION IF EXISTS `fnSanitizePrivs`;
DELIMITER //
CREATE FUNCTION `fnSanitizePrivs`(
	`iPrivs` VARCHAR(5000)
) RETURNS varchar(5000) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
  RETURN REPLACE(REPLACE(iPrivs, '\n', ''),'\r','');
END//
DELIMITER ;

-- Dumping structure for function AAA.fnSanitizeToken
DROP FUNCTION IF EXISTS `fnSanitizeToken`;
DELIMITER //
CREATE FUNCTION `fnSanitizeToken`(
	`iToken` VARCHAR(50)
) RETURNS varchar(100) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
  RETURN LOWER(REPLACE(REPLACE(REPLACE(REPLACE(iToken, '-',''), ' ', ''), '_', ''),',',''));
END//
DELIMITER ;

-- Dumping structure for event AAA.evExpireSession
DROP EVENT IF EXISTS `evExpireSession`;
DELIMITER //
CREATE EVENT `evExpireSession` ON SCHEDULE EVERY 15 MINUTE STARTS '2020-04-14 15:18:12' ON COMPLETION NOT PRESERVE ENABLE DO BEGIN

END//
DELIMITER ;

-- Dumping structure for trigger AAA.trg_tblActionLogs_before_delete
DROP TRIGGER IF EXISTS `trg_tblActionLogs_before_delete`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblActionLogs_before_delete` BEFORE DELETE ON `tblActionLogs` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:DELETE is not allowed on tblActionLogs';
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblActionLogs_before_update
DROP TRIGGER IF EXISTS `trg_tblActionLogs_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblActionLogs_before_update` BEFORE UPDATE ON `tblActionLogs` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblActionLogs';
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblActiveSessions_after_update
DROP TRIGGER IF EXISTS `trg_tblActiveSessions_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblActiveSessions_after_update` AFTER UPDATE ON `tblActiveSessions` FOR EACH ROW BEGIN
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
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblAPITokenValidIPs_after_update
DROP TRIGGER IF EXISTS `trg_tblAPITokenValidIPs_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblAPITokenValidIPs_after_update` AFTER UPDATE ON `tblAPITokenValidIPs` FOR EACH ROW BEGIN
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
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblAPIToken_after_update
DROP TRIGGER IF EXISTS `trg_tblAPIToken_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblAPIToken_after_update` AFTER UPDATE ON `tblAPITokens` FOR EACH ROW BEGIN
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
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblBlockingRules_after_update
DROP TRIGGER IF EXISTS `trg_tblBlockingRules_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblBlockingRules_after_update` AFTER UPDATE ON `tblBlockingRules` FOR EACH ROW BEGIN
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
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblIPBin_before_update
DROP TRIGGER IF EXISTS `trg_tblIPBin_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblIPBin_before_update` BEFORE UPDATE ON `tblIPBin` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblIPBin';
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblIPStats_before_update
DROP TRIGGER IF EXISTS `trg_tblIPStats_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblIPStats_before_update` BEFORE UPDATE ON `tblIPStats` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblIPStats';
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblOfflinePayments_before_update
DROP TRIGGER IF EXISTS `trg_tblOfflinePayments_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblOfflinePayments_before_update` BEFORE UPDATE ON `tblOfflinePayments` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblOfflinePayments';
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblOnlinePayments_before_update
DROP TRIGGER IF EXISTS `trg_tblOnlinePayments_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblOnlinePayments_before_update` BEFORE UPDATE ON `tblOnlinePayments` FOR EACH ROW BEGIN
	SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblOnlinePayments';
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblUserExtraInfo_after_update
DROP TRIGGER IF EXISTS `trg_tblUserExtraInfo_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblUserExtraInfo_after_update` AFTER UPDATE ON `tblUserExtraInfo` FOR EACH ROW BEGIN
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
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblUser_after_update
DROP TRIGGER IF EXISTS `trg_tblUser_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblUser_after_update` AFTER UPDATE ON `tblUser` FOR EACH ROW BEGIN
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
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblVoucher_before_update
DROP TRIGGER IF EXISTS `trg_tblVoucher_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblVoucher_before_update` BEFORE UPDATE ON `tblVoucher` FOR EACH ROW BEGIN
	IF OLD.vchCreationDateTime != NEW.vchCreationDateTime
     OR OLD.vch_usrID != NEW.vch_usrID
     OR OLD.vchType != NEW.vchType
     OR OLD.vchTotalAmount != NEW.vchTotalAmount
  THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:tblVoucher update is not allowed except for status';
  END IF;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblWalletsTransactions_after_insert
DROP TRIGGER IF EXISTS `trg_tblWalletsTransactions_after_insert`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblWalletsTransactions_after_insert` AFTER INSERT ON `tblWalletsTransactions` FOR EACH ROW Proc: BEGIN
  DECLARE LastTransactionDate DATETIME;
  DECLARE LastBalance BIGINT;
  DECLARE LastIncome BIGINT;
  DECLARE LastExpense BIGINT;
  DECLARE MultipleInsert BOOL;
  
  IF NEW.wltStatus != 'N' THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:Transaction status must be New on insertion';
  END IF;
        
  CALL sp_SYSTEM_prepareBalanceInfo(NEW.wlt_walID, NEW.wltID, NEW.wltDateTime, LastTransactionDate, LastBalance, LastIncome, LastExpense, MultipleInsert);
     

  IF ISNULL(MultipleInsert) THEN
    INSERT INTO tblWalletBalances(SELECT NEW.wltID, IFNULL(LastBalance,0), IFNULL(LastIncome,0), IFNULL(LastExpense,0));
  ELSE 
    SET @LastBalance = IFNULL(LastBalance,0);
    INSERT INTO tblWalletBalances (
      SELECT tblWalletsTransactions.wltID,
             (@LastBalance := @LastBalance + 
              IF (tblWalletsTransactions.wltID = NEW.wltID, 
                  0, 
                  IF(tblWalletsTransactions.wltStatus = 'P', 
                     tblWalletsTransactions.wltAmount, 
                     0
                    )
                 )
              )
        FROM tblWalletsTransactions
        JOIN (SELECT @LastBalance := IFNULL(LastBalance,0)) r
       WHERE tblWalletsTransactions.wltDateTime > LastTransactionDate 
       ORDER BY tblWalletsTransactions.wltDateTime ASC, tblWalletsTransactions.wltID ASC
    );  
    SET LastBalance = @LastBalance;
  END IF;

  UPDATE tblUserWallets
     SET tblUserWallets.walLastBalance = IFNULL(LastBalance,0) - IF(NEW.wltAmount<0,NEW.wltAmount, 0)
   WHERE tblUserWallets.walID = NEW.wlt_walID;
  
  -- TODO Update Last Income and LastExpenses
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblWalletsTransactions_after_update
DROP TRIGGER IF EXISTS `trg_tblWalletsTransactions_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblWalletsTransactions_after_update` AFTER UPDATE ON `tblWalletsTransactions` FOR EACH ROW Proc: BEGIN

  DECLARE LastTransactionDate DATETIME;
  DECLARE LastBalance BIGINT;
  DECLARE MultipleInsert BOOL;
  
  IF NEW.wltStatus = 'N' THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:Wallet status can not be reset to new';
  END IF;
        
  CALL sp_SYSTEM_deleteOldBalances(NEW.wlt_walID, NEW.wltID, NEW.wltDateTime, LastTransactionDate, LastBalance, LastIncome, LastExpense, MultipleInsert);

  IF (IFNULL(LastBalance,0) + NEW.wltAmount < 0) THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '401:Wallet balance can not be negative';
  END IF;

  IF ISNULL(MultipleInsert) THEN
    UPDATE tblWalletBalances 
       SET tblWalletBalances.wblBalance = IFNULL(LastBalance,0) + IF(NEW.wltStatus IN ('Y', 'A'), NEW.wltAmount, 0)
     WHERE tblWalletBalances.wbl_wltID  = NEW.wltID;
     SET LastBalance = IFNULL(LastBalance,0) + IF(NEW.wltStatus IN ('Y', 'A'), NEW.wltAmount, 0);
  ELSE 
    SET @LastBalance = IFNULL(LastBalance,0);
    DELETE FROM tblWalletBalances WHERE tblWalletBalances.wbl_wltID  = NEW.wltID;
    INSERT INTO tblWalletBalances (
      SELECT tblWalletsTransactions.wltID,
             (@LastBalance := @LastBalance + 
      				 	IF(tblWalletsTransactions.wltStatus IN ('Y', 'A'), tblWalletsTransactions.wltAmount, 0 ))
        FROM tblWalletsTransactions
        JOIN (SELECT @LastBalance := IFNULL(LastBalance,0)) r
       WHERE tblWalletsTransactions.wltDateTime > LastTransactionDate 
       ORDER BY tblWalletsTransactions.wltDateTime ASC, tblWalletsTransactions.wltID ASC
    );  
    SET LastBalance = @LastBalance;
  END IF;

  UPDATE tblUserWallets
     SET tblUserWallets.walLastBalance = IFNULL(LastBalance,0)
   WHERE tblUserWallets.walID = NEW.wlt_walID;
  
  -- TODO Update Last Income and LastExpenses
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger AAA.trg_tblWalletsTransactions_before_update
DROP TRIGGER IF EXISTS `trg_tblWalletsTransactions_before_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `trg_tblWalletsTransactions_before_update` BEFORE UPDATE ON `tblWalletsTransactions` FOR EACH ROW BEGIN
	IF OLD.wlt_walID != NEW.wlt_walID
     OR OLD.wlt_invID != NEW.wlt_invID
     OR OLD.wltType != NEW.wltType
     OR OLD.wltAmount != NEW.wltAmount
  THEN 
   		SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '500:tblWallet update is not allowed except for status';
  END IF;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;


-- Dumping database structure for Advert
DROP DATABASE IF EXISTS `Advert`;
CREATE DATABASE IF NOT EXISTS `Advert` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `Advert`;

-- Dumping structure for table Advert.tblAccountDiscounts
DROP TABLE IF EXISTS `tblAccountDiscounts`;
CREATE TABLE IF NOT EXISTS `tblAccountDiscounts` (
  `disID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `disCode` varchar(20) NOT NULL DEFAULT '0',
  `disType` char(1) NOT NULL DEFAULT '%' COMMENT '%: Percent, $:Amount, Z: Free',
  `disPackageBasedAmount` json NOT NULL,
  `disValidFrom` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `disExpiryTime` datetime DEFAULT NULL,
  `disPrimaryCount` int(10) unsigned NOT NULL DEFAULT '1',
  `disUsedCount` int(10) unsigned NOT NULL DEFAULT '0',
  `disCreatedBy_usrID` int(10) unsigned NOT NULL,
  `disCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `disUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `disStatus` char(50) DEFAULT NULL COMMENT 'A: Active, Removed',
  PRIMARY KEY (`disID`),
  UNIQUE KEY `disCode` (`disCode`),
  KEY `disType` (`disType`),
  KEY `disExpiryTime` (`disExpiryTime`),
  KEY `disCreatedBy_usrID` (`disCreatedBy_usrID`),
  KEY `disCreationDateTime` (`disCreationDateTime`),
  KEY `disUpdatedBy_usrID` (`disUpdatedBy_usrID`),
  KEY `disStatus` (`disStatus`),
  KEY `disValidFrom` (`disValidFrom`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblAccountDiscounts: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountDiscounts` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountDiscounts` ENABLE KEYS */;

-- Dumping structure for table Advert.tblAccountPackages
DROP TABLE IF EXISTS `tblAccountPackages`;
CREATE TABLE IF NOT EXISTS `tblAccountPackages` (
  `pkgID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `pkgCode` varchar(20) NOT NULL,
  `pkgType` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: Normal, S: Special',
  `pkgValidFromDate` date DEFAULT NULL,
  `pkgValidToDate` date DEFAULT NULL,
  `pkgValidFromTime` tinyint(4) DEFAULT NULL,
  `pkgValidToTime` tinyint(4) DEFAULT NULL,
  `pkg_locID` int(10) unsigned NOT NULL,
  `pkgDuration` int(10) NOT NULL DEFAULT '-1',
  `pkgShowPerDay` int(11) NOT NULL DEFAULT '-1',
  `pkgShowTotal` int(11) NOT NULL DEFAULT '-1',
  `pkgClicksPerDay` int(11) NOT NULL DEFAULT '-1',
  `pkgClicksPerMonth` int(11) NOT NULL DEFAULT '-1',
  `pkgClicksTotal` bigint(20) NOT NULL DEFAULT '-1',
  `pkgPrivs` json DEFAULT NULL,
  `pkgPrice` int(10) unsigned NOT NULL,
  `pkgCanBePurchasedSince` datetime DEFAULT CURRENT_TIMESTAMP,
  `pkgNotAvailableSince` datetime DEFAULT NULL,
  `pkgInvoiceTemplate` text NOT NULL,
  `pkgCreatedBy_usrID` int(10) unsigned NOT NULL,
  `pkgCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `pkgUpdatedBy_usrID` int(10) unsigned NOT NULL,
  `pkgStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`pkgID`),
  UNIQUE KEY `pkgCode` (`pkgCode`,`pkgStatus`),
  KEY `FK_tblAccountPackages_tblLocations` (`pkg_locID`),
  KEY `pkgCreationDateTime` (`pkgCreationDateTime`),
  KEY `pkgStatus` (`pkgStatus`),
  KEY `pkgValidFrom` (`pkgValidFromDate`),
  KEY `pkgValidTo` (`pkgValidToDate`),
  KEY `pkgCreatedBy_usrID` (`pkgCreatedBy_usrID`),
  KEY `pkgUpdatedBy_usrID` (`pkgUpdatedBy_usrID`),
  KEY `pkgValidFromTime` (`pkgValidFromTime`),
  KEY `pkgValidToTime` (`pkgValidToTime`),
  KEY `pkgType` (`pkgType`),
  KEY `pkgCanBePurchasedFrom` (`pkgCanBePurchasedSince`),
  KEY `pkgNotAvailableSince` (`pkgNotAvailableSince`),
  CONSTRAINT `FK_tblAccountPackages_tblLocations` FOREIGN KEY (`pkg_locID`) REFERENCES `tblLocations` (`locid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblAccountPackages: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountPackages` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountPackages` ENABLE KEYS */;

-- Dumping structure for table Advert.tblAccountUsage
DROP TABLE IF EXISTS `tblAccountUsage`;
CREATE TABLE IF NOT EXISTS `tblAccountUsage` (
  `usg_aupID` bigint(20) unsigned NOT NULL,
  `usgRemainingDays` smallint(6) NOT NULL DEFAULT '0',
  `usgDayShow` bigint(20) unsigned NOT NULL DEFAULT '0',
  `usgTotalShow` bigint(20) unsigned NOT NULL DEFAULT '0',
  `usgDayClicks` int(10) unsigned NOT NULL DEFAULT '0',
  `usgMonthClicks` int(10) unsigned NOT NULL DEFAULT '0',
  `usgTotalClicks` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`usg_aupID`),
  KEY `usg_usrID` (`usg_aupID`),
  CONSTRAINT `FK_tblAccountUsage_tblAccountUserPackages` FOREIGN KEY (`usg_aupID`) REFERENCES `tblAccountUserPackages` (`aupID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblAccountUsage: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountUsage` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountUsage` ENABLE KEYS */;

-- Dumping structure for table Advert.tblAccountUserPackages
DROP TABLE IF EXISTS `tblAccountUserPackages`;
CREATE TABLE IF NOT EXISTS `tblAccountUserPackages` (
  `aupID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `aup_usrID` int(10) unsigned NOT NULL,
  `aup_pkgID` int(10) unsigned NOT NULL,
  `aupPrefered` bit(1) DEFAULT NULL,
  `aupPurchaseRequestDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `aupPaymentDateTime` datetime DEFAULT NULL,
  `aup_vchID` bigint(20) unsigned NOT NULL,
  `aupUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `aupStatus` char(1) NOT NULL DEFAULT 'P' COMMENT 'P: Pending, A: Active, R: Removed, B: Blocked',
  PRIMARY KEY (`aupID`),
  UNIQUE KEY `aup_usrID_aup_pkgID_aupPrefered` (`aup_usrID`,`aup_pkgID`,`aupPrefered`,`aupStatus`),
  KEY `aupStatus` (`aupStatus`),
  KEY `aup_usrID` (`aup_usrID`),
  KEY `FK_tblAccountUserPackages_tblAccountPackages` (`aup_pkgID`),
  KEY `aupPurchaseDate` (`aupPurchaseRequestDateTime`),
  KEY `aupPaymentDataTime` (`aupPaymentDateTime`),
  KEY `aupUpdatedBy_usrID` (`aupUpdatedBy_usrID`),
  KEY `aup_invID` (`aup_vchID`) USING BTREE,
  CONSTRAINT `FK_tblAccountUserPackages_tblAccountPackages` FOREIGN KEY (`aup_pkgID`) REFERENCES `tblAccountPackages` (`pkgID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblAccountUserPackages: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountUserPackages` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountUserPackages` ENABLE KEYS */;

-- Dumping structure for table Advert.tblActionLogs
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

-- Dumping data for table Advert.tblActionLogs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActionLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActionLogs` ENABLE KEYS */;

-- Dumping structure for table Advert.tblActiveAds
DROP TABLE IF EXISTS `tblActiveAds`;
CREATE TABLE IF NOT EXISTS `tblActiveAds` (
  `act_binID` int(10) unsigned NOT NULL,
  `act_locID` int(10) unsigned NOT NULL,
  `actOrder` char(1) NOT NULL,
  `actOnKeyword` varchar(50) DEFAULT NULL,
  KEY `FK_tblActiveAds_tblBin` (`act_binID`),
  KEY `FK_tblActiveAds_tblLocations` (`act_locID`),
  KEY `actOrder` (`actOrder`),
  KEY `actOnKeyword` (`actOnKeyword`),
  CONSTRAINT `FK_tblActiveAds_tblBin` FOREIGN KEY (`act_binID`) REFERENCES `tblBin` (`binid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblActiveAds_tblLocations` FOREIGN KEY (`act_locID`) REFERENCES `tblLocations` (`locid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblActiveAds: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActiveAds` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActiveAds` ENABLE KEYS */;

-- Dumping structure for table Advert.tblBanners
DROP TABLE IF EXISTS `tblBanners`;
CREATE TABLE IF NOT EXISTS `tblBanners` (
  `bnr_binID` int(10) unsigned NOT NULL,
  `bnrImage` text NOT NULL,
  `bnrSize` char(1) NOT NULL COMMENT 'Check c++ code it is too complex',
  `bnrUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  KEY `bnrSize` (`bnrSize`),
  KEY `FK_tblBanners_tblBin` (`bnr_binID`),
  KEY `bnrUpdatedBy_usrID` (`bnrUpdatedBy_usrID`),
  CONSTRAINT `FK_tblBanners_tblBin` FOREIGN KEY (`bnr_binID`) REFERENCES `tblBin` (`binid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblBanners: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblBanners` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblBanners` ENABLE KEYS */;

-- Dumping structure for table Advert.tblBin
DROP TABLE IF EXISTS `tblBin`;
CREATE TABLE IF NOT EXISTS `tblBin` (
  `binID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `binType` char(1) NOT NULL DEFAULT 'T' COMMENT 'T: Text, I:Image',
  `binTitle` varchar(100) NOT NULL,
  `binDesc` varchar(400) DEFAULT NULL,
  `binPrettyURL` varchar(100) DEFAULT NULL,
  `binURL` varchar(400) NOT NULL,
  `binShown` bigint(20) NOT NULL DEFAULT '0',
  `binClicks` bigint(20) NOT NULL DEFAULT '0',
  `binCreatedBy_usrID` int(11) unsigned DEFAULT NULL,
  `binCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `binUpdatedBy_usrID` int(11) unsigned DEFAULT NULL,
  `binStatus` char(1) NOT NULL DEFAULT 'P' COMMENT 'P:Pending, A:Active, B:Blocked, R:Removed',
  PRIMARY KEY (`binID`),
  KEY `advStatus` (`binStatus`),
  KEY `adbType` (`binType`),
  KEY `adbShown` (`binShown`),
  KEY `adbCreationDateTime` (`binCreationDateTime`),
  KEY `adbUpdatedBy_usrID` (`binUpdatedBy_usrID`),
  KEY `adbClicks` (`binClicks`),
  KEY `binCreatedBy_usrID` (`binCreatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblBin: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblBin` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblBin` ENABLE KEYS */;

-- Dumping structure for table Advert.tblClicks
DROP TABLE IF EXISTS `tblClicks`;
CREATE TABLE IF NOT EXISTS `tblClicks` (
  `clkID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `clk_binID` int(10) unsigned NOT NULL,
  `clk_locID` int(10) unsigned NOT NULL,
  `clkDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `clkIP` varchar(50) NOT NULL,
  `clkDevice` varchar(50) DEFAULT NULL,
  `clkScreenSize` varchar(50) DEFAULT NULL,
  `clkOS` varchar(50) DEFAULT NULL,
  `clkBrowser` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`clkID`),
  KEY `clkDevice` (`clkDevice`),
  KEY `clkScreenSize` (`clkScreenSize`),
  KEY `clkOS` (`clkOS`),
  KEY `clkBrowser` (`clkBrowser`),
  KEY `clkIP` (`clkIP`),
  KEY `FK_tblClicks_tblBin` (`clk_binID`),
  KEY `FK_tblClicks_tblLocations` (`clk_locID`),
  KEY `clkDateTime` (`clkDateTime`),
  CONSTRAINT `FK_tblClicks_tblBin` FOREIGN KEY (`clk_binID`) REFERENCES `tblBin` (`binid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblClicks_tblLocations` FOREIGN KEY (`clk_locID`) REFERENCES `tblLocations` (`locid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblClicks: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblClicks` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblClicks` ENABLE KEYS */;

-- Dumping structure for table Advert.tblLocations
DROP TABLE IF EXISTS `tblLocations`;
CREATE TABLE IF NOT EXISTS `tblLocations` (
  `locID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `locURL` varchar(200) NOT NULL,
  `locPlaceCode` char(3) NOT NULL,
  `locCreatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `locCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `locUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `locStatus` char(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`locID`),
  KEY `locURL` (`locURL`),
  KEY `locStatus` (`locStatus`),
  KEY `locPlaceCode` (`locPlaceCode`),
  KEY `locCreator_usrID` (`locCreatedBy_usrID`),
  KEY `locCreationDateTime` (`locCreationDateTime`),
  KEY `locUpdater_usrID` (`locUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblLocations: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblLocations` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblLocations` ENABLE KEYS */;

-- Dumping structure for table Advert.tblProps
DROP TABLE IF EXISTS `tblProps`;
CREATE TABLE IF NOT EXISTS `tblProps` (
  `prp_binID` int(10) unsigned NOT NULL,
  `prp_locID` int(10) unsigned NOT NULL,
  `prpOrder` char(1) NOT NULL COMMENT 'F: First, S: Second, T: Third, H: HugeReq, R: Normal',
  `prpKeyword` varchar(50) DEFAULT NULL,
  `prpStartDate` date NOT NULL,
  `prpEndDate` date NOT NULL,
  `prpCreatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `prpCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `prpUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  UNIQUE KEY `prp_binID_prp_locID_adpOrder_adpStartDate` (`prp_binID`,`prp_locID`,`prpOrder`,`prpStartDate`,`prpKeyword`),
  KEY `adpOrder` (`prpOrder`),
  KEY `adpStartDate` (`prpStartDate`),
  KEY `adpEndDate` (`prpEndDate`),
  KEY `FK_tblProps_tblLocations` (`prp_locID`),
  KEY `prp_binID` (`prp_binID`),
  KEY `prpKeyword` (`prpKeyword`),
  KEY `prpCreatedBy_usrID` (`prpCreatedBy_usrID`),
  KEY `prpCreationDateTime` (`prpCreationDateTime`),
  KEY `prpUpdatedBy_usrID` (`prpUpdatedBy_usrID`),
  CONSTRAINT `FK_tblProps_tblBin` FOREIGN KEY (`prp_binID`) REFERENCES `tblBin` (`binid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblProps_tblLocations` FOREIGN KEY (`prp_locID`) REFERENCES `tblLocations` (`locid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Advert.tblProps: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblProps` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblProps` ENABLE KEYS */;

-- Dumping structure for procedure Advert.sp_UPDATE_setAsPrefered
DROP PROCEDURE IF EXISTS `sp_UPDATE_setAsPrefered`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_setAsPrefered`(
	IN `iUserID` INT UNSIGNED,
	IN `iAUPID` BIGINT UNSIGNED
)
BEGIN
  DECLARE PackageUser INT UNSIGNED;
  
  SELECT tblAccountUserPackages INTO PackageUser
    FROM tblAccountUserPackages
   WHERE tblAccountUserPackages.aupID = iAUPID;
   
  IF PackageUser IS NULL THEN
		SIGNAL SQLSTATE '45403'
	      SET MESSAGE_TEXT = "403:UserPackage Not Found";
  END IF;
  
  START TRANSACTION;
    UPDATE tblAccountUserPackages
       SET tblAccountUserPackages.aupPrefered = NULL,
           tblAccountUserPackages.aupUpdatedBy_usrID = iUserID
     WHERE tblAccountUserPackages.aup_usrID = PackageUser
       AND tblAccountUserPackages.aupID != iAUPID;

    UPDATE tblAccountUserPackages
       SET tblAccountUserPackages.aupPrefered = TRUE,
           tblAccountUserPackages.aupUpdatedBy_usrID = iUserID
     WHERE tblAccountUserPackages.aup_usrID = PackageUser
       AND tblAccountUserPackages.aupID = iAUPID;
  COMMIT;
  
  
END//
DELIMITER ;

-- Dumping structure for trigger Advert.tblBanners_after_update
DROP TRIGGER IF EXISTS `tblBanners_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `tblBanners_after_update` AFTER UPDATE ON `tblBanners` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";
  
  IF NEW.bnrImage           != OLD.bnrImage           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("bnrImage", OLD.bnrImage)); END IF;
  IF NEW.bnrSize            != OLD.bnrSize            THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("bnrSize", OLD.bnrSize)); END IF;
  
  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.bnrUpdatedBy_usrID,
         tblActionLogs.atlType = "tblBanners-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger Advert.tblBin_after_update
DROP TRIGGER IF EXISTS `tblBin_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `tblBin_after_update` AFTER UPDATE ON `tblBin` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";
  
  IF NEW.binType          != OLD.binType         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("binType", OLD.binType)); END IF;
  IF NEW.binTitle         != OLD.binTitle        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("binTitle", OLD.binTitle)); END IF;
  IF NEW.binDesc          != OLD.binDesc         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("binDesc", OLD.binDesc)); END IF;
  IF NEW.binPrettyURL     != OLD.binPrettyURL    THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("binPrettyURL", OLD.binPrettyURL)); END IF;
  IF NEW.binURL           != OLD.binURL          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("binURL", OLD.binURL)); END IF;
  IF NEW.binStatus        != OLD.binStatus       THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("binStatus", OLD.binStatus)); END IF;
  
  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.binUpdatedBy_usrID,
         tblActionLogs.atlType = "tblBin-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger Advert.tblLocations_after_update
DROP TRIGGER IF EXISTS `tblLocations_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `tblLocations_after_update` AFTER UPDATE ON `tblLocations` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";
  
  IF NEW.locURL          != OLD.locURL         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("locURL", OLD.locURL)); END IF;
  IF NEW.locPlaceCode    != OLD.locPlaceCode   THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("locPlaceCode", OLD.locPlaceCode)); END IF;
  IF NEW.locStatus       != OLD.locStatus      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("locStatus", OLD.locStatus)); END IF;
  
  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.locUpdatedBy_usrID,
         tblActionLogs.atlType = "locStatus-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Dumping structure for trigger Advert.tblProps_after_update
DROP TRIGGER IF EXISTS `tblProps_after_update`;
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `tblProps_after_update` AFTER UPDATE ON `tblProps` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";
  
  IF NEW.prp_binID         != OLD.prp_binID         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("prp_binID", OLD.prp_binID)); END IF;
  IF NEW.prp_locID         != OLD.prp_locID         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("prp_locID", OLD.prp_locID)); END IF;
  IF NEW.prpOrder          != OLD.prpOrder          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("prpOrder", OLD.prpOrder)); END IF;
  IF NEW.prpKeyword        != OLD.prpKeyword        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("prpKeyword", OLD.prpKeyword)); END IF;
  IF NEW.prpStartDate      != OLD.prpStartDate      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("prpStartDate", OLD.prpStartDate)); END IF;
  IF NEW.prpEndDate        != OLD.prpEndDate        THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("prpEndDate", OLD.prpEndDate)); END IF;
  
  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.prpUpdatedBy_usrID,
         tblActionLogs.atlType = "tblProps-Updated",
         tblActionLogs.atlDescription = Changes;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;


-- Dumping database structure for Common
DROP DATABASE IF EXISTS `Common`;
CREATE DATABASE IF NOT EXISTS `Common` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `Common`;

-- Dumping structure for table Common.tblAlerts
DROP TABLE IF EXISTS `tblAlerts`;
CREATE TABLE IF NOT EXISTS `tblAlerts` (
  `alrID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `alrType` char(1) NOT NULL DEFAULT 'I' COMMENT 'I: Informational, C: Critical, W:Warning',
  `alr_usrID` int(11) unsigned NOT NULL,
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
) ENGINE=InnoDB AUTO_INCREMENT=453 DEFAULT CHARSET=utf8;

-- Dumping data for table Common.tblAlerts: ~451 rows (approximately)
/*!40000 ALTER TABLE `tblAlerts` DISABLE KEYS */;
INSERT INTO `tblAlerts` (`alrID`, `alrType`, `alr_usrID`, `alrReplacedContactInfo`, `alr_altCode`, `alrReplacements`, `alrCreateDate`, `alrSentDate`, `alrStatus`) VALUES
	(1, 'I', 100, NULL, 'passReset', '{"via": "Web", "UUID": "55c4b4a7f5cad4c9338cd82be670cdb4", "usrName": "aaa", "usrFamily": "aaaa"}', '2019-10-01 14:45:29', NULL, 'N'),
	(2, 'I', 103, NULL, 'passReset', '{"via": "0", "UUID": "d2a84cdfb1c75ff102c91d53bff15172", "usrName": null, "usrFamily": null}', '2019-11-02 15:19:05', NULL, 'N'),
	(3, 'I', 100, NULL, 'passReset', '{"via": "0", "UUID": "5cdabccf6a5b544e164f552af36f5a1b", "usrName": "mehran", "usrFamily": "ziabary"}', '2019-11-09 11:40:07', NULL, 'N'),
	(4, 'I', 100, NULL, 'passReset', '{"via": "0", "UUID": "72b5eede128b8e4b11fb0c9d64b589eb", "usrName": "mehran", "usrFamily": "ziabary"}', '2019-11-09 11:40:33', NULL, 'N'),
	(5, 'I', 106, NULL, 'passReset', '{"via": "0", "UUID": "b47265fd46cbdede2eb3705b81e34ddf", "usrName": "arian", "usrFamily": "atefi"}', '2019-11-09 11:40:57', NULL, 'N'),
	(6, 'I', 149, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "06256d14fc906e63f7777204613a4d6b"}', '2019-11-14 11:02:26', NULL, 'N'),
	(7, 'I', 151, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "315b0d0d72b229ae0e57cf435bb89163"}', '2019-11-14 11:02:46', NULL, 'N'),
	(8, 'I', 153, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "48fd71a61910459fe1ae3a9c9db59bce"}', '2019-11-14 11:03:13', NULL, 'N'),
	(9, 'I', 155, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c4bd78b6a1db8354ecb0cbaf22d5959c"}', '2019-11-14 11:03:39', NULL, 'N'),
	(10, 'I', 156, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0255910cfbf4a4e48f69ae299692afc9"}', '2019-11-14 11:04:15', NULL, 'N'),
	(11, 'I', 158, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "20a49aa9a8db340dde18214703ff63d7"}', '2019-11-14 11:05:22', NULL, 'N'),
	(12, 'I', 160, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0c175877bea59e95bfba7c474de7df07"}', '2019-11-14 11:19:54', NULL, 'N'),
	(13, 'I', 162, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3eb679a352d71d9ea6fa8531a9a52add"}', '2019-11-14 11:20:15', NULL, 'N'),
	(14, 'I', 193, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0aae20772aa4efedd3205169ff5f229b"}', '2019-11-14 15:51:44', NULL, 'N'),
	(15, 'I', 195, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "19dc3c5a298704c09d2e460d869b8d89"}', '2019-11-16 05:28:28', NULL, 'N'),
	(16, 'I', 198, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "6d5a701cbfc202b8811ca886348ab5b6"}', '2019-11-16 05:34:45', NULL, 'N'),
	(17, 'I', 201, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "1fb0dbdb9b405278905626c16f63d129"}', '2019-11-16 05:44:01', NULL, 'N'),
	(18, 'I', 204, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "baf4c4c0dba327c4ea6be845de6a4cc3"}', '2019-11-16 05:50:07', NULL, 'N'),
	(19, 'I', 206, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b1a3eb6a6511def1d4d70be6376d15a9"}', '2019-11-16 05:54:26', NULL, 'N'),
	(20, 'I', 207, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f078e9fe2543de284192f8fbf988a2a5"}', '2019-11-16 05:56:31', NULL, 'N'),
	(21, 'I', 209, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "41605ad7781451cad8958f048176f634"}', '2019-11-16 05:58:24', NULL, 'N'),
	(22, 'I', 211, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "cf95c7ad0da2019e2b59e061bea0d7c7"}', '2019-11-16 06:02:00', NULL, 'N'),
	(23, 'I', 213, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a9ebf1b6b1320d566641cf80d6c7b5b8"}', '2019-11-16 07:38:26', NULL, 'N'),
	(24, 'I', 214, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "841e26f84fe91363176cb66ad6186af9"}', '2019-11-16 07:40:50', NULL, 'N'),
	(25, 'I', 215, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3eee977d8e1854022e19e2fa32785e64"}', '2019-11-16 07:48:55', NULL, 'N'),
	(26, 'I', 216, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7eb7fb01ab94bf3be11b393be95580da"}', '2019-11-16 07:49:48', NULL, 'N'),
	(27, 'I', 217, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c8359467286bfec27632a5f45561243a"}', '2019-11-16 07:50:27', NULL, 'N'),
	(28, 'I', 218, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c512280666d1f679166cefd35aea2335"}', '2019-11-16 08:05:05', NULL, 'N'),
	(29, 'I', 219, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "71f7c52ccfd6496cdcf73f92dea81b5a"}', '2019-11-16 08:07:14', NULL, 'N'),
	(30, 'I', 220, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7ad675ccb2f29054cb720aa9f31ab630"}', '2019-11-16 08:59:17', NULL, 'N'),
	(31, 'I', 221, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "1e5728103628c2a65c8c0e92dcc3ac86"}', '2019-11-16 09:41:07', NULL, 'N'),
	(32, 'I', 222, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "182d84ddfba9deb71a90e178a2c39992"}', '2019-11-16 09:56:25', NULL, 'N'),
	(33, 'I', 223, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "27071975ae0c9f979d5a0c577b56af05"}', '2019-11-16 09:57:53', NULL, 'N'),
	(34, 'I', 224, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "609867d15c4386aec908003dbe1fa999"}', '2019-11-16 09:58:59', NULL, 'N'),
	(35, 'I', 225, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "74a9dce461f1c8d649c322fdd8bd585c"}', '2019-11-16 10:00:26', NULL, 'N'),
	(36, 'I', 226, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d88ab936ea018a2f64e52e5ad0ea55f2"}', '2019-11-16 10:05:16', NULL, 'N'),
	(37, 'I', 227, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d39f24a3ca8b8ed22ba9d5e53d1c2d87"}', '2019-11-16 10:06:59', NULL, 'N'),
	(38, 'I', 228, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "8551e4e6dde4e8208a50e68ea1756dd6"}', '2019-11-16 10:07:34', NULL, 'N'),
	(39, 'I', 229, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ce87a638deb688e43d8b025ed445478f"}', '2019-11-16 10:08:53', NULL, 'N'),
	(40, 'I', 230, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b6e04cbd2ac5f96395bf7b93dca06ca9"}', '2019-11-16 10:09:15', NULL, 'N'),
	(41, 'I', 231, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "073f38d4d9fe8f49af1905bc69bb0d85"}', '2019-11-16 10:09:44', NULL, 'N'),
	(42, 'I', 232, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "47b96976b87d811404a57e84cdde6793"}', '2019-11-16 10:10:19', NULL, 'N'),
	(43, 'I', 233, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ddbe4762ddcb1bc1028acbef2eb4a6f9"}', '2019-11-16 10:27:23', NULL, 'N'),
	(44, 'I', 234, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "aaaad82105b732de6157b2d8d6cfe396"}', '2019-11-16 10:33:48', NULL, 'N'),
	(45, 'I', 235, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "29d765235a7a73a01287113c92d48af3"}', '2019-11-16 10:38:33', NULL, 'N'),
	(46, 'I', 236, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "5558f16bb3b2c3993a1d552dd02b2a32"}', '2019-11-16 10:45:22', NULL, 'N'),
	(47, 'I', 237, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7cc3f721b18df40645a8ae19089eeddf"}', '2019-11-16 10:51:11', NULL, 'N'),
	(48, 'I', 238, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a9771c80078ff1b42a427dbef47685b8"}', '2019-11-16 10:58:35', NULL, 'N'),
	(49, 'I', 239, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "6755aacd55c06e9aec8121c3f196b6f4"}', '2019-11-16 11:07:53', NULL, 'N'),
	(50, 'I', 240, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "82e4fb116305a44d66d4ee85c40873aa"}', '2019-11-16 11:15:18', NULL, 'N'),
	(51, 'I', 241, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "cdac513e58e4048aa121c1dd7739c664"}', '2019-11-16 11:20:33', NULL, 'N'),
	(52, 'I', 242, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "366126fa72b2d5a1a0980eae2c686958"}', '2019-11-16 11:23:35', NULL, 'N'),
	(53, 'I', 243, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "53a893f82cd8c812b2b92aafcebb445a"}', '2019-11-16 11:26:53', NULL, 'N'),
	(54, 'I', 244, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c860857b8149c806f0fcea788787845d"}', '2019-11-16 11:37:05', NULL, 'N'),
	(55, 'I', 245, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "15aee239581fc3cdb170e23db83ee838"}', '2019-11-16 11:38:05', NULL, 'N'),
	(56, 'I', 246, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c0689120be7761eaae37119bdb6d56cb"}', '2019-11-16 11:59:52', NULL, 'N'),
	(57, 'I', 246, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "14637"}', '2019-11-16 11:59:54', NULL, 'N'),
	(58, 'I', 247, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d26695764501c0845b83b5a20afee172"}', '2019-11-16 12:01:25', NULL, 'N'),
	(59, 'I', 248, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e8e7f4bf047be8f1c11ab2ff3ca11357"}', '2019-11-16 12:03:09', NULL, 'N'),
	(60, 'I', 248, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "68281"}', '2019-11-16 12:03:13', NULL, 'N'),
	(61, 'I', 249, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "855d83997503e6809f3bd37f790ae34b"}', '2019-11-16 12:03:49', NULL, 'N'),
	(62, 'I', 249, NULL, 'passReset', '{"via": "0", "UUID": "df62760d990c1aafcc391a0f73707dfa", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:03:54', NULL, 'N'),
	(63, 'I', 249, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "84649"}', '2019-11-16 12:03:56', NULL, 'N'),
	(64, 'I', 250, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d550d25261129e1045e99015599bd35e"}', '2019-11-16 12:04:50', NULL, 'N'),
	(65, 'I', 250, NULL, 'passReset', '{"via": "0", "UUID": "c58331ef3c104bed3f6d0cb4da73d70b", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:04:53', NULL, 'N'),
	(66, 'I', 250, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "24612"}', '2019-11-16 12:04:54', NULL, 'N'),
	(67, 'I', 251, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "47bb94cc1bedd70f414ed487a00fa04e"}', '2019-11-16 12:07:09', NULL, 'N'),
	(68, 'I', 251, NULL, 'passReset', '{"via": "0", "UUID": "146220b6ec0a66472128c33c89709f93", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:07:10', NULL, 'N'),
	(69, 'I', 251, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "90949"}', '2019-11-16 12:07:10', NULL, 'N'),
	(70, 'I', 252, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b1a3bdc75dc25057db88c8f8c05fbfac"}', '2019-11-16 12:07:39', NULL, 'N'),
	(71, 'I', 252, NULL, 'passReset', '{"via": "0", "UUID": "ede95c8a988c2fc652200c1f0e1e0050", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:07:41', NULL, 'N'),
	(72, 'I', 252, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "72909"}', '2019-11-16 12:07:41', NULL, 'N'),
	(73, 'I', 253, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "fc9693b8b1c5797c6a87886cd5726fe2"}', '2019-11-16 12:08:30', NULL, 'N'),
	(74, 'I', 253, NULL, 'passReset', '{"via": "Web", "UUID": "160d7cfa281ec87cf4bc5e5259ea6f0a", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:08:31', NULL, 'N'),
	(75, 'I', 253, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "93368"}', '2019-11-16 12:08:32', NULL, 'N'),
	(76, 'I', 254, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "05c673496834024d37c176ac635884dd"}', '2019-11-16 12:11:37', NULL, 'N'),
	(77, 'I', 254, NULL, 'passReset', '{"via": "Web", "UUID": "56d0ac8f1f5d241329d303d43574280b", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:11:38', NULL, 'N'),
	(78, 'I', 255, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "6ef19b70b7c13274e95f0e2fdfccc104"}', '2019-11-16 12:12:43', NULL, 'N'),
	(79, 'I', 255, NULL, 'passReset', '{"via": "Web", "UUID": "e9f66fba97e459509daa8eb8f0174989", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:12:45', NULL, 'N'),
	(80, 'I', 255, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "42335"}', '2019-11-16 12:12:46', NULL, 'N'),
	(81, 'I', 256, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0dcd0360601a47c67e24a3beffcffbc0"}', '2019-11-16 12:37:34', NULL, 'N'),
	(82, 'I', 256, NULL, 'passReset', '{"via": "Web", "UUID": "343e9f0100edc0ae32750cf70a7f1805", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 12:37:35', NULL, 'N'),
	(83, 'I', 256, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "17487"}', '2019-11-16 12:37:36', NULL, 'N'),
	(84, 'I', 257, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d9905823b5dec4a690d04ac796b52caa"}', '2019-11-16 14:22:24', NULL, 'N'),
	(85, 'I', 257, NULL, 'passReset', '{"via": "Web", "UUID": "c69cd81f73b03b644e57129ac162b83f", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 14:22:24', NULL, 'N'),
	(86, 'I', 258, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "31d9569f295c684445d13114e64c1baf"}', '2019-11-16 14:28:27', NULL, 'N'),
	(87, 'I', 259, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "4b04f227eed305a6b8f313d03f8a5f4a"}', '2019-11-16 14:29:00', NULL, 'N'),
	(88, 'I', 259, NULL, 'passReset', '{"via": "Web", "UUID": "eda989014ed3b403307c03edd65484d0", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 14:29:01', NULL, 'N'),
	(89, 'I', 260, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "16da3302a70d938e38e533e127a62b92"}', '2019-11-16 14:31:00', NULL, 'N'),
	(90, 'I', 261, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3004ac4806ef0a10007a86d1a1469c32"}', '2019-11-16 15:10:18', NULL, 'N'),
	(91, 'I', 262, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ab0dae44fd1a48a3a4b838f58488ddff"}', '2019-11-16 15:16:45', NULL, 'N'),
	(92, 'I', 263, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "13ea2027f97222c839ce6b2349c642e3"}', '2019-11-16 15:17:09', NULL, 'N'),
	(93, 'I', 264, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c9cd1278471bbecf035e731c943b7bf3"}', '2019-11-16 15:17:50', NULL, 'N'),
	(94, 'I', 265, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "48c431f25063ebb149365c0e422a2196"}', '2019-11-16 15:19:00', NULL, 'N'),
	(95, 'I', 266, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "dfadefa933358f78b6a08ba6f4724f16"}', '2019-11-16 15:19:51', NULL, 'N'),
	(96, 'I', 267, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f8f06038cf4d9172022f91dd14205a03"}', '2019-11-16 15:21:14', NULL, 'N'),
	(97, 'I', 268, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "04f7920eac2a1aaf881935fe9142aed6"}', '2019-11-16 15:22:22', NULL, 'N'),
	(98, 'I', 268, NULL, 'passReset', '{"via": "Web", "UUID": "a8a27382eaeddc934c4d60646a49e8d5", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:22:26', NULL, 'N'),
	(99, 'I', 268, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "57386"}', '2019-11-16 15:22:26', NULL, 'N'),
	(100, 'I', 269, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d0e24de0862f4cd3e11ac377bd59e2be"}', '2019-11-16 15:23:32', NULL, 'N'),
	(101, 'I', 269, NULL, 'passReset', '{"via": "Web", "UUID": "9603fc86a7357f4b491d103c5aadf4ea", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:23:36', NULL, 'N'),
	(102, 'I', 269, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "20637"}', '2019-11-16 15:23:37', NULL, 'N'),
	(103, 'I', 270, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "6a6790f25bb7ad9e103a9b38a3b9afc7"}', '2019-11-16 15:24:46', NULL, 'N'),
	(104, 'I', 270, NULL, 'passReset', '{"via": "Web", "UUID": "29f1df32430a577bf558cc8f4a01a942", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:24:54', NULL, 'N'),
	(105, 'I', 270, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "33820"}', '2019-11-16 15:24:56', NULL, 'N'),
	(106, 'I', 271, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "904e56a2f04d98b5e5a743346552cbdd"}', '2019-11-16 15:27:21', NULL, 'N'),
	(107, 'I', 271, NULL, 'passReset', '{"via": "Web", "UUID": "21e1953484d20a6b5fc6e995d456ee57", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:27:27', NULL, 'N'),
	(108, 'I', 271, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "17137"}', '2019-11-16 15:27:29', NULL, 'N'),
	(109, 'I', 272, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2e9d7f4be13b2810d5347bdea51a94a8"}', '2019-11-16 15:29:07', NULL, 'N'),
	(110, 'I', 272, NULL, 'passReset', '{"via": "Web", "UUID": "b6798f66cae5da2670d14692eb4f6066", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:29:10', NULL, 'N'),
	(111, 'I', 272, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "36530"}', '2019-11-16 15:29:12', NULL, 'N'),
	(112, 'I', 273, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "9f0087c1465fb3a9d8e8ef36e5a11536"}', '2019-11-16 15:32:27', NULL, 'N'),
	(113, 'I', 273, NULL, 'passReset', '{"via": "Web", "UUID": "e01d217d4c480006a0122f0819dd4b56", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:32:29', NULL, 'N'),
	(114, 'I', 273, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "58381"}', '2019-11-16 15:32:31', NULL, 'N'),
	(115, 'I', 274, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7b07589555f383e3a3406c9bf4ee2dae"}', '2019-11-16 15:33:55', NULL, 'N'),
	(116, 'I', 274, NULL, 'passReset', '{"via": "Web", "UUID": "099440b814c73b3513b26ecee49c0da8", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:33:57', NULL, 'N'),
	(117, 'I', 274, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "22837"}', '2019-11-16 15:33:58', NULL, 'N'),
	(118, 'I', 275, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c4d2c50ef43c334ba5e29b3d51e32da1"}', '2019-11-16 15:37:33', NULL, 'N'),
	(119, 'I', 275, NULL, 'passReset', '{"via": "Web", "UUID": "21a4eba78070e985dfe060a715a3f51a", "usrName": "unit", "usrFamily": "test"}', '2019-11-16 15:37:35', NULL, 'N'),
	(120, 'I', 275, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "99420"}', '2019-11-16 15:37:36', NULL, 'N'),
	(121, 'I', 276, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7bfbdb01b9a54dfd5e48c4f393b98bdc"}', '2019-11-19 10:17:28', NULL, 'N'),
	(122, 'I', 276, NULL, 'passReset', '{"via": "Web", "UUID": "ca2025440d22c3c73e86120f36876a2a", "usrName": "unit", "usrFamily": "test"}', '2019-11-19 10:17:30', NULL, 'N'),
	(123, 'I', 276, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "38108"}', '2019-11-19 10:17:31', NULL, 'N'),
	(124, 'I', 277, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a42bc83080c2947375567d3bb869d348"}', '2019-11-19 10:39:18', NULL, 'N'),
	(125, 'I', 277, NULL, 'passReset', '{"via": "Web", "UUID": "1d231ee09fb180092363099955c8da66", "usrName": "unit", "usrFamily": "test"}', '2019-11-19 10:39:43', NULL, 'N'),
	(126, 'I', 277, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "75619"}', '2019-11-19 10:39:44', NULL, 'N'),
	(127, 'I', 278, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "69f501bb6e9384acda20ca3d37f6d442"}', '2019-11-19 10:39:55', NULL, 'N'),
	(128, 'I', 278, NULL, 'passReset', '{"via": "Web", "UUID": "cf968b52e183b1263ed3d34a80b45241", "usrName": "unit", "usrFamily": "test"}', '2019-11-19 10:40:00', NULL, 'N'),
	(129, 'I', 278, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "75318"}', '2019-11-19 10:40:02', NULL, 'N'),
	(130, 'I', 279, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e3b692d28ef206bb118f77dc0b07319c"}', '2019-11-19 10:42:33', NULL, 'N'),
	(131, 'I', 279, NULL, 'passReset', '{"via": "Web", "UUID": "356caec33c46a5f2dd2f7bc10325f552", "usrName": "unit", "usrFamily": "test"}', '2019-11-19 10:42:37', NULL, 'N'),
	(132, 'I', 279, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "97857"}', '2019-11-19 10:42:40', NULL, 'N'),
	(133, 'I', 280, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0839c3fb1710532f197f1b33044f018a"}', '2019-11-20 11:39:28', NULL, 'N'),
	(134, 'I', 280, NULL, 'passReset', '{"via": "Web", "UUID": "01d4f53075234e9f1922b4a4e97cabc6", "usrName": "unit", "usrFamily": "test"}', '2019-11-20 11:39:29', NULL, 'N'),
	(135, 'I', 280, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "81137"}', '2019-11-20 11:39:30', NULL, 'N'),
	(136, 'I', 281, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "70b6da629d02cc51abddfc381041ac2f"}', '2019-11-20 13:04:20', NULL, 'N'),
	(137, 'I', 281, NULL, 'passReset', '{"via": "Web", "UUID": "744a7827a0dd2a48844dcb96fc96267c", "usrName": "unit", "usrFamily": "test"}', '2019-11-20 13:04:42', NULL, 'N'),
	(138, 'I', 281, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "96838"}', '2019-11-20 13:05:05', NULL, 'N'),
	(139, 'I', 282, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "9cbe9b14a93866abe2c33cd21a00e3cf"}', '2019-11-20 13:06:56', NULL, 'N'),
	(140, 'I', 282, NULL, 'passReset', '{"via": "Web", "UUID": "f2c4d30043547844bed1d738ada0dc3d", "usrName": "unit", "usrFamily": "test"}', '2019-11-20 13:07:07', NULL, 'N'),
	(141, 'I', 282, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "20639"}', '2019-11-20 13:07:13', NULL, 'N'),
	(142, 'I', 283, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d0953175c7037a97dfb4d86760eae5ce"}', '2019-11-20 13:09:08', NULL, 'N'),
	(143, 'I', 283, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "32623"}', '2019-11-20 13:09:39', NULL, 'N'),
	(144, 'I', 284, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "1f5c04b6b3df75f5f333b0f4b78def3b"}', '2019-11-20 19:22:46', NULL, 'N'),
	(145, 'I', 284, NULL, 'passReset', '{"via": "Web", "UUID": "b4510438c6caf6a8a57ab721a6cb1f81", "usrName": "unit", "usrFamily": "test"}', '2019-11-20 19:23:10', NULL, 'N'),
	(146, 'I', 284, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "66817"}', '2019-11-20 19:23:16', NULL, 'N'),
	(147, 'I', 285, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "587435b12c69d8c956c9f225352e3453"}', '2019-11-20 19:24:03', NULL, 'N'),
	(148, 'I', 285, NULL, 'passReset', '{"via": "Web", "UUID": "be3d0e7cd2ddfea0063d97d0ef91dcf0", "usrName": "unit", "usrFamily": "test"}', '2019-11-20 19:24:08', NULL, 'N'),
	(149, 'I', 286, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7f9ecae4e01c7d41b66e3150f74a75cc"}', '2019-11-20 19:24:24', NULL, 'N'),
	(150, 'I', 286, NULL, 'passReset', '{"via": "Web", "UUID": "8e8d97cab6b47be9b97fe9dafd5789f3", "usrName": "unit", "usrFamily": "test"}', '2019-11-20 19:24:28', NULL, 'N'),
	(151, 'I', 287, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a8284277e4550d224c2cadf2981c9652"}', '2019-11-21 14:15:12', NULL, 'N'),
	(152, 'I', 287, NULL, 'passReset', '{"via": "Web", "UUID": "2b11aea4c2414a8aef83098174418c41", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 14:15:17', NULL, 'N'),
	(153, 'I', 288, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "390b79c39b3910eece57b92dae8f1b2a"}', '2019-11-21 15:47:26', NULL, 'N'),
	(154, 'I', 288, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "60391"}', '2019-11-21 15:47:57', NULL, 'N'),
	(155, 'I', 288, NULL, 'passReset', '{"via": "Web", "UUID": "6c3012ed870d9461225eeb67af33fc40", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 15:50:55', NULL, 'N'),
	(156, 'I', 289, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e2a5dc1afe434ce4d050cc239a147a1a"}', '2019-11-21 15:54:49', NULL, 'N'),
	(157, 'I', 289, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "38576"}', '2019-11-21 15:55:20', NULL, 'N'),
	(158, 'I', 290, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "82b9f4954fef4c88c78f3e44309d237e"}', '2019-11-21 15:56:16', NULL, 'N'),
	(159, 'I', 290, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "16028"}', '2019-11-21 15:56:54', NULL, 'N'),
	(160, 'I', 291, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3a32b54bec436911fe1da8b0d9e2c3c3"}', '2019-11-21 16:17:42', NULL, 'N'),
	(161, 'I', 291, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "43939"}', '2019-11-21 16:18:19', NULL, 'N'),
	(162, 'I', 292, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0c8cdf222dc969f1e18bc759ae273f15"}', '2019-11-21 16:18:46', NULL, 'N'),
	(163, 'I', 292, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "77374"}', '2019-11-21 16:19:17', NULL, 'N'),
	(164, 'I', 293, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "5eba0049861f774a3ba9e1bfb838a73a"}', '2019-11-21 16:43:36', NULL, 'N'),
	(165, 'I', 293, NULL, 'passReset', '{"via": "Web", "UUID": "0cf3b471f792973a8f1326c9c7fc0fa7", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 16:43:38', NULL, 'N'),
	(166, 'I', 293, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "24399"}', '2019-11-21 16:43:39', NULL, 'N'),
	(167, 'I', 294, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "5a9b95d8f16e71f81a650b066fdcd695"}', '2019-11-21 16:44:44', NULL, 'N'),
	(168, 'I', 294, NULL, 'passReset', '{"via": "Web", "UUID": "57c484c6461bd4636377ebc2a60f0194", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 16:44:47', NULL, 'N'),
	(169, 'I', 294, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "87112"}', '2019-11-21 16:44:48', NULL, 'N'),
	(170, 'I', 295, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "eb136472169d017f75d4cb8fcd4eb54f"}', '2019-11-21 18:25:38', NULL, 'N'),
	(171, 'I', 296, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2059170564dea1c802417dbc6ae3bc6f"}', '2019-11-21 18:26:15', NULL, 'N'),
	(172, 'I', 296, NULL, 'passReset', '{"via": "Web", "UUID": "b1fa76c661ab71617fef18b34d4dc908", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 18:26:23', NULL, 'N'),
	(173, 'I', 296, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "18492"}', '2019-11-21 18:26:25', NULL, 'N'),
	(174, 'I', 297, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e7f7db1805b5cdd8ddcc0a94cfbdeeff"}', '2019-11-21 18:48:53', NULL, 'N'),
	(175, 'I', 297, NULL, 'passReset', '{"via": "Web", "UUID": "ce55240816f44e0b27ea0187a92b938e", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 18:49:01', NULL, 'N'),
	(176, 'I', 297, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "79015"}', '2019-11-21 18:49:03', NULL, 'N'),
	(177, 'I', 298, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "101f90fb999d4131907279fc12d0c0a4"}', '2019-11-21 18:49:31', NULL, 'N'),
	(178, 'I', 298, NULL, 'passReset', '{"via": "Web", "UUID": "0493b709b718c08419b75afb04a262ec", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 18:49:39', NULL, 'N'),
	(179, 'I', 298, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "46076"}', '2019-11-21 18:49:40', NULL, 'N'),
	(180, 'I', 299, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "564f5fa7e6f0bdfa6829d34229a694e7"}', '2019-11-21 19:23:03', NULL, 'N'),
	(181, 'I', 299, NULL, 'passReset', '{"via": "Web", "UUID": "dac3c54494a91842dab1b5944ed618f2", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 19:23:09', NULL, 'N'),
	(182, 'I', 299, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "84803"}', '2019-11-21 19:23:10', NULL, 'N'),
	(183, 'I', 300, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ffe8b642a544c7a7a16f8f4f8af0443a"}', '2019-11-21 20:18:22', NULL, 'N'),
	(184, 'I', 300, NULL, 'passReset', '{"via": "Web", "UUID": "9b55458142c446607eb3d153706ac472", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 20:18:28', NULL, 'N'),
	(185, 'I', 300, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "49654"}', '2019-11-21 20:18:30', NULL, 'N'),
	(186, 'I', 301, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "66b60f86a7b5b322747556700d210a9d"}', '2019-11-21 20:26:17', NULL, 'N'),
	(187, 'I', 301, NULL, 'passReset', '{"via": "Web", "UUID": "1600c1d198471fe04a299d10652a9b94", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 20:26:23', NULL, 'N'),
	(188, 'I', 301, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "59295"}', '2019-11-21 20:26:25', NULL, 'N'),
	(189, 'I', 302, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7332e1a656f82eaac3c046615410d14c"}', '2019-11-21 23:16:12', NULL, 'N'),
	(190, 'I', 302, NULL, 'passReset', '{"via": "Web", "UUID": "8964fb0c2c3641b5afab0ad4bd5e89f6", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 23:16:19', NULL, 'N'),
	(191, 'I', 302, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "81373"}', '2019-11-21 23:16:21', NULL, 'N'),
	(192, 'I', 303, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "808699c8ecdcbbda6c0d8f26faeacc7e"}', '2019-11-21 23:16:45', NULL, 'N'),
	(193, 'I', 303, NULL, 'passReset', '{"via": "Web", "UUID": "67b7a8c59f082662f8aacdbf972998ff", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 23:16:54', NULL, 'N'),
	(194, 'I', 303, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "32218"}', '2019-11-21 23:16:54', NULL, 'N'),
	(195, 'I', 304, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3cb0cb992fb44b389c6b0ca97e85a136"}', '2019-11-21 23:17:59', NULL, 'N'),
	(196, 'I', 304, NULL, 'passReset', '{"via": "Web", "UUID": "3d2bcc64f083703aa798a8bfc3f273c7", "usrName": "unit", "usrFamily": "test"}', '2019-11-21 23:18:03', NULL, 'N'),
	(197, 'I', 304, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "59448"}', '2019-11-21 23:18:06', NULL, 'N'),
	(198, 'I', 305, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "37d02523a54a448f348e1147dabcdb69"}', '2019-11-23 05:34:29', NULL, 'N'),
	(199, 'I', 305, NULL, 'passReset', '{"via": "Web", "UUID": "96e65ba41baff298e5686bf8824b68a0", "usrName": "unit", "usrFamily": "test"}', '2019-11-23 05:34:31', NULL, 'N'),
	(200, 'I', 305, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "71416"}', '2019-11-23 05:34:33', NULL, 'N'),
	(201, 'I', 306, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f71009760862cc5f96e0d3628a7e9d2d"}', '2019-11-24 12:02:16', NULL, 'N'),
	(202, 'I', 306, NULL, 'passReset', '{"via": "Web", "UUID": "7f9f72401d29f23418396580d89c49a9", "usrName": "unit", "usrFamily": "test"}', '2019-11-24 12:02:23', NULL, 'N'),
	(203, 'I', 306, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "60367"}', '2019-11-24 12:02:33', NULL, 'N'),
	(204, 'I', 307, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "710ebaa67803f1c7127a231835938dce"}', '2019-11-24 12:16:21', NULL, 'N'),
	(205, 'I', 307, NULL, 'passReset', '{"via": "Web", "UUID": "1717777b64039aba05fd2cca14838e28", "usrName": "unit", "usrFamily": "test"}', '2019-11-24 12:16:23', NULL, 'N'),
	(206, 'I', 307, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "58763"}', '2019-11-24 12:16:23', NULL, 'N'),
	(207, 'I', 308, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f60fbb056d32da34c43b4eb899f1d98f"}', '2019-11-24 12:19:16', NULL, 'N'),
	(208, 'I', 308, NULL, 'passReset', '{"via": "Web", "UUID": "d591f231ca80f45d0e84523e575874eb", "usrName": "unit", "usrFamily": "test"}', '2019-11-24 12:19:24', NULL, 'N'),
	(209, 'I', 309, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "77ad1d144bb218ab09c992ff0fa589a8"}', '2019-11-24 13:56:55', NULL, 'N'),
	(210, 'I', 309, NULL, 'passReset', '{"via": "Web", "UUID": "1f1743499715a46043b10a018a3d9072", "usrName": "unit", "usrFamily": "test"}', '2019-11-24 13:56:56', NULL, 'N'),
	(211, 'I', 309, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "95595"}', '2019-11-24 13:56:57', NULL, 'N'),
	(212, 'I', 310, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "541fc73dcadf3ea52e8cfac07ffc81d6"}', '2019-11-24 14:13:06', NULL, 'N'),
	(213, 'I', 310, NULL, 'passReset', '{"via": "Web", "UUID": "bab9017b039a73a8a96b0d0c16950e33", "usrName": "unit", "usrFamily": "test"}', '2019-11-24 14:13:24', NULL, 'N'),
	(214, 'I', 310, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "35495"}', '2019-11-24 14:13:28', NULL, 'N'),
	(215, 'I', 311, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "cdb5ee5d64c136966bf71b0721d9ef43"}', '2019-11-24 15:10:53', NULL, 'N'),
	(216, 'I', 311, NULL, 'passReset', '{"via": "Web", "UUID": "723b531e4455a7769d3e391c21809fee", "usrName": "unit", "usrFamily": "test"}', '2019-11-24 15:10:54', NULL, 'N'),
	(217, 'I', 311, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "76648"}', '2019-11-24 15:10:54', NULL, 'N'),
	(218, 'I', 312, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a47376bcec10f4b60f1b586c197427b6"}', '2019-12-01 05:19:27', NULL, 'N'),
	(219, 'I', 312, NULL, 'passReset', '{"via": "Web", "UUID": "8217e869716e372f337a06c1e6c99122", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:19:29', NULL, 'N'),
	(220, 'I', 312, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "25420"}', '2019-12-01 05:19:30', NULL, 'N'),
	(221, 'I', 313, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d39074e5f82aff82a920605ad81d5d97"}', '2019-12-01 05:20:07', NULL, 'N'),
	(222, 'I', 313, NULL, 'passReset', '{"via": "Web", "UUID": "16e8a2415c205a187035248250342488", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:20:09', NULL, 'N'),
	(223, 'I', 313, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "38205"}', '2019-12-01 05:20:10', NULL, 'N'),
	(224, 'I', 314, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d46f5589423ee6b9ac1c7f1604837bad"}', '2019-12-01 05:20:34', NULL, 'N'),
	(225, 'I', 314, NULL, 'passReset', '{"via": "Web", "UUID": "d6d6a91928740295e831358e8376d5eb", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:20:37', NULL, 'N'),
	(226, 'I', 314, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "96041"}', '2019-12-01 05:20:39', NULL, 'N'),
	(227, 'I', 315, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "727e840637c6406dc35b33113096edeb"}', '2019-12-01 05:22:29', NULL, 'N'),
	(228, 'I', 315, NULL, 'passReset', '{"via": "Web", "UUID": "17be61702682a9bcdee2c171c5fbb487", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:22:29', NULL, 'N'),
	(229, 'I', 316, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b4aaa0888f91bde04984f76b8e9c0430"}', '2019-12-01 05:22:35', NULL, 'N'),
	(230, 'I', 316, NULL, 'passReset', '{"via": "Web", "UUID": "f5e44789726f4556e613707da417a75a", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:22:37', NULL, 'N'),
	(231, 'I', 316, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "78726"}', '2019-12-01 05:22:38', NULL, 'N'),
	(232, 'I', 317, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "457ee4ce0476afa2b31f656742129635"}', '2019-12-01 05:22:58', NULL, 'N'),
	(233, 'I', 317, NULL, 'passReset', '{"via": "Web", "UUID": "cba2cad787a66ccc72326a763fd378f6", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:23:00', NULL, 'N'),
	(234, 'I', 317, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "78368"}', '2019-12-01 05:23:01', NULL, 'N'),
	(235, 'I', 318, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e255bb83c48f7f3e432709b6437a1393"}', '2019-12-01 05:23:36', NULL, 'N'),
	(236, 'I', 318, NULL, 'passReset', '{"via": "Web", "UUID": "e0c6b90d2087d10e164eec33aea2fd1f", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:23:39', NULL, 'N'),
	(237, 'I', 318, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "38834"}', '2019-12-01 05:23:42', NULL, 'N'),
	(238, 'I', 319, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3cf404f135a71af48f654bb03fb176c9"}', '2019-12-01 05:24:17', NULL, 'N'),
	(239, 'I', 319, NULL, 'passReset', '{"via": "Web", "UUID": "01a92a3df9fbc11e2da3d3fb0e38d075", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:24:18', NULL, 'N'),
	(240, 'I', 320, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "446e5d73c6e63e65066299d95c540735"}', '2019-12-01 05:24:27', NULL, 'N'),
	(241, 'I', 320, NULL, 'passReset', '{"via": "Web", "UUID": "a92d9ed3d415f528ef9cd3559dcd3370", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:24:29', NULL, 'N'),
	(242, 'I', 320, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "16095"}', '2019-12-01 05:24:30', NULL, 'N'),
	(243, 'I', 321, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2aac73318ae743647b471bcc9b95c7eb"}', '2019-12-01 05:31:23', NULL, 'N'),
	(244, 'I', 321, NULL, 'passReset', '{"via": "Web", "UUID": "c0f1d68bb720bc9cabeedfaacb0571a5", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 05:31:25', NULL, 'N'),
	(245, 'I', 321, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "92531"}', '2019-12-01 05:31:26', NULL, 'N'),
	(246, 'I', 322, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f55d9c81a2e7d83d845518a11373145f"}', '2019-12-01 09:59:57', NULL, 'N'),
	(247, 'I', 322, NULL, 'passReset', '{"via": "Web", "UUID": "c3a32da69f1c97ec7691611f72b4820f", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 09:59:58', NULL, 'N'),
	(248, 'I', 322, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "88149"}', '2019-12-01 09:59:59', NULL, 'N'),
	(249, 'I', 323, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "cea9a1ecf40f71346fc10eecb09ea1da"}', '2019-12-01 10:05:09', NULL, 'N'),
	(250, 'I', 323, NULL, 'passReset', '{"via": "Web", "UUID": "900ec696bf1302560fedfc821bc8b08b", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:05:11', NULL, 'N'),
	(251, 'I', 323, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "20027"}', '2019-12-01 10:05:12', NULL, 'N'),
	(252, 'I', 324, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "32e84192d5d8997e0e70df6bf9dbc3e6"}', '2019-12-01 10:05:44', NULL, 'N'),
	(253, 'I', 324, NULL, 'passReset', '{"via": "Web", "UUID": "ac8e487a7e6556c18b061654a547395d", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:05:47', NULL, 'N'),
	(254, 'I', 324, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "48482"}', '2019-12-01 10:05:48', NULL, 'N'),
	(255, 'I', 325, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a933bed66631be87047e424cb8e1a36f"}', '2019-12-01 10:09:00', NULL, 'N'),
	(256, 'I', 325, NULL, 'passReset', '{"via": "Web", "UUID": "0eaa333b4c24d04dadb7c3b6cbecbb70", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:09:05', NULL, 'N'),
	(257, 'I', 325, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "65856"}', '2019-12-01 10:09:06', NULL, 'N'),
	(258, 'I', 326, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "47c020fa0c1998b4602bac159094d1e2"}', '2019-12-01 10:11:04', NULL, 'N'),
	(259, 'I', 326, NULL, 'passReset', '{"via": "Web", "UUID": "125680e4d80b961f786681576ea09ee9", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:11:06', NULL, 'N'),
	(260, 'I', 326, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "20146"}', '2019-12-01 10:11:08', NULL, 'N'),
	(261, 'I', 327, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "07f3cf8579e261dcda8fcd78fd3ed9a9"}', '2019-12-01 10:16:40', NULL, 'N'),
	(262, 'I', 327, NULL, 'passReset', '{"via": "Web", "UUID": "cea27602cd2866c693c878cef2afeeb7", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:16:43', NULL, 'N'),
	(263, 'I', 327, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "29998"}', '2019-12-01 10:16:45', NULL, 'N'),
	(264, 'I', 328, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "da8bd6572c3875992f805ba99b5d89ac"}', '2019-12-01 10:18:29', NULL, 'N'),
	(265, 'I', 328, NULL, 'passReset', '{"via": "Web", "UUID": "ecb5cd9f4d3c681ba9f644598c429023", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:18:33', NULL, 'N'),
	(266, 'I', 328, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "88256"}', '2019-12-01 10:18:34', NULL, 'N'),
	(267, 'I', 329, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d403c775b4f5d8fb40665cb630abdc0b"}', '2019-12-01 10:19:26', NULL, 'N'),
	(268, 'I', 329, NULL, 'passReset', '{"via": "Web", "UUID": "053f46dbcd25f465b9c597d5b58735cb", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:19:30', NULL, 'N'),
	(269, 'I', 329, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "69933"}', '2019-12-01 10:19:31', NULL, 'N'),
	(270, 'I', 330, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7d6c257b8851259f34853004f16f2bc3"}', '2019-12-01 10:34:52', NULL, 'N'),
	(271, 'I', 330, NULL, 'passReset', '{"via": "Web", "UUID": "bb04269097397e2ee66084e7c134076f", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:34:54', NULL, 'N'),
	(272, 'I', 330, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "28322"}', '2019-12-01 10:34:55', NULL, 'N'),
	(273, 'I', 331, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ded141b495186ff6713d5717c1b678e8"}', '2019-12-01 10:46:11', NULL, 'N'),
	(274, 'I', 331, NULL, 'passReset', '{"via": "Web", "UUID": "0b998b84f54b612fb0fe2805f16f5a83", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:46:13', NULL, 'N'),
	(275, 'I', 331, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "73572"}', '2019-12-01 10:46:13', NULL, 'N'),
	(276, 'I', 332, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f2a94305547b9cc6798b9bc834ed1568"}', '2019-12-01 10:47:19', NULL, 'N'),
	(277, 'I', 332, NULL, 'passReset', '{"via": "Web", "UUID": "c6a5c6763b98670340bf52f3467e4768", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:47:21', NULL, 'N'),
	(278, 'I', 332, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "86238"}', '2019-12-01 10:47:23', NULL, 'N'),
	(279, 'I', 333, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "959ec162853aa4cbd9e55bde7539f57f"}', '2019-12-01 10:48:25', NULL, 'N'),
	(280, 'I', 333, NULL, 'passReset', '{"via": "Web", "UUID": "8208393866cc4c227474b42e45da384c", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:48:27', NULL, 'N'),
	(281, 'I', 333, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "26399"}', '2019-12-01 10:48:29', NULL, 'N'),
	(282, 'I', 334, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "817e9a66fa83fc1f8bdbee489b8f5aa9"}', '2019-12-01 10:49:50', NULL, 'N'),
	(283, 'I', 334, NULL, 'passReset', '{"via": "Web", "UUID": "b07b04320037f0373a3e112f640abac5", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:49:51', NULL, 'N'),
	(284, 'I', 335, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "293f78e5ba2e6fc9f98abd99629d345a"}', '2019-12-01 10:51:02', NULL, 'N'),
	(285, 'I', 335, NULL, 'passReset', '{"via": "Web", "UUID": "b26a911451eb49120c6090f75ad81d99", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:51:06', NULL, 'N'),
	(286, 'I', 336, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "4f88e7bb8d292e90a43b553797ab9f28"}', '2019-12-01 10:51:38', NULL, 'N'),
	(287, 'I', 336, NULL, 'passReset', '{"via": "Web", "UUID": "621773aaf902d7ea1363865639dea508", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 10:51:42', NULL, 'N'),
	(288, 'I', 336, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "95148"}', '2019-12-01 10:51:42', NULL, 'N'),
	(289, 'I', 337, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a1790dd654cfd3b1476401480799b3bb"}', '2019-12-01 11:07:34', NULL, 'N'),
	(290, 'I', 337, NULL, 'passReset', '{"via": "Web", "UUID": "70c5059c577462d4774cfa4c1418f1b6", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:07:38', NULL, 'N'),
	(291, 'I', 337, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "52323"}', '2019-12-01 11:07:39', NULL, 'N'),
	(292, 'I', 338, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "36f2eeeaa6eb06efed1437af46ba5b99"}', '2019-12-01 11:08:36', NULL, 'N'),
	(293, 'I', 338, NULL, 'passReset', '{"via": "Web", "UUID": "ee25402546b268ede5614c449a12cfea", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:08:37', NULL, 'N'),
	(294, 'I', 338, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "53542"}', '2019-12-01 11:08:38', NULL, 'N'),
	(295, 'I', 339, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a461905c9bdf007119018245ad7f83f3"}', '2019-12-01 11:10:06', NULL, 'N'),
	(296, 'I', 339, NULL, 'passReset', '{"via": "Web", "UUID": "50dca0944d96ff75c3f867fd06579235", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:10:07', NULL, 'N'),
	(297, 'I', 339, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "29366"}', '2019-12-01 11:10:08', NULL, 'N'),
	(298, 'I', 340, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7ae7282780e425a4d9e0f28c02aafb45"}', '2019-12-01 11:11:08', NULL, 'N'),
	(299, 'I', 340, NULL, 'passReset', '{"via": "Web", "UUID": "d1f8b9a974aa1bec961542b6711f4bf2", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:11:10', NULL, 'N'),
	(300, 'I', 340, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "53651"}', '2019-12-01 11:11:11', NULL, 'N'),
	(301, 'I', 341, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0286be08a1c203833e72e817ac67402c"}', '2019-12-01 11:12:43', NULL, 'N'),
	(302, 'I', 341, NULL, 'passReset', '{"via": "Web", "UUID": "cdbc1bf3d7c6094d0ad6a3d404d3b342", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:12:45', NULL, 'N'),
	(303, 'I', 341, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "53253"}', '2019-12-01 11:12:46', NULL, 'N'),
	(304, 'I', 342, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "45d362360456c371ff77b736306e5458"}', '2019-12-01 11:20:10', NULL, 'N'),
	(305, 'I', 342, NULL, 'passReset', '{"via": "Web", "UUID": "4184276a57f4d651f7b4958f759bc888", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:20:12', NULL, 'N'),
	(306, 'I', 342, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "64858"}', '2019-12-01 11:20:13', NULL, 'N'),
	(307, 'I', 343, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "860e25d315340d49ca3a8710a5fd5a12"}', '2019-12-01 11:21:33', NULL, 'N'),
	(308, 'I', 343, NULL, 'passReset', '{"via": "Web", "UUID": "5ca38f8e06bfc0c1e1aa992a470e2331", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:21:35', NULL, 'N'),
	(309, 'I', 343, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "92414"}', '2019-12-01 11:21:36', NULL, 'N'),
	(310, 'I', 344, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "1d8bbd21cd8d4ca32dc0de6f74afa5c6"}', '2019-12-01 11:23:38', NULL, 'N'),
	(311, 'I', 344, NULL, 'passReset', '{"via": "Web", "UUID": "106f689f51587664b4c29d96afe0cb32", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:23:39', NULL, 'N'),
	(312, 'I', 344, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "75190"}', '2019-12-01 11:23:40', NULL, 'N'),
	(313, 'I', 345, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "7b834ffd82e2adf9bff34cd2bb319a4b"}', '2019-12-01 11:24:18', NULL, 'N'),
	(314, 'I', 345, NULL, 'passReset', '{"via": "Web", "UUID": "2d9528afb85981c13dfc652d3d8a2657", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:24:21', NULL, 'N'),
	(315, 'I', 345, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "34472"}', '2019-12-01 11:24:23', NULL, 'N'),
	(316, 'I', 346, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "28636387ff231f9fd87b3caeed6b1520"}', '2019-12-01 11:24:48', NULL, 'N'),
	(317, 'I', 346, NULL, 'passReset', '{"via": "Web", "UUID": "c407c4c3c211189cc1bbfe303627ca27", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:24:49', NULL, 'N'),
	(318, 'I', 346, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "45326"}', '2019-12-01 11:24:50', NULL, 'N'),
	(319, 'I', 347, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "d0d3c338a44ec8c3846117a1cb856f9a"}', '2019-12-01 11:25:04', NULL, 'N'),
	(320, 'I', 347, NULL, 'passReset', '{"via": "Web", "UUID": "6b578b1b3e920b55c0938703a2c30ada", "usrName": "unit", "usrFamily": "test"}', '2019-12-01 11:25:05', NULL, 'N'),
	(321, 'I', 347, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "46968"}', '2019-12-01 11:25:06', NULL, 'N'),
	(323, 'I', 349, 'asd@bsd.ir', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ec8c2cb2c4759a4dd127a93cd5a8bca5"}', '2020-02-10 13:32:12', NULL, 'N'),
	(324, 'I', 351, 'asd2@bsd.ir', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "8f7ea16a2a8526ecbfbd8b8e480c60d3"}', '2020-02-10 13:54:21', NULL, 'N'),
	(325, 'I', 352, '09121233333', 'approveMobile', '{"usrName": null, "usrFamily": null, "ApprovalCode": "93869"}', '2020-02-10 14:38:08', NULL, 'N'),
	(326, 'I', 349, NULL, 'passReset', '{"via": "E", "UUID": "3a5b1769f567f0f10d42c172fb6f40ec", "usrName": null, "usrFamily": null}', '2020-02-10 15:42:03', NULL, 'N'),
	(327, 'I', 100, 'mehran@gmail.com', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری", "ApprovalCode": "ac44bc6041ed1252d36f5a591d15542b"}', '2020-02-18 07:23:01', NULL, 'N'),
	(328, 'I', 100, '09126174250', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری", "ApprovalCode": "a53692e228cc8d22496c5ef520835fbc"}', '2020-02-18 07:23:02', NULL, 'N'),
	(329, 'I', 100, 'mehran@gmail.com', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری", "ApprovalCode": "2db35df0127801cb2f7c8b44c6061d29"}', '2020-02-18 07:28:25', NULL, 'N'),
	(330, 'I', 100, '09126174250', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری", "ApprovalCode": "03c844f7b6325baba5a6099c8c05291d"}', '2020-02-18 07:28:25', NULL, 'N'),
	(331, 'I', 100, 'mehran@gmail.com', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری1", "ApprovalCode": "70113602d23521c197a9b844ad3bf419"}', '2020-02-18 07:28:33', NULL, 'N'),
	(332, 'I', 100, '09126174250', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری1", "ApprovalCode": "6a733130908d406516b1770ea08c1fad"}', '2020-02-18 07:28:33', NULL, 'N'),
	(333, 'I', 100, 'mehran@gmail.com', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری", "ApprovalCode": "c6b2eb5d0c30dc56803fdafaa6d843ba"}', '2020-02-18 07:28:37', NULL, 'N'),
	(334, 'I', 100, '09126174250', 'approveEmail', '{"usrName": "مهران", "usrFamily": "ضیابری", "ApprovalCode": "ae1a193c9c4f366fe14713842eb3fa3f"}', '2020-02-18 07:28:38', NULL, 'N'),
	(335, 'I', 353, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b995aeeb06155c1edc03621cb705f8d8"}', '2020-02-19 13:01:54', NULL, 'N'),
	(336, 'I', 354, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e313617b10b74b8dbddcca3319e82e1d"}', '2020-02-19 13:02:13', NULL, 'N'),
	(337, 'I', 355, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "55fef6c7b4400aa52c6d83e91b2edf37"}', '2020-02-19 13:03:29', NULL, 'N'),
	(338, 'I', 356, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b7f2ae0c9c466ac5adf9443d04868164"}', '2020-02-19 13:05:39', NULL, 'N'),
	(339, 'I', 357, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "235ebd8ef79e9af83f3f39df123b02d5"}', '2020-02-19 13:06:23', NULL, 'N'),
	(340, 'I', 358, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "394c70a3e10fa40d600f8489254b5192"}', '2020-02-19 13:08:44', NULL, 'N'),
	(341, 'I', 359, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0d8b539cfe29be6bbe5ed0278587df99"}', '2020-02-19 13:09:36', NULL, 'N'),
	(342, 'I', 360, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "cdabec32f32e0bc9a31bb18436a0ff48"}', '2020-02-19 13:10:06', NULL, 'N'),
	(343, 'I', 360, NULL, 'passReset', '{"via": "E", "UUID": "fc5973c2de0ef5ac1023d6ca796930d5", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:10:09', NULL, 'N'),
	(344, 'I', 361, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "e0d0a206bbfbc3e8f835094f97db7d7a"}', '2020-02-19 13:11:35', NULL, 'N'),
	(345, 'I', 361, NULL, 'passReset', '{"via": "E", "UUID": "76942b76817ef9aa918b4e355ced5cf7", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:11:37', NULL, 'N'),
	(346, 'I', 362, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "9b90110031b6a2aa1cb011a479937467"}', '2020-02-19 13:13:03', NULL, 'N'),
	(347, 'I', 362, NULL, 'passReset', '{"via": "E", "UUID": "b47cd795f85e48887703e197460a98b6", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:13:05', NULL, 'N'),
	(348, 'I', 363, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "75ef20ea879657b4bdcda188b333aa0f"}', '2020-02-19 13:14:16', NULL, 'N'),
	(349, 'I', 363, NULL, 'passReset', '{"via": "E", "UUID": "32958601074a58096fde744a4c873c99", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:14:18', NULL, 'N'),
	(350, 'I', 364, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "911176ce6cabd2eef60664152b10a358"}', '2020-02-19 13:16:10', NULL, 'N'),
	(351, 'I', 364, NULL, 'passReset', '{"via": "E", "UUID": "86b7911398157d59a20c6e01b3d8b3d1", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:16:12', NULL, 'N'),
	(352, 'I', 365, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "1b23ad70b11b10fa17b110731521f90a"}', '2020-02-19 13:19:21', NULL, 'N'),
	(353, 'I', 365, NULL, 'passReset', '{"via": "E", "UUID": "ec9eae3ef67117174acd0f787a8967dd", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:19:22', NULL, 'N'),
	(354, 'I', 365, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "91586"}', '2020-02-19 13:19:27', NULL, 'N'),
	(355, 'I', 366, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ae622883a6ef6557ea192adbbe379ea2"}', '2020-02-19 13:20:22', NULL, 'N'),
	(356, 'I', 366, NULL, 'passReset', '{"via": "E", "UUID": "33ec47f781e17ecff7842aa56e638db0", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:20:24', NULL, 'N'),
	(357, 'I', 366, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "69216"}', '2020-02-19 13:20:26', NULL, 'N'),
	(358, 'I', 367, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "abcb349a42f0f709d079a01cd5adf039"}', '2020-02-19 13:22:46', NULL, 'N'),
	(359, 'I', 367, NULL, 'passReset', '{"via": "E", "UUID": "e33340b1808bd62dff09658e70df89a6", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:22:47', NULL, 'N'),
	(360, 'I', 367, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "96975"}', '2020-02-19 13:22:48', NULL, 'N'),
	(361, 'I', 368, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "b9919efe1ac4cf121d0da4c84c5296be"}', '2020-02-19 13:24:14', NULL, 'N'),
	(362, 'I', 368, NULL, 'passReset', '{"via": "E", "UUID": "9d5a77961945e60ab3c4195a00c0cd55", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:24:16', NULL, 'N'),
	(363, 'I', 368, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "31180"}', '2020-02-19 13:24:17', NULL, 'N'),
	(364, 'I', 369, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "610e37cfa8e31eae2296d32c930b874d"}', '2020-02-19 13:25:25', NULL, 'N'),
	(365, 'I', 369, NULL, 'passReset', '{"via": "E", "UUID": "bfe35199a5970c3960e71002d2823226", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:25:28', NULL, 'N'),
	(366, 'I', 369, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "41628"}', '2020-02-19 13:25:28', NULL, 'N'),
	(367, 'I', 370, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "394843d8ac5f51b02ee864486374b999"}', '2020-02-19 13:25:56', NULL, 'N'),
	(368, 'I', 370, NULL, 'passReset', '{"via": "E", "UUID": "3ff00e70b6836d80fc9e0bfb0e8f2c9b", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:25:59', NULL, 'N'),
	(369, 'I', 370, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "55646"}', '2020-02-19 13:26:00', NULL, 'N'),
	(370, 'I', 371, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f8ff4519fc3dd96ab2e282302adcf599"}', '2020-02-19 13:32:34', NULL, 'N'),
	(371, 'I', 371, NULL, 'passReset', '{"via": "E", "UUID": "4ac6e92d286de529fed1bdd7bc31db0b", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:32:36', NULL, 'N'),
	(372, 'I', 371, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "64667"}', '2020-02-19 13:32:37', NULL, 'N'),
	(373, 'I', 372, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "bae3c6312d60cb2c9416ef534079b9e8"}', '2020-02-19 13:35:07', NULL, 'N'),
	(374, 'I', 372, NULL, 'passReset', '{"via": "E", "UUID": "0cb1690d0fb741598906a738ce50f374", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:35:10', NULL, 'N'),
	(375, 'I', 372, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "68415"}', '2020-02-19 13:35:11', NULL, 'N'),
	(376, 'I', 373, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "24ca6c7c8f751dd5d437b1bf005a2eef"}', '2020-02-19 13:38:02', NULL, 'N'),
	(377, 'I', 373, NULL, 'passReset', '{"via": "E", "UUID": "31f9eb7fbe0d72eda5337714a817ae71", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 13:38:04', NULL, 'N'),
	(378, 'I', 373, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "31118"}', '2020-02-19 13:38:05', NULL, 'N'),
	(379, 'I', 374, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "441de585c31d7200636c4a1baab20e32"}', '2020-02-19 14:14:06', NULL, 'N'),
	(380, 'I', 374, NULL, 'passReset', '{"via": "E", "UUID": "28a9a6ccd11840bb75365353db5716cc", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 14:14:08', NULL, 'N'),
	(381, 'I', 374, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "57575"}', '2020-02-19 14:14:08', NULL, 'N'),
	(382, 'I', 375, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "8642fdf0433d5605584084c96b181205"}', '2020-02-19 14:16:23', NULL, 'N'),
	(383, 'I', 375, NULL, 'passReset', '{"via": "E", "UUID": "59ad293f6103fa9d3a12b44a88b99515", "usrName": "unit", "usrFamily": "test"}', '2020-02-19 14:16:25', NULL, 'N'),
	(384, 'I', 375, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "96978"}', '2020-02-19 14:16:25', NULL, 'N'),
	(385, 'I', 376, 'customer@ctp.ir', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "3ced8b2b1516bd10fb399c5ac37fbb87"}', '2020-06-09 12:09:56', NULL, 'N'),
	(386, 'I', 377, 'sir.ri4n@gmail.com', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2e43e91b86ba57f73b3da4d3ffc5053b"}', '2020-06-10 07:03:01', NULL, 'N'),
	(387, 'I', 377, NULL, 'passReset', '{"via": "E", "UUID": "53860270a2b05f5ef00d96ebc8443ef4", "usrName": "arian", "usrFamily": "atefi"}', '2020-06-10 07:19:30', NULL, 'N'),
	(388, 'I', 377, NULL, 'passReset', '{"via": "E", "UUID": "6c1b29ccdbf15a29851f147a917183bd", "usrName": "arian", "usrFamily": "atefi"}', '2020-06-10 07:30:56', NULL, 'N'),
	(389, 'I', 377, NULL, 'passReset', '{"via": "E", "UUID": "5444ec30382a09d3a64133902f0af4f9", "usrName": "arian", "usrFamily": "atefi"}', '2020-06-10 07:44:42', NULL, 'N'),
	(390, 'I', 377, NULL, 'passReset', '{"via": "E", "UUID": "3bec48e18f697c5993e7b8b64ebfc457", "usrName": "arian", "usrFamily": "atefi"}', '2020-06-10 07:52:05', NULL, 'N'),
	(391, 'I', 377, NULL, 'passReset', '{"via": "E", "UUID": "45929df5515ec18ffed672af6965ac47", "usrName": "arian", "usrFamily": "atefi"}', '2020-06-10 09:13:50', NULL, 'N'),
	(392, 'I', 377, NULL, 'passReset', '{"via": "E", "UUID": "9b24aa9c47fba1311c1b1b7b7a0c5d59", "usrName": "arian", "usrFamily": "atefi"}', '2020-06-10 11:36:46', NULL, 'N'),
	(393, 'I', 379, 'wasddasdasdasd@gmail.com', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "c163dd8ed41a818bebf62d0087386372"}', '2020-06-10 12:07:04', NULL, 'N'),
	(394, 'I', 380, 'casdasdustomer@ctp.ir', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "622612078bd6ee09c06c68397eadeebf"}', '2020-06-10 12:42:01', NULL, 'N'),
	(395, 'I', 381, 'kvlsoj@gmail.com', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "31042051fac18c09ba689f9f3dc82e50"}', '2020-06-13 06:59:15', NULL, 'N'),
	(396, 'I', 383, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2bed70156c5398c16514fcd4e0548a38"}', '2020-07-04 08:48:59', NULL, 'N'),
	(397, 'I', 384, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "75f34d10a83691fbbbbe7907e74d8dee"}', '2020-07-04 08:49:59', NULL, 'N'),
	(398, 'I', 385, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "889f1e8fa78008a12f0b28ab9630f85c"}', '2020-07-04 08:50:31', NULL, 'N'),
	(399, 'I', 386, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2aee819f2bb93e9aeb4437ca1a59acff"}', '2020-07-04 08:52:15', NULL, 'N'),
	(400, 'I', 387, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "ebe2e3b02037a8c876395ef1e108c42d"}', '2020-07-04 08:52:45', NULL, 'N'),
	(401, 'I', 387, NULL, 'passReset', '{"via": "E", "UUID": "597c338fd1fa72dec19bdec45f989bf4", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 08:52:50', NULL, 'N'),
	(402, 'I', 388, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "96b7c8275dc4f9eecd133b1e57a7250e"}', '2020-07-04 08:53:16', NULL, 'N'),
	(403, 'I', 388, NULL, 'passReset', '{"via": "E", "UUID": "74edecff1af2c650b24454e6f6b40354", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 08:53:22', NULL, 'N'),
	(404, 'I', 388, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "88679"}', '2020-07-04 08:53:24', NULL, 'N'),
	(405, 'I', 389, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "0f374dd1033aa75050ad318ddc64a678"}', '2020-07-04 08:53:44', NULL, 'N'),
	(406, 'I', 389, NULL, 'passReset', '{"via": "E", "UUID": "e269055de3c454e945897cf4e00b17d7", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 08:53:49', NULL, 'N'),
	(407, 'I', 389, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "21415"}', '2020-07-04 08:53:50', NULL, 'N'),
	(408, 'I', 390, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "9f1e8666449da364ac6fbc6b3f792f0e"}', '2020-07-04 09:58:28', NULL, 'N'),
	(409, 'I', 390, NULL, 'passReset', '{"via": "E", "UUID": "1629def10652107fe90893ec8f2632e0", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 09:58:30', NULL, 'N'),
	(410, 'I', 390, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "22588"}', '2020-07-04 09:58:31', NULL, 'N'),
	(411, 'I', 391, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "8ec3f05df6a5a138808f3256fe100d5d"}', '2020-07-04 10:01:12', NULL, 'N'),
	(412, 'I', 391, NULL, 'passReset', '{"via": "E", "UUID": "bb161f96d10b2643b94755e3223b4a48", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:01:16', NULL, 'N'),
	(413, 'I', 391, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "78748"}', '2020-07-04 10:01:17', NULL, 'N'),
	(414, 'I', 392, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "a009046b2424e9c5c4763124a436f101"}', '2020-07-04 10:07:37', NULL, 'N'),
	(415, 'I', 392, NULL, 'passReset', '{"via": "E", "UUID": "64583045467002da6d8ede85a40c5139", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:07:39', NULL, 'N'),
	(416, 'I', 392, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "10436"}', '2020-07-04 10:07:40', NULL, 'N'),
	(417, 'I', 393, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "9273a17c8696583280f045faa886e423"}', '2020-07-04 10:10:44', NULL, 'N'),
	(418, 'I', 393, NULL, 'passReset', '{"via": "E", "UUID": "3b7cc7068559e4fc39e121dd9d36cd55", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:10:47', NULL, 'N'),
	(419, 'I', 393, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "62732"}', '2020-07-04 10:10:49', NULL, 'N'),
	(420, 'I', 394, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "6ca7a8aad4d84d88905a882e3f752e63"}', '2020-07-04 10:11:54', NULL, 'N'),
	(421, 'I', 394, NULL, 'passReset', '{"via": "E", "UUID": "7c813d12005302a8c11c18dbe9c3e41b", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:11:57', NULL, 'N'),
	(422, 'I', 394, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "23162"}', '2020-07-04 10:11:58', NULL, 'N'),
	(423, 'I', 395, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "61d24b3b635e8c02ab0ffcbcdbc4a5ac"}', '2020-07-04 10:13:41', NULL, 'N'),
	(424, 'I', 395, NULL, 'passReset', '{"via": "E", "UUID": "400d70fa5b5eba42684b872057939211", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:13:43', NULL, 'N'),
	(425, 'I', 395, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "92544"}', '2020-07-04 10:13:44', NULL, 'N'),
	(426, 'I', 396, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "2a063710aec2541a63ad3999eab57332"}', '2020-07-04 10:15:08', NULL, 'N'),
	(427, 'I', 396, NULL, 'passReset', '{"via": "E", "UUID": "0e78f4613170b51f7dfba6aece54c633", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:15:12', NULL, 'N'),
	(428, 'I', 396, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "92076"}', '2020-07-04 10:15:13', NULL, 'N'),
	(429, 'I', 397, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "46d259af89f9b6382693778143a9bb6b"}', '2020-07-04 10:16:11', NULL, 'N'),
	(430, 'I', 397, NULL, 'passReset', '{"via": "E", "UUID": "bb9946dbae8b8dc2fd5a7f9e288194c2", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:16:17', NULL, 'N'),
	(431, 'I', 397, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "11021"}', '2020-07-04 10:16:18', NULL, 'N'),
	(432, 'I', 398, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "6e6b607480b63146ba78d7a45aafc9a9"}', '2020-07-04 10:20:01', NULL, 'N'),
	(433, 'I', 398, NULL, 'passReset', '{"via": "E", "UUID": "0b854a0ec365da6607337bf49d54ce5b", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:20:03', NULL, 'N'),
	(434, 'I', 398, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "82994"}', '2020-07-04 10:20:04', NULL, 'N'),
	(435, 'I', 399, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "9851532b85ada2f6eaca53979a192983"}', '2020-07-04 10:25:24', NULL, 'N'),
	(436, 'I', 399, NULL, 'passReset', '{"via": "E", "UUID": "1e3c18699b0838e6ea5706f0fe422f15", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:25:27', NULL, 'N'),
	(437, 'I', 399, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "65026"}', '2020-07-04 10:25:29', NULL, 'N'),
	(438, 'I', 400, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "5ff72da873b99d9f8aec201c291697ec"}', '2020-07-04 10:26:56', NULL, 'N'),
	(439, 'I', 400, NULL, 'passReset', '{"via": "E", "UUID": "ee4c457e5790741e08409b503cb73499", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 10:26:58', NULL, 'N'),
	(440, 'I', 400, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "36004"}', '2020-07-04 10:26:59', NULL, 'N'),
	(441, 'I', 401, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "83883d3ca92e664dc887abf2cddeb105"}', '2020-07-04 12:07:16', NULL, 'N'),
	(442, 'I', 401, NULL, 'passReset', '{"via": "E", "UUID": "5a848c6cc4ddcdda7882d79394a63a9c", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 12:07:17', NULL, 'N'),
	(443, 'I', 401, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "26637"}', '2020-07-04 12:07:18', NULL, 'N'),
	(444, 'I', 402, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "bdf0b21626c56d45a27d3735cc90412f"}', '2020-07-04 12:09:41', NULL, 'N'),
	(445, 'I', 402, NULL, 'passReset', '{"via": "E", "UUID": "d3abc9f272755f0ef2e69c159a572202", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 12:09:43', NULL, 'N'),
	(446, 'I', 402, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "53867"}', '2020-07-04 12:09:43', NULL, 'N'),
	(447, 'I', 403, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "1cd18fab83ef357727942f7c8cfe7b90"}', '2020-07-04 12:27:38', NULL, 'N'),
	(448, 'I', 403, NULL, 'passReset', '{"via": "E", "UUID": "94bd97d7d8e32144833e0fae7a2d8431", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 12:27:40', NULL, 'N'),
	(449, 'I', 403, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "20377"}', '2020-07-04 12:27:40', NULL, 'N'),
	(450, 'I', 404, 'unit_test@unittest.test', 'approveEmail', '{"usrName": null, "usrFamily": null, "ApprovalCode": "f7698f77f9b74c0eeb2a51da0adac408"}', '2020-07-04 12:35:27', NULL, 'N'),
	(451, 'I', 404, NULL, 'passReset', '{"via": "E", "UUID": "509e8ca62ce41e8e81ed329db0a0d70e", "usrName": "unit", "usrFamily": "test"}', '2020-07-04 12:35:29', NULL, 'N'),
	(452, 'I', 404, '09121234567', 'approveMobile', '{"usrName": "unit", "usrFamily": "test", "ApprovalCode": "34492"}', '2020-07-04 12:35:30', NULL, 'N');
/*!40000 ALTER TABLE `tblAlerts` ENABLE KEYS */;

-- Dumping structure for table Common.tblAlertTemplates
DROP TABLE IF EXISTS `tblAlertTemplates`;
CREATE TABLE IF NOT EXISTS `tblAlertTemplates` (
  `altlID` int(10) unsigned NOT NULL AUTO_INCREMENT,
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
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- Dumping data for table Common.tblAlertTemplates: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAlertTemplates` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAlertTemplates` ENABLE KEYS */;

-- Dumping structure for table Common.tblDBG
DROP TABLE IF EXISTS `tblDBG`;
CREATE TABLE IF NOT EXISTS `tblDBG` (
  `dbgID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `dbgFrom` varchar(50) DEFAULT NULL,
  `dbgTimestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `dbgInfo` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`dbgID`),
  KEY `dbgTimestamp` (`dbgTimestamp`),
  KEY `dbgFrom` (`dbgFrom`)
) ENGINE=MyISAM AUTO_INCREMENT=39 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Common.tblDBG: 38 rows
/*!40000 ALTER TABLE `tblDBG` DISABLE KEYS */;
INSERT INTO `tblDBG` (`dbgID`, `dbgFrom`, `dbgTimestamp`, `dbgInfo`) VALUES
	(1, 'afterInsertTransaction', '2020-04-12 06:06:54', '{"iWalID": 3, "iWltID": 33, "iWltDateTime": "2020-04-12 10:30:43.000000", "oLastTransactionDate": "2020-04-12 10:30:04.000000"}'),
	(2, 'afterInsertTransaction', '2020-04-12 06:08:36', '{"iWalID": 3, "iWltID": 34, "compare": false, "iWltDateTime": "2020-04-12 10:30:43.000000", "oLastTransactionDate": "2020-04-12 10:30:04.000000"}'),
	(3, 'afterInsertTransaction', '2020-04-12 06:13:10', '{"iWalID": 3, "iWltID": 35, "compare": false, "iWltDateTime": "2020-04-12 10:30:43.000000", "oLastTransactionDate": "2020-04-12 10:30:04.000000"}'),
	(4, 'afterInsertTransaction', '2020-04-12 06:14:04', '{"iWalID": 3, "iWltID": 35, "compare": false, "iWltDateTime": "2020-04-12 10:30:43.000000", "oLastTransactionDate": "2020-04-12 10:30:04.000000"}'),
	(5, 'afterInsertTransaction', '2020-04-12 06:14:36', '{"iWalID": 3, "iWltID": 35, "compare": false, "iWltDateTime": "2020-04-12 10:30:43.000000", "oLastTransactionDate": "2020-04-12 10:30:04.000000"}'),
	(6, 'afterInsertTransaction', '2020-04-12 06:15:41', '{"iWalID": 3, "iWltID": 36, "compare": true, "iWltDateTime": "2020-04-12 06:15:41.000000", "oLastTransactionDate": "2020-04-12 10:30:04.000000"}'),
	(7, 'afterInsertTransaction', '2020-04-12 06:21:09', '{"iWalID": 3, "iWltID": 37, "compare": null, "iWltDateTime": "2020-04-12 10:51:06.000000", "oLastTransactionDate": null}'),
	(8, 'afterInsertTransaction', '2020-04-12 06:22:02', '{"iWalID": 3, "iWltID": 38, "compare": null, "iWltDateTime": "2020-04-12 10:52:00.000000", "oLastTransactionDate": null}'),
	(9, 'afterInsertTransaction', '2020-04-12 06:22:12', '{"iWalID": 3, "iWltID": 38, "compare": null, "iWltDateTime": "2020-04-12 10:52:00.000000", "oLastTransactionDate": null}'),
	(10, 'AAA', '2020-06-09 12:09:56', 'signup'),
	(11, 'AAA', '2020-06-10 07:03:01', 'signup'),
	(12, 'AAA', '2020-06-10 12:06:58', 'signup'),
	(13, 'AAA', '2020-06-10 12:07:04', 'signup'),
	(14, 'AAA', '2020-06-10 12:42:01', 'signup'),
	(15, 'AAA', '2020-06-13 06:59:15', 'signup'),
	(16, 'AAA', '2020-06-13 06:59:21', 'signup'),
	(17, 'AAA', '2020-07-04 08:48:59', 'signup'),
	(18, 'AAA', '2020-07-04 08:49:59', 'signup'),
	(19, 'AAA', '2020-07-04 08:50:31', 'signup'),
	(20, 'AAA', '2020-07-04 08:52:14', 'signup'),
	(21, 'AAA', '2020-07-04 08:52:45', 'signup'),
	(22, 'AAA', '2020-07-04 08:53:16', 'signup'),
	(23, 'AAA', '2020-07-04 08:53:44', 'signup'),
	(24, 'AAA', '2020-07-04 09:58:28', 'signup'),
	(25, 'AAA', '2020-07-04 10:01:12', 'signup'),
	(26, 'AAA', '2020-07-04 10:07:37', 'signup'),
	(27, 'AAA', '2020-07-04 10:10:44', 'signup'),
	(28, 'AAA', '2020-07-04 10:11:54', 'signup'),
	(29, 'AAA', '2020-07-04 10:13:41', 'signup'),
	(30, 'AAA', '2020-07-04 10:15:08', 'signup'),
	(31, 'AAA', '2020-07-04 10:16:11', 'signup'),
	(32, 'AAA', '2020-07-04 10:20:01', 'signup'),
	(33, 'AAA', '2020-07-04 10:25:24', 'signup'),
	(34, 'AAA', '2020-07-04 10:26:56', 'signup'),
	(35, 'AAA', '2020-07-04 12:07:16', 'signup'),
	(36, 'AAA', '2020-07-04 12:09:41', 'signup'),
	(37, 'AAA', '2020-07-04 12:27:38', 'signup'),
	(38, 'AAA', '2020-07-04 12:35:27', 'signup');
/*!40000 ALTER TABLE `tblDBG` ENABLE KEYS */;

-- Dumping structure for table Common.tblDownloadRequests
DROP TABLE IF EXISTS `tblDownloadRequests`;
CREATE TABLE IF NOT EXISTS `tblDownloadRequests` (
  `dwrID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dwr_ssnID` bigint(20) unsigned NOT NULL,
  `dwrUUID` varchar(50) NOT NULL,
  `dwrCreationTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `dwrFilePath` varchar(200) NOT NULL,
  `dwrStatus` char(1) NOT NULL DEFAULT 'A',
  PRIMARY KEY (`dwrID`),
  KEY `dwrUUID` (`dwrUUID`),
  KEY `dwrStatus` (`dwrStatus`),
  KEY `dwrSSID` (`dwr_ssnID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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


-- Dumping database structure for CommonFuncs
DROP DATABASE IF EXISTS `CommonFuncs`;
CREATE DATABASE IF NOT EXISTS `CommonFuncs` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `CommonFuncs`;

-- Dumping structure for function CommonFuncs.guid
DROP FUNCTION IF EXISTS `guid`;
DELIMITER //
CREATE FUNCTION `guid`(
	`iPrefix` VARCHAR(50)
) RETURNS varchar(50) CHARSET utf8mb4
    NO SQL
BEGIN
  RETURN CONCAT_WS('-',iPrefix,MD5(CONCAT(RAND(), UUID())));
END//
DELIMITER ;

-- Dumping structure for function CommonFuncs.randomStr
DROP FUNCTION IF EXISTS `randomStr`;
DELIMITER //
CREATE FUNCTION `randomStr`(
	`iPrefix` VARCHAR(10)
) RETURNS varchar(50) CHARSET utf8mb4
    NO SQL
BEGIN
  RETURN  CONCAT_WS('', 
	           iPrefix, 
    				  SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1)
         );
         
END//
DELIMITER ;


-- Dumping database structure for I18N
DROP DATABASE IF EXISTS `I18N`;
CREATE DATABASE IF NOT EXISTS `I18N` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `I18N`;

-- Dumping structure for function I18N.farsiDigit
DROP FUNCTION IF EXISTS `farsiDigit`;
DELIMITER //
CREATE FUNCTION `farsiDigit`(
	`iChar` CHAR(1)
) RETURNS varchar(1000) CHARSET utf8
    NO SQL
    DETERMINISTIC
BEGIN
  CASE iChar
    WHEN '1' THEN RETURN '۱';
    WHEN '2' THEN RETURN '۲';
    WHEN '3' THEN RETURN '۳';
    WHEN '4' THEN RETURN '۴';
    WHEN '5' THEN RETURN '۵';
    WHEN '6' THEN RETURN '۶';
    WHEN '7' THEN RETURN '۷';
    WHEN '8' THEN RETURN '۸';
    WHEN '9' THEN RETURN '۹';
    WHEN '0' THEN RETURN '۰';
    ELSE   RETURN iChar;
  END CASE;
END//
DELIMITER ;

-- Dumping structure for function I18N.farsiMonthName
DROP FUNCTION IF EXISTS `farsiMonthName`;
DELIMITER //
CREATE FUNCTION `farsiMonthName`(
	`iMonth` TINYINT
) RETURNS varchar(100) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.1

	CASE iMonth
		WHEN 1  THEN RETURN 'فروردین';
		WHEN 2  THEN RETURN 'اردیبهشت';
		WHEN 3  THEN RETURN 'خرداد';
		WHEN 4  THEN RETURN 'تیر';
		WHEN 5  THEN RETURN 'مرداد';
		WHEN 6  THEN RETURN 'شهریور';
		WHEN 7  THEN RETURN 'مهر';
		WHEN 8  THEN RETURN 'آبان';
		WHEN 9  THEN RETURN 'آذر';
		WHEN 10 THEN RETURN	'دی';
		WHEN 11 THEN RETURN	'بهمن';
		WHEN 12 THEN RETURN	'اسفند';
    ELSE RETURN CONCAT_WS(':','ERROR',iMonth);
	END CASE;

END//
DELIMITER ;

-- Dumping structure for function I18N.farsiNumerals
DROP FUNCTION IF EXISTS `farsiNumerals`;
DELIMITER //
CREATE FUNCTION `farsiNumerals`(
	`iText` VARCHAR(5000)
) RETURNS varchar(5000) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
  DECLARE PersianText VARCHAR(1000) DEFAULT '';

  AllChars: LOOP
    IF LENGTH(iText) = 0 THEN  LEAVE AllChars; END IF;
  
    SET PersianText = CONCAT(PersianText, I18N.farsiDigit(SUBSTR(iText,1,1)));
    SET iText = SUBSTR(iText,2);
    
  END LOOP;
  RETURN PersianText;

END//
DELIMITER ;

-- Dumping structure for function I18N.fromJalali
DROP FUNCTION IF EXISTS `fromJalali`;
DELIMITER //
CREATE FUNCTION `fromJalali`(
	`iJalaliYear` smallint,
	`iJalaliMonth` TINYINT,
	`iJalaliDay` TINYINT




) RETURNS date
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012 Mehran . M . Spitman
# WebLog :spitman.azdaa.com
# Version V1.0.1

	DECLARE
		i, j, e, k, mo,
		gy, gm, gd,
		g_day_no, j_day_no, bkab, jmm, mday, g_day_mo, bkab1, j1
	INT DEFAULT 0; /* Can be unsigned int? */
	DECLARE resout char(100);
	DECLARE fdate datetime;

	
  SET bkab = MOD(iJalaliYear,33);

  IF (bkab = 1 or bkab= 5 or bkab = 9 or bkab = 13 or bkab = 17 or bkab = 22 or bkab = 26 or bkab = 30) THEN
    SET j=1;
  end IF;

  SET bkab1 = MOD(iJalaliYear+1,33);

  IF (bkab1 = 1 or bkab1= 5 or bkab1 = 9 or bkab1 = 13 or bkab1 = 17 or bkab1 = 22 or bkab1 = 26 or bkab1 = 30) THEN
    SET j1=1;
  end IF;

	CASE iJalaliMonth
		WHEN 1 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 2 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 3 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 4 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 5 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 6 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 7 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 8 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 9 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 10 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 11 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
		WHEN 12 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth)+j or iJalaliDay <= 0 THEN SET e=1; end IF;
	END CASE;
  IF iJalaliMonth > 12 or iJalaliMonth <= 0 THEN SET e=1; end IF;
  IF iJalaliYear <= 0 THEN SET e=1; end IF;

  IF e>0 THEN
    RETURN 0;
  end IF;

  IF (iJalaliMonth>=11) or (iJalaliMonth=10 and iJalaliDay>=11 and j=0) or (iJalaliMonth=10 and iJalaliDay>11 and j=1) THEN
    SET i=1;
  end IF;
  SET gy = iJalaliYear + 621 + i;

  IF (MOD(gy,4)=0) THEN
    SET k=1;
  end IF;
	
	IF (MOD(gy,100)=0) and (MOD(gy,400)<>0) THEN
		SET k=0;
	END IF;

  SET jmm=iJalaliMonth-1;

  WHILE (jmm > 0) do
    SET mday=mday+I18N._jdmarray2(jmm);
    SET jmm=jmm-1;
  end WHILE;

  SET j_day_no=(iJalaliYear-1)*365+(I18N._intDiv(iJalaliYear,4))+mday+iJalaliDay;
  SET g_day_no=j_day_no+226899;


  SET g_day_no=g_day_no-(I18N._intDiv(gy-1,4));
  SET g_day_mo=MOD(g_day_no,365);

	IF (k=1 and j=1) THEN
		IF (g_day_mo=0) THEN
			RETURN CONCAT_WS('-',gy,'12','30');
		END IF;
		IF (g_day_mo=1) THEN
			RETURN CONCAT_WS('-',gy,'12','31');
		END IF;
	END IF;

	IF (g_day_mo=0) THEN
		RETURN CONCAT_WS('-',gy,'12','31');
	END IF;
			

  SET mo=0;
  SET gm=gm+1;
  while g_day_mo>I18N._gdmarray2(mo,k) do
		SET g_day_mo=g_day_mo-I18N._gdmarray2(mo,k);
    SET mo=mo+1;
    SET gm=gm+1;
  end WHILE;
  SET gd=g_day_mo;

  RETURN CONCAT_WS('-',gy,gm,gd);
END//
DELIMITER ;

-- Dumping structure for function I18N.fromJalaliStr
DROP FUNCTION IF EXISTS `fromJalaliStr`;
DELIMITER //
CREATE FUNCTION `fromJalaliStr`(
	`iJalaliDate` VARCHAR(50)





) RETURNS datetime
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012 Mehran . M . Spitman
# WebLog spitman.azdaa.com
# Version V1.0.1

	DECLARE jdd, jyd, jmd, jt, jd, jm, jy, jTime varchar(100);
  
  IF LOCATE(' ', iJalaliDate) THEN
    SET jTime = SUBSTRING_INDEX(iJalaliDate, ' ', -1);
    SET iJalaliDate = SUBSTRING_INDEX(iJalaliDate, ' ', 1);
  END IF;
  SET iJalaliDate = REPLACE (iJalaliDate, '/', '-');

	SET jdd = SUBSTRING_INDEX(iJalaliDate, '-', -1);
	SET jt  = SUBSTRING_INDEX(iJalaliDate, '-', 2);
	SET jyd = SUBSTRING_INDEX(jt, '-', 1);
	SET jmd = SUBSTRING_INDEX(jt, '-', -1);
	SET jd  = CAST(jdd AS UNSIGNED);
	SET jy  = CAST(jyd AS UNSIGNED);
	SET jm  = CAST(jmd AS UNSIGNED);
  
  RETURN CONCAT_WS(' ',I18N.fromJalali(jy,jm,jd), jTime);
  
END//
DELIMITER ;

-- Dumping structure for function I18N.jalaliDateTime
DROP FUNCTION IF EXISTS `jalaliDateTime`;
DELIMITER //
CREATE FUNCTION `jalaliDateTime`(
	`iDateTime` DATETIME

) RETURNS varchar(50) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

	DECLARE 
		i,
		gy, gm, gd,
		g_day_no, j_day_no, j_np,
		jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */
	DECLARE resout char(100);
	DECLARE ttime CHAR(20);

	SET gy = YEAR(iDateTime) - 1600;
	SET gm = MONTH(iDateTime) - 1;
	SET gd = DAY(iDateTime) - 1;
	SET ttime = TIME(iDateTime);
	SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - I18N._intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
	SET i = 0;

	WHILE (i < gm) do
		SET g_day_no = g_day_no + I18N._gdmarray(i);
		SET i = i + 1; 
	END WHILE;

	IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN 
		SET g_day_no =	g_day_no + 1;
	END IF;
	
	SET g_day_no = g_day_no + gd; 
	SET j_day_no = g_day_no - 79;
	SET j_np = j_day_no DIV 12053;
	SET j_day_no = j_day_no % 12053;
	SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
	SET j_day_no = j_day_no % 1461;

	IF j_day_no >= 366 then 
		SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
		SET j_day_no = (j_day_no - 1) % 365;
	END IF;

	SET i = 0;

	WHILE (i < 11 and j_day_no >= I18N._jdmarray(i)) do
		SET j_day_no = j_day_no - I18N._jdmarray(i);
		SET i = i + 1;
	END WHILE;

	SET jm = i + 1;
	SET jd = j_day_no + 1;
	SET resout = CONCAT_WS ('-', jy, CONCAT(IF(jm<10,'0',''), jm), CONCAT(IF(jd<10,'0',''), jd));

	IF (ttime <> '00:00:00') then
		SET resout = CONCAT_WS(' ', resout, ttime);
	END IF;

	RETURN resout;
END//
DELIMITER ;

-- Dumping structure for function I18N.jalaliDay
DROP FUNCTION IF EXISTS `jalaliDay`;
DELIMITER //
CREATE FUNCTION `jalaliDay`(
	`iDate` DATE


) RETURNS tinyint(4)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

	DECLARE 
		i,
		gy, gm, gd,
		g_day_no, j_day_no, j_np,
		jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */

	SET gy = YEAR(iDate) - 1600;
	SET gm = MONTH(iDate) - 1;
	SET gd = DAY(iDate) - 1;
	SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - _intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
	SET i = 0;

	WHILE (i < gm) do
		SET g_day_no = g_day_no + I18N._gdmarray(i);
		SET i = i + 1; 
	END WHILE;

	IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN 
		SET g_day_no =	g_day_no + 1;
	END IF;
	
	SET g_day_no = g_day_no + gd; 
	SET j_day_no = g_day_no - 79;
	SET j_np = j_day_no DIV 12053;
	SET j_day_no = j_day_no % 12053;
	SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
	SET j_day_no = j_day_no % 1461;

	IF j_day_no >= 366 then 
		SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
		SET j_day_no = (j_day_no - 1) % 365;
	END IF;

	SET i = 0;

	WHILE (i < 11 and j_day_no >= I18N._jdmarray(i)) do
		SET j_day_no = j_day_no - I18N._jdmarray(i);
		SET i = i + 1;
	END WHILE;

	SET jm = i + 1;
	SET jd = j_day_no + 1;

	RETURN jd;

END//
DELIMITER ;

-- Dumping structure for function I18N.jalaliMonth
DROP FUNCTION IF EXISTS `jalaliMonth`;
DELIMITER //
CREATE FUNCTION `jalaliMonth`(
	`iDate` DATE



) RETURNS tinyint(4)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

	DECLARE 
		i,
		gy, gm, gd,
		g_day_no, j_day_no, j_np,
		jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */

	SET gy = YEAR(iDate) - 1600;
	SET gm = MONTH(iDate) - 1;
	SET gd = DAY(iDate) - 1;
	SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - I18N._intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
	SET i = 0;

	WHILE (i < gm) do
		SET g_day_no = g_day_no + I18N._gdmarray(i);
		SET i = i + 1; 
	END WHILE;

	IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN 
		SET g_day_no =	g_day_no + 1;
	END IF;
	
	SET g_day_no = g_day_no + gd; 
	SET j_day_no = g_day_no - 79;
	SET j_np = j_day_no DIV 12053;
	SET j_day_no = j_day_no % 12053;
	SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
	SET j_day_no = j_day_no % 1461;

	IF j_day_no >= 366 then 
		SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
		SET j_day_no = (j_day_no - 1) % 365;
	END IF;

	SET i = 0;

	WHILE (i < 11 and j_day_no >= I18N._jdmarray(i)) do
		SET j_day_no = j_day_no - I18N._jdmarray(i);
		SET i = i + 1;
	END WHILE;

	SET jm = i + 1;
--	SET jd = j_day_no + 1;

	RETURN jm;
END//
DELIMITER ;

-- Dumping structure for function I18N.jalaliYear
DROP FUNCTION IF EXISTS `jalaliYear`;
DELIMITER //
CREATE FUNCTION `jalaliYear`(
	`iDate` DATE

) RETURNS smallint(5) unsigned
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

	DECLARE 
		i,
		gy, gm, gd,
		g_day_no, j_day_no, j_np,
		jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */

	SET gy = YEAR(iDate) - 1600;
	SET gm = MONTH(iDate) - 1;
	SET gd = DAY(iDate) - 1;
	SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - I18N._intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
	SET i = 0;

	WHILE (i < gm) do
		SET g_day_no = g_day_no + I18N._gdmarray(i);
		SET i = i + 1; 
	END WHILE;

	IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN 
		SET g_day_no =	g_day_no + 1;
	END IF;
	
	SET g_day_no = g_day_no + gd; 
	SET j_day_no = g_day_no - 79;
	SET j_np = j_day_no DIV 12053;
	SET j_day_no = j_day_no % 12053;
	SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
	SET j_day_no = j_day_no % 1461;

	IF j_day_no >= 366 then 
		SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
	END IF;

	RETURN jy;
END//
DELIMITER ;

-- Dumping structure for function I18N._gdmarray
DROP FUNCTION IF EXISTS `_gdmarray`;
DELIMITER //
CREATE FUNCTION `_gdmarray`(
	`m` smallint
) RETURNS smallint(2)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.1

	CASE m
		WHEN 0  THEN RETURN 31;
		WHEN 1  THEN RETURN 28;
		WHEN 2  THEN RETURN 31;
		WHEN 3  THEN RETURN 30;
		WHEN 4  THEN RETURN 31;
		WHEN 5  THEN RETURN 30;
		WHEN 6  THEN RETURN 31;
		WHEN 7  THEN RETURN 31;
		WHEN 8  THEN RETURN 30;
		WHEN 9  THEN RETURN 31;
		WHEN 10 THEN RETURN 30;
		WHEN 11 THEN RETURN 31;
	END CASE;

END//
DELIMITER ;

-- Dumping structure for function I18N._gdmarray2
DROP FUNCTION IF EXISTS `_gdmarray2`;
DELIMITER //
CREATE FUNCTION `_gdmarray2`(
	`m` smallint,
	`k` SMALLINT
) RETURNS tinyint(4)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012  Mehran . M . Spitman
# WebLog :spitman.azdaa.com
# Version V1.0

	CASE m
		WHEN 0  THEN RETURN 31;
		WHEN 1  THEN RETURN 28+k;
		WHEN 2  THEN RETURN 31;
		WHEN 3  THEN RETURN 30;
		WHEN 4  THEN RETURN 31;
		WHEN 5  THEN RETURN 30;
		WHEN 6  THEN RETURN 31;
		WHEN 7  THEN RETURN 31;
		WHEN 8  THEN RETURN 30;
		WHEN 9  THEN RETURN 31;
		WHEN 10 THEN RETURN 30;
		WHEN 11 THEN RETURN 31;
	END CASE;
   

END//
DELIMITER ;

-- Dumping structure for function I18N._intDiv
DROP FUNCTION IF EXISTS `_intDiv`;
DELIMITER //
CREATE FUNCTION `_intDiv`(
	`a` int,
	`b` int
) RETURNS bigint(20)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

	return FLOOR(a / b);
END//
DELIMITER ;

-- Dumping structure for function I18N._jdmarray
DROP FUNCTION IF EXISTS `_jdmarray`;
DELIMITER //
CREATE FUNCTION `_jdmarray`(
	`m` smallint
) RETURNS smallint(2)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.1

	CASE m
		WHEN 0  THEN RETURN 31;
		WHEN 1  THEN RETURN 31;
		WHEN 2  THEN RETURN 31;
		WHEN 3  THEN RETURN 31;
		WHEN 4  THEN RETURN 31;
		WHEN 5  THEN RETURN 31;
		WHEN 6  THEN RETURN 30;
		WHEN 7  THEN RETURN 30;
		WHEN 8  THEN RETURN 30;
		WHEN 9  THEN RETURN 30;
		WHEN 10 THEN RETURN 30;
		WHEN 11 THEN RETURN 29;
	END CASE;

END//
DELIMITER ;

-- Dumping structure for function I18N._jdmarray2
DROP FUNCTION IF EXISTS `_jdmarray2`;
DELIMITER //
CREATE FUNCTION `_jdmarray2`(`m` smallint) RETURNS smallint(2)
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012 Mehran . M . Spitman
# WebLog :spitman.azdaa.com
# Version V1.0.1

	CASE m
		WHEN 1 THEN RETURN 31;
		WHEN 2 THEN RETURN 31;
		WHEN 3 THEN RETURN 31;
		WHEN 4 THEN RETURN 31;
		WHEN 5 THEN RETURN 31;
		WHEN 6 THEN RETURN 31;
		WHEN 7 THEN RETURN 30;
		WHEN 8 THEN RETURN 30;
		WHEN 9 THEN RETURN 30;
		WHEN 10 THEN RETURN 30;
		WHEN 11 THEN RETURN 30;
		WHEN 12 THEN RETURN 29;
	END CASE;

END//
DELIMITER ;


-- Dumping database structure for MT
DROP DATABASE IF EXISTS `MT`;
CREATE DATABASE IF NOT EXISTS `MT` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `MT`;

-- Dumping structure for table MT.tblActionLog
DROP TABLE IF EXISTS `tblActionLog`;
CREATE TABLE IF NOT EXISTS `tblActionLog` (
  `atlID` int(11) NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` int(10) unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table MT.tblActionLog: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActionLog` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActionLog` ENABLE KEYS */;

-- Dumping structure for table MT.tblCorrectionRules
DROP TABLE IF EXISTS `tblCorrectionRules`;
CREATE TABLE IF NOT EXISTS `tblCorrectionRules` (
  `crlID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `crlType` char(1) NOT NULL DEFAULT 'R' COMMENT 'R: Replace, T: Translation',
  `crlPattern` varchar(250) NOT NULL,
  `crlReplacement` varchar(500) NOT NULL,
  PRIMARY KEY (`crlID`),
  UNIQUE KEY `crlPattern` (`crlPattern`),
  KEY `crlType` (`crlType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table MT.tblCorrectionRules: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblCorrectionRules` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblCorrectionRules` ENABLE KEYS */;

-- Dumping structure for table MT.tblDigestedTranslationLogs
DROP TABLE IF EXISTS `tblDigestedTranslationLogs`;
CREATE TABLE IF NOT EXISTS `tblDigestedTranslationLogs` (
  `dtlDateTime` datetime NOT NULL,
  `dtl_aptID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `dtl_usrID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `dtlEngine` char(3) NOT NULL,
  `dtlDir` char(5) NOT NULL,
  `dtlTotalWordsRequested` bigint(20) unsigned NOT NULL,
  `dtlTotalTranslatedWords` bigint(20) unsigned NOT NULL,
  `dtlAverageTranslationTime` double NOT NULL,
  `dtlMaxSpeed` double NOT NULL,
  `dtlAverageSourceWordCount` double NOT NULL,
  `dtlMaxSourceWordCount` int(11) NOT NULL,
  `dtlMinSourceWordCount` int(11) NOT NULL,
  `dtlOkCount` bigint(20) unsigned NOT NULL,
  `dtlErrLongCount` bigint(20) unsigned NOT NULL,
  `dtlErrNoResourceCount` bigint(20) unsigned NOT NULL,
  `dtlErrOtherCount` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`dtlDateTime`,`dtl_aptID`,`dtlEngine`,`dtlDir`,`dtl_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci ROW_FORMAT=COMPACT;

-- Dumping data for table MT.tblDigestedTranslationLogs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblDigestedTranslationLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblDigestedTranslationLogs` ENABLE KEYS */;

-- Dumping structure for table MT.tblMultiDic
DROP TABLE IF EXISTS `tblMultiDic`;
CREATE TABLE IF NOT EXISTS `tblMultiDic` (
  `dicID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `dicSourceLang` char(2) NOT NULL,
  `dicWord` varchar(100) NOT NULL,
  `dicTranslation` json DEFAULT NULL,
  `dicInsertedBy_usrID` int(11) NOT NULL,
  `dicInsertionDate` datetime DEFAULT CURRENT_TIMESTAMP,
  `dicUpdatedBy_urID` int(11) DEFAULT NULL,
  `dicStatus` char(50) DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`dicID`),
  UNIQUE KEY `dicWord` (`dicWord`),
  KEY `dicLang` (`dicSourceLang`),
  KEY `dicInsertedBy_usrID` (`dicInsertedBy_usrID`),
  KEY `dicInsertionDate` (`dicInsertionDate`),
  KEY `dicUpdatedBy_urID` (`dicUpdatedBy_urID`),
  KEY `dicStatus` (`dicStatus`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table MT.tblMultiDic: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblMultiDic` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblMultiDic` ENABLE KEYS */;

-- Dumping structure for table MT.tblTokenStats
DROP TABLE IF EXISTS `tblTokenStats`;
CREATE TABLE IF NOT EXISTS `tblTokenStats` (
  `tks_tokID` bigint(20) unsigned NOT NULL,
  `tksEngine` varchar(50) NOT NULL,
  `tksDir` varchar(50) NOT NULL,
  `tksTodayWords` int(10) unsigned NOT NULL DEFAULT '0',
  `tksThisMonthWords` int(10) unsigned NOT NULL DEFAULT '0',
  `tksTotalWords` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`tks_tokID`,`tksEngine`,`tksDir`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table MT.tblTokenStats: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblTokenStats` DISABLE KEYS */;
INSERT INTO `tblTokenStats` (`tks_tokID`, `tksEngine`, `tksDir`, `tksTodayWords`, `tksThisMonthWords`, `tksTotalWords`) VALUES
	(0, 'NMT', 'en2fa', 0, 0, 0);
/*!40000 ALTER TABLE `tblTokenStats` ENABLE KEYS */;

-- Dumping structure for table MT.tblTranslatedPhrases
DROP TABLE IF EXISTS `tblTranslatedPhrases`;
CREATE TABLE IF NOT EXISTS `tblTranslatedPhrases` (
  `tps_tlsID` bigint(20) unsigned NOT NULL,
  `tpsSourceString` longtext NOT NULL,
  `tpsTargetString` longtext,
  PRIMARY KEY (`tps_tlsID`),
  CONSTRAINT `FK__tbl_SMT_TranslationLogs` FOREIGN KEY (`tps_tlsID`) REFERENCES `tblTranslationLogs` (`tlsid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table MT.tblTranslatedPhrases: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblTranslatedPhrases` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblTranslatedPhrases` ENABLE KEYS */;

-- Dumping structure for table MT.tblTranslationLogs
DROP TABLE IF EXISTS `tblTranslationLogs`;
CREATE TABLE IF NOT EXISTS `tblTranslationLogs` (
  `tlsID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `tls_usrID` int(11) unsigned NOT NULL,
  `tls_aptID` int(11) unsigned DEFAULT NULL,
  `tlsRequestTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tlsDir` char(5) NOT NULL COMMENT 'Translation like en2fa or fa2en',
  `tlsEngine` char(3) NOT NULL COMMENT 'SMT, NMT, ENT, DIC',
  `tlsClass` varchar(10) NOT NULL,
  `tlsServer` bigint(20) DEFAULT NULL,
  `tlsSourceWordsCount` int(10) NOT NULL,
  `tlsTranslationTime` float NOT NULL,
  `tlsErrNo` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`tlsID`),
  KEY `tlsTime` (`tlsRequestTime`),
  KEY `tls_tfsID` (`tlsDir`),
  KEY `tlsErrNo` (`tlsErrNo`),
  KEY `tlsTranslationTime` (`tlsTranslationTime`),
  KEY `tlsEngine` (`tlsEngine`),
  KEY `tls_aptID` (`tls_aptID`),
  KEY `tlsServer` (`tlsServer`),
  KEY `tlsClass` (`tlsClass`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table MT.tblTranslationLogs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblTranslationLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblTranslationLogs` ENABLE KEYS */;

-- Dumping structure for trigger MT.tblMultiDic_after_update
DROP TRIGGER IF EXISTS `tblMultiDic_after_update`;
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


-- Dumping database structure for TargomanBlog
DROP DATABASE IF EXISTS `TargomanBlog`;
CREATE DATABASE IF NOT EXISTS `TargomanBlog` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `TargomanBlog`;

-- Dumping structure for table TargomanBlog.tblCategories
DROP TABLE IF EXISTS `tblCategories`;
CREATE TABLE IF NOT EXISTS `tblCategories` (
  `catID` mediumint(9) unsigned NOT NULL AUTO_INCREMENT,
  `catName` varchar(50) NOT NULL,
  `catPersianName` varchar(50) NOT NULL,
  PRIMARY KEY (`catID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblCategories: ~2 rows (approximately)
/*!40000 ALTER TABLE `tblCategories` DISABLE KEYS */;
INSERT INTO `tblCategories` (`catID`, `catName`, `catPersianName`) VALUES
	(1, 'Offline Requests', 'تقاضاهای ترجمه'),
	(3, 'News', 'اخبار');
/*!40000 ALTER TABLE `tblCategories` ENABLE KEYS */;

-- Dumping structure for table TargomanBlog.tblComments
DROP TABLE IF EXISTS `tblComments`;
CREATE TABLE IF NOT EXISTS `tblComments` (
  `cmtID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `cmt_bpsID` bigint(20) unsigned NOT NULL,
  `cmtInReplyTo_cmtID` bigint(20) unsigned DEFAULT NULL,
  `cmtText` varchar(5000) NOT NULL,
  `cmtWrittenOn` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cmtWrittenBy` int(10) unsigned NOT NULL,
  `cmtWrittenByFullName` varchar(100) NOT NULL,
  `cmtModeratedBy_usrID` int(10) unsigned DEFAULT NULL,
  `cmtPublishedOn` timestamp NULL DEFAULT NULL,
  `cmtHasReply` bit(1) NOT NULL DEFAULT b'0',
  `cmtCanReply` bit(1) NOT NULL DEFAULT b'1',
  `cmtStatus` char(1) DEFAULT 'N' COMMENT 'N: New, P: Published, D: Discarded, M: Modified, R: Removed',
  PRIMARY KEY (`cmtID`),
  KEY `FK_tblComments_tblPosts` (`cmt_bpsID`),
  KEY `cmtWrittenOn` (`cmtWrittenOn`),
  KEY `cmtStatus` (`cmtStatus`),
  KEY `cmtPublishedOn` (`cmtPublishedOn`),
  KEY `cmtModeratedBy_usrID` (`cmtModeratedBy_usrID`),
  KEY `FK_tblComments_tblComments` (`cmtInReplyTo_cmtID`),
  KEY `cmtWrittenBy` (`cmtWrittenBy`),
  KEY `cmtHasReply` (`cmtHasReply`),
  CONSTRAINT `FK_tblComments_tblComments` FOREIGN KEY (`cmtInReplyTo_cmtID`) REFERENCES `tblComments` (`cmtid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblComments_tblPosts` FOREIGN KEY (`cmt_bpsID`) REFERENCES `tblPosts` (`bpsid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblComments: ~21 rows (approximately)
/*!40000 ALTER TABLE `tblComments` DISABLE KEYS */;
INSERT INTO `tblComments` (`cmtID`, `cmt_bpsID`, `cmtInReplyTo_cmtID`, `cmtText`, `cmtWrittenOn`, `cmtWrittenBy`, `cmtWrittenByFullName`, `cmtModeratedBy_usrID`, `cmtPublishedOn`, `cmtHasReply`, `cmtCanReply`, `cmtStatus`) VALUES
	(1, 2, NULL, 'آخه این چه مزخرفیه که نوشتی؟', '2017-07-10 12:46:07', 19, 'مهران مهرانی‌فر', NULL, NULL, b'1', b'1', 'P'),
	(3, 2, NULL, 'این هم شد نوشته/', '2017-07-10 12:46:22', 19, 'رضا داوطلب', NULL, NULL, b'1', b'1', 'P'),
	(5, 2, 1, 'خیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که باباخیلی خوبه که بابا', '2017-07-10 12:46:29', 19, 'کاربر مهمان', NULL, NULL, b'1', b'1', 'P'),
	(7, 2, NULL, 'ssdfs sdf sdsf sd', '2017-07-10 13:12:22', 19, 'کاربر مهمان', NULL, NULL, b'0', b'1', 'P'),
	(9, 2, NULL, 'مبیسن تبن سمنبتینمبتیمنبتمنت سمنبیسنمبمیسنبتمیسن تسینمتبیسنم ت', '2017-07-11 13:39:52', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(11, 2, 9, 'gfdg dg dgdf gdfgd dfg', '2017-07-11 14:02:30', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'1', b'1', 'N'),
	(13, 9, NULL, 'dslfkjdfl ewrrewlrjewrjkl we rlewjreklwrjlk jewl elfkjlfkjdsklfjdl slfslfkjdlfkjdlfjdlfj ldsklfjdsklfjdsjklfldkfjdslkfjdslkfjkl', '2017-07-11 14:03:42', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(15, 11, NULL, 'fds f rer', '2017-07-11 14:06:16', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(17, 2, 11, 'wrewr werewr rewrw re', '2017-07-11 14:08:04', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(19, 6299, NULL, 'dgdf gdf dgf', '2017-07-12 12:28:28', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'1', b'1', 'N'),
	(21, 6299, 19, 'fgfdgdf dgf dfgf fg dfg', '2017-07-12 16:51:01', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(23, 6299, NULL, 'fffd dg dgfd gfdg gfdg eet rtertert', '2017-07-12 16:51:15', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(25, 6299, NULL, 'rtretret reter ertertertretretertr ert', '2017-07-12 16:51:36', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(27, 6299, NULL, 'xfdd rgr retr e ert rter ert', '2017-07-12 17:07:56', 19, 'Seyyed Mohammad mohammadzadeh', NULL, NULL, b'0', b'1', 'N'),
	(29, 2, NULL, 'fgdg gdf dfgfdgdfgdfgfg', '2017-07-12 19:12:17', 15, 'ssfsdf sdfdsf', NULL, NULL, b'0', b'1', 'N'),
	(31, 2, NULL, 'geg gertrt', '2017-07-12 19:12:49', 15, 'ssfsdf sdfdsf', NULL, NULL, b'0', b'1', 'N'),
	(33, 2, NULL, 'dfg gfg dfgfd gf', '2017-07-12 19:14:25', 15, 'ssfsdf sdfdsf', NULL, NULL, b'0', b'1', 'N'),
	(35, 2, NULL, 'dgf dfgfdgfd dfg f', '2017-07-12 19:16:24', 15, 'ssfsdf sdfdsf', NULL, NULL, b'0', b'1', 'N'),
	(37, 2, 3, 'sdsf sfdsfsfdsf', '2017-09-02 14:25:34', 15, 'بهرام گورآبادی فرد', NULL, NULL, b'0', b'1', 'N'),
	(39, 2, 5, 'trhtrhrthtr htryhtrytrytrytry\ntytrytryt', '2017-10-29 12:17:29', 13, 'ba', NULL, NULL, b'1', b'1', 'N'),
	(41, 2, 39, 'wetrw vwt vwertr w', '2017-10-29 12:50:45', 13, 'kjsdfgjk hkj dsfs', NULL, NULL, b'0', b'1', 'N');
/*!40000 ALTER TABLE `tblComments` ENABLE KEYS */;

-- Dumping structure for table TargomanBlog.tblCommentsLikes
DROP TABLE IF EXISTS `tblCommentsLikes`;
CREATE TABLE IF NOT EXISTS `tblCommentsLikes` (
  `cml_cmtID` bigint(20) unsigned DEFAULT NULL,
  `cml_usrID` int(10) unsigned DEFAULT NULL,
  `cmlTimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `cmlOpinion` tinyint(4) NOT NULL COMMENT '1: Like -1:Dislike',
  UNIQUE KEY `cml_cmtID_cml_usrID` (`cml_cmtID`,`cml_usrID`),
  KEY `cmlType` (`cmlOpinion`),
  KEY `cmlTimeStamp` (`cmlTimeStamp`),
  CONSTRAINT `FK__tblComments` FOREIGN KEY (`cml_cmtID`) REFERENCES `tblComments` (`cmtid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblCommentsLikes: ~12 rows (approximately)
/*!40000 ALTER TABLE `tblCommentsLikes` DISABLE KEYS */;
INSERT INTO `tblCommentsLikes` (`cml_cmtID`, `cml_usrID`, `cmlTimeStamp`, `cmlOpinion`) VALUES
	(1, 19, '2017-07-10 18:52:36', -1),
	(7, 19, '2017-07-11 12:06:26', -1),
	(3, 19, '2017-07-11 12:06:28', 1),
	(19, 19, '2017-07-12 12:28:47', -1),
	(27, 19, '2017-07-12 17:09:55', -1),
	(25, 19, '2017-07-12 17:09:59', -1),
	(35, 15, '2017-09-02 14:25:15', 1),
	(33, 15, '2017-09-02 14:25:16', -1),
	(29, 15, '2017-07-12 19:17:36', 1),
	(7, 15, '2017-07-12 19:17:40', -1),
	(5, 15, '2017-09-02 14:25:21', 1),
	(37, 15, '2017-09-02 14:26:20', -1);
/*!40000 ALTER TABLE `tblCommentsLikes` ENABLE KEYS */;

-- Dumping structure for table TargomanBlog.tblDigestedBlogPosts
DROP TABLE IF EXISTS `tblDigestedBlogPosts`;
CREATE TABLE IF NOT EXISTS `tblDigestedBlogPosts` (
  `dbpID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `dbpFor_usrID` bigint(20) unsigned DEFAULT NULL,
  `dbpDate` date NOT NULL,
  `dbpJalaliMonth` tinyint(4) NOT NULL,
  `dbpJalaliYear` tinyint(4) NOT NULL,
  PRIMARY KEY (`dbpID`),
  KEY `dbpFor_usrID` (`dbpFor_usrID`),
  KEY `dbpDate` (`dbpDate`),
  KEY `dbpJalaliMonth` (`dbpJalaliMonth`),
  KEY `dbpJalaliYear` (`dbpJalaliYear`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblDigestedBlogPosts: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblDigestedBlogPosts` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblDigestedBlogPosts` ENABLE KEYS */;

-- Dumping structure for table TargomanBlog.tblPostRead
DROP TABLE IF EXISTS `tblPostRead`;
CREATE TABLE IF NOT EXISTS `tblPostRead` (
  `bpr_bpsID` bigint(20) unsigned NOT NULL,
  `bprReadBy_usrID` int(10) unsigned NOT NULL,
  `bprDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `bprOpinion` tinyint(4) DEFAULT NULL COMMENT '1: Like -1: Dislike',
  PRIMARY KEY (`bpr_bpsID`,`bprReadBy_usrID`),
  KEY `bprDate` (`bprDate`),
  KEY `bprOption` (`bprOpinion`),
  KEY `FK_tbl_BLOG_PostRead_tbl_AAA_User` (`bprReadBy_usrID`),
  CONSTRAINT `FK_tblPostRead_tblPosts` FOREIGN KEY (`bpr_bpsID`) REFERENCES `tblPosts` (`bpsid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblPostRead: ~48 rows (approximately)
/*!40000 ALTER TABLE `tblPostRead` DISABLE KEYS */;
INSERT INTO `tblPostRead` (`bpr_bpsID`, `bprReadBy_usrID`, `bprDate`, `bprOpinion`) VALUES
	(1, 1, '2017-07-15 12:29:18', NULL),
	(1, 9, '2017-10-03 18:04:57', NULL),
	(1, 15, '2017-07-14 19:19:14', NULL),
	(1, 19, '2017-07-13 15:52:48', NULL),
	(1, 21, '2017-07-13 18:37:02', -1),
	(2, 1, '2017-07-15 12:29:18', NULL),
	(2, 9, '2017-10-03 18:04:57', NULL),
	(2, 13, '2017-10-29 12:17:14', NULL),
	(2, 15, '2017-07-14 19:19:14', 1),
	(2, 19, '2017-07-13 15:52:48', NULL),
	(2, 21, '2017-07-13 18:37:02', -1),
	(3, 19, '2017-07-13 15:52:48', NULL),
	(9, 19, '2017-07-13 15:52:45', NULL),
	(11, 19, '2017-07-13 15:52:58', NULL),
	(17, 19, '2017-07-13 15:52:58', NULL),
	(31, 19, '2017-07-13 15:52:58', NULL),
	(93, 19, '2017-07-13 15:52:58', NULL),
	(2695, 19, '2017-07-13 15:52:43', NULL),
	(2697, 19, '2017-07-13 15:52:59', NULL),
	(2699, 19, '2017-07-13 15:52:59', NULL),
	(2701, 19, '2017-07-13 15:52:59', NULL),
	(2703, 19, '2017-07-13 15:52:41', NULL),
	(2705, 19, '2017-07-13 15:52:41', NULL),
	(2707, 19, '2017-07-13 15:52:57', NULL),
	(2709, 19, '2017-07-13 15:52:57', NULL),
	(3183, 19, '2017-07-13 15:52:57', NULL),
	(3185, 19, '2017-07-13 15:52:57', NULL),
	(3187, 19, '2017-07-13 15:52:39', NULL),
	(3189, 19, '2017-07-13 15:52:58', NULL),
	(3193, 19, '2017-07-13 15:52:58', NULL),
	(3199, 19, '2017-07-13 15:52:58', NULL),
	(3201, 19, '2017-07-13 15:52:37', NULL),
	(3203, 19, '2017-07-13 15:52:37', NULL),
	(3205, 19, '2017-07-13 15:46:08', NULL),
	(3207, 19, '2017-07-13 15:45:04', NULL),
	(3209, 19, '2017-07-13 15:45:04', NULL),
	(3211, 19, '2017-07-13 15:46:08', NULL),
	(3227, 19, '2017-07-13 15:45:25', NULL),
	(3529, 19, '2017-07-13 15:45:01', NULL),
	(3531, 19, '2017-07-13 15:45:01', NULL),
	(3533, 19, '2017-07-13 15:45:01', NULL),
	(3659, 19, '2017-07-13 15:46:08', NULL),
	(4399, 19, '2017-07-13 15:46:08', NULL),
	(4401, 19, '2017-07-13 15:45:01', NULL),
	(6275, 19, '2017-07-13 15:45:01', NULL),
	(6277, 19, '2017-07-13 15:46:08', NULL),
	(6285, 19, '2017-07-13 15:44:52', -1),
	(6299, 19, '2017-07-13 15:44:52', NULL);
/*!40000 ALTER TABLE `tblPostRead` ENABLE KEYS */;

-- Dumping structure for table TargomanBlog.tblPosts
DROP TABLE IF EXISTS `tblPosts`;
CREATE TABLE IF NOT EXISTS `tblPosts` (
  `bpsID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `bpsTarget_usrID` int(11) unsigned DEFAULT NULL,
  `bpsTitle` varchar(300) NOT NULL,
  `bpsMarkdown` text NOT NULL,
  `bpsLanguage` char(2) DEFAULT NULL,
  `bpsInsertionDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `bps_catID` mediumint(9) unsigned NOT NULL,
  `bps_ptlCode` varchar(50) DEFAULT NULL,
  `bpsShowOnTop` bit(1) NOT NULL DEFAULT b'0',
  `bpsCommentsAllowed` bit(1) NOT NULL DEFAULT b'1',
  `bpsHasComment` bit(1) NOT NULL DEFAULT b'0',
  `bpsStatus` char(1) DEFAULT 'N' COMMENT 'N: New, V: Visited, R: Deleted',
  PRIMARY KEY (`bpsID`),
  KEY `FK_tbl_COMMON_BlogPosts_tbl_AAA_User` (`bpsTarget_usrID`),
  KEY `bpsStatus` (`bpsStatus`),
  KEY `bpsShowOnTop` (`bpsShowOnTop`),
  KEY `bpsInsertionDate` (`bpsInsertionDate`),
  KEY `bpsCategory` (`bps_catID`),
  KEY `bpsLanguage` (`bpsLanguage`),
  KEY `FK_tblPosts_tblPostTemplates` (`bps_ptlCode`),
  KEY `bpsCommentsAllowed` (`bpsCommentsAllowed`),
  KEY `bpsHasComment` (`bpsHasComment`),
  CONSTRAINT `FK_tblPosts_tblCategories` FOREIGN KEY (`bps_catID`) REFERENCES `tblCategories` (`catid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblPosts_tblPostTemplates` FOREIGN KEY (`bps_ptlCode`) REFERENCES `tblPostTemplates` (`ptlcode`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=6966 DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblPosts: ~3,465 rows (approximately)
/*!40000 ALTER TABLE `tblPosts` DISABLE KEYS */;
INSERT INTO `tblPosts` (`bpsID`, `bpsTarget_usrID`, `bpsTitle`, `bpsMarkdown`, `bpsLanguage`, `bpsInsertionDate`, `bps_catID`, `bps_ptlCode`, `bpsShowOnTop`, `bpsCommentsAllowed`, `bpsHasComment`, `bpsStatus`) VALUES
	(1, NULL, 'امکان ترجمه اسناد اضافه شد', 'آیا می دانید که در ترگمان می توانید علاوه بر متن ساده فایلها را نیز ترجمه کنید؟', 'fa', '2016-02-13 15:30:07', 3, NULL, b'0', b'1', b'0', 'N'),
	(2, NULL, 'اسناد نهایی معماری، طراحی، و آزمایش سامانه ترگمان منتشر شد', 'در راستای اهداف حمایتی حامی مالی پروژه ([مرکز تحقیقات مخابرات ایران – وزارت ارتباطات و فناوری اطلاعات](http://itrc.ac.ir/))  و [متن باز سازی سامانه ترگمان](http://blog.targoman.com/?p=108) اسناد کامل طراحی و معماری این سامانه در اختیار عموم قرار می گیرد. این اسناد با مجوز CC-BY-NC-SA نسخه چهارم منتشر شده اند.\r\n\r\n<pageBreak/>\r\n\r\n*   [سند معماری، طراحی و پیاده سازی سامانه](http://blog.targoman.com/wp-content/uploads/2016/04/Targoman-Architecture-and-Design-P5-v2.pdf)\r\n*   [سند آزمایشات سامانه](http://blog.targoman.com/wp-content/uploads/2016/04/Targoman-OSS-BP-Report-v2.pdf)\r\n*   [سند مدل‌های پیشنهادی کسب‌و‌کار مبتنی بر سامانه بومی ترگمان](http://blog.targoman.com/wp-content/uploads/2016/04/Targoman-OSS-BP-Report-v2.pdf)\r\n\r\n\r\n\r\n', 'fa', '2016-02-13 16:40:01', 3, NULL, b'0', b'1', b'1', 'N'),
	(3, 19, '', '{"FileName":"sample-fa.docx","UserName":"سید محمد","ReqNo":"387"}', NULL, '2016-04-28 08:54:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(9, 19, '', '{"FileName":"sample-f\\"\\"a.docx","UserName":"سید محمد","ReqNo":"387"}', NULL, '2016-04-28 09:13:28', 1, 'OFR.Error', b'0', b'1', b'1', 'N'),
	(11, 19, '', '{"FileName":"sample-f\\"\\"a.docx","UserName":"سید محمد","ReqNo":"387"}', NULL, '2016-04-28 09:13:41', 1, 'OFR.Ok', b'0', b'1', b'1', 'N'),
	(13, 11219, '', '{"FileName":"sage14.pdf","UserName":"وحید","ReqNo":"389"}', NULL, '2016-04-28 09:52:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(15, 11219, '', '{"FileName":"sage14.pdf","UserName":"وحید","ReqNo":"389"}', NULL, '2016-04-28 10:00:17', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(17, 19, '', '{"FileName":"sample-f\\"\\"a.docx","UserName":"سید محمد","ReqNo":"387"}', NULL, '2016-04-28 15:25:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(19, 11219, '', '{"FileName":"sage14.pdf","UserName":"وحید","ReqNo":"389"}', NULL, '2016-04-28 15:41:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(21, 11219, '', '{"FileName":"sage14.pdf","UserName":"وحید","ReqNo":"389"}', NULL, '2016-04-28 15:45:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(23, 2, '', '{"FileName":"نامه اختلال در ترگمان.docx","UserName":"پیمان","ReqNo":"5"}', NULL, '2016-04-28 16:00:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(25, 1411, '', '{"FileName":"نظریه اختلاف.docx","UserName":"عطیه","ReqNo":"7"}', NULL, '2016-04-28 16:00:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(27, 3921, '', '{"FileName":"1.PDF","UserName":"sajad","ReqNo":"13"}', NULL, '2016-04-28 16:00:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(29, 3929, '', '{"FileName":"تا کنون مطالعات آزمایشگاهی بسیار زیادی برای بررسی تغییر ترشوندگی.docx","UserName":"رضا","ReqNo":"17"}', NULL, '2016-04-28 16:00:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(31, 19, '', '{"FileName":"TSD-TLG-SAR-001-v01.pdf","UserName":"سید محمد","ReqNo":"27"}', NULL, '2016-04-28 16:00:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(33, 4879, '', '{"FileName":"10.1016-J.IJMACHTOOLS.2006.07.006-Mechanism_of_improvement_of_formability_in_pulsating_hydroforming_","UserName":"محمد","ReqNo":"57"}', NULL, '2016-04-28 16:00:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(35, 4879, '', '{"FileName":"10.1016-J.IJMACHTOOLS.2006.07.006-Mechanism_of_improvement_of_formability_in_pulsating_hydroforming_","UserName":"محمد","ReqNo":"59"}', NULL, '2016-04-28 16:00:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(37, 4947, '', '{"FileName":"10.1016_J.JFINECO.2015.08.010-Assessing-Asset-Pricing-Models-using-Revealed-Preference.pdf","UserName":"جمشید","ReqNo":"63"}', NULL, '2016-04-28 16:00:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(39, 4989, '', '{"FileName":"10.1016_J.ECOLIND.2015.06.022-Integrated-municipal-waste-management-systems-An-indicator-to-assess-t","UserName":"HASAN","ReqNo":"65"}', NULL, '2016-04-28 16:00:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(41, 4989, '', '{"FileName":"10.1016_J.ECOLIND.2015.06.022-Integrated-municipal-waste-management-systems-An-indicator-to-assess-t","UserName":"HASAN","ReqNo":"71"}', NULL, '2016-04-28 16:00:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(43, 4989, '', '{"FileName":"Integrated municipal waste management systems.docx","UserName":"HASAN","ReqNo":"73"}', NULL, '2016-04-28 16:00:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(45, 5305, '', '{"FileName":"5965-7707E (2).pdf","UserName":"amir","ReqNo":"81"}', NULL, '2016-04-28 16:00:27', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(47, 6209, '', '{"FileName":"1.pdf","UserName":"nasrin","ReqNo":"85"}', NULL, '2016-04-28 16:00:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(49, 6229, '', '{"FileName":"1.docx","UserName":"ma","ReqNo":"87"}', NULL, '2016-04-28 16:01:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(51, 6241, '', '{"FileName":"اعلامیه ی جهانی آموزش برای همه.pdf","UserName":"زینب السادات","ReqNo":"89"}', NULL, '2016-04-28 16:03:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(53, 4947, '', '{"FileName":"Author.docx","UserName":"جمشید","ReqNo":"91"}', NULL, '2016-04-28 16:25:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(55, 3013, '', '{"FileName":"111.pdf","UserName":"hossein","ReqNo":"93"}', NULL, '2016-04-28 16:25:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(57, 3013, '', '{"FileName":"2222.pdf","UserName":"hossein","ReqNo":"95"}', NULL, '2016-04-28 16:26:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(59, 6191, '', '{"FileName":"GSM+Different kind of performance.pdf","UserName":"فاطمه","ReqNo":"97"}', NULL, '2016-04-28 16:27:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(61, 6191, '', '{"FileName":"[elearnica.ir]-Impact_of_lean_manufacturing_and_environmental_management_on_business_perfo.pdf","UserName":"فاطمه","ReqNo":"99"}', NULL, '2016-04-28 16:30:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(63, 6191, '', '{"FileName":"[elearnica.ir]-Impact_of_lean_manufacturing_and_environmental_management_on_business_perfo.pdf","UserName":"فاطمه","ReqNo":"99"}', NULL, '2016-04-28 16:32:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(65, 6283, '', '{"FileName":"مدیریت بحران شهری -- لاتین.pdf","UserName":"علیرضا","ReqNo":"101"}', NULL, '2016-04-28 16:32:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(67, 6283, '', '{"FileName":"مقدمه ای بر سیمولیشن شهری  -- لاتین.pdf","UserName":"علیرضا","ReqNo":"103"}', NULL, '2016-04-28 16:32:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(69, 6283, '', '{"FileName":"مدیریت بحران شهری -- لاتین.pdf","UserName":"علیرضا","ReqNo":"101"}', NULL, '2016-04-28 16:32:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(71, 6283, '', '{"FileName":"مقدمه ای بر سیمولیشن شهری  -- لاتین.pdf","UserName":"علیرضا","ReqNo":"103"}', NULL, '2016-04-28 16:32:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(73, 6359, '', '{"FileName":"ADSQCD.pdf","UserName":"مهرداد","ReqNo":"105"}', NULL, '2016-04-28 16:32:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(75, 6359, '', '{"FileName":"ADSQCD.pdf","UserName":"مهرداد","ReqNo":"105"}', NULL, '2016-04-28 16:33:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(77, 6455, '', '{"FileName":"30.docx","UserName":"سهراب","ReqNo":"107"}', NULL, '2016-04-28 16:43:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(79, 6537, '', '{"FileName":"PDF_20160308_085454 (1).pdf","UserName":"مونا","ReqNo":"109"}', NULL, '2016-04-28 16:43:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(81, 6537, '', '{"FileName":"PDF_20160308_085454 (1).pdf","UserName":"مونا","ReqNo":"111"}', NULL, '2016-04-28 16:43:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(83, 6547, '', '{"FileName":"B4ACodeSnippets.docx","UserName":"محمد","ReqNo":"113"}', NULL, '2016-04-28 16:47:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(85, 6455, '', '{"FileName":"30.docx","UserName":"سهراب","ReqNo":"107"}', NULL, '2016-04-28 16:47:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(87, 6537, '', '{"FileName":"PDF_20160308_085454 (1).pdf","UserName":"مونا","ReqNo":"109"}', NULL, '2016-04-28 16:47:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(89, 6537, '', '{"FileName":"PDF_20160308_085454 (1).pdf","UserName":"مونا","ReqNo":"111"}', NULL, '2016-04-28 16:47:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(91, 6547, '', '{"FileName":"B4ACodeSnippets.docx","UserName":"محمد","ReqNo":"113"}', NULL, '2016-04-28 16:51:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(93, 19, '', '{"FileName":"sample-f\\"\\"a.docx","UserName":"سید محمد","ReqNo":"387"}', NULL, '2016-04-28 16:51:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(95, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"115"}', NULL, '2016-04-28 16:51:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(97, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"115"}', NULL, '2016-04-28 16:53:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(99, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"117"}', NULL, '2016-04-28 16:54:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(101, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"117"}', NULL, '2016-04-28 16:56:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(103, 6631, '', '{"FileName":"standards-of-zoo-practice.pdf","UserName":"mohammadjavad","ReqNo":"119"}', NULL, '2016-04-28 16:58:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(105, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"121"}', NULL, '2016-04-28 17:00:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(107, 6631, '', '{"FileName":"standards-of-zoo-practice.pdf","UserName":"mohammadjavad","ReqNo":"119"}', NULL, '2016-04-28 17:00:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(109, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"121"}', NULL, '2016-04-28 17:02:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(111, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"121"}', NULL, '2016-04-28 17:02:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(113, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"123"}', NULL, '2016-04-28 17:03:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(115, 6347, '', '{"FileName":"منابع.pdf","UserName":"مهدیه","ReqNo":"125"}', NULL, '2016-04-28 17:04:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(117, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"123"}', NULL, '2016-04-28 17:05:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(119, 3887, '', '{"FileName":"FCO NO JEO and SON COMPANY Ltd-1.pdf","UserName":"محمد","ReqNo":"127"}', NULL, '2016-04-28 17:05:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(121, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"123"}', NULL, '2016-04-28 17:05:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(123, 6347, '', '{"FileName":"منابع.pdf","UserName":"مهدیه","ReqNo":"125"}', NULL, '2016-04-28 17:06:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(125, 6975, '', '{"FileName":"87.pdf","UserName":"محمود","ReqNo":"129"}', NULL, '2016-04-28 17:06:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(127, 6347, '', '{"FileName":"منابع.pdf","UserName":"مهدیه","ReqNo":"125"}', NULL, '2016-04-28 17:07:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(129, 3887, '', '{"FileName":"FCO NO JEO and SON COMPANY Ltd-1.pdf","UserName":"محمد","ReqNo":"127"}', NULL, '2016-04-28 17:07:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(131, 6975, '', '{"FileName":"کیفیت زندگی ورزشکاران.pdf","UserName":"محمود","ReqNo":"131"}', NULL, '2016-04-28 17:07:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(133, 3887, '', '{"FileName":"FCO NO JEO and SON COMPANY Ltd-1.pdf","UserName":"محمد","ReqNo":"127"}', NULL, '2016-04-28 17:08:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(135, 6975, '', '{"FileName":"87.pdf","UserName":"محمود","ReqNo":"129"}', NULL, '2016-04-28 17:08:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(137, 3887, '', '{"FileName":"پروسیجر طلای سوییس 200 تن.pdf","UserName":"محمد","ReqNo":"133"}', NULL, '2016-04-28 17:08:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(139, 6975, '', '{"FileName":"87.pdf","UserName":"محمود","ReqNo":"129"}', NULL, '2016-04-28 17:09:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(141, 6975, '', '{"FileName":"کیفیت زندگی ورزشکاران.pdf","UserName":"محمود","ReqNo":"131"}', NULL, '2016-04-28 17:09:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(143, 6921, '', '{"FileName":"enlightenment.docx","UserName":"مهدی","ReqNo":"135"}', NULL, '2016-04-28 17:09:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(145, 6975, '', '{"FileName":"کیفیت زندگی ورزشکاران.pdf","UserName":"محمود","ReqNo":"131"}', NULL, '2016-04-28 17:10:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(147, 3887, '', '{"FileName":"پروسیجر طلای سوییس 200 تن.pdf","UserName":"محمد","ReqNo":"133"}', NULL, '2016-04-28 17:10:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(149, 6921, '', '{"FileName":"Critical Theory.docx","UserName":"مهدی","ReqNo":"137"}', NULL, '2016-04-28 17:11:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(151, 7169, '', '{"FileName":"تئوری2.pdf","UserName":"عابد","ReqNo":"139"}', NULL, '2016-04-28 17:11:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(153, 3887, '', '{"FileName":"پروسیجر طلای سوییس 200 تن.pdf","UserName":"محمد","ReqNo":"133"}', NULL, '2016-04-28 17:11:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(155, 6921, '', '{"FileName":"enlightenment.docx","UserName":"مهدی","ReqNo":"135"}', NULL, '2016-04-28 17:11:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(157, 7169, '', '{"FileName":"4_333521983109070869.pdf","UserName":"عابد","ReqNo":"141"}', NULL, '2016-04-28 17:12:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(159, 6921, '', '{"FileName":"enlightenment.docx","UserName":"مهدی","ReqNo":"135"}', NULL, '2016-04-28 17:12:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(161, 6921, '', '{"FileName":"Critical Theory.docx","UserName":"مهدی","ReqNo":"137"}', NULL, '2016-04-28 17:13:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(163, 7169, '', '{"FileName":"تئوری2.pdf","UserName":"عابد","ReqNo":"139"}', NULL, '2016-04-28 17:13:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(165, 7209, '', '{"FileName":"We Are What We Quote.docx","UserName":"مریم","ReqNo":"143"}', NULL, '2016-04-28 17:13:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(167, 6921, '', '{"FileName":"Critical Theory.docx","UserName":"مهدی","ReqNo":"137"}', NULL, '2016-04-28 17:13:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(169, 7169, '', '{"FileName":"تئوری2.pdf","UserName":"عابد","ReqNo":"139"}', NULL, '2016-04-28 17:13:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(171, 7169, '', '{"FileName":"4_333521983109070869.pdf","UserName":"عابد","ReqNo":"141"}', NULL, '2016-04-28 17:15:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(173, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"145"}', NULL, '2016-04-28 17:15:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(175, 7169, '', '{"FileName":"4_333521983109070869.pdf","UserName":"عابد","ReqNo":"141"}', NULL, '2016-04-28 17:15:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(177, 7209, '', '{"FileName":"We Are What We Quote.docx","UserName":"مریم","ReqNo":"143"}', NULL, '2016-04-28 17:16:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(179, 7387, '', '{"FileName":"1-Effects_of_organic_additives_on_crystallization_process_and_the_adsorption_.pdf","UserName":"سجاد","ReqNo":"147"}', NULL, '2016-04-28 17:16:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(181, 7397, '', '{"FileName":"Untitled.docx","UserName":"amirhosein","ReqNo":"149"}', NULL, '2016-04-28 17:16:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(183, 7407, '', '{"FileName":"36_D02_Pages_308_329 (22).pdf","UserName":"روژین","ReqNo":"151"}', NULL, '2016-04-28 17:16:27', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(185, 7209, '', '{"FileName":"We Are What We Quote.docx","UserName":"مریم","ReqNo":"143"}', NULL, '2016-04-28 17:16:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(187, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"153"}', NULL, '2016-04-28 17:16:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(189, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"145"}', NULL, '2016-04-28 17:17:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(191, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"145"}', NULL, '2016-04-28 17:17:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(193, 7443, '', '{"FileName":"soa1.pdf","UserName":"amir","ReqNo":"155"}', NULL, '2016-04-28 17:17:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(195, 7387, '', '{"FileName":"1-Effects_of_organic_additives_on_crystallization_process_and_the_adsorption_.pdf","UserName":"سجاد","ReqNo":"147"}', NULL, '2016-04-28 17:18:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(197, 7387, '', '{"FileName":"1-Effects_of_organic_additives_on_crystallization_process_and_the_adsorption_.pdf","UserName":"سجاد","ReqNo":"147"}', NULL, '2016-04-28 17:18:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(199, 7397, '', '{"FileName":"Untitled.docx","UserName":"amirhosein","ReqNo":"149"}', NULL, '2016-04-28 17:18:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(201, 7407, '', '{"FileName":"36_D02_Pages_308_329 (22).pdf","UserName":"روژین","ReqNo":"151"}', NULL, '2016-04-28 17:18:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(203, 3759, '', '{"FileName":"20110002743.pdf","UserName":"حمید","ReqNo":"157"}', NULL, '2016-04-28 17:19:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(205, 7397, '', '{"FileName":"Untitled.docx","UserName":"amirhosein","ReqNo":"149"}', NULL, '2016-04-28 17:19:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(207, 7407, '', '{"FileName":"36_D02_Pages_308_329 (22).pdf","UserName":"روژین","ReqNo":"151"}', NULL, '2016-04-28 17:19:21', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(209, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"153"}', NULL, '2016-04-28 17:20:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(211, 7527, '', '{"FileName":"Figure 18.docx","UserName":"ali","ReqNo":"159"}', NULL, '2016-04-28 17:20:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(213, 7535, '', '{"FileName":"Analog CMOS Realization of Fuzzy Logic Membership  Functions.pdf","UserName":"محسن","ReqNo":"161"}', NULL, '2016-04-28 17:20:17', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(215, 7585, '', '{"FileName":"unit 8.docx","UserName":"احمد","ReqNo":"163"}', NULL, '2016-04-28 17:20:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(217, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"153"}', NULL, '2016-04-28 17:20:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(219, 7443, '', '{"FileName":"soa1.pdf","UserName":"amir","ReqNo":"155"}', NULL, '2016-04-28 17:21:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(221, 7443, '', '{"FileName":"soa1.pdf","UserName":"amir","ReqNo":"155"}', NULL, '2016-04-28 17:21:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(223, 3759, '', '{"FileName":"20110002743.pdf","UserName":"حمید","ReqNo":"157"}', NULL, '2016-04-28 17:22:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(225, 3759, '', '{"FileName":"20110002743.pdf","UserName":"حمید","ReqNo":"157"}', NULL, '2016-04-28 17:22:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(227, 7527, '', '{"FileName":"Figure 18.docx","UserName":"ali","ReqNo":"159"}', NULL, '2016-04-28 17:23:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(229, 7535, '', '{"FileName":"Analog CMOS Realization of Fuzzy Logic Membership  Functions.pdf","UserName":"محسن","ReqNo":"161"}', NULL, '2016-04-28 17:23:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(231, 7527, '', '{"FileName":"Figure 18.docx","UserName":"ali","ReqNo":"159"}', NULL, '2016-04-28 17:23:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(233, 7535, '', '{"FileName":"Analog CMOS Realization of Fuzzy Logic Membership  Functions.pdf","UserName":"محسن","ReqNo":"161"}', NULL, '2016-04-28 17:23:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(235, 7585, '', '{"FileName":"unit 8.docx","UserName":"احمد","ReqNo":"163"}', NULL, '2016-04-28 17:23:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(237, 7341, '', '{"FileName":"1750110.pdf","UserName":"علی","ReqNo":"165"}', NULL, '2016-04-28 17:24:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(239, 7585, '', '{"FileName":"unit 8.docx","UserName":"احمد","ReqNo":"163"}', NULL, '2016-04-28 17:24:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(241, 7341, '', '{"FileName":"Efficient Decentralized Economic Dispatch for Microgrids with Wind Power Integration.pdf","UserName":"علی","ReqNo":"167"}', NULL, '2016-04-28 17:25:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(243, 7341, '', '{"FileName":"10.1007_s40565-015-0168-1.pdf","UserName":"علی","ReqNo":"169"}', NULL, '2016-04-28 17:25:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(245, 7341, '', '{"FileName":"10.1109-IECON.2013.6699440-Consensus-based-approach-for-economic-dispatch-problem-in-a-smart-grid.pd","UserName":"علی","ReqNo":"171"}', NULL, '2016-04-28 17:25:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(247, 7341, '', '{"FileName":"10.1109-JSAC.2012.120705-Decentralized-Economic-Dispatch-in-Microgrids-via-Heterogeneous-Wireless-Ne","UserName":"علی","ReqNo":"173"}', NULL, '2016-04-28 17:25:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(249, 7341, '', '{"FileName":"10.1109-PES.2011.6039422-Incremental-cost-consensus-algorithm-in-a-smart-grid-environment.pdf","UserName":"علی","ReqNo":"175"}', NULL, '2016-04-28 17:26:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(251, 7341, '', '{"FileName":"10.1109-TAC.2004.834113-Consensus-problems-in-networks-of-agents-with-switching-topology-and-time-de","UserName":"علی","ReqNo":"177"}', NULL, '2016-04-28 17:26:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(253, 7341, '', '{"FileName":"10.1109-TDC.2005.1546751-Economic-Dispatch-of-Power-Systems-Based-on-the-Modified-Particle-Swarm-Opt","UserName":"علی","ReqNo":"179"}', NULL, '2016-04-28 17:26:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(255, 7341, '', '{"FileName":"10.1109-TEVC.2002.806788-Evolutionary-programming-techniques-for-economic-load-dispatch.pdf","UserName":"علی","ReqNo":"181"}', NULL, '2016-04-28 17:27:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(257, 7341, '', '{"FileName":"10.1109-TII.2013.2287807-A-Distributed-Auction-Based-Algorithm-for-the-Non-Convex-Economic-Dispatch-","UserName":"علی","ReqNo":"183"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(259, 7341, '', '{"FileName":"10.1109-TPEL.2011.2175750-Generalized-Frequency-Domain-Formulation-of-the-Switching-Frequency-for-Hy","UserName":"علی","ReqNo":"185"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(261, 7341, '', '{"FileName":"10.1109-TPWRS.2003.814889-Particle-swarm-optimization-to-solving-the-economic-dispatch-considering-t","UserName":"علی","ReqNo":"187"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(263, 7341, '', '{"FileName":"10.1109-TPWRS.2005.857939-Taguchi-method-for-solving-the-economic-dispatch-problem-with-nonsmooth-co","UserName":"علی","ReqNo":"189"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(265, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"191"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(267, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"193"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(269, 7341, '', '{"FileName":"10.1109-TPWRS.2013.2273539-Optimal-Allocation-of-HTS-FCL-for-Power-System-Security-and-Stability-Enh","UserName":"علی","ReqNo":"195"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(271, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"197"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(273, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"199"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(275, 7341, '', '{"FileName":"10.1109-TSG.2012.2233773-Power-Scheduling-of-Distributed-Generators-for-Economic-and-Stable-Operatio","UserName":"علی","ReqNo":"201"}', NULL, '2016-04-28 17:27:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(277, 7341, '', '{"FileName":"1750110.pdf","UserName":"علی","ReqNo":"165"}', NULL, '2016-04-28 17:28:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(279, 7341, '', '{"FileName":"Efficient Decentralized Economic Dispatch for Microgrids with Wind Power Integration.pdf","UserName":"علی","ReqNo":"167"}', NULL, '2016-04-28 17:28:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(281, 7341, '', '{"FileName":"10.1007_s40565-015-0168-1.pdf","UserName":"علی","ReqNo":"169"}', NULL, '2016-04-28 17:28:42', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(283, 7341, '', '{"FileName":"10.1109-IECON.2013.6699440-Consensus-based-approach-for-economic-dispatch-problem-in-a-smart-grid.pd","UserName":"علی","ReqNo":"171"}', NULL, '2016-04-28 17:28:42', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(285, 7341, '', '{"FileName":"10.1109-JSAC.2012.120705-Decentralized-Economic-Dispatch-in-Microgrids-via-Heterogeneous-Wireless-Ne","UserName":"علی","ReqNo":"173"}', NULL, '2016-04-28 17:28:42', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(287, 7341, '', '{"FileName":"1750110.pdf","UserName":"علی","ReqNo":"165"}', NULL, '2016-04-28 17:29:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(289, 7341, '', '{"FileName":"10.1109-PES.2011.6039422-Incremental-cost-consensus-algorithm-in-a-smart-grid-environment.pdf","UserName":"علی","ReqNo":"175"}', NULL, '2016-04-28 17:29:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(291, 7341, '', '{"FileName":"10.1109-TAC.2004.834113-Consensus-problems-in-networks-of-agents-with-switching-topology-and-time-de","UserName":"علی","ReqNo":"177"}', NULL, '2016-04-28 17:29:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(293, 7341, '', '{"FileName":"10.1109-TDC.2005.1546751-Economic-Dispatch-of-Power-Systems-Based-on-the-Modified-Particle-Swarm-Opt","UserName":"علی","ReqNo":"179"}', NULL, '2016-04-28 17:29:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(295, 7313, '', '{"FileName":"Institutions1.pdf","UserName":"sajad","ReqNo":"203"}', NULL, '2016-04-28 17:30:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(297, 7341, '', '{"FileName":"Efficient Decentralized Economic Dispatch for Microgrids with Wind Power Integration.pdf","UserName":"علی","ReqNo":"167"}', NULL, '2016-04-28 17:30:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(299, 7341, '', '{"FileName":"10.1007_s40565-015-0168-1.pdf","UserName":"علی","ReqNo":"169"}', NULL, '2016-04-28 17:30:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(301, 7341, '', '{"FileName":"10.1109-IECON.2013.6699440-Consensus-based-approach-for-economic-dispatch-problem-in-a-smart-grid.pd","UserName":"علی","ReqNo":"171"}', NULL, '2016-04-28 17:30:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(303, 7341, '', '{"FileName":"10.1109-JSAC.2012.120705-Decentralized-Economic-Dispatch-in-Microgrids-via-Heterogeneous-Wireless-Ne","UserName":"علی","ReqNo":"173"}', NULL, '2016-04-28 17:30:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(305, 7341, '', '{"FileName":"10.1109-TEVC.2002.806788-Evolutionary-programming-techniques-for-economic-load-dispatch.pdf","UserName":"علی","ReqNo":"181"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(307, 7341, '', '{"FileName":"10.1109-TII.2013.2287807-A-Distributed-Auction-Based-Algorithm-for-the-Non-Convex-Economic-Dispatch-","UserName":"علی","ReqNo":"183"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(309, 7341, '', '{"FileName":"10.1109-TPEL.2011.2175750-Generalized-Frequency-Domain-Formulation-of-the-Switching-Frequency-for-Hy","UserName":"علی","ReqNo":"185"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(311, 7341, '', '{"FileName":"10.1109-TPWRS.2003.814889-Particle-swarm-optimization-to-solving-the-economic-dispatch-considering-t","UserName":"علی","ReqNo":"187"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(313, 7341, '', '{"FileName":"10.1109-TPWRS.2005.857939-Taguchi-method-for-solving-the-economic-dispatch-problem-with-nonsmooth-co","UserName":"علی","ReqNo":"189"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(315, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"191"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(317, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"193"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(319, 7341, '', '{"FileName":"10.1109-TPWRS.2013.2273539-Optimal-Allocation-of-HTS-FCL-for-Power-System-Security-and-Stability-Enh","UserName":"علی","ReqNo":"195"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(321, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"197"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(323, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"199"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(325, 7341, '', '{"FileName":"10.1109-TSG.2012.2233773-Power-Scheduling-of-Distributed-Generators-for-Economic-and-Stable-Operatio","UserName":"علی","ReqNo":"201"}', NULL, '2016-04-28 17:30:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(327, 7341, '', '{"FileName":"10.1109-PES.2011.6039422-Incremental-cost-consensus-algorithm-in-a-smart-grid-environment.pdf","UserName":"علی","ReqNo":"175"}', NULL, '2016-04-28 17:31:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(329, 7341, '', '{"FileName":"10.1109-TAC.2004.834113-Consensus-problems-in-networks-of-agents-with-switching-topology-and-time-de","UserName":"علی","ReqNo":"177"}', NULL, '2016-04-28 17:31:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(331, 7341, '', '{"FileName":"10.1109-TDC.2005.1546751-Economic-Dispatch-of-Power-Systems-Based-on-the-Modified-Particle-Swarm-Opt","UserName":"علی","ReqNo":"179"}', NULL, '2016-04-28 17:31:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(333, 7313, '', '{"FileName":"Institutions1.pdf","UserName":"sajad","ReqNo":"203"}', NULL, '2016-04-28 17:32:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(335, 7313, '', '{"FileName":"Institutions2.pdf","UserName":"sajad","ReqNo":"205"}', NULL, '2016-04-28 17:32:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(337, 7341, '', '{"FileName":"10.1109-TEVC.2002.806788-Evolutionary-programming-techniques-for-economic-load-dispatch.pdf","UserName":"علی","ReqNo":"181"}', NULL, '2016-04-28 17:32:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(339, 7341, '', '{"FileName":"10.1109-TII.2013.2287807-A-Distributed-Auction-Based-Algorithm-for-the-Non-Convex-Economic-Dispatch-","UserName":"علی","ReqNo":"183"}', NULL, '2016-04-28 17:32:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(341, 7341, '', '{"FileName":"10.1109-TPEL.2011.2175750-Generalized-Frequency-Domain-Formulation-of-the-Switching-Frequency-for-Hy","UserName":"علی","ReqNo":"185"}', NULL, '2016-04-28 17:32:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(343, 7341, '', '{"FileName":"10.1109-TPWRS.2003.814889-Particle-swarm-optimization-to-solving-the-economic-dispatch-considering-t","UserName":"علی","ReqNo":"187"}', NULL, '2016-04-28 17:32:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(345, 7341, '', '{"FileName":"10.1109-TPWRS.2005.857939-Taguchi-method-for-solving-the-economic-dispatch-problem-with-nonsmooth-co","UserName":"علی","ReqNo":"189"}', NULL, '2016-04-28 17:32:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(347, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"191"}', NULL, '2016-04-28 17:32:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(349, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"193"}', NULL, '2016-04-28 17:32:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(351, 7341, '', '{"FileName":"10.1109-TPWRS.2013.2273539-Optimal-Allocation-of-HTS-FCL-for-Power-System-Security-and-Stability-Enh","UserName":"علی","ReqNo":"195"}', NULL, '2016-04-28 17:32:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(353, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"197"}', NULL, '2016-04-28 17:32:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(355, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"199"}', NULL, '2016-04-28 17:32:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(357, 7341, '', '{"FileName":"10.1109-TSG.2012.2233773-Power-Scheduling-of-Distributed-Generators-for-Economic-and-Stable-Operatio","UserName":"علی","ReqNo":"201"}', NULL, '2016-04-28 17:32:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(359, 7313, '', '{"FileName":"Institutions1.pdf","UserName":"sajad","ReqNo":"203"}', NULL, '2016-04-28 17:33:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(361, 7341, '', '{"FileName":"10.1109-TSG.2013.2263847-A-Hybrid-Microgrid-With-DC-Connection-at-Back-to-Back-Converters.pdf","UserName":"علی","ReqNo":"207"}', NULL, '2016-04-28 17:33:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(363, 7341, '', '{"FileName":"10.1109-TSMC.2013.2258909-Decentralized-Economic-Dispatch-in-Smart-Grids-by-Self-Organizing-Dynamic-","UserName":"علی","ReqNo":"209"}', NULL, '2016-04-28 17:33:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(365, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"211"}', NULL, '2016-04-28 17:33:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(367, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"213"}', NULL, '2016-04-28 17:33:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(369, 7313, '', '{"FileName":"Institutions2.pdf","UserName":"sajad","ReqNo":"205"}', NULL, '2016-04-28 17:34:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(371, 7313, '', '{"FileName":"Institutions1.pdf","UserName":"sajad","ReqNo":"203"}', NULL, '2016-04-28 17:34:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(373, 7689, '', '{"FileName":"تنوع جغرافیایی و دو و سه فرزندی.pdf","UserName":"طاهره","ReqNo":"215"}', NULL, '2016-04-28 17:34:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(375, 7313, '', '{"FileName":"Institutions2.pdf","UserName":"sajad","ReqNo":"205"}', NULL, '2016-04-28 17:35:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(377, 7341, '', '{"FileName":"10.1109-TSG.2013.2263847-A-Hybrid-Microgrid-With-DC-Connection-at-Back-to-Back-Converters.pdf","UserName":"علی","ReqNo":"207"}', NULL, '2016-04-28 17:35:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(379, 7341, '', '{"FileName":"10.1109-TSMC.2013.2258909-Decentralized-Economic-Dispatch-in-Smart-Grids-by-Self-Organizing-Dynamic-","UserName":"علی","ReqNo":"209"}', NULL, '2016-04-28 17:35:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(381, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"211"}', NULL, '2016-04-28 17:35:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(383, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"213"}', NULL, '2016-04-28 17:35:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(385, 7341, '', '{"FileName":"10.1109-TSG.2013.2263847-A-Hybrid-Microgrid-With-DC-Connection-at-Back-to-Back-Converters.pdf","UserName":"علی","ReqNo":"207"}', NULL, '2016-04-28 17:36:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(387, 7341, '', '{"FileName":"10.1109-TSMC.2013.2258909-Decentralized-Economic-Dispatch-in-Smart-Grids-by-Self-Organizing-Dynamic-","UserName":"علی","ReqNo":"209"}', NULL, '2016-04-28 17:36:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(389, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"211"}', NULL, '2016-04-28 17:36:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(391, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"213"}', NULL, '2016-04-28 17:36:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(393, 7689, '', '{"FileName":"تنوع جغرافیایی و دو و سه فرزندی.pdf","UserName":"طاهره","ReqNo":"215"}', NULL, '2016-04-28 17:36:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(395, 7313, '', '{"FileName":"Institutions2.pdf","UserName":"sajad","ReqNo":"205"}', NULL, '2016-04-28 17:36:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(397, 7689, '', '{"FileName":"تفاوت در تمایلات باروری .انگلیس.pdf","UserName":"طاهره","ReqNo":"217"}', NULL, '2016-04-28 17:37:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(399, 7689, '', '{"FileName":"تنوع جغرافیایی و دو و سه فرزندی.pdf","UserName":"طاهره","ReqNo":"215"}', NULL, '2016-04-28 17:37:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(401, 7341, '', '{"FileName":"10.1109-TSG.2013.2263847-A-Hybrid-Microgrid-With-DC-Connection-at-Back-to-Back-Converters.pdf","UserName":"علی","ReqNo":"207"}', NULL, '2016-04-28 17:38:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(403, 7341, '', '{"FileName":"10.1109-TSMC.2013.2258909-Decentralized-Economic-Dispatch-in-Smart-Grids-by-Self-Organizing-Dynamic-","UserName":"علی","ReqNo":"209"}', NULL, '2016-04-28 17:38:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(405, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"211"}', NULL, '2016-04-28 17:38:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(407, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"213"}', NULL, '2016-04-28 17:38:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(409, 7711, '', '{"FileName":"10.1007_s12666-015-0723-5-Indian-China-Clays-Vagaries-in-Process-and-Flow-Sheet-Development.pdf","UserName":"majid","ReqNo":"219"}', NULL, '2016-04-28 17:38:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(411, 7689, '', '{"FileName":"تفاوت در تمایلات باروری .انگلیس.pdf","UserName":"طاهره","ReqNo":"217"}', NULL, '2016-04-28 17:38:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(413, 7689, '', '{"FileName":"تنوع جغرافیایی و دو و سه فرزندی.pdf","UserName":"طاهره","ReqNo":"215"}', NULL, '2016-04-28 17:39:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(415, 7689, '', '{"FileName":"تفاوت در تمایلات باروری .انگلیس.pdf","UserName":"طاهره","ReqNo":"217"}', NULL, '2016-04-28 17:39:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(417, 7711, '', '{"FileName":"10.1007_s12666-015-0723-5-Indian-China-Clays-Vagaries-in-Process-and-Flow-Sheet-Development.pdf","UserName":"majid","ReqNo":"219"}', NULL, '2016-04-28 17:39:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(419, 7785, '', '{"FileName":"5.pdf","UserName":"سمیره","ReqNo":"221"}', NULL, '2016-04-28 17:40:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(421, 7711, '', '{"FileName":"10.1007_s12666-015-0723-5-Indian-China-Clays-Vagaries-in-Process-and-Flow-Sheet-Development.pdf","UserName":"majid","ReqNo":"219"}', NULL, '2016-04-28 17:40:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(423, 7407, '', '{"FileName":"36_D05_Pages_294_306 (13).pdf","UserName":"روژین","ReqNo":"223"}', NULL, '2016-04-28 17:41:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(425, 7995, '', '{"FileName":"نمونه سوالDoc1.docx","UserName":"جواد","ReqNo":"225"}', NULL, '2016-04-28 17:41:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(427, 7689, '', '{"FileName":"تفاوت در تمایلات باروری .انگلیس.pdf","UserName":"طاهره","ReqNo":"217"}', NULL, '2016-04-28 17:41:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(429, 7785, '', '{"FileName":"5.pdf","UserName":"سمیره","ReqNo":"221"}', NULL, '2016-04-28 17:41:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(431, 6975, '', '{"FileName":"کیفیت وتاثیر بچهها.pdf","UserName":"محمود","ReqNo":"227"}', NULL, '2016-04-28 17:42:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(433, 7711, '', '{"FileName":"10.1007_s12666-015-0723-5-Indian-China-Clays-Vagaries-in-Process-and-Flow-Sheet-Development.pdf","UserName":"majid","ReqNo":"219"}', NULL, '2016-04-28 17:42:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(435, 7785, '', '{"FileName":"5.pdf","UserName":"سمیره","ReqNo":"221"}', NULL, '2016-04-28 17:42:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(437, 7407, '', '{"FileName":"36_D05_Pages_294_306 (13).pdf","UserName":"روژین","ReqNo":"223"}', NULL, '2016-04-28 17:42:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(439, 7995, '', '{"FileName":"نمونه سوالDoc1.docx","UserName":"جواد","ReqNo":"225"}', NULL, '2016-04-28 17:42:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(441, 7407, '', '{"FileName":"36_D05_Pages_294_306 (13).pdf","UserName":"روژین","ReqNo":"223"}', NULL, '2016-04-28 17:43:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(443, 7995, '', '{"FileName":"نمونه سوالDoc1.docx","UserName":"جواد","ReqNo":"225"}', NULL, '2016-04-28 17:43:40', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(445, 6975, '', '{"FileName":"کیفیت وتاثیر بچهها.pdf","UserName":"محمود","ReqNo":"227"}', NULL, '2016-04-28 17:43:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(447, 7785, '', '{"FileName":"5.pdf","UserName":"سمیره","ReqNo":"221"}', NULL, '2016-04-28 17:44:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(449, 6975, '', '{"FileName":"کیفیت وتاثیر بچهها.pdf","UserName":"محمود","ReqNo":"227"}', NULL, '2016-04-28 17:44:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(451, 7407, '', '{"FileName":"36_D05_Pages_294_306 (13).pdf","UserName":"روژین","ReqNo":"223"}', NULL, '2016-04-28 17:45:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(453, 7995, '', '{"FileName":"نمونه سوالDoc1.docx","UserName":"جواد","ReqNo":"225"}', NULL, '2016-04-28 17:45:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(455, 6975, '', '{"FileName":"کیفیت وتاثیر بچهها.pdf","UserName":"محمود","ReqNo":"227"}', NULL, '2016-04-28 17:46:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(457, 4271, '', '{"FileName":"اب.doc","UserName":"مهناز","ReqNo":"229"}', NULL, '2016-04-28 17:56:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(459, 4271, '', '{"FileName":"اب.doc","UserName":"مهناز","ReqNo":"229"}', NULL, '2016-04-28 17:56:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(461, 4271, '', '{"FileName":"اب.doc","UserName":"مهناز","ReqNo":"229"}', NULL, '2016-04-28 17:57:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(463, 8721, '', '{"FileName":"Hoshin Kanri.docx","UserName":"احسان","ReqNo":"231"}', NULL, '2016-04-28 17:57:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(465, 8721, '', '{"FileName":"Hoshin Kanri.docx","UserName":"احسان","ReqNo":"231"}', NULL, '2016-04-28 17:57:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(467, 4271, '', '{"FileName":"اب.doc","UserName":"مهناز","ReqNo":"229"}', NULL, '2016-04-28 17:58:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(469, 8721, '', '{"FileName":"Hoshin Kanri.docx","UserName":"احسان","ReqNo":"231"}', NULL, '2016-04-28 17:58:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(471, 8721, '', '{"FileName":"Hoshin Kanri.docx","UserName":"احسان","ReqNo":"231"}', NULL, '2016-04-28 17:59:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(473, 8721, '', '{"FileName":"What is project efficiency and effectiveness.docx","UserName":"احسان","ReqNo":"233"}', NULL, '2016-04-28 18:00:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(475, 8721, '', '{"FileName":"What is project efficiency and effectiveness.docx","UserName":"احسان","ReqNo":"233"}', NULL, '2016-04-28 18:00:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(477, 8721, '', '{"FileName":"What is project efficiency and effectiveness.docx","UserName":"احسان","ReqNo":"233"}', NULL, '2016-04-28 18:01:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(479, 9041, '', '{"FileName":"talent.docx","UserName":"یونس","ReqNo":"235"}', NULL, '2016-04-28 18:01:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(481, 9041, '', '{"FileName":"talent.docx","UserName":"یونس","ReqNo":"235"}', NULL, '2016-04-28 18:02:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(483, 8721, '', '{"FileName":"What is project efficiency and effectiveness.docx","UserName":"احسان","ReqNo":"233"}', NULL, '2016-04-28 18:02:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(485, 8721, '', '{"FileName":"What is project efficiency and effectiveness.docx","UserName":"احسان","ReqNo":"233"}', NULL, '2016-04-28 18:02:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(487, 9041, '', '{"FileName":"talent.docx","UserName":"یونس","ReqNo":"235"}', NULL, '2016-04-28 18:02:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(489, 9059, '', '{"FileName":"subcont.pdf","UserName":"میثم","ReqNo":"237"}', NULL, '2016-04-28 18:02:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(491, 9059, '', '{"FileName":"subcont.pdf","UserName":"میثم","ReqNo":"237"}', NULL, '2016-04-28 18:03:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(493, 9059, '', '{"FileName":"subcont.pdf","UserName":"میثم","ReqNo":"237"}', NULL, '2016-04-28 18:03:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(495, 9041, '', '{"FileName":"talent.docx","UserName":"یونس","ReqNo":"235"}', NULL, '2016-04-28 18:03:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(497, 4947, '', '{"FileName":"عرضه اولیه.docx","UserName":"جمشید","ReqNo":"239"}', NULL, '2016-04-28 18:03:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(499, 4947, '', '{"FileName":"عرضه اولیه.docx","UserName":"جمشید","ReqNo":"239"}', NULL, '2016-04-28 18:04:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(501, 9041, '', '{"FileName":"talent.docx","UserName":"یونس","ReqNo":"235"}', NULL, '2016-04-28 18:04:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(503, 4947, '', '{"FileName":"عرضه اولیه.docx","UserName":"جمشید","ReqNo":"239"}', NULL, '2016-04-28 18:04:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(505, 9059, '', '{"FileName":"subcont.pdf","UserName":"میثم","ReqNo":"237"}', NULL, '2016-04-28 18:04:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(507, 9059, '', '{"FileName":"subcont.pdf","UserName":"میثم","ReqNo":"237"}', NULL, '2016-04-28 18:05:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(509, 4947, '', '{"FileName":"عرضه اولیه.docx","UserName":"جمشید","ReqNo":"239"}', NULL, '2016-04-28 18:05:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(511, 4947, '', '{"FileName":"شرایط شرکت های موجود.docx","UserName":"جمشید","ReqNo":"241"}', NULL, '2016-04-28 18:05:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(513, 4947, '', '{"FileName":"شرایط شرکت های موجود.docx","UserName":"جمشید","ReqNo":"241"}', NULL, '2016-04-28 18:06:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(515, 4947, '', '{"FileName":"شرایط شرکت های موجود.docx","UserName":"جمشید","ReqNo":"241"}', NULL, '2016-04-28 18:06:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(517, 4947, '', '{"FileName":"عرضه اولیه.docx","UserName":"جمشید","ReqNo":"239"}', NULL, '2016-04-28 18:06:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(519, 4947, '', '{"FileName":"شرایط شرکت های درلیست خروج.docx","UserName":"جمشید","ReqNo":"243"}', NULL, '2016-04-28 18:06:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(521, 4947, '', '{"FileName":"شرایط شرکت های درلیست خروج.docx","UserName":"جمشید","ReqNo":"243"}', NULL, '2016-04-28 18:07:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(523, 4947, '', '{"FileName":"شرایط شرکت های درلیست خروج.docx","UserName":"جمشید","ReqNo":"243"}', NULL, '2016-04-28 18:07:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(525, 4947, '', '{"FileName":"شرایط شرکت های موجود.docx","UserName":"جمشید","ReqNo":"241"}', NULL, '2016-04-28 18:07:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(527, 4947, '', '{"FileName":"شرایط شرکت های موجود.docx","UserName":"جمشید","ReqNo":"241"}', NULL, '2016-04-28 18:08:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(529, 4947, '', '{"FileName":"شرایط شرکت های درلیست خروج.docx","UserName":"جمشید","ReqNo":"243"}', NULL, '2016-04-28 18:08:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(531, 4947, '', '{"FileName":"شرایط شرکت های درلیست خروج.docx","UserName":"جمشید","ReqNo":"243"}', NULL, '2016-04-28 18:09:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(533, 9181, '', '{"FileName":"Document2.docx","UserName":"جواد","ReqNo":"245"}', NULL, '2016-04-28 18:10:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(535, 9259, '', '{"FileName":"exchang  marriage.PDF","UserName":"kkk","ReqNo":"247"}', NULL, '2016-04-28 18:10:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(537, 9181, '', '{"FileName":"Document2.docx","UserName":"جواد","ReqNo":"245"}', NULL, '2016-04-28 18:10:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(539, 9259, '', '{"FileName":"exchang  marriage.PDF","UserName":"kkk","ReqNo":"247"}', NULL, '2016-04-28 18:10:15', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(541, 9181, '', '{"FileName":"Document2.docx","UserName":"جواد","ReqNo":"245"}', NULL, '2016-04-28 18:10:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(543, 9259, '', '{"FileName":"exchang  marriage.PDF","UserName":"kkk","ReqNo":"247"}', NULL, '2016-04-28 18:10:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(545, 9181, '', '{"FileName":"Document2.docx","UserName":"جواد","ReqNo":"245"}', NULL, '2016-04-28 18:11:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(547, 9259, '', '{"FileName":"exchang  marriage.PDF","UserName":"kkk","ReqNo":"247"}', NULL, '2016-04-28 18:11:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(549, 9309, '', '{"FileName":"69-129-1-SM - Copy.pdf","UserName":"حسین","ReqNo":"249"}', NULL, '2016-04-28 18:11:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(551, 9309, '', '{"FileName":"69-129-1-SM - Copy.pdf","UserName":"حسین","ReqNo":"249"}', NULL, '2016-04-28 18:11:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(553, 9309, '', '{"FileName":"69-129-1-SM - Copy.pdf","UserName":"حسین","ReqNo":"249"}', NULL, '2016-04-28 18:12:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(555, 9309, '', '{"FileName":"69-129-1-SM - Copy.pdf","UserName":"حسین","ReqNo":"249"}', NULL, '2016-04-28 18:13:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(557, 9181, '', '{"FileName":"Document2.docx","UserName":"جواد","ReqNo":"245"}', NULL, '2016-04-28 18:13:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(559, 9259, '', '{"FileName":"exchang  marriage.PDF","UserName":"kkk","ReqNo":"247"}', NULL, '2016-04-28 18:13:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(561, 9293, '', '{"FileName":"ifrs2_en.pdf","UserName":"فرید","ReqNo":"251"}', NULL, '2016-04-28 18:14:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(563, 9309, '', '{"FileName":"69-129-1-SM - Copy.pdf","UserName":"حسین","ReqNo":"249"}', NULL, '2016-04-28 18:15:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(565, 9293, '', '{"FileName":"ifrs2_en.pdf","UserName":"فرید","ReqNo":"251"}', NULL, '2016-04-28 18:15:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(567, 9293, '', '{"FileName":"ifrs2_en.pdf","UserName":"فرید","ReqNo":"251"}', NULL, '2016-04-28 18:16:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(569, 9293, '', '{"FileName":"ifrs2_en.pdf","UserName":"فرید","ReqNo":"251"}', NULL, '2016-04-28 18:16:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(571, 9293, '', '{"FileName":"ias2_en.pdf","UserName":"فرید","ReqNo":"253"}', NULL, '2016-04-28 18:16:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(573, 9293, '', '{"FileName":"ifrs2_en.pdf","UserName":"فرید","ReqNo":"251"}', NULL, '2016-04-28 18:16:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(575, 9293, '', '{"FileName":"ias2_en.pdf","UserName":"فرید","ReqNo":"253"}', NULL, '2016-04-28 18:16:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(577, 9293, '', '{"FileName":"ias2_en.pdf","UserName":"فرید","ReqNo":"253"}', NULL, '2016-04-28 18:17:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(579, 9293, '', '{"FileName":"ias7_en.pdf","UserName":"فرید","ReqNo":"255"}', NULL, '2016-04-28 18:17:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(581, 9293, '', '{"FileName":"ias2_en.pdf","UserName":"فرید","ReqNo":"253"}', NULL, '2016-04-28 18:17:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(583, 9293, '', '{"FileName":"ias2_en.pdf","UserName":"فرید","ReqNo":"253"}', NULL, '2016-04-28 18:17:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(585, 9293, '', '{"FileName":"ias7_en.pdf","UserName":"فرید","ReqNo":"255"}', NULL, '2016-04-28 18:17:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(587, 9293, '', '{"FileName":"ias7_en.pdf","UserName":"فرید","ReqNo":"255"}', NULL, '2016-04-28 18:18:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(589, 9293, '', '{"FileName":"ias8_en.pdf","UserName":"فرید","ReqNo":"257"}', NULL, '2016-04-28 18:18:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(591, 9293, '', '{"FileName":"ias7_en.pdf","UserName":"فرید","ReqNo":"255"}', NULL, '2016-04-28 18:18:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(593, 9293, '', '{"FileName":"ias7_en.pdf","UserName":"فرید","ReqNo":"255"}', NULL, '2016-04-28 18:18:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(595, 9293, '', '{"FileName":"ias8_en.pdf","UserName":"فرید","ReqNo":"257"}', NULL, '2016-04-28 18:18:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(597, 9293, '', '{"FileName":"ias8_en.pdf","UserName":"فرید","ReqNo":"257"}', NULL, '2016-04-28 18:19:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(599, 9293, '', '{"FileName":"ias8_en.pdf","UserName":"فرید","ReqNo":"257"}', NULL, '2016-04-28 18:19:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(601, 9293, '', '{"FileName":"ias8_en.pdf","UserName":"فرید","ReqNo":"257"}', NULL, '2016-04-28 18:19:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(603, 9293, '', '{"FileName":"ias10_en.pdf","UserName":"فرید","ReqNo":"259"}', NULL, '2016-04-28 18:19:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(605, 9293, '', '{"FileName":"ias10_en.pdf","UserName":"فرید","ReqNo":"259"}', NULL, '2016-04-28 18:19:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(607, 9293, '', '{"FileName":"ias10_en.pdf","UserName":"فرید","ReqNo":"259"}', NULL, '2016-04-28 18:20:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(609, 9293, '', '{"FileName":"ias10_en.pdf","UserName":"فرید","ReqNo":"259"}', NULL, '2016-04-28 18:20:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(611, 9293, '', '{"FileName":"ias11_en.pdf","UserName":"فرید","ReqNo":"261"}', NULL, '2016-04-28 18:20:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(613, 9293, '', '{"FileName":"ias10_en.pdf","UserName":"فرید","ReqNo":"259"}', NULL, '2016-04-28 18:20:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(615, 9293, '', '{"FileName":"ias11_en.pdf","UserName":"فرید","ReqNo":"261"}', NULL, '2016-04-28 18:20:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(617, 9293, '', '{"FileName":"ias11_en.pdf","UserName":"فرید","ReqNo":"261"}', NULL, '2016-04-28 18:21:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(619, 9293, '', '{"FileName":"ias11_en.pdf","UserName":"فرید","ReqNo":"261"}', NULL, '2016-04-28 18:21:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(621, 9293, '', '{"FileName":"ias11_en.pdf","UserName":"فرید","ReqNo":"261"}', NULL, '2016-04-28 18:22:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(623, 9293, '', '{"FileName":"ias12_en.pdf","UserName":"فرید","ReqNo":"263"}', NULL, '2016-04-28 18:23:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(625, 9293, '', '{"FileName":"ias12_en.pdf","UserName":"فرید","ReqNo":"263"}', NULL, '2016-04-28 18:23:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(627, 9293, '', '{"FileName":"ias12_en.pdf","UserName":"فرید","ReqNo":"263"}', NULL, '2016-04-28 18:24:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(629, 9293, '', '{"FileName":"ias12_en.pdf","UserName":"فرید","ReqNo":"263"}', NULL, '2016-04-28 18:24:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(631, 9293, '', '{"FileName":"ias12_en.pdf","UserName":"فرید","ReqNo":"263"}', NULL, '2016-04-28 18:24:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(633, 9293, '', '{"FileName":"ias17_en.pdf","UserName":"فرید","ReqNo":"265"}', NULL, '2016-04-28 18:24:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(635, 9293, '', '{"FileName":"ias17_en.pdf","UserName":"فرید","ReqNo":"265"}', NULL, '2016-04-28 18:24:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(637, 9293, '', '{"FileName":"ias17_en.pdf","UserName":"فرید","ReqNo":"265"}', NULL, '2016-04-28 18:25:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(639, 9293, '', '{"FileName":"ias17_en.pdf","UserName":"فرید","ReqNo":"265"}', NULL, '2016-04-28 18:25:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(641, 9293, '', '{"FileName":"ias17_en.pdf","UserName":"فرید","ReqNo":"265"}', NULL, '2016-04-28 18:25:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(643, 9293, '', '{"FileName":"ias18_en.pdf","UserName":"فرید","ReqNo":"267"}', NULL, '2016-04-28 18:25:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(645, 9293, '', '{"FileName":"ias18_en.pdf","UserName":"فرید","ReqNo":"267"}', NULL, '2016-04-28 18:25:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(647, 9293, '', '{"FileName":"ias18_en.pdf","UserName":"فرید","ReqNo":"267"}', NULL, '2016-04-28 18:26:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(649, 9293, '', '{"FileName":"ias18_en.pdf","UserName":"فرید","ReqNo":"267"}', NULL, '2016-04-28 18:26:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(651, 9293, '', '{"FileName":"ias18_en.pdf","UserName":"فرید","ReqNo":"267"}', NULL, '2016-04-28 18:26:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(653, 9293, '', '{"FileName":"ias20_en.pdf","UserName":"فرید","ReqNo":"269"}', NULL, '2016-04-28 18:26:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(655, 9293, '', '{"FileName":"ias20_en.pdf","UserName":"فرید","ReqNo":"269"}', NULL, '2016-04-28 18:26:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(657, 9293, '', '{"FileName":"ias20_en.pdf","UserName":"فرید","ReqNo":"269"}', NULL, '2016-04-28 18:27:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(659, 9293, '', '{"FileName":"ias20_en.pdf","UserName":"فرید","ReqNo":"269"}', NULL, '2016-04-28 18:27:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(661, 9293, '', '{"FileName":"ias20_en.pdf","UserName":"فرید","ReqNo":"269"}', NULL, '2016-04-28 18:27:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(663, 9293, '', '{"FileName":"ias40_en.pdf","UserName":"فرید","ReqNo":"271"}', NULL, '2016-04-28 18:28:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(665, 9293, '', '{"FileName":"ias40_en.pdf","UserName":"فرید","ReqNo":"271"}', NULL, '2016-04-28 18:28:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(667, 9293, '', '{"FileName":"ias40_en.pdf","UserName":"فرید","ReqNo":"271"}', NULL, '2016-04-28 18:28:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(669, 9293, '', '{"FileName":"ias40_en.pdf","UserName":"فرید","ReqNo":"271"}', NULL, '2016-04-28 18:28:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(671, 9293, '', '{"FileName":"ias40_en.pdf","UserName":"فرید","ReqNo":"271"}', NULL, '2016-04-28 18:28:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(673, 9515, '', '{"FileName":"بدون عنوان.docx","UserName":"آریا","ReqNo":"273"}', NULL, '2016-04-28 18:29:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(675, 9515, '', '{"FileName":"بدون عنوان.docx","UserName":"آریا","ReqNo":"273"}', NULL, '2016-04-28 18:30:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(677, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-04-28 18:30:27', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(679, 9515, '', '{"FileName":"بدون عنوان.docx","UserName":"آریا","ReqNo":"273"}', NULL, '2016-04-28 18:30:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(681, 9515, '', '{"FileName":"بدون عنوان.docx","UserName":"آریا","ReqNo":"273"}', NULL, '2016-04-28 18:30:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(683, 9515, '', '{"FileName":"بدون عنوان.docx","UserName":"آریا","ReqNo":"273"}', NULL, '2016-04-28 18:30:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(685, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-04-28 18:30:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(687, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-04-28 18:30:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(689, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-04-28 18:30:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(691, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-04-28 18:30:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(693, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-04-28 18:30:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(695, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-04-28 18:31:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(697, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-04-28 18:31:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(699, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-04-28 18:31:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(701, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-04-28 18:31:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(703, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-04-28 18:31:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(705, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-04-28 18:31:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(707, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-04-28 18:31:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(709, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-04-28 18:31:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(711, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-04-28 18:31:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(713, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-04-28 18:31:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(715, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-04-28 18:31:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(717, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-04-28 18:32:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(719, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-04-28 18:32:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(721, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-04-28 18:32:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(723, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-04-28 18:32:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(725, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-04-28 18:32:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(727, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-04-28 18:32:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(729, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-04-28 18:32:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(731, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-04-28 18:32:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(733, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-04-28 18:32:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(735, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-04-28 18:33:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(737, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-04-28 18:33:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(739, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-04-28 18:33:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(741, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-04-28 18:33:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(743, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-04-28 18:33:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(745, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-04-28 18:33:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(747, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-04-28 18:33:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(749, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-04-28 18:33:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(751, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-04-28 18:33:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(753, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-04-28 18:33:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(755, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-04-28 18:33:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(757, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-04-28 18:33:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(759, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-04-28 18:33:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(761, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-04-28 18:33:34', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(763, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-04-28 18:33:34', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(765, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-04-28 18:33:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(767, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-04-28 18:33:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(769, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-04-28 18:33:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(771, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-04-28 18:34:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(773, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-04-28 18:34:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(775, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-04-28 18:34:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(777, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-04-28 18:34:05', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(779, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-04-28 18:34:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(781, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-04-28 18:34:10', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(783, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-04-28 18:34:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(785, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"293"}', NULL, '2016-04-28 18:34:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(787, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-04-28 18:34:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(789, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-04-28 18:34:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(791, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"293"}', NULL, '2016-04-28 18:36:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(793, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"293"}', NULL, '2016-04-28 18:36:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(795, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"293"}', NULL, '2016-04-28 18:36:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(797, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"293"}', NULL, '2016-04-28 18:36:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(799, 9801, '', '{"FileName":"ترجمه.pdf","UserName":"ایوب","ReqNo":"295"}', NULL, '2016-04-28 18:37:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(801, 9801, '', '{"FileName":"ترجمه.pdf","UserName":"ایوب","ReqNo":"295"}', NULL, '2016-04-28 18:38:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(803, 9801, '', '{"FileName":"ترجمه.pdf","UserName":"ایوب","ReqNo":"295"}', NULL, '2016-04-28 18:38:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(805, 9801, '', '{"FileName":"ترجمه.pdf","UserName":"ایوب","ReqNo":"295"}', NULL, '2016-04-28 18:38:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(807, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-04-28 18:48:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(809, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"293"}', NULL, '2016-04-28 18:49:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(811, 9801, '', '{"FileName":"ترجمه.pdf","UserName":"ایوب","ReqNo":"295"}', NULL, '2016-04-28 18:51:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(813, 9949, '', '{"FileName":"rezdeb_664463.docx","UserName":"mohamad","ReqNo":"297"}', NULL, '2016-04-28 18:56:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(815, 9949, '', '{"FileName":"rezdeb_664463.docx","UserName":"mohamad","ReqNo":"297"}', NULL, '2016-04-28 18:56:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(817, 9949, '', '{"FileName":"rezdeb_664463.docx","UserName":"mohamad","ReqNo":"297"}', NULL, '2016-04-28 18:56:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(819, 9949, '', '{"FileName":"rezdeb_664463.docx","UserName":"mohamad","ReqNo":"297"}', NULL, '2016-04-28 18:56:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(821, 9949, '', '{"FileName":"paper10.pdf","UserName":"mohamad","ReqNo":"299"}', NULL, '2016-04-28 18:56:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(823, 9949, '', '{"FileName":"rezdeb_664463.docx","UserName":"mohamad","ReqNo":"297"}', NULL, '2016-04-28 18:56:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(825, 9949, '', '{"FileName":"paper10.pdf","UserName":"mohamad","ReqNo":"299"}', NULL, '2016-04-28 18:56:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(827, 9949, '', '{"FileName":"paper10.pdf","UserName":"mohamad","ReqNo":"299"}', NULL, '2016-04-28 18:56:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(829, 9949, '', '{"FileName":"paper10.pdf","UserName":"mohamad","ReqNo":"299"}', NULL, '2016-04-28 18:56:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(831, 9949, '', '{"FileName":"paper10.pdf","UserName":"mohamad","ReqNo":"299"}', NULL, '2016-04-28 18:56:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(833, 9949, '', '{"FileName":"570e437b8915b994d7b9a3e1107c9c6c5da5 (1).pdf","UserName":"mohamad","ReqNo":"301"}', NULL, '2016-04-28 18:56:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(835, 9949, '', '{"FileName":"570e437b8915b994d7b9a3e1107c9c6c5da5 (1).pdf","UserName":"mohamad","ReqNo":"301"}', NULL, '2016-04-28 18:56:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(837, 9949, '', '{"FileName":"570e437b8915b994d7b9a3e1107c9c6c5da5 (1).pdf","UserName":"mohamad","ReqNo":"301"}', NULL, '2016-04-28 18:57:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(839, 9949, '', '{"FileName":"570e437b8915b994d7b9a3e1107c9c6c5da5 (1).pdf","UserName":"mohamad","ReqNo":"301"}', NULL, '2016-04-28 18:57:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(841, 9949, '', '{"FileName":"570e437b8915b994d7b9a3e1107c9c6c5da5 (1).pdf","UserName":"mohamad","ReqNo":"301"}', NULL, '2016-04-28 18:57:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(843, 9949, '', '{"FileName":"IJCSI-9-6-3-466-473.pdf","UserName":"mohamad","ReqNo":"303"}', NULL, '2016-04-28 18:57:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(845, 9949, '', '{"FileName":"IJCSI-9-6-3-466-473.pdf","UserName":"mohamad","ReqNo":"303"}', NULL, '2016-04-28 18:57:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(847, 9949, '', '{"FileName":"IJCSI-9-6-3-466-473.pdf","UserName":"mohamad","ReqNo":"303"}', NULL, '2016-04-28 18:57:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(849, 9949, '', '{"FileName":"IJCSI-9-6-3-466-473.pdf","UserName":"mohamad","ReqNo":"303"}', NULL, '2016-04-28 18:57:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(851, 9949, '', '{"FileName":"IJCSI-9-6-3-466-473.pdf","UserName":"mohamad","ReqNo":"303"}', NULL, '2016-04-28 18:57:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(853, 9949, '', '{"FileName":"IJEIT1412201506_19.pdf","UserName":"mohamad","ReqNo":"305"}', NULL, '2016-04-28 18:57:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(855, 9949, '', '{"FileName":"IJEIT1412201506_19.pdf","UserName":"mohamad","ReqNo":"305"}', NULL, '2016-04-28 18:57:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(857, 9949, '', '{"FileName":"IJEIT1412201506_19.pdf","UserName":"mohamad","ReqNo":"305"}', NULL, '2016-04-28 18:57:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(859, 9949, '', '{"FileName":"IJEIT1412201506_19.pdf","UserName":"mohamad","ReqNo":"305"}', NULL, '2016-04-28 18:57:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(861, 9949, '', '{"FileName":"IJEIT1412201506_19.pdf","UserName":"mohamad","ReqNo":"305"}', NULL, '2016-04-28 18:57:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(863, 6083, '', '{"FileName":"New-order-preserving-encryption-model-for-outsourced-databases-in-cloud-environments-2016.pdf","UserName":"اصغر","ReqNo":"307"}', NULL, '2016-04-28 18:57:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(865, 6083, '', '{"FileName":"An-improved-database-synchronization-mechanism-for-mobile-data-using-software-defined-networking-con","UserName":"اصغر","ReqNo":"309"}', NULL, '2016-04-28 18:57:43', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(867, 6083, '', '{"FileName":"New-order-preserving-encryption-model-for-outsourced-databases-in-cloud-environments-2016.pdf","UserName":"اصغر","ReqNo":"307"}', NULL, '2016-04-28 18:57:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(869, 6083, '', '{"FileName":"An-improved-database-synchronization-mechanism-for-mobile-data-using-software-defined-networking-con","UserName":"اصغر","ReqNo":"309"}', NULL, '2016-04-28 18:57:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(871, 6083, '', '{"FileName":"New-order-preserving-encryption-model-for-outsourced-databases-in-cloud-environments-2016.pdf","UserName":"اصغر","ReqNo":"307"}', NULL, '2016-04-28 18:57:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(873, 6083, '', '{"FileName":"An-improved-database-synchronization-mechanism-for-mobile-data-using-software-defined-networking-con","UserName":"اصغر","ReqNo":"309"}', NULL, '2016-04-28 18:57:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(875, 6083, '', '{"FileName":"New-order-preserving-encryption-model-for-outsourced-databases-in-cloud-environments-2016.pdf","UserName":"اصغر","ReqNo":"307"}', NULL, '2016-04-28 18:57:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(877, 6083, '', '{"FileName":"An-improved-database-synchronization-mechanism-for-mobile-data-using-software-defined-networking-con","UserName":"اصغر","ReqNo":"309"}', NULL, '2016-04-28 18:57:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(879, 6083, '', '{"FileName":"New-order-preserving-encryption-model-for-outsourced-databases-in-cloud-environments-2016.pdf","UserName":"اصغر","ReqNo":"307"}', NULL, '2016-04-28 18:57:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(881, 6083, '', '{"FileName":"An-improved-database-synchronization-mechanism-for-mobile-data-using-software-defined-networking-con","UserName":"اصغر","ReqNo":"309"}', NULL, '2016-04-28 18:57:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(883, 2057, '', '{"FileName":"[Elearnica] -An_improved_multihop-based_localization_algorithm_for_wireless_sensor_netwo.pdf","UserName":"محمد موسی","ReqNo":"311"}', NULL, '2016-04-28 18:58:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(885, 2057, '', '{"FileName":"[Elearnica] -An_improved_multihop-based_localization_algorithm_for_wireless_sensor_netwo.pdf","UserName":"محمد موسی","ReqNo":"311"}', NULL, '2016-04-28 18:58:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(887, 2057, '', '{"FileName":"[Elearnica] -An_improved_multihop-based_localization_algorithm_for_wireless_sensor_netwo.pdf","UserName":"محمد موسی","ReqNo":"311"}', NULL, '2016-04-28 18:58:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(889, 2057, '', '{"FileName":"[Elearnica] -An_improved_multihop-based_localization_algorithm_for_wireless_sensor_netwo.pdf","UserName":"محمد موسی","ReqNo":"311"}', NULL, '2016-04-28 18:58:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(891, 2057, '', '{"FileName":"[Elearnica] -An_improved_multihop-based_localization_algorithm_for_wireless_sensor_netwo.pdf","UserName":"محمد موسی","ReqNo":"311"}', NULL, '2016-04-28 18:58:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(893, 10067, '', '{"FileName":"3.pdf","UserName":"وارش","ReqNo":"313"}', NULL, '2016-04-28 18:58:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(895, 10067, '', '{"FileName":"3.pdf","UserName":"وارش","ReqNo":"313"}', NULL, '2016-04-28 18:58:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(897, 10067, '', '{"FileName":"3.pdf","UserName":"وارش","ReqNo":"313"}', NULL, '2016-04-28 18:58:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(899, 10067, '', '{"FileName":"3.pdf","UserName":"وارش","ReqNo":"313"}', NULL, '2016-04-28 18:58:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(901, 10067, '', '{"FileName":"3.pdf","UserName":"وارش","ReqNo":"313"}', NULL, '2016-04-28 18:58:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(903, 9949, '', '{"FileName":"From-single-watermark-to-dual-watermark-A-new-approach-for-image-watermarking.pdf","UserName":"mohamad","ReqNo":"315"}', NULL, '2016-04-28 18:58:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(905, 9949, '', '{"FileName":"From-single-watermark-to-dual-watermark-A-new-approach-for-image-watermarking.pdf","UserName":"mohamad","ReqNo":"315"}', NULL, '2016-04-28 18:58:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(907, 9949, '', '{"FileName":"From-single-watermark-to-dual-watermark-A-new-approach-for-image-watermarking.pdf","UserName":"mohamad","ReqNo":"315"}', NULL, '2016-04-28 18:58:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(909, 9949, '', '{"FileName":"From-single-watermark-to-dual-watermark-A-new-approach-for-image-watermarking.pdf","UserName":"mohamad","ReqNo":"315"}', NULL, '2016-04-28 18:58:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(911, 9949, '', '{"FileName":"From-single-watermark-to-dual-watermark-A-new-approach-for-image-watermarking.pdf","UserName":"mohamad","ReqNo":"315"}', NULL, '2016-04-28 18:58:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(913, 10165, '', '{"FileName":"Right_to_be_forgotten_-_Wikipedia,_the_free_encyclopedia.pdf","UserName":"جمشید","ReqNo":"317"}', NULL, '2016-04-28 18:58:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(915, 10165, '', '{"FileName":"Right_to_be_forgotten_-_Wikipedia,_the_free_encyclopedia.pdf","UserName":"جمشید","ReqNo":"317"}', NULL, '2016-04-28 18:59:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(917, 10165, '', '{"FileName":"Right_to_be_forgotten_-_Wikipedia,_the_free_encyclopedia.pdf","UserName":"جمشید","ReqNo":"317"}', NULL, '2016-04-28 18:59:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(919, 10165, '', '{"FileName":"Right_to_be_forgotten_-_Wikipedia,_the_free_encyclopedia.pdf","UserName":"جمشید","ReqNo":"317"}', NULL, '2016-04-28 18:59:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(921, 10165, '', '{"FileName":"Right_to_be_forgotten_-_Wikipedia,_the_free_encyclopedia.pdf","UserName":"جمشید","ReqNo":"317"}', NULL, '2016-04-28 18:59:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(923, 10195, '', '{"FileName":"2014062311065413837.pdf","UserName":"reza","ReqNo":"319"}', NULL, '2016-04-28 18:59:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(925, 10195, '', '{"FileName":"2014062311065413837.pdf","UserName":"reza","ReqNo":"319"}', NULL, '2016-04-28 18:59:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(927, 10195, '', '{"FileName":"2014062311065413837.pdf","UserName":"reza","ReqNo":"319"}', NULL, '2016-04-28 18:59:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(929, 10195, '', '{"FileName":"2014062311065413837.pdf","UserName":"reza","ReqNo":"319"}', NULL, '2016-04-28 18:59:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(931, 10195, '', '{"FileName":"2014062311065413837.pdf","UserName":"reza","ReqNo":"319"}', NULL, '2016-04-28 18:59:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(933, 7583, '', '{"FileName":"Introducing Small Basic.docx","UserName":"رضا","ReqNo":"321"}', NULL, '2016-04-28 18:59:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(935, 7583, '', '{"FileName":"Introducing Small Basic.docx","UserName":"رضا","ReqNo":"321"}', NULL, '2016-04-28 18:59:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(937, 7583, '', '{"FileName":"Introducing Small Basic.docx","UserName":"رضا","ReqNo":"321"}', NULL, '2016-04-28 18:59:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(939, 7583, '', '{"FileName":"Introducing Small Basic.docx","UserName":"رضا","ReqNo":"321"}', NULL, '2016-04-28 18:59:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(941, 7583, '', '{"FileName":"Introducing Small Basic.docx","UserName":"رضا","ReqNo":"321"}', NULL, '2016-04-28 18:59:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(943, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-04-28 19:08:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(945, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-04-28 19:17:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(947, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-04-28 19:18:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(949, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-04-28 19:18:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(951, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-04-28 19:19:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(953, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-04-28 19:20:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(955, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"325"}', NULL, '2016-04-28 19:20:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(957, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"325"}', NULL, '2016-04-28 19:20:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(959, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"325"}', NULL, '2016-04-28 19:20:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(961, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"325"}', NULL, '2016-04-28 19:20:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(963, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"325"}', NULL, '2016-04-28 19:20:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(965, 9905, '', '{"FileName":"1-s2.0-S0017931012001524-main.pdf","UserName":"علی","ReqNo":"325"}', NULL, '2016-04-28 19:20:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(967, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-04-28 19:20:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(969, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-04-28 19:20:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(971, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-04-28 19:20:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(973, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-04-28 19:20:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(975, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-04-28 19:20:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(977, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-04-28 19:21:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(979, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-04-28 19:21:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(981, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-04-28 19:21:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(983, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-04-28 19:21:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(985, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-04-28 19:21:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(987, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-04-28 19:21:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(989, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-04-28 19:21:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(991, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-04-28 19:21:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(993, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-04-28 19:21:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(995, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-04-28 19:21:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(997, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-04-28 19:21:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(999, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-04-28 19:21:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1001, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-04-28 19:21:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1003, 10707, '', '{"FileName":"25.docx","UserName":"زهره","ReqNo":"333"}', NULL, '2016-04-28 19:21:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1005, 10707, '', '{"FileName":"25.docx","UserName":"زهره","ReqNo":"333"}', NULL, '2016-04-28 19:21:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1007, 10707, '', '{"FileName":"25.docx","UserName":"زهره","ReqNo":"333"}', NULL, '2016-04-28 19:21:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1009, 10707, '', '{"FileName":"25.docx","UserName":"زهره","ReqNo":"333"}', NULL, '2016-04-28 19:21:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1011, 10727, '', '{"FileName":"out.pdf","UserName":"isa","ReqNo":"335"}', NULL, '2016-04-28 19:21:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1013, 10707, '', '{"FileName":"25.docx","UserName":"زهره","ReqNo":"333"}', NULL, '2016-04-28 19:21:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1015, 10727, '', '{"FileName":"out.pdf","UserName":"isa","ReqNo":"335"}', NULL, '2016-04-28 19:21:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1017, 10707, '', '{"FileName":"25.docx","UserName":"زهره","ReqNo":"333"}', NULL, '2016-04-28 19:22:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1019, 10727, '', '{"FileName":"out.pdf","UserName":"isa","ReqNo":"335"}', NULL, '2016-04-28 19:22:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1021, 10727, '', '{"FileName":"out.pdf","UserName":"isa","ReqNo":"335"}', NULL, '2016-04-28 19:22:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1023, 10727, '', '{"FileName":"out.pdf","UserName":"isa","ReqNo":"335"}', NULL, '2016-04-28 19:23:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1025, 10727, '', '{"FileName":"out.pdf","UserName":"isa","ReqNo":"335"}', NULL, '2016-04-28 19:24:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1027, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-04-28 19:24:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1029, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-04-28 19:24:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1031, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-04-28 19:24:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1033, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-04-28 19:24:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1035, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-04-28 19:25:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1037, 10815, '', '{"FileName":"1.pdf","UserName":"amir","ReqNo":"339"}', NULL, '2016-04-28 19:25:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1039, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-04-28 19:25:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1041, 10815, '', '{"FileName":"1.pdf","UserName":"amir","ReqNo":"339"}', NULL, '2016-04-28 19:25:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1043, 10815, '', '{"FileName":"1.pdf","UserName":"amir","ReqNo":"339"}', NULL, '2016-04-28 19:26:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1045, 10815, '', '{"FileName":"1.pdf","UserName":"amir","ReqNo":"339"}', NULL, '2016-04-28 19:26:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1047, 10815, '', '{"FileName":"1.pdf","UserName":"amir","ReqNo":"339"}', NULL, '2016-04-28 19:26:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1049, 10815, '', '{"FileName":"2.pdf","UserName":"amir","ReqNo":"341"}', NULL, '2016-04-28 19:26:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1051, 10815, '', '{"FileName":"1.pdf","UserName":"amir","ReqNo":"339"}', NULL, '2016-04-28 19:26:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1053, 10815, '', '{"FileName":"2.pdf","UserName":"amir","ReqNo":"341"}', NULL, '2016-04-28 19:26:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1055, 10815, '', '{"FileName":"2.pdf","UserName":"amir","ReqNo":"341"}', NULL, '2016-04-28 19:26:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1057, 10815, '', '{"FileName":"2.pdf","UserName":"amir","ReqNo":"341"}', NULL, '2016-04-28 19:27:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1059, 10815, '', '{"FileName":"3.pdf","UserName":"amir","ReqNo":"343"}', NULL, '2016-04-28 19:27:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1061, 10815, '', '{"FileName":"2.pdf","UserName":"amir","ReqNo":"341"}', NULL, '2016-04-28 19:27:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1063, 10815, '', '{"FileName":"3.pdf","UserName":"amir","ReqNo":"343"}', NULL, '2016-04-28 19:27:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1065, 10815, '', '{"FileName":"3.pdf","UserName":"amir","ReqNo":"343"}', NULL, '2016-04-28 19:27:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1067, 10815, '', '{"FileName":"2.pdf","UserName":"amir","ReqNo":"341"}', NULL, '2016-04-28 19:27:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1069, 10815, '', '{"FileName":"3.pdf","UserName":"amir","ReqNo":"343"}', NULL, '2016-04-28 19:27:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1071, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-04-28 19:28:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1073, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-04-28 19:28:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1075, 10815, '', '{"FileName":"3.pdf","UserName":"amir","ReqNo":"343"}', NULL, '2016-04-28 19:28:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1077, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-04-28 19:28:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1079, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-04-28 19:28:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1081, 10815, '', '{"FileName":"3.pdf","UserName":"amir","ReqNo":"343"}', NULL, '2016-04-28 19:28:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1083, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-04-28 19:28:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1085, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-04-28 19:28:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1087, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-04-28 19:29:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(1089, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 09:26:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1091, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 09:32:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1093, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 09:37:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1095, 11453, '', '{"FileName":"Overcoming(90).pdf","UserName":"sahar","ReqNo":"397"}', NULL, '2016-04-29 14:54:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1097, 11453, '', '{"FileName":"Overcoming(91).pdf","UserName":"sahar","ReqNo":"399"}', NULL, '2016-04-29 14:56:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1099, 11453, '', '{"FileName":"Overcoming(92).pdf","UserName":"sahar","ReqNo":"401"}', NULL, '2016-04-29 14:57:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1101, 11453, '', '{"FileName":"Overcoming(93).pdf","UserName":"sahar","ReqNo":"403"}', NULL, '2016-04-29 14:59:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1103, 11453, '', '{"FileName":"Overcoming(95).pdf","UserName":"sahar","ReqNo":"405"}', NULL, '2016-04-29 14:59:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(1105, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1107, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1109, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1111, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1113, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1115, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1117, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1119, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1121, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1123, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1125, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1127, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1129, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1131, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1133, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1135, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1137, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1139, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1141, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1143, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1145, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1147, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1149, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1151, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1153, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1155, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1157, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1159, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1161, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1163, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1165, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1167, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1169, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1171, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1173, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1175, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1177, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1179, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1181, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1183, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1185, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1187, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1189, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1191, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1193, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1195, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1197, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1199, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1201, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1203, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1205, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1207, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1209, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1211, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1213, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1215, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-04-29 18:16:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1217, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:21', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1219, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:21', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1221, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1223, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1225, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1227, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1229, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1231, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1233, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1235, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1237, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1239, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1241, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1243, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1245, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1247, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1249, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1251, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1253, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1255, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1257, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1259, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1261, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1263, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1265, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1267, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1269, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1271, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1273, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1275, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1277, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1279, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1281, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1283, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1285, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1287, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1289, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1291, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1293, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1295, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1297, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1299, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1301, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1303, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1305, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1307, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1309, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1311, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1313, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1315, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1317, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1319, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1321, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1323, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1325, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1327, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1329, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1331, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1333, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1335, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1337, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1339, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1341, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1343, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1345, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1347, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1349, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1351, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1353, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1355, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1357, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1359, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1361, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1363, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1365, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1367, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1369, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1371, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1373, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1375, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1377, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1379, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1381, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1383, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1385, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1387, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1389, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1391, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1393, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1395, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1397, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1399, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1401, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1403, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1405, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1407, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1409, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1411, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:17:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1413, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1415, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1417, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-04-29 18:17:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1419, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-04-29 18:17:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1421, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1423, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1425, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1427, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1429, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1431, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1433, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1435, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1437, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1439, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1441, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1443, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1445, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1447, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1449, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1451, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1453, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1455, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:17:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1457, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1459, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1461, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1463, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1465, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1467, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1469, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1471, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1473, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1475, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1477, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1479, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1481, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1483, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1485, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1487, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1489, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1491, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1493, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1495, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1497, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1499, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1501, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1503, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1505, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1507, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1509, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1511, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1513, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1515, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1517, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1519, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1521, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1523, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1525, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1527, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1529, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1531, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1533, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1535, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1537, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1539, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1541, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1543, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1545, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1547, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1549, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-04-29 18:18:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1551, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1553, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1555, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1557, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1559, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1561, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1563, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1565, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1567, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1569, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1571, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1573, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1575, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1577, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1579, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1581, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1583, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1585, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:18:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1587, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1589, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1591, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1593, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1595, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1597, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1599, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1601, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1603, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1605, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1607, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1609, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1611, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1613, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1615, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1617, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1619, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1621, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1623, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1625, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1627, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1629, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1631, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1633, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1635, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1637, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1639, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1641, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1643, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1645, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1647, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1649, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1651, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1653, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1655, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1657, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1659, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:18:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1661, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1663, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-04-29 18:18:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1665, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-04-29 18:18:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1667, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1669, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1671, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1673, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1675, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:18:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1677, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1679, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1681, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1683, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1685, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:19:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1687, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:19:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1689, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1691, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1693, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1695, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1697, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:19:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1699, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:19:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1701, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1703, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1705, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1707, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1709, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1711, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1713, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1715, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1717, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1719, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1721, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1723, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1725, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1727, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1729, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1731, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1733, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1735, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1737, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1739, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1741, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1743, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1745, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1747, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1749, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1751, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1753, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1755, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1757, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1759, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1761, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1763, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1765, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1767, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1769, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1771, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-04-29 18:19:27', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1773, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1775, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1777, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1779, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1781, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1783, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1785, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1787, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1789, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1791, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1793, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1795, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1797, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1799, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1801, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1803, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1805, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1807, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1809, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1811, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1813, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1815, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1817, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1819, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1821, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1823, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1825, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1827, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1829, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1831, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1833, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1835, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1837, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1839, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1841, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1843, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1845, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1847, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1849, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1851, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1853, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1855, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1857, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1859, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1861, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1863, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1865, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1867, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1869, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1871, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1873, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1875, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-04-29 18:19:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1877, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1879, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1881, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1883, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:19:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1885, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1887, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1889, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1891, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:00', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1893, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1895, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1897, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1899, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1901, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1903, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1905, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1907, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:20:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1909, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:20:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1911, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1913, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1915, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1917, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1919, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1921, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1923, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1925, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1927, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1929, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1931, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1933, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1935, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1937, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1939, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1941, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1943, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1945, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1947, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1949, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1951, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1953, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1955, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1957, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1959, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1961, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1963, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1965, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1967, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1969, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1971, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1973, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1975, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1977, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1979, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-04-29 18:20:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1981, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1983, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1985, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1987, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1989, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1991, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1993, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1995, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1997, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(1999, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2001, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2003, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2005, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2007, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2009, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2011, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2013, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2015, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2017, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2019, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2021, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2023, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2025, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2027, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2029, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2031, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2033, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2035, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2037, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2039, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2041, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2043, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2045, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2047, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2049, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2051, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2053, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2055, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2057, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2059, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2061, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2063, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2065, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2067, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2069, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2071, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2073, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2075, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2077, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2079, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2081, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2083, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2085, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2087, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2089, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2091, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2093, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2095, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2097, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2099, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2101, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2103, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2105, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2107, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2109, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2111, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2113, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2115, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2117, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2119, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2121, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2123, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2125, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2127, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2129, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2131, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2133, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2135, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2137, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2139, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2141, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2143, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2145, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2147, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2149, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2151, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2153, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2155, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2157, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2159, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2161, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2163, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2165, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2167, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2169, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2171, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2173, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2175, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2177, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2179, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2181, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2183, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2185, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2187, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2189, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2191, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2193, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2195, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2197, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2199, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2201, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2203, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2205, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2207, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2209, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2211, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2213, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2215, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2217, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2219, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2221, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2223, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2225, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2227, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2229, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2231, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2233, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2235, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2237, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2239, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2241, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2243, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2245, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2247, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2249, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2251, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2253, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2255, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2257, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2259, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2261, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2263, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2265, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2267, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2269, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2271, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2273, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2275, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2277, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2279, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:20:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2281, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2283, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2285, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2287, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2289, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2291, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2293, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2295, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2297, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2299, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2301, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2303, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2305, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2307, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2309, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2311, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2313, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2315, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2317, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2319, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2321, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2323, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2325, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2327, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2329, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2331, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2333, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2335, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2337, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2339, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2341, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2343, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-04-29 18:21:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2345, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2347, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2349, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2351, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2353, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2355, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2357, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2359, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2361, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2363, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2365, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2367, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2369, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2371, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2373, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2375, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2377, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2379, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2381, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2383, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2385, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2387, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2389, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2391, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2393, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2395, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2397, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2399, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2401, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2403, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2405, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2407, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-04-29 18:21:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2409, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2411, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2413, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2415, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2417, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2419, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2421, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2423, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2425, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2427, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2429, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2431, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2433, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2435, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2437, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2439, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2441, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2443, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2445, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2447, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2449, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2451, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2453, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2455, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2457, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2459, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2461, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2463, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2465, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2467, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2469, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2471, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2473, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2475, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2477, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2479, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2481, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2483, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2485, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2487, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2489, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2491, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2493, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2495, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2497, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2499, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2501, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2503, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2505, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2507, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2509, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2511, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2513, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2515, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2517, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2519, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2521, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-04-29 18:21:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2523, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2525, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2527, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2529, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2531, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2533, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2535, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2537, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2539, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2541, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2543, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2545, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2547, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2549, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2551, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2553, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2555, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2557, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2559, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2561, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:21:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2563, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:22:17', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2565, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:22:17', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2567, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2569, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2571, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2573, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2575, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2577, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2579, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2581, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2583, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:22:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2585, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:22:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2587, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:22:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2589, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:22:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2591, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:22:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2593, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:23:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2595, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:23:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2597, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:23:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2599, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:23:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2601, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-04-29 18:23:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2603, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-04-29 18:25:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2605, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-04-29 18:25:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2607, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-04-29 18:25:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2609, 11453, '', '{"FileName":"Overcoming(90).pdf","UserName":"sahar","ReqNo":"397"}', NULL, '2016-04-29 18:46:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2611, 11453, '', '{"FileName":"Overcoming(91).pdf","UserName":"sahar","ReqNo":"399"}', NULL, '2016-04-29 18:47:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2613, 11453, '', '{"FileName":"Overcoming(92).pdf","UserName":"sahar","ReqNo":"401"}', NULL, '2016-04-29 18:47:15', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2615, 11453, '', '{"FileName":"Overcoming(93).pdf","UserName":"sahar","ReqNo":"403"}', NULL, '2016-04-29 18:47:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2617, 11453, '', '{"FileName":"Overcoming(95).pdf","UserName":"sahar","ReqNo":"405"}', NULL, '2016-04-29 18:48:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2619, 6689, '', '{"FileName":"[freeScience.ir]-10.1109-JSEN.2015.2445796.pdf","UserName":"محمد","ReqNo":"407"}', NULL, '2016-04-30 01:03:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2621, 11581, '', '{"FileName":"582.pdf","UserName":"فاطمه","ReqNo":"409"}', NULL, '2016-04-30 12:34:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2623, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"411"}', NULL, '2016-04-30 14:25:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2625, 11547, '', '{"FileName":"1.pdf","UserName":"ابوالفضل","ReqNo":"413"}', NULL, '2016-04-30 18:21:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2627, 6689, '', '{"FileName":"[freeScience.ir]-10.1109-JSEN.2015.2445796.pdf","UserName":"محمد","ReqNo":"407"}', NULL, '2016-04-30 20:26:43', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2629, 11581, '', '{"FileName":"582.pdf","UserName":"فاطمه","ReqNo":"409"}', NULL, '2016-04-30 20:29:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2631, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"411"}', NULL, '2016-04-30 20:33:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2633, 11547, '', '{"FileName":"1.pdf","UserName":"ابوالفضل","ReqNo":"413"}', NULL, '2016-04-30 20:34:16', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2635, 11547, '', '{"FileName":"ـاثیر جیبرلیک اسید روی بذر کاهو.pdf","UserName":"ابوالفضل","ReqNo":"415"}', NULL, '2016-05-01 00:36:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2637, 11547, '', '{"FileName":"Auxin Biosynthesis and Metabolism.pdf","UserName":"ابوالفضل","ReqNo":"417"}', NULL, '2016-05-01 00:39:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2639, 11547, '', '{"FileName":"Auxin conjugates their role for plant development and in the.pdf","UserName":"ابوالفضل","ReqNo":"419"}', NULL, '2016-05-01 00:39:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2641, 11547, '', '{"FileName":"Polar transport of the plant hormone auxin – the role of.pdf","UserName":"ابوالفضل","ReqNo":"421"}', NULL, '2016-05-01 00:40:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2643, 11727, '', '{"FileName":"CROWNCOM_2011.pdf","UserName":"faez","ReqNo":"423"}', NULL, '2016-05-01 12:16:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2645, 11547, '', '{"FileName":"ـاثیر جیبرلیک اسید روی بذر کاهو.pdf","UserName":"ابوالفضل","ReqNo":"415"}', NULL, '2016-05-01 14:42:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2647, 11547, '', '{"FileName":"Auxin Biosynthesis and Metabolism.pdf","UserName":"ابوالفضل","ReqNo":"417"}', NULL, '2016-05-01 14:42:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2649, 11547, '', '{"FileName":"Auxin conjugates their role for plant development and in the.pdf","UserName":"ابوالفضل","ReqNo":"419"}', NULL, '2016-05-01 14:43:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2651, 11547, '', '{"FileName":"Polar transport of the plant hormone auxin – the role of.pdf","UserName":"ابوالفضل","ReqNo":"421"}', NULL, '2016-05-01 14:44:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2653, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-05-01 14:47:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2655, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-05-01 14:48:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2657, 11453, '', '{"FileName":"Overcoming(90).pdf","UserName":"sahar","ReqNo":"397"}', NULL, '2016-05-01 14:48:20', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2659, 11453, '', '{"FileName":"Overcoming(91).pdf","UserName":"sahar","ReqNo":"399"}', NULL, '2016-05-01 14:48:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2661, 1713, '', '{"FileName":"تخت جمشید و آکروپلیس.pdf","UserName":"رضوان","ReqNo":"391"}', NULL, '2016-05-01 15:05:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2663, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-05-01 15:05:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2665, 1713, '', '{"FileName":"The Parthenon Frieze and the Apadana Reliefs at Persepolis:.pdf","UserName":"رضوان","ReqNo":"395"}', NULL, '2016-05-01 15:07:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2667, 11453, '', '{"FileName":"Overcoming(90).pdf","UserName":"sahar","ReqNo":"397"}', NULL, '2016-05-01 15:07:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2669, 11453, '', '{"FileName":"Overcoming(91).pdf","UserName":"sahar","ReqNo":"399"}', NULL, '2016-05-01 15:08:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2671, 11453, '', '{"FileName":"Overcoming(92).pdf","UserName":"sahar","ReqNo":"401"}', NULL, '2016-05-01 15:09:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2673, 11453, '', '{"FileName":"Overcoming(93).pdf","UserName":"sahar","ReqNo":"403"}', NULL, '2016-05-01 15:09:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2675, 11453, '', '{"FileName":"Overcoming(95).pdf","UserName":"sahar","ReqNo":"405"}', NULL, '2016-05-01 15:10:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2677, 6689, '', '{"FileName":"[freeScience.ir]-10.1109-JSEN.2015.2445796.pdf","UserName":"محمد","ReqNo":"407"}', NULL, '2016-05-01 15:11:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2679, 11581, '', '{"FileName":"582.pdf","UserName":"فاطمه","ReqNo":"409"}', NULL, '2016-05-01 15:12:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2681, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"411"}', NULL, '2016-05-01 15:13:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2683, 11547, '', '{"FileName":"1.pdf","UserName":"ابوالفضل","ReqNo":"413"}', NULL, '2016-05-01 15:14:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2685, 11547, '', '{"FileName":"ـاثیر جیبرلیک اسید روی بذر کاهو.pdf","UserName":"ابوالفضل","ReqNo":"415"}', NULL, '2016-05-01 15:15:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2687, 11547, '', '{"FileName":"Auxin Biosynthesis and Metabolism.pdf","UserName":"ابوالفضل","ReqNo":"417"}', NULL, '2016-05-01 15:17:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2689, 11547, '', '{"FileName":"Auxin conjugates their role for plant development and in the.pdf","UserName":"ابوالفضل","ReqNo":"419"}', NULL, '2016-05-01 15:19:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2691, 11547, '', '{"FileName":"Polar transport of the plant hormone auxin – the role of.pdf","UserName":"ابوالفضل","ReqNo":"421"}', NULL, '2016-05-01 15:19:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2693, 11727, '', '{"FileName":"CROWNCOM_2011.pdf","UserName":"faez","ReqNo":"423"}', NULL, '2016-05-01 15:19:15', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2695, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.pdf","UserName":"سید محمد","ReqNo":"425"}', NULL, '2016-05-01 16:10:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2697, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.pdf","UserName":"سید محمد","ReqNo":"427"}', NULL, '2016-05-01 16:10:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2699, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.pdf","UserName":"سید محمد","ReqNo":"425"}', NULL, '2016-05-01 16:15:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2701, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.pdf","UserName":"سید محمد","ReqNo":"427"}', NULL, '2016-05-01 16:15:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2703, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.docx","UserName":"سید محمد","ReqNo":"429"}', NULL, '2016-05-01 16:16:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2705, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.docx","UserName":"سید محمد","ReqNo":"429"}', NULL, '2016-05-01 16:16:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2707, 19, '', '{"FileName":"User_Manual_NEX607_120724.pdf","UserName":"سید محمد","ReqNo":"431"}', NULL, '2016-05-01 16:17:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2709, 19, '', '{"FileName":"User_Manual_NEX607_120724.pdf","UserName":"سید محمد","ReqNo":"431"}', NULL, '2016-05-01 16:19:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2711, 11837, '', '{"FileName":"4.pdf","UserName":"roya","ReqNo":"433"}', NULL, '2016-05-02 09:40:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2713, 11837, '', '{"FileName":"4.pdf","UserName":"roya","ReqNo":"433"}', NULL, '2016-05-02 10:00:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2715, 11865, '', '{"FileName":"order-z-1462076694-1378.pdf","UserName":"افشین","ReqNo":"435"}', NULL, '2016-05-02 14:00:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2717, 11865, '', '{"FileName":"order-z-1462076694-1378.pdf","UserName":"افشین","ReqNo":"435"}', NULL, '2016-05-02 14:30:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2719, 11997, '', '{"FileName":"10.1016_J.RENENE.2008.03.004-Reliability-and-economic-evaluation-of-small-autonomous-power-systems-c","UserName":"ali","ReqNo":"437"}', NULL, '2016-05-03 16:14:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2721, 11997, '', '{"FileName":"10.1016_J.RENENE.2008.03.004-Reliability-and-economic-evaluation-of-small-autonomous-power-systems-c","UserName":"ali","ReqNo":"437"}', NULL, '2016-05-03 16:30:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2723, 12197, '', '{"FileName":"VOICE (2).pdf","UserName":"mohsen","ReqNo":"439"}', NULL, '2016-05-05 10:47:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2725, 12197, '', '{"FileName":"VOICE (2).pdf","UserName":"mohsen","ReqNo":"439"}', NULL, '2016-05-05 11:00:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2727, 12217, '', '{"FileName":"Common Sense Guide to Mitigating Insider Threats.pdf","UserName":"سید محمد","ReqNo":"441"}', NULL, '2016-05-05 12:38:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2729, 12217, '', '{"FileName":"1.docx","UserName":"سید محمد","ReqNo":"443"}', NULL, '2016-05-05 12:40:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2731, 12217, '', '{"FileName":"2.pdf","UserName":"سید محمد","ReqNo":"445"}', NULL, '2016-05-05 12:40:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2733, 12217, '', '{"FileName":"Common Sense Guide to Mitigating Insider Threats.pdf","UserName":"سید محمد","ReqNo":"441"}', NULL, '2016-05-05 13:00:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2735, 12217, '', '{"FileName":"1.docx","UserName":"سید محمد","ReqNo":"443"}', NULL, '2016-05-05 13:00:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2737, 12217, '', '{"FileName":"2.pdf","UserName":"سید محمد","ReqNo":"445"}', NULL, '2016-05-05 13:00:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2739, 12217, '', '{"FileName":"2.pdf","UserName":"سید محمد","ReqNo":"447"}', NULL, '2016-05-05 14:28:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2741, 12217, '', '{"FileName":"Common Sense Guide to Mitigating Insider Threats.pdf","UserName":"سید محمد","ReqNo":"449"}', NULL, '2016-05-05 14:28:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2743, 12217, '', '{"FileName":"1.docx","UserName":"سید محمد","ReqNo":"451"}', NULL, '2016-05-05 14:28:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2745, 12217, '', '{"FileName":"2.pdf","UserName":"سید محمد","ReqNo":"447"}', NULL, '2016-05-05 14:30:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2747, 12217, '', '{"FileName":"Common Sense Guide to Mitigating Insider Threats.pdf","UserName":"سید محمد","ReqNo":"449"}', NULL, '2016-05-05 14:35:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2749, 12217, '', '{"FileName":"1.docx","UserName":"سید محمد","ReqNo":"451"}', NULL, '2016-05-05 14:39:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2751, 12273, '', '{"FileName":"1-s2.0-S0022169414003205-main.pdf","UserName":"سهند","ReqNo":"453"}', NULL, '2016-05-05 20:44:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2753, 12273, '', '{"FileName":"1-s2.0-S0022169414003205-main.pdf","UserName":"سهند","ReqNo":"453"}', NULL, '2016-05-05 21:01:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2755, 12297, '', '{"FileName":"بیان مسئله.docx","UserName":"بحجت","ReqNo":"455"}', NULL, '2016-05-06 01:02:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2757, 12297, '', '{"FileName":"بیان مسئله.docx","UserName":"بحجت","ReqNo":"455"}', NULL, '2016-05-06 01:30:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2759, 12309, '', '{"FileName":"FNR-59-30034.pdf","UserName":"ماندی","ReqNo":"457"}', NULL, '2016-05-06 04:47:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2761, 12309, '', '{"FileName":"fin3132.pdf","UserName":"ماندی","ReqNo":"459"}', NULL, '2016-05-06 04:47:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2763, 12309, '', '{"FileName":"FNR-59-30034.pdf","UserName":"ماندی","ReqNo":"457"}', NULL, '2016-05-06 05:00:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2765, 12309, '', '{"FileName":"fin3132.pdf","UserName":"ماندی","ReqNo":"459"}', NULL, '2016-05-06 05:00:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2767, 12343, '', '{"FileName":"ترجمه_part2_part1_part1.pdf","UserName":"احسان","ReqNo":"461"}', NULL, '2016-05-06 15:10:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2769, 12343, '', '{"FileName":"ترجمه_part2_part1_part1.pdf","UserName":"احسان","ReqNo":"461"}', NULL, '2016-05-06 15:30:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2771, 12383, '', '{"FileName":"10.pdf","UserName":"محمد","ReqNo":"463"}', NULL, '2016-05-06 19:18:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2773, 12383, '', '{"FileName":"10.pdf","UserName":"محمد","ReqNo":"463"}', NULL, '2016-05-06 19:31:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2775, 12423, '', '{"FileName":"magazine.pdf","UserName":"rezvaneh","ReqNo":"465"}', NULL, '2016-05-07 00:30:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2777, 12423, '', '{"FileName":"magazine.pdf","UserName":"rezvaneh","ReqNo":"465"}', NULL, '2016-05-07 01:05:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2779, 12475, '', '{"FileName":"1-s2.0-S0149197015300573-main.pdf","UserName":"محمد","ReqNo":"467"}', NULL, '2016-05-07 15:07:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2781, 12475, '', '{"FileName":"1-s2.0-S0306454914003600-main-1.pdf","UserName":"محمد","ReqNo":"469"}', NULL, '2016-05-07 15:09:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2783, 12475, '', '{"FileName":"1-s2.0-S0306454914003600-main-2.pdf","UserName":"محمد","ReqNo":"471"}', NULL, '2016-05-07 15:10:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2785, 12475, '', '{"FileName":"1-s2.0-S1738573316300456-main.pdf","UserName":"محمد","ReqNo":"473"}', NULL, '2016-05-07 15:11:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2787, 12475, '', '{"FileName":"1-s2.0-S0149197015300573-main.pdf","UserName":"محمد","ReqNo":"475"}', NULL, '2016-05-07 15:13:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2789, 12475, '', '{"FileName":"1-s2.0-S0149197015300573-main.pdf","UserName":"محمد","ReqNo":"467"}', NULL, '2016-05-07 15:31:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2791, 12475, '', '{"FileName":"1-s2.0-S0306454914003600-main-1.pdf","UserName":"محمد","ReqNo":"469"}', NULL, '2016-05-07 15:31:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2793, 12475, '', '{"FileName":"1-s2.0-S0306454914003600-main-2.pdf","UserName":"محمد","ReqNo":"471"}', NULL, '2016-05-07 15:31:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2795, 12475, '', '{"FileName":"1-s2.0-S1738573316300456-main.pdf","UserName":"محمد","ReqNo":"473"}', NULL, '2016-05-07 15:32:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2797, 12475, '', '{"FileName":"1-s2.0-S0149197015300573-main.pdf","UserName":"محمد","ReqNo":"475"}', NULL, '2016-05-07 15:32:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2799, 12501, '', '{"FileName":"PB_CF (3).pdf","UserName":"سید مجتبی","ReqNo":"477"}', NULL, '2016-05-07 17:36:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2801, 12501, '', '{"FileName":"20160507174049.pdf","UserName":"سید مجتبی","ReqNo":"479"}', NULL, '2016-05-07 17:43:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2803, 12501, '', '{"FileName":"PB_CF (3).pdf","UserName":"سید مجتبی","ReqNo":"477"}', NULL, '2016-05-07 18:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2805, 12501, '', '{"FileName":"20160507174049.pdf","UserName":"سید مجتبی","ReqNo":"479"}', NULL, '2016-05-07 18:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2807, 12475, '', '{"FileName":"S1350-4487(01)00238-4-Measurement-of-fast-fission-factor-for-heavy-water-zero-power-reactor-HWZPR-by","UserName":"محمد","ReqNo":"481"}', NULL, '2016-05-07 21:45:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2809, 12475, '', '{"FileName":"S1350-4487(01)00238-4-Measurement-of-fast-fission-factor-for-heavy-water-zero-power-reactor-HWZPR-by","UserName":"محمد","ReqNo":"481"}', NULL, '2016-05-07 22:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2811, 12529, '', '{"FileName":"1-hardware–software partitioning-negin goudarzi.pdf","UserName":"ngo","ReqNo":"483"}', NULL, '2016-05-07 23:30:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2813, 11865, '', '{"FileName":"order-z-1462076694-1378.pdf","UserName":"افشین","ReqNo":"485"}', NULL, '2016-05-07 23:36:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2815, 12537, '', '{"FileName":"10.1016-J.BUSHOR.2014.11.008-Negotiations-between-auditors-and-their-clients-regarding-adjustments-t","UserName":"اسعد","ReqNo":"487"}', NULL, '2016-05-07 23:42:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2817, 12529, '', '{"FileName":"1-hardware–software partitioning-negin goudarzi.pdf","UserName":"ngo","ReqNo":"483"}', NULL, '2016-05-08 00:00:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2819, 11865, '', '{"FileName":"order-z-1462076694-1378.pdf","UserName":"افشین","ReqNo":"485"}', NULL, '2016-05-08 00:00:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2821, 12537, '', '{"FileName":"10.1016-J.BUSHOR.2014.11.008-Negotiations-between-auditors-and-their-clients-regarding-adjustments-t","UserName":"اسعد","ReqNo":"487"}', NULL, '2016-05-08 00:00:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2823, 12677, '', '{"FileName":"Cruise terminals site selection process An institutional analysis of the Kai Tak Cruise Terminal in ","UserName":"احمد","ReqNo":"489"}', NULL, '2016-05-08 22:54:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2825, 12677, '', '{"FileName":"Improving Logistics Management Using FoldableCollapsible Containers A Case Study.pdf","UserName":"احمد","ReqNo":"491"}', NULL, '2016-05-08 22:55:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2827, 12677, '', '{"FileName":"Cruise terminals site selection process An institutional analysis of the Kai Tak Cruise Terminal in ","UserName":"احمد","ReqNo":"489"}', NULL, '2016-05-08 23:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2829, 12677, '', '{"FileName":"Improving Logistics Management Using FoldableCollapsible Containers A Case Study.pdf","UserName":"احمد","ReqNo":"491"}', NULL, '2016-05-08 23:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2831, 12791, '', '{"FileName":"farsi.docx","UserName":"noro","ReqNo":"493"}', NULL, '2016-05-09 13:39:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2833, 12791, '', '{"FileName":"farsi.docx","UserName":"noro","ReqNo":"493"}', NULL, '2016-05-09 14:12:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2835, 12833, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"اشکان","ReqNo":"495"}', NULL, '2016-05-09 21:44:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2837, 12915, '', '{"FileName":"4_562780667285340166.pdf","UserName":"محمد","ReqNo":"497"}', NULL, '2016-05-10 12:26:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2839, 12991, '', '{"FileName":"13613_2015_Article_59.pdf","UserName":"میثم","ReqNo":"499"}', NULL, '2016-05-10 16:55:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2841, 12833, '', '{"FileName":"4_524717984208388104.doc","UserName":"اشکان","ReqNo":"501"}', NULL, '2016-05-10 17:54:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2843, 13017, '', '{"FileName":"p72-nerur.pdf","UserName":"mehrnoosh","ReqNo":"503"}', NULL, '2016-05-10 21:28:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2845, 12995, '', '{"FileName":"متن اصلی.docx","UserName":"سیما","ReqNo":"505"}', NULL, '2016-05-11 01:08:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2847, 12995, '', '{"FileName":"چکیده.docx","UserName":"سیما","ReqNo":"507"}', NULL, '2016-05-11 01:21:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2849, 13055, '', '{"FileName":"زیست‌شناسی.docx","UserName":"parviz","ReqNo":"509"}', NULL, '2016-05-11 01:52:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2851, 13103, '', '{"FileName":"SCOPE.docx","UserName":"ایمان","ReqNo":"511"}', NULL, '2016-05-11 12:01:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2853, 13103, '', '{"FileName":"operation & Maintenance Procedure.docx","UserName":"ایمان","ReqNo":"513"}', NULL, '2016-05-11 12:10:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2855, 13219, '', '{"FileName":"In the previos chapter.docx","UserName":"Zohreh","ReqNo":"515"}', NULL, '2016-05-12 12:48:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2857, 13289, '', '{"FileName":"US1881_rev016.pdf","UserName":"elnaz","ReqNo":"517"}', NULL, '2016-05-12 17:36:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2859, 13289, '', '{"FileName":"1881_014.pdf","UserName":"elnaz","ReqNo":"519"}', NULL, '2016-05-12 17:37:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2861, 13325, '', '{"FileName":"magic_number_500_CA.pdf","UserName":"mohsen","ReqNo":"521"}', NULL, '2016-05-12 20:05:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2863, 13325, '', '{"FileName":"500 filip.doc","UserName":"mohsen","ReqNo":"523"}', NULL, '2016-05-12 21:31:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2865, 13385, '', '{"FileName":"IJISSM129051448915400_2(hossein).pdf","UserName":"حسین","ReqNo":"525"}', NULL, '2016-05-13 11:59:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2867, 13401, '', '{"FileName":"1-s2.0-S0191886915005413-main.pdf","UserName":"Hossein","ReqNo":"527"}', NULL, '2016-05-13 13:12:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2869, 13413, '', '{"FileName":"DDSG_01.pdf","UserName":"abolfazl","ReqNo":"529"}', NULL, '2016-05-13 15:49:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2871, 12833, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"اشکان","ReqNo":"495"}', NULL, '2016-05-13 16:11:27', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2873, 12915, '', '{"FileName":"4_562780667285340166.pdf","UserName":"محمد","ReqNo":"497"}', NULL, '2016-05-13 16:12:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2875, 12991, '', '{"FileName":"13613_2015_Article_59.pdf","UserName":"میثم","ReqNo":"499"}', NULL, '2016-05-13 16:13:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2877, 12833, '', '{"FileName":"4_524717984208388104.doc","UserName":"اشکان","ReqNo":"501"}', NULL, '2016-05-13 16:16:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2879, 13017, '', '{"FileName":"p72-nerur.pdf","UserName":"mehrnoosh","ReqNo":"503"}', NULL, '2016-05-13 16:16:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2881, 12995, '', '{"FileName":"متن اصلی.docx","UserName":"سیما","ReqNo":"505"}', NULL, '2016-05-13 16:17:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2883, 12995, '', '{"FileName":"چکیده.docx","UserName":"سیما","ReqNo":"507"}', NULL, '2016-05-13 16:17:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2885, 13055, '', '{"FileName":"زیست‌شناسی.docx","UserName":"parviz","ReqNo":"509"}', NULL, '2016-05-13 16:18:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2887, 13103, '', '{"FileName":"SCOPE.docx","UserName":"ایمان","ReqNo":"511"}', NULL, '2016-05-13 16:18:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2889, 13103, '', '{"FileName":"operation & Maintenance Procedure.docx","UserName":"ایمان","ReqNo":"513"}', NULL, '2016-05-13 16:21:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2891, 13219, '', '{"FileName":"In the previos chapter.docx","UserName":"Zohreh","ReqNo":"515"}', NULL, '2016-05-13 16:21:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2893, 13289, '', '{"FileName":"US1881_rev016.pdf","UserName":"elnaz","ReqNo":"517"}', NULL, '2016-05-13 16:21:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2895, 13289, '', '{"FileName":"1881_014.pdf","UserName":"elnaz","ReqNo":"519"}', NULL, '2016-05-13 16:22:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2897, 13325, '', '{"FileName":"magic_number_500_CA.pdf","UserName":"mohsen","ReqNo":"521"}', NULL, '2016-05-13 16:23:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2899, 13325, '', '{"FileName":"500 filip.doc","UserName":"mohsen","ReqNo":"523"}', NULL, '2016-05-13 16:24:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2901, 13385, '', '{"FileName":"IJISSM129051448915400_2(hossein).pdf","UserName":"حسین","ReqNo":"525"}', NULL, '2016-05-13 16:25:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2903, 13401, '', '{"FileName":"1-s2.0-S0191886915005413-main.pdf","UserName":"Hossein","ReqNo":"527"}', NULL, '2016-05-13 16:27:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2905, 13413, '', '{"FileName":"DDSG_01.pdf","UserName":"abolfazl","ReqNo":"529"}', NULL, '2016-05-13 16:27:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2907, 13239, '', '{"FileName":"0b647508e61e8e5c4c60d33eb672df77.pdf","UserName":"مهدی","ReqNo":"531"}', NULL, '2016-05-14 08:37:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2909, 13239, '', '{"FileName":"0b647508e61e8e5c4c60d33eb672df77.pdf","UserName":"مهدی","ReqNo":"531"}', NULL, '2016-05-14 09:00:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2911, 13593, '', '{"FileName":"opinion-paper-armed-conflict.pdf","UserName":"اعظم","ReqNo":"533"}', NULL, '2016-05-14 18:47:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2913, 13593, '', '{"FileName":"opinion-paper-armed-conflict.pdf","UserName":"اعظم","ReqNo":"533"}', NULL, '2016-05-14 19:00:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2915, 13793, '', '{"FileName":"ترجمه.docx","UserName":"امیر","ReqNo":"535"}', NULL, '2016-05-16 01:24:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2917, 13793, '', '{"FileName":"ترجمه.docx","UserName":"امیر","ReqNo":"535"}', NULL, '2016-05-16 01:30:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2919, 9801, '', '{"FileName":"How to Burn More Fat and Build More Muscle While Playing Sports.pdf","UserName":"ایوب","ReqNo":"537"}', NULL, '2016-05-16 16:26:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2921, 9801, '', '{"FileName":"How to Burn More Fat and Build More Muscle While Playing Sports.pdf","UserName":"ایوب","ReqNo":"537"}', NULL, '2016-05-16 16:30:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2923, 13923, '', '{"FileName":"DISTRIBUTOR AGREEMENT.pdf","UserName":"Saeid","ReqNo":"539"}', NULL, '2016-05-16 20:03:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2925, 13923, '', '{"FileName":"DISTRIBUTOR AGREEMENT.pdf","UserName":"Saeid","ReqNo":"539"}', NULL, '2016-05-16 20:30:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2927, 13991, '', '{"FileName":"مقدمه ساده.pdf","UserName":"hossein","ReqNo":"541"}', NULL, '2016-05-17 08:52:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2929, 13991, '', '{"FileName":"مقدمه ساده.pdf","UserName":"hossein","ReqNo":"541"}', NULL, '2016-05-17 09:00:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2931, 14051, '', '{"FileName":"Chapter 3.docx","UserName":"سپهر","ReqNo":"543"}', NULL, '2016-05-17 14:22:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2933, 14051, '', '{"FileName":"Chapter 3.docx","UserName":"سپهر","ReqNo":"543"}', NULL, '2016-05-17 14:30:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2935, 11965, '', '{"FileName":"2.pdf","UserName":"مینا","ReqNo":"545"}', NULL, '2016-05-17 15:47:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2937, 11965, '', '{"FileName":"2.pdf","UserName":"مینا","ReqNo":"545"}', NULL, '2016-05-17 16:00:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2939, 14125, '', '{"FileName":"New PDF Document.pdf","UserName":"hojjat","ReqNo":"547"}', NULL, '2016-05-17 23:35:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2941, 14125, '', '{"FileName":"New PDF Document.pdf","UserName":"hojjat","ReqNo":"547"}', NULL, '2016-05-18 00:00:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2943, 14129, '', '{"FileName":"A&C_Black_Dictionary_of_Accounting_Over_6_000_terms_clearly_defined__2008.pdf","UserName":"عباس","ReqNo":"549"}', NULL, '2016-05-18 00:29:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2945, 14129, '', '{"FileName":"A&C_Black_Dictionary_of_Accounting_Over_6_000_terms_clearly_defined__2008.pdf","UserName":"عباس","ReqNo":"549"}', NULL, '2016-05-18 00:34:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2947, 14181, '', '{"FileName":"Making a Complaint or Criticism.docx","UserName":"غزاله","ReqNo":"551"}', NULL, '2016-05-18 15:36:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2949, 14181, '', '{"FileName":"Making a Complaint or Criticism.docx","UserName":"غزاله","ReqNo":"551"}', NULL, '2016-05-18 16:00:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2951, 14227, '', '{"FileName":"مقاله2.pdf","UserName":"محسن","ReqNo":"553"}', NULL, '2016-05-18 22:37:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2953, 14227, '', '{"FileName":"مقاله2.pdf","UserName":"محسن","ReqNo":"553"}', NULL, '2016-05-18 23:01:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2955, 14335, '', '{"FileName":"Essentials.of.Research.Design.and.Methodology.2005.pdf","UserName":"عارفه","ReqNo":"555"}', NULL, '2016-05-19 18:35:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2957, 14335, '', '{"FileName":"Essentials.of.Research.Design.and.Methodology.2005.pdf","UserName":"عارفه","ReqNo":"555"}', NULL, '2016-05-19 19:01:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2959, 14377, '', '{"FileName":"makinggames.pdf","UserName":"محمدرضا","ReqNo":"557"}', NULL, '2016-05-19 23:14:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2961, 14385, '', '{"FileName":"Predicting multicultural effectiveness of.pdf","UserName":"نادیا","ReqNo":"559"}', NULL, '2016-05-19 23:15:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2963, 14377, '', '{"FileName":"makinggames.pdf","UserName":"محمدرضا","ReqNo":"557"}', NULL, '2016-05-19 23:31:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2965, 14385, '', '{"FileName":"Predicting multicultural effectiveness of.pdf","UserName":"نادیا","ReqNo":"559"}', NULL, '2016-05-19 23:34:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2967, 14385, '', '{"FileName":"3- Training Japanese listeners to identify English.docx","UserName":"نادیا","ReqNo":"561"}', NULL, '2016-05-19 23:59:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2969, 14385, '', '{"FileName":"3- Training Japanese listeners to identify English.docx","UserName":"نادیا","ReqNo":"561"}', NULL, '2016-05-20 00:01:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2971, 14411, '', '{"FileName":"kerio-operator-stepbystep-en-2.2.6-3042.docx","UserName":"مهدی","ReqNo":"563"}', NULL, '2016-05-20 10:42:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2973, 14411, '', '{"FileName":"kerio-operator-adminguide-en-2.4.7-5666-p1.docx","UserName":"مهدی","ReqNo":"565"}', NULL, '2016-05-20 10:43:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2975, 14411, '', '{"FileName":"kerio-operator-stepbystep-en-2.2.6-3042.docx","UserName":"مهدی","ReqNo":"563"}', NULL, '2016-05-20 11:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2977, 14411, '', '{"FileName":"kerio-operator-adminguide-en-2.4.7-5666-p1.docx","UserName":"مهدی","ReqNo":"565"}', NULL, '2016-05-20 11:03:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2979, 14463, '', '{"FileName":"Library_7p.pdf","UserName":"masoud","ReqNo":"567"}', NULL, '2016-05-20 20:10:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2981, 14463, '', '{"FileName":"Library_7p.pdf","UserName":"masoud","ReqNo":"567"}', NULL, '2016-05-20 20:30:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(2983, 14465, '', '{"FileName":"history of organazation theory.pdf","UserName":"Ali","ReqNo":"569"}', NULL, '2016-05-20 20:34:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2985, 14465, '', '{"FileName":"history of organazation theory.pdf","UserName":"Ali","ReqNo":"569"}', NULL, '2016-05-20 21:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2987, 14385, '', '{"FileName":"International Journal of Intercultural Relations 26.docx","UserName":"نادیا","ReqNo":"571"}', NULL, '2016-05-20 22:55:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2989, 14385, '', '{"FileName":"International Journal of Intercultural Relations 26.docx","UserName":"نادیا","ReqNo":"573"}', NULL, '2016-05-20 22:56:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2991, 14385, '', '{"FileName":"International Journal of Intercultural Relations 26.docx","UserName":"نادیا","ReqNo":"571"}', NULL, '2016-05-20 23:00:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2993, 14385, '', '{"FileName":"International Journal of Intercultural Relations 26.docx","UserName":"نادیا","ReqNo":"573"}', NULL, '2016-05-20 23:00:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2995, 14385, '', '{"FileName":"International Journal of Intercultural Relations 26.docx","UserName":"نادیا","ReqNo":"575"}', NULL, '2016-05-20 23:06:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(2997, 14385, '', '{"FileName":"International Journal of Intercultural Relations 26.docx","UserName":"نادیا","ReqNo":"575"}', NULL, '2016-05-20 23:30:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(2999, 14499, '', '{"FileName":"mccarthy2012.pdf","UserName":"mehran","ReqNo":"577"}', NULL, '2016-05-21 03:20:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3001, 14499, '', '{"FileName":"mccarthy2012.pdf","UserName":"mehran","ReqNo":"577"}', NULL, '2016-05-21 03:30:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3003, 14463, '', '{"FileName":"Library_7p.pdf","UserName":"masoud","ReqNo":"579"}', NULL, '2016-05-21 09:58:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3005, 14463, '', '{"FileName":"Library_7p.pdf","UserName":"masoud","ReqNo":"579"}', NULL, '2016-05-21 10:00:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3007, 14529, '', '{"FileName":"101016jpolymer201311003.pdf","UserName":"احسان","ReqNo":"581"}', NULL, '2016-05-21 11:53:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3009, 14529, '', '{"FileName":"101016jpolymer201411006.pdf","UserName":"احسان","ReqNo":"583"}', NULL, '2016-05-21 11:56:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3011, 14529, '', '{"FileName":"Multi-Wall-Carbon-Nanotubes-Styrene-Butadiene-Rubber-SBR-Nanocomposite.pdf","UserName":"احسان","ReqNo":"585"}', NULL, '2016-05-21 11:58:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3013, 14529, '', '{"FileName":"101016jclay201507009.pdf","UserName":"احسان","ReqNo":"587"}', NULL, '2016-05-21 11:59:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3015, 14529, '', '{"FileName":"101016jpolymer201311003.pdf","UserName":"احسان","ReqNo":"581"}', NULL, '2016-05-21 12:00:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3017, 14529, '', '{"FileName":"101016jpolymer201411006.pdf","UserName":"احسان","ReqNo":"583"}', NULL, '2016-05-21 12:00:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3019, 14529, '', '{"FileName":"Multi-Wall-Carbon-Nanotubes-Styrene-Butadiene-Rubber-SBR-Nanocomposite.pdf","UserName":"احسان","ReqNo":"585"}', NULL, '2016-05-21 12:00:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3021, 14529, '', '{"FileName":"101016jclay201507009.pdf","UserName":"احسان","ReqNo":"587"}', NULL, '2016-05-21 12:02:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3023, 14529, '', '{"FileName":"101016jclay201507009.pdf","UserName":"احسان","ReqNo":"589"}', NULL, '2016-05-21 13:07:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3025, 14529, '', '{"FileName":"101016jclay201507009.pdf","UserName":"احسان","ReqNo":"589"}', NULL, '2016-05-21 13:31:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3027, 14529, '', '{"FileName":"101016jpolymer201311003.pdf","UserName":"احسان","ReqNo":"591"}', NULL, '2016-05-21 17:10:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3029, 14529, '', '{"FileName":"101016jpolymer201411006.pdf","UserName":"احسان","ReqNo":"593"}', NULL, '2016-05-21 17:12:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3031, 14529, '', '{"FileName":"101016jpolymer201311003.pdf","UserName":"احسان","ReqNo":"591"}', NULL, '2016-05-21 17:30:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3033, 14529, '', '{"FileName":"101016jpolymer201411006.pdf","UserName":"احسان","ReqNo":"593"}', NULL, '2016-05-21 17:30:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3035, 14385, '', '{"FileName":"A first report-Training Japanese listeners to identify English.docx","UserName":"نادیا","ReqNo":"595"}', NULL, '2016-05-21 22:40:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3037, 14385, '', '{"FileName":"A first report-Training Japanese listeners to identify English.docx","UserName":"نادیا","ReqNo":"595"}', NULL, '2016-05-21 23:00:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3039, 14499, '', '{"FileName":"mccarthy2012.translated.pdf","UserName":"mehran","ReqNo":"597"}', NULL, '2016-05-22 02:32:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3041, 14499, '', '{"FileName":"mccarthy2012.translated.pdf","UserName":"mehran","ReqNo":"597"}', NULL, '2016-05-22 03:00:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3043, 14633, '', '{"FileName":"Final.docx","UserName":"فرهود","ReqNo":"599"}', NULL, '2016-05-22 03:46:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3045, 14633, '', '{"FileName":"Final.docx","UserName":"فرهود","ReqNo":"599"}', NULL, '2016-05-22 04:01:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3047, 14633, '', '{"FileName":"Final.docx","UserName":"فرهود","ReqNo":"601"}', NULL, '2016-05-22 04:40:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3049, 14633, '', '{"FileName":"Final.docx","UserName":"فرهود","ReqNo":"601"}', NULL, '2016-05-22 05:00:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3051, 14563, '', '{"FileName":"Abstract.pdf","UserName":"مهدي","ReqNo":"603"}', NULL, '2016-05-22 13:12:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3053, 14563, '', '{"FileName":"Abstract.pdf","UserName":"مهدي","ReqNo":"603"}', NULL, '2016-05-22 13:30:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3055, 14679, '', '{"FileName":"A multi-phase approach for supply chain design with product life cycle considerations.pdf","UserName":"فرشاد","ReqNo":"605"}', NULL, '2016-05-22 14:28:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3057, 14679, '', '{"FileName":"A multi-phase approach for supply chain design with product life cycle considerations.pdf","UserName":"فرشاد","ReqNo":"605"}', NULL, '2016-05-22 14:30:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3059, 14693, '', '{"FileName":"4536-Instant-Messaging.pdf","UserName":"مائده","ReqNo":"607"}', NULL, '2016-05-22 16:26:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3061, 14693, '', '{"FileName":"4536-Instant-Messaging.pdf","UserName":"مائده","ReqNo":"607"}', NULL, '2016-05-22 16:30:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3063, 14807, '', '{"FileName":"tmp_12553-Emailing 10.1016_J.PMCJ.2014.09.006-A-utility-paradigm-for-IoT-The-sensing-Cloud-113867067","UserName":"محسن","ReqNo":"609"}', NULL, '2016-05-23 19:27:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3065, 14807, '', '{"FileName":"tmp_12553-Emailing 10.1016_J.PMCJ.2014.09.006-A-utility-paradigm-for-IoT-The-sensing-Cloud-113867067","UserName":"محسن","ReqNo":"609"}', NULL, '2016-05-23 19:30:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3067, 14871, '', '{"FileName":"SCAME MANUAL JADID.pdf","UserName":"میثم","ReqNo":"611"}', NULL, '2016-05-24 14:40:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3069, 14871, '', '{"FileName":"SCAME MANUAL JADID.pdf","UserName":"میثم","ReqNo":"611"}', NULL, '2016-05-24 15:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3071, 14923, '', '{"FileName":"10.1016_j.jcsr.2015.02.013-Effects-of-floor-slab-on-progressive-collapse-resistance-of-steel-moment-","UserName":"saeid","ReqNo":"613"}', NULL, '2016-05-24 19:41:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3073, 14923, '', '{"FileName":"10.1016_j.jcsr.2015.02.013-Effects-of-floor-slab-on-progressive-collapse-resistance-of-steel-moment-","UserName":"saeid","ReqNo":"615"}', NULL, '2016-05-24 19:47:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3075, 14923, '', '{"FileName":"10.1016_j.jcsr.2015.02.013-Effects-of-floor-slab-on-progressive-collapse-resistance-of-steel-moment-","UserName":"saeid","ReqNo":"613"}', NULL, '2016-05-24 20:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3077, 14923, '', '{"FileName":"10.1016_j.jcsr.2015.02.013-Effects-of-floor-slab-on-progressive-collapse-resistance-of-steel-moment-","UserName":"saeid","ReqNo":"615"}', NULL, '2016-05-24 20:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3079, 14563, '', '{"FileName":"In this paper.pdf","UserName":"مهدي","ReqNo":"617"}', NULL, '2016-05-25 01:41:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3081, 14563, '', '{"FileName":"In this paper.pdf","UserName":"مهدي","ReqNo":"617"}', NULL, '2016-05-25 02:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3083, 14807, '', '{"FileName":"tmp_8923-Emailing 10.1016_J.PMCJ.2014.09.006-A-utility-paradigm-for-IoT-The-sensing-Cloud1869178662.","UserName":"محسن","ReqNo":"619"}', NULL, '2016-05-25 14:51:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3085, 14807, '', '{"FileName":"tmp_8923-Emailing 10.1016_J.PMCJ.2014.09.006-A-utility-paradigm-for-IoT-The-sensing-Cloud1869178662.","UserName":"محسن","ReqNo":"619"}', NULL, '2016-05-25 15:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3087, 15029, '', '{"FileName":"[Elearnica] -Human_action_recognition_using_genetic_algorithms_and_convolutional_neural_.pdf","UserName":"balal","ReqNo":"621"}', NULL, '2016-05-25 21:25:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3089, 15029, '', '{"FileName":"[Elearnica] -Human_action_recognition_using_genetic_algorithms_and_convolutional_neural_.pdf","UserName":"balal","ReqNo":"621"}', NULL, '2016-05-25 21:30:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3091, 15029, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"balal","ReqNo":"623"}', NULL, '2016-05-25 23:17:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3093, 15029, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"balal","ReqNo":"623"}', NULL, '2016-05-25 23:30:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3095, 15059, '', '{"FileName":"houston2003.doc","UserName":"حمید","ReqNo":"625"}', NULL, '2016-05-26 09:59:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3097, 15059, '', '{"FileName":"houston2003.doc","UserName":"حمید","ReqNo":"625"}', NULL, '2016-05-26 10:01:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3099, 15125, '', '{"FileName":"ebook3101(www.takbook.com).pdf","UserName":"پویا","ReqNo":"627"}', NULL, '2016-05-27 10:17:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3101, 15129, '', '{"FileName":"seed priming a technique.pdf","UserName":"بهرام","ReqNo":"629"}', NULL, '2016-05-27 11:02:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3103, 15133, '', '{"FileName":"16_CybersecurityStrategyoftheEur - Copy.docx","UserName":"ایمان","ReqNo":"631"}', NULL, '2016-05-27 11:55:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3105, 14807, '', '{"FileName":"Mobiles.docx","UserName":"محسن","ReqNo":"633"}', NULL, '2016-05-27 14:00:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3107, 15161, '', '{"FileName":"IFCGEM+Brochure+sec+5-6.pdf","UserName":"Meysam","ReqNo":"635"}', NULL, '2016-05-27 17:04:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3109, 2081, '', '{"FileName":"30.docx","UserName":"alireza","ReqNo":"637"}', NULL, '2016-05-28 03:50:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3111, 2081, '', '{"FileName":"30.docx","UserName":"alireza","ReqNo":"639"}', NULL, '2016-05-28 03:52:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3113, 15291, '', '{"FileName":"DBSCAN(4).pdf","UserName":"ابوذر","ReqNo":"641"}', NULL, '2016-05-28 16:34:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3115, 15315, '', '{"FileName":"02-23 Omitiran_2.pdf","UserName":"حامد","ReqNo":"643"}', NULL, '2016-05-28 19:19:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3117, 15433, '', '{"FileName":"1-s2.0-S187704281502354X-main(1).pdf","UserName":"سمانه","ReqNo":"645"}', NULL, '2016-05-29 17:13:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3119, 15541, '', '{"FileName":"paper - Copy.doc","UserName":"najja","ReqNo":"647"}', NULL, '2016-05-30 09:53:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3121, 15603, '', '{"FileName":"4_294606392541577248.pdf","UserName":"farid","ReqNo":"649"}', NULL, '2016-05-30 17:33:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3123, 15091, '', '{"FileName":"[Reza_Hasmath]_Managing_Ethnic_Diversity_(Research(BookZZ.org).pdf","UserName":"mohsen","ReqNo":"651"}', NULL, '2016-05-30 23:00:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3125, 15641, '', '{"FileName":"مقاله 1.pdf","UserName":"مونا","ReqNo":"653"}', NULL, '2016-05-31 02:06:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3127, 15433, '', '{"FileName":"1-s2.0-S187704281502354X-main(1).pdf","UserName":"سمانه","ReqNo":"655"}', NULL, '2016-05-31 08:48:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3129, 15717, '', '{"FileName":"Tarjomeiran.com_ProjectFile_27862_swoJgVkBp0X7.pdf","UserName":"علی رضا","ReqNo":"657"}', NULL, '2016-05-31 18:17:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3131, 15717, '', '{"FileName":"Tarjomeiran.com_ProjectFile_20981_eimL7NXSWrHd.pdf","UserName":"علی رضا","ReqNo":"659"}', NULL, '2016-05-31 18:23:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3133, 15743, '', '{"FileName":"1-s2.0-S0720048X12000903-main.pdf","UserName":"elnaz","ReqNo":"661"}', NULL, '2016-06-01 00:33:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3135, 15743, '', '{"FileName":"1-s2.0-S0169500211004211-main.pdf","UserName":"elnaz","ReqNo":"663"}', NULL, '2016-06-01 00:36:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3137, 15125, '', '{"FileName":"ebook3101(www.takbook.com).pdf","UserName":"پویا","ReqNo":"627"}', NULL, '2016-06-01 08:33:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3139, 15129, '', '{"FileName":"seed priming a technique.pdf","UserName":"بهرام","ReqNo":"629"}', NULL, '2016-06-01 08:35:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3141, 15133, '', '{"FileName":"16_CybersecurityStrategyoftheEur - Copy.docx","UserName":"ایمان","ReqNo":"631"}', NULL, '2016-06-01 08:36:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3143, 14807, '', '{"FileName":"Mobiles.docx","UserName":"محسن","ReqNo":"633"}', NULL, '2016-06-01 08:40:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3145, 15161, '', '{"FileName":"IFCGEM+Brochure+sec+5-6.pdf","UserName":"Meysam","ReqNo":"635"}', NULL, '2016-06-01 08:40:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3147, 2081, '', '{"FileName":"30.docx","UserName":"alireza","ReqNo":"637"}', NULL, '2016-06-01 08:46:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3149, 2081, '', '{"FileName":"30.docx","UserName":"alireza","ReqNo":"639"}', NULL, '2016-06-01 08:51:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3151, 15291, '', '{"FileName":"DBSCAN(4).pdf","UserName":"ابوذر","ReqNo":"641"}', NULL, '2016-06-01 08:52:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3153, 15315, '', '{"FileName":"02-23 Omitiran_2.pdf","UserName":"حامد","ReqNo":"643"}', NULL, '2016-06-01 08:52:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3155, 15433, '', '{"FileName":"1-s2.0-S187704281502354X-main(1).pdf","UserName":"سمانه","ReqNo":"645"}', NULL, '2016-06-01 08:53:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3157, 15541, '', '{"FileName":"paper - Copy.doc","UserName":"najja","ReqNo":"647"}', NULL, '2016-06-01 08:53:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3159, 15603, '', '{"FileName":"4_294606392541577248.pdf","UserName":"farid","ReqNo":"649"}', NULL, '2016-06-01 08:55:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3161, 15091, '', '{"FileName":"[Reza_Hasmath]_Managing_Ethnic_Diversity_(Research(BookZZ.org).pdf","UserName":"mohsen","ReqNo":"651"}', NULL, '2016-06-01 09:05:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3163, 15641, '', '{"FileName":"مقاله 1.pdf","UserName":"مونا","ReqNo":"653"}', NULL, '2016-06-01 09:07:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3165, 15433, '', '{"FileName":"1-s2.0-S187704281502354X-main(1).pdf","UserName":"سمانه","ReqNo":"655"}', NULL, '2016-06-01 09:08:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3167, 15717, '', '{"FileName":"Tarjomeiran.com_ProjectFile_27862_swoJgVkBp0X7.pdf","UserName":"علی رضا","ReqNo":"657"}', NULL, '2016-06-01 09:08:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3169, 15717, '', '{"FileName":"Tarjomeiran.com_ProjectFile_20981_eimL7NXSWrHd.pdf","UserName":"علی رضا","ReqNo":"659"}', NULL, '2016-06-01 09:08:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3171, 15743, '', '{"FileName":"1-s2.0-S0720048X12000903-main.pdf","UserName":"elnaz","ReqNo":"661"}', NULL, '2016-06-01 09:09:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3173, 15743, '', '{"FileName":"1-s2.0-S0169500211004211-main.pdf","UserName":"elnaz","ReqNo":"663"}', NULL, '2016-06-01 09:10:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3175, 15785, '', '{"FileName":"New Doc 3.pdf","UserName":"کیوان","ReqNo":"665"}', NULL, '2016-06-01 12:09:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3177, 15785, '', '{"FileName":"New Doc 4.pdf","UserName":"کیوان","ReqNo":"667"}', NULL, '2016-06-01 12:20:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3179, 15785, '', '{"FileName":"New Doc 3.pdf","UserName":"کیوان","ReqNo":"665"}', NULL, '2016-06-01 12:30:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3181, 15785, '', '{"FileName":"New Doc 4.pdf","UserName":"کیوان","ReqNo":"667"}', NULL, '2016-06-01 12:30:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3183, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-01 19:31:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3185, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-01 19:34:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3187, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-01 19:47:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3189, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-01 20:01:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3191, 15717, '', '{"FileName":"Untitleddocument.pdf","UserName":"علی رضا","ReqNo":"671"}', NULL, '2016-06-01 20:50:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3193, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 10:53:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3195, 15717, '', '{"FileName":"Untitleddocument.pdf","UserName":"علی رضا","ReqNo":"671"}', NULL, '2016-06-02 10:53:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3197, 15717, '', '{"FileName":"Untitleddocument.pdf","UserName":"علی رضا","ReqNo":"671"}', NULL, '2016-06-02 10:54:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3199, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 10:57:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3201, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 11:01:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3203, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 11:12:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3205, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 11:13:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3207, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 11:18:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3209, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 11:23:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3211, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-02 11:33:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3213, 2, '', '{"FileName":"نامه اختلال در ترگمان.docx","UserName":"پیمان","ReqNo":"5"}', NULL, '2016-06-02 11:34:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3215, 1411, '', '{"FileName":"نظریه اختلاف.docx","UserName":"عطیه","ReqNo":"7"}', NULL, '2016-06-02 11:34:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3217, 3873, '', '{"FileName":"SAMA.pdf","UserName":"mahdi","ReqNo":"9"}', NULL, '2016-06-02 11:34:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3219, 3921, '', '{"FileName":"1.PDF","UserName":"sajad","ReqNo":"13"}', NULL, '2016-06-02 11:34:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3221, 3929, '', '{"FileName":"تا کنون مطالعات آزمایشگاهی بسیار زیادی برای بررسی تغییر ترشوندگی.docx","UserName":"رضا","ReqNo":"17"}', NULL, '2016-06-02 11:34:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3223, 3967, '', '{"FileName":"hhhhhhhhhh.docx","UserName":"سیدحسین","ReqNo":"19"}', NULL, '2016-06-02 11:35:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3225, 3759, '', '{"FileName":"AIAA-2011-3591.pdf","UserName":"حمید","ReqNo":"25"}', NULL, '2016-06-02 11:35:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3227, 19, '', '{"FileName":"TSD-TLG-SAR-001-v01.pdf","UserName":"سید محمد","ReqNo":"27"}', NULL, '2016-06-02 11:35:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3229, 4487, '', '{"FileName":"پستانداران در ايران.docx","UserName":"مهدیه","ReqNo":"35"}', NULL, '2016-06-02 11:36:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3231, 4543, '', '{"FileName":"MMF-14.pdf","UserName":"محمدرضا","ReqNo":"37"}', NULL, '2016-06-02 11:38:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3233, 4617, '', '{"FileName":"متن قانون اساسی بوتسوانا با اصلاحات 2006.pdf","UserName":"حامد","ReqNo":"41"}', NULL, '2016-06-02 11:50:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3235, 4727, '', '{"FileName":"ccr-9501-clark.pdf","UserName":"محمد حسن","ReqNo":"47"}', NULL, '2016-06-02 11:54:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3237, 4879, '', '{"FileName":"10.1016-J.IJMACHTOOLS.2006.07.006-Mechanism_of_improvement_of_formability_in_pulsating_hydroforming_","UserName":"محمد","ReqNo":"57"}', NULL, '2016-06-02 11:54:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3239, 4879, '', '{"FileName":"10.1016-J.IJMACHTOOLS.2006.07.006-Mechanism_of_improvement_of_formability_in_pulsating_hydroforming_","UserName":"محمد","ReqNo":"59"}', NULL, '2016-06-02 11:54:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3241, 4947, '', '{"FileName":"10.1016_J.JFINECO.2015.08.010-Assessing-Asset-Pricing-Models-using-Revealed-Preference.pdf","UserName":"جمشید","ReqNo":"63"}', NULL, '2016-06-02 11:54:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3243, 4989, '', '{"FileName":"10.1016_J.ECOLIND.2015.06.022-Integrated-municipal-waste-management-systems-An-indicator-to-assess-t","UserName":"HASAN","ReqNo":"65"}', NULL, '2016-06-02 11:54:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3245, 5011, '', '{"FileName":"ji2015.pdf","UserName":"محمد","ReqNo":"67"}', NULL, '2016-06-02 11:54:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3247, 4989, '', '{"FileName":"10.1016_J.ECOLIND.2015.06.022-Integrated-municipal-waste-management-systems-An-indicator-to-assess-t","UserName":"HASAN","ReqNo":"71"}', NULL, '2016-06-02 11:54:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3249, 4989, '', '{"FileName":"Integrated municipal waste management systems.docx","UserName":"HASAN","ReqNo":"73"}', NULL, '2016-06-02 11:57:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3251, 5305, '', '{"FileName":"5965-7707E (2).pdf","UserName":"amir","ReqNo":"81"}', NULL, '2016-06-02 11:57:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3253, 6209, '', '{"FileName":"1.pdf","UserName":"nasrin","ReqNo":"85"}', NULL, '2016-06-02 11:59:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3255, 6229, '', '{"FileName":"1.docx","UserName":"ma","ReqNo":"87"}', NULL, '2016-06-02 12:01:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3257, 6241, '', '{"FileName":"اعلامیه ی جهانی آموزش برای همه.pdf","UserName":"زینب السادات","ReqNo":"89"}', NULL, '2016-06-02 12:10:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3259, 4947, '', '{"FileName":"Author.docx","UserName":"جمشید","ReqNo":"91"}', NULL, '2016-06-02 12:25:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3261, 3013, '', '{"FileName":"111.pdf","UserName":"hossein","ReqNo":"93"}', NULL, '2016-06-02 12:28:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3263, 3013, '', '{"FileName":"2222.pdf","UserName":"hossein","ReqNo":"95"}', NULL, '2016-06-02 12:29:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3265, 6191, '', '{"FileName":"GSM+Different kind of performance.pdf","UserName":"فاطمه","ReqNo":"97"}', NULL, '2016-06-02 12:34:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3267, 6191, '', '{"FileName":"[elearnica.ir]-Impact_of_lean_manufacturing_and_environmental_management_on_business_perfo.pdf","UserName":"فاطمه","ReqNo":"99"}', NULL, '2016-06-02 12:37:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3269, 6283, '', '{"FileName":"مدیریت بحران شهری -- لاتین.pdf","UserName":"علیرضا","ReqNo":"101"}', NULL, '2016-06-02 12:39:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3271, 6283, '', '{"FileName":"مقدمه ای بر سیمولیشن شهری  -- لاتین.pdf","UserName":"علیرضا","ReqNo":"103"}', NULL, '2016-06-02 12:39:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3273, 6359, '', '{"FileName":"ADSQCD.pdf","UserName":"مهرداد","ReqNo":"105"}', NULL, '2016-06-02 12:41:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3275, 6455, '', '{"FileName":"30.docx","UserName":"سهراب","ReqNo":"107"}', NULL, '2016-06-02 12:47:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3277, 6537, '', '{"FileName":"PDF_20160308_085454 (1).pdf","UserName":"مونا","ReqNo":"109"}', NULL, '2016-06-02 12:47:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3279, 6537, '', '{"FileName":"PDF_20160308_085454 (1).pdf","UserName":"مونا","ReqNo":"111"}', NULL, '2016-06-02 12:47:10', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3281, 6547, '', '{"FileName":"B4ACodeSnippets.docx","UserName":"محمد","ReqNo":"113"}', NULL, '2016-06-02 12:47:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3283, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"115"}', NULL, '2016-06-02 12:57:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3285, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"117"}', NULL, '2016-06-02 13:04:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3287, 6631, '', '{"FileName":"standards-of-zoo-practice.pdf","UserName":"mohammadjavad","ReqNo":"119"}', NULL, '2016-06-02 13:15:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3289, 6651, '', '{"FileName":"msl_2015_81.pdf","UserName":"سیمین","ReqNo":"121"}', NULL, '2016-06-02 13:16:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3291, 6651, '', '{"FileName":"admsci-05-00090.pdf","UserName":"سیمین","ReqNo":"123"}', NULL, '2016-06-02 13:16:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3293, 3887, '', '{"FileName":"FCO NO JEO and SON COMPANY Ltd-1.pdf","UserName":"محمد","ReqNo":"127"}', NULL, '2016-06-02 13:16:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3295, 3887, '', '{"FileName":"پروسیجر طلای سوییس 200 تن.pdf","UserName":"محمد","ReqNo":"133"}', NULL, '2016-06-02 13:16:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3297, 7169, '', '{"FileName":"تئوری2.pdf","UserName":"عابد","ReqNo":"139"}', NULL, '2016-06-02 13:16:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3299, 7169, '', '{"FileName":"4_333521983109070869.pdf","UserName":"عابد","ReqNo":"141"}', NULL, '2016-06-02 13:17:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3301, 7209, '', '{"FileName":"We Are What We Quote.docx","UserName":"مریم","ReqNo":"143"}', NULL, '2016-06-02 13:17:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3303, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"145"}', NULL, '2016-06-02 13:17:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3305, 7387, '', '{"FileName":"1-Effects_of_organic_additives_on_crystallization_process_and_the_adsorption_.pdf","UserName":"سجاد","ReqNo":"147"}', NULL, '2016-06-02 13:18:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3307, 7397, '', '{"FileName":"Untitled.docx","UserName":"amirhosein","ReqNo":"149"}', NULL, '2016-06-02 13:18:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3309, 7407, '', '{"FileName":"36_D02_Pages_308_329 (22).pdf","UserName":"روژین","ReqNo":"151"}', NULL, '2016-06-02 13:18:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3311, 7327, '', '{"FileName":"ieeebeaca90e-6040-20160109111420.pdf","UserName":"hamed","ReqNo":"153"}', NULL, '2016-06-02 13:18:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3313, 7443, '', '{"FileName":"soa1.pdf","UserName":"amir","ReqNo":"155"}', NULL, '2016-06-02 13:18:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3315, 7527, '', '{"FileName":"Figure 18.docx","UserName":"ali","ReqNo":"159"}', NULL, '2016-06-02 13:18:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3317, 7535, '', '{"FileName":"Analog CMOS Realization of Fuzzy Logic Membership  Functions.pdf","UserName":"محسن","ReqNo":"161"}', NULL, '2016-06-02 13:18:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3319, 7585, '', '{"FileName":"unit 8.docx","UserName":"احمد","ReqNo":"163"}', NULL, '2016-06-02 13:18:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3321, 7341, '', '{"FileName":"1750110.pdf","UserName":"علی","ReqNo":"165"}', NULL, '2016-06-02 13:19:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3323, 7341, '', '{"FileName":"10.1007_s40565-015-0168-1.pdf","UserName":"علی","ReqNo":"169"}', NULL, '2016-06-02 13:19:40', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3325, 7341, '', '{"FileName":"10.1109-IECON.2013.6699440-Consensus-based-approach-for-economic-dispatch-problem-in-a-smart-grid.pd","UserName":"علی","ReqNo":"171"}', NULL, '2016-06-02 13:19:40', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3327, 7341, '', '{"FileName":"10.1109-JSAC.2012.120705-Decentralized-Economic-Dispatch-in-Microgrids-via-Heterogeneous-Wireless-Ne","UserName":"علی","ReqNo":"173"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3329, 7341, '', '{"FileName":"10.1109-TAC.2004.834113-Consensus-problems-in-networks-of-agents-with-switching-topology-and-time-de","UserName":"علی","ReqNo":"177"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3331, 7341, '', '{"FileName":"10.1109-TDC.2005.1546751-Economic-Dispatch-of-Power-Systems-Based-on-the-Modified-Particle-Swarm-Opt","UserName":"علی","ReqNo":"179"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3333, 7341, '', '{"FileName":"10.1109-TII.2013.2287807-A-Distributed-Auction-Based-Algorithm-for-the-Non-Convex-Economic-Dispatch-","UserName":"علی","ReqNo":"183"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3335, 7341, '', '{"FileName":"10.1109-TPEL.2011.2175750-Generalized-Frequency-Domain-Formulation-of-the-Switching-Frequency-for-Hy","UserName":"علی","ReqNo":"185"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3337, 7341, '', '{"FileName":"10.1109-TPWRS.2003.814889-Particle-swarm-optimization-to-solving-the-economic-dispatch-considering-t","UserName":"علی","ReqNo":"187"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3339, 7341, '', '{"FileName":"10.1109-TPWRS.2005.857939-Taguchi-method-for-solving-the-economic-dispatch-problem-with-nonsmooth-co","UserName":"علی","ReqNo":"189"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3341, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"191"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3343, 7341, '', '{"FileName":"10.1109-TPWRS.2012.2188912-Convergence-Analysis-of-the-Incremental-Cost-Consensus-Algorithm-Under-Di","UserName":"علی","ReqNo":"193"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3345, 7341, '', '{"FileName":"10.1109-TPWRS.2013.2273539-Optimal-Allocation-of-HTS-FCL-for-Power-System-Security-and-Stability-Enh","UserName":"علی","ReqNo":"195"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3347, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"197"}', NULL, '2016-06-02 13:19:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3349, 7341, '', '{"FileName":"10.1109-TSG.2010.2057458-Economic-Load-Dispatch-Constrained-by-Wind-Power-Availability-A-Wait-and-Se","UserName":"علی","ReqNo":"199"}', NULL, '2016-06-02 13:19:42', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3351, 7341, '', '{"FileName":"10.1109-TSG.2012.2233773-Power-Scheduling-of-Distributed-Generators-for-Economic-and-Stable-Operatio","UserName":"علی","ReqNo":"201"}', NULL, '2016-06-02 13:19:42', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3353, 7313, '', '{"FileName":"Institutions1.pdf","UserName":"sajad","ReqNo":"203"}', NULL, '2016-06-02 13:20:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3355, 7313, '', '{"FileName":"Institutions2.pdf","UserName":"sajad","ReqNo":"205"}', NULL, '2016-06-02 13:20:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3357, 7341, '', '{"FileName":"10.1109-TSMC.2013.2258909-Decentralized-Economic-Dispatch-in-Smart-Grids-by-Self-Organizing-Dynamic-","UserName":"علی","ReqNo":"209"}', NULL, '2016-06-02 13:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3359, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"211"}', NULL, '2016-06-02 13:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3361, 7689, '', '{"FileName":"تعامل  همسر.pdf","UserName":"طاهره","ReqNo":"213"}', NULL, '2016-06-02 13:20:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3363, 7689, '', '{"FileName":"تنوع جغرافیایی و دو و سه فرزندی.pdf","UserName":"طاهره","ReqNo":"215"}', NULL, '2016-06-02 13:24:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3365, 7689, '', '{"FileName":"تفاوت در تمایلات باروری .انگلیس.pdf","UserName":"طاهره","ReqNo":"217"}', NULL, '2016-06-02 13:27:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3367, 7711, '', '{"FileName":"10.1007_s12666-015-0723-5-Indian-China-Clays-Vagaries-in-Process-and-Flow-Sheet-Development.pdf","UserName":"majid","ReqNo":"219"}', NULL, '2016-06-02 13:29:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3369, 7785, '', '{"FileName":"5.pdf","UserName":"سمیره","ReqNo":"221"}', NULL, '2016-06-02 13:33:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3371, 7995, '', '{"FileName":"نمونه سوالDoc1.docx","UserName":"جواد","ReqNo":"225"}', NULL, '2016-06-02 13:33:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3373, 4271, '', '{"FileName":"اب.doc","UserName":"مهناز","ReqNo":"229"}', NULL, '2016-06-02 13:37:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3375, 8721, '', '{"FileName":"Hoshin Kanri.docx","UserName":"احسان","ReqNo":"231"}', NULL, '2016-06-02 13:38:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3377, 8721, '', '{"FileName":"What is project efficiency and effectiveness.docx","UserName":"احسان","ReqNo":"233"}', NULL, '2016-06-02 13:39:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3379, 9041, '', '{"FileName":"talent.docx","UserName":"یونس","ReqNo":"235"}', NULL, '2016-06-02 13:41:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3381, 9059, '', '{"FileName":"subcont.pdf","UserName":"میثم","ReqNo":"237"}', NULL, '2016-06-02 13:44:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3383, 4947, '', '{"FileName":"عرضه اولیه.docx","UserName":"جمشید","ReqNo":"239"}', NULL, '2016-06-02 13:45:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3385, 4947, '', '{"FileName":"شرایط شرکت های موجود.docx","UserName":"جمشید","ReqNo":"241"}', NULL, '2016-06-02 13:47:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3387, 4947, '', '{"FileName":"شرایط شرکت های درلیست خروج.docx","UserName":"جمشید","ReqNo":"243"}', NULL, '2016-06-02 13:48:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3389, 9181, '', '{"FileName":"Document2.docx","UserName":"جواد","ReqNo":"245"}', NULL, '2016-06-02 13:50:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3391, 9259, '', '{"FileName":"exchang  marriage.PDF","UserName":"kkk","ReqNo":"247"}', NULL, '2016-06-02 13:50:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3393, 9309, '', '{"FileName":"69-129-1-SM - Copy.pdf","UserName":"حسین","ReqNo":"249"}', NULL, '2016-06-02 13:55:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3395, 15907, '', '{"FileName":"DISTRIBUTION AGREEMENT INDUSTRY_GA.docx","UserName":"محمدرضا","ReqNo":"673"}', NULL, '2016-06-02 13:57:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3397, 9293, '', '{"FileName":"ifrs2_en.pdf","UserName":"فرید","ReqNo":"251"}', NULL, '2016-06-02 14:01:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3399, 9293, '', '{"FileName":"ias2_en.pdf","UserName":"فرید","ReqNo":"253"}', NULL, '2016-06-02 14:02:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3401, 9293, '', '{"FileName":"ias7_en.pdf","UserName":"فرید","ReqNo":"255"}', NULL, '2016-06-02 14:04:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3403, 9293, '', '{"FileName":"ias8_en.pdf","UserName":"فرید","ReqNo":"257"}', NULL, '2016-06-02 14:08:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3405, 9293, '', '{"FileName":"ias10_en.pdf","UserName":"فرید","ReqNo":"259"}', NULL, '2016-06-02 14:09:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3407, 9293, '', '{"FileName":"ias11_en.pdf","UserName":"فرید","ReqNo":"261"}', NULL, '2016-06-02 14:12:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3409, 9293, '', '{"FileName":"ias12_en.pdf","UserName":"فرید","ReqNo":"263"}', NULL, '2016-06-02 14:18:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3411, 9293, '', '{"FileName":"ias17_en.pdf","UserName":"فرید","ReqNo":"265"}', NULL, '2016-06-02 14:21:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3413, 9293, '', '{"FileName":"ias18_en.pdf","UserName":"فرید","ReqNo":"267"}', NULL, '2016-06-02 14:24:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3415, 9293, '', '{"FileName":"ias20_en.pdf","UserName":"فرید","ReqNo":"269"}', NULL, '2016-06-02 14:26:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3417, 15913, '', '{"FileName":"10.pdf","UserName":"علی","ReqNo":"675"}', NULL, '2016-06-02 14:29:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3419, 9293, '', '{"FileName":"ias40_en.pdf","UserName":"فرید","ReqNo":"271"}', NULL, '2016-06-02 14:30:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3421, 9515, '', '{"FileName":"بدون عنوان.docx","UserName":"آریا","ReqNo":"273"}', NULL, '2016-06-02 14:30:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3423, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"275"}', NULL, '2016-06-02 14:32:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3425, 9671, '', '{"FileName":"chapter10.pdf","UserName":"علی","ReqNo":"277"}', NULL, '2016-06-02 14:37:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3427, 9721, '', '{"FileName":"intermediate_uveitis.pdf","UserName":"mosi","ReqNo":"279"}', NULL, '2016-06-02 14:38:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3429, 9801, '', '{"FileName":"20160411191119.pdf","UserName":"ایوب","ReqNo":"281"}', NULL, '2016-06-02 14:41:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3431, 9561, '', '{"FileName":"blumer+race+prejudice+as+a+sense+of+group+position-1.pdf","UserName":"زهرا","ReqNo":"283"}', NULL, '2016-06-02 14:43:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3433, 9825, '', '{"FileName":"03.pdf","UserName":"meysam","ReqNo":"285"}', NULL, '2016-06-02 14:45:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3435, 9867, '', '{"FileName":"Simplifying the Search for Effective Ranking of Fuzzy Numbers.pdf","UserName":"سحر","ReqNo":"287"}', NULL, '2016-06-02 14:45:43', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3437, 9875, '', '{"FileName":"XR1350.pdf","UserName":"ایمان","ReqNo":"289"}', NULL, '2016-06-02 14:45:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3439, 9895, '', '{"FileName":"rdf.doc","UserName":"احمد","ReqNo":"291"}', NULL, '2016-06-02 14:47:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3441, 9949, '', '{"FileName":"rezdeb_664463.docx","UserName":"mohamad","ReqNo":"297"}', NULL, '2016-06-02 14:47:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3443, 9949, '', '{"FileName":"570e437b8915b994d7b9a3e1107c9c6c5da5 (1).pdf","UserName":"mohamad","ReqNo":"301"}', NULL, '2016-06-02 14:48:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3445, 9949, '', '{"FileName":"IJCSI-9-6-3-466-473.pdf","UserName":"mohamad","ReqNo":"303"}', NULL, '2016-06-02 14:48:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3447, 9949, '', '{"FileName":"IJEIT1412201506_19.pdf","UserName":"mohamad","ReqNo":"305"}', NULL, '2016-06-02 14:48:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3449, 6083, '', '{"FileName":"New-order-preserving-encryption-model-for-outsourced-databases-in-cloud-environments-2016.pdf","UserName":"اصغر","ReqNo":"307"}', NULL, '2016-06-02 14:49:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3451, 6083, '', '{"FileName":"An-improved-database-synchronization-mechanism-for-mobile-data-using-software-defined-networking-con","UserName":"اصغر","ReqNo":"309"}', NULL, '2016-06-02 14:49:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3453, 10067, '', '{"FileName":"3.pdf","UserName":"وارش","ReqNo":"313"}', NULL, '2016-06-02 14:49:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3455, 10165, '', '{"FileName":"Right_to_be_forgotten_-_Wikipedia,_the_free_encyclopedia.pdf","UserName":"جمشید","ReqNo":"317"}', NULL, '2016-06-02 14:49:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3457, 10195, '', '{"FileName":"2014062311065413837.pdf","UserName":"reza","ReqNo":"319"}', NULL, '2016-06-02 14:49:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3459, 7583, '', '{"FileName":"Introducing Small Basic.docx","UserName":"رضا","ReqNo":"321"}', NULL, '2016-06-02 14:49:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3461, 10271, '', '{"FileName":"sahar2.pdf","UserName":"سحر","ReqNo":"323"}', NULL, '2016-06-02 14:51:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3463, 10307, '', '{"FileName":"1-s2.0-0011684X86902741-main.pdf","UserName":"mohmood","ReqNo":"327"}', NULL, '2016-06-02 14:52:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3465, 10315, '', '{"FileName":"barnhartmichellebusinessdevelopmentvalidationtechnology.pdf","UserName":"هادی","ReqNo":"329"}', NULL, '2016-06-02 14:52:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3467, 10319, '', '{"FileName":"Knowledge Creation and Transfer.docx","UserName":"بابک","ReqNo":"331"}', NULL, '2016-06-02 14:53:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3469, 15917, '', '{"FileName":"pxc3880446.pdf","UserName":"فائزه","ReqNo":"677"}', NULL, '2016-06-02 16:40:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3471, 15917, '', '{"FileName":"D411012736.pdf","UserName":"فائزه","ReqNo":"679"}', NULL, '2016-06-02 16:44:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3473, 15917, '', '{"FileName":"IJRET20140313001.pdf","UserName":"فائزه","ReqNo":"681"}', NULL, '2016-06-02 16:44:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3475, 15717, '', '{"FileName":"11.PNG.docx","UserName":"علی رضا","ReqNo":"683"}', NULL, '2016-06-02 17:20:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3477, 15953, '', '{"FileName":"a2.pdf","UserName":"amir","ReqNo":"685"}', NULL, '2016-06-03 00:23:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3479, 10737, '', '{"FileName":"Wiley_Big_Data,_Data_Mining_and_Machine_Learning_(2014)[1].pdf","UserName":"عاطفه","ReqNo":"337"}', NULL, '2016-06-03 09:28:17', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3481, 10815, '', '{"FileName":"4.pdf","UserName":"amir","ReqNo":"345"}', NULL, '2016-06-03 09:30:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3483, 10815, '', '{"FileName":"5.pdf","UserName":"amir","ReqNo":"347"}', NULL, '2016-06-03 09:32:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3485, 10827, '', '{"FileName":"10.1016_J.CONBUILDMAT.2010.04.002-Deterioration-of-compressive-property-of-concrete-under-simulated-","UserName":"Alireza","ReqNo":"349"}', NULL, '2016-06-03 09:32:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3487, 10801, '', '{"FileName":"biomimetics 1.docx","UserName":"احمد","ReqNo":"351"}', NULL, '2016-06-03 09:32:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3489, 10855, '', '{"FileName":"4_588146254509244432.pdf","UserName":"Hossein","ReqNo":"353"}', NULL, '2016-06-03 09:33:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3491, 10871, '', '{"FileName":"ACFrOgBvaOUhnll6alyKPX2tzYubnci2pGbbFi1UgwHg8X3ETbA4HPJQiBnEymiWvDdqIL2fJKWvVGOeFTWVQtuLbNw176ocoh89","UserName":"hakim","ReqNo":"355"}', NULL, '2016-06-03 09:33:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3493, 10927, '', '{"FileName":"Doc2.pdf","UserName":"یاور","ReqNo":"357"}', NULL, '2016-06-03 09:33:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3495, 10927, '', '{"FileName":"Doc3.pdf","UserName":"یاور","ReqNo":"359"}', NULL, '2016-06-03 09:33:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3497, 10927, '', '{"FileName":"Doc4.pdf","UserName":"یاور","ReqNo":"361"}', NULL, '2016-06-03 09:33:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3499, 10927, '', '{"FileName":"Doc5.pdf","UserName":"یاور","ReqNo":"363"}', NULL, '2016-06-03 09:33:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3501, 10927, '', '{"FileName":"Doc6.pdf","UserName":"یاور","ReqNo":"365"}', NULL, '2016-06-03 09:33:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3503, 10953, '', '{"FileName":"vivado.pdf","UserName":"مهدی","ReqNo":"367"}', NULL, '2016-06-03 09:36:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3505, 11007, '', '{"FileName":"7-laser welding.pdf","UserName":"حسین","ReqNo":"369"}', NULL, '2016-06-03 09:37:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3507, 9561, '', '{"FileName":"The-Relationship-between-Television-Fiction-and-Fear-of-Crime-mpi01odl.pdf","UserName":"زهرا","ReqNo":"371"}', NULL, '2016-06-03 09:38:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3509, 10991, '', '{"FileName":"ترجمه انگلیسی توضیح نمودار.doc","UserName":"حسن","ReqNo":"373"}', NULL, '2016-06-03 09:40:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3511, 11083, '', '{"FileName":"kitagawa2010.pdf","UserName":"علی","ReqNo":"375"}', NULL, '2016-06-03 09:41:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3513, 11127, '', '{"FileName":"download-1461757714285.doc","UserName":"احمد","ReqNo":"377"}', NULL, '2016-06-03 09:43:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3515, 11141, '', '{"FileName":"language_project_1[1].pdf","UserName":"sami","ReqNo":"379"}', NULL, '2016-06-03 10:00:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3517, 11159, '', '{"FileName":"The use of risk 97.doc","UserName":"محمد","ReqNo":"381"}', NULL, '2016-06-03 10:02:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3519, 11219, '', '{"FileName":"sage14.pdf","UserName":"وحید","ReqNo":"389"}', NULL, '2016-06-03 10:06:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3521, 1713, '', '{"FileName":"THE  ACROPOLIS   AND   PERSEPOLIS.pdf","UserName":"رضوان","ReqNo":"393"}', NULL, '2016-06-03 10:06:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3523, 6689, '', '{"FileName":"[freeScience.ir]-10.1109-JSEN.2015.2445796.pdf","UserName":"محمد","ReqNo":"407"}', NULL, '2016-06-03 10:08:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3525, 11581, '', '{"FileName":"582.pdf","UserName":"فاطمه","ReqNo":"409"}', NULL, '2016-06-03 10:09:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3527, 11727, '', '{"FileName":"CROWNCOM_2011.pdf","UserName":"faez","ReqNo":"423"}', NULL, '2016-06-03 10:09:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3529, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.pdf","UserName":"سید محمد","ReqNo":"425"}', NULL, '2016-06-03 10:09:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3531, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.pdf","UserName":"سید محمد","ReqNo":"427"}', NULL, '2016-06-03 10:09:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3533, 19, '', '{"FileName":"فرم اوليه نيازسنجي منابع  پردازشي ابري.docx","UserName":"سید محمد","ReqNo":"429"}', NULL, '2016-06-03 10:09:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3535, 11997, '', '{"FileName":"10.1016_J.RENENE.2008.03.004-Reliability-and-economic-evaluation-of-small-autonomous-power-systems-c","UserName":"ali","ReqNo":"437"}', NULL, '2016-06-03 10:09:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3537, 12217, '', '{"FileName":"Common Sense Guide to Mitigating Insider Threats.pdf","UserName":"سید محمد","ReqNo":"441"}', NULL, '2016-06-03 10:17:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3539, 12217, '', '{"FileName":"2.pdf","UserName":"سید محمد","ReqNo":"447"}', NULL, '2016-06-03 10:17:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3541, 12217, '', '{"FileName":"Common Sense Guide to Mitigating Insider Threats.pdf","UserName":"سید محمد","ReqNo":"449"}', NULL, '2016-06-03 10:25:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3543, 12217, '', '{"FileName":"1.docx","UserName":"سید محمد","ReqNo":"451"}', NULL, '2016-06-03 10:27:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3545, 12297, '', '{"FileName":"بیان مسئله.docx","UserName":"بحجت","ReqNo":"455"}', NULL, '2016-06-03 10:28:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3547, 12423, '', '{"FileName":"magazine.pdf","UserName":"rezvaneh","ReqNo":"465"}', NULL, '2016-06-03 11:01:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3549, 12501, '', '{"FileName":"PB_CF (3).pdf","UserName":"سید مجتبی","ReqNo":"477"}', NULL, '2016-06-03 11:01:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3551, 12475, '', '{"FileName":"S1350-4487(01)00238-4-Measurement-of-fast-fission-factor-for-heavy-water-zero-power-reactor-HWZPR-by","UserName":"محمد","ReqNo":"481"}', NULL, '2016-06-03 11:01:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3553, 11865, '', '{"FileName":"order-z-1462076694-1378.pdf","UserName":"افشین","ReqNo":"485"}', NULL, '2016-06-03 11:01:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3555, 12537, '', '{"FileName":"10.1016-J.BUSHOR.2014.11.008-Negotiations-between-auditors-and-their-clients-regarding-adjustments-t","UserName":"اسعد","ReqNo":"487"}', NULL, '2016-06-03 11:01:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3557, 12677, '', '{"FileName":"Cruise terminals site selection process An institutional analysis of the Kai Tak Cruise Terminal in ","UserName":"احمد","ReqNo":"489"}', NULL, '2016-06-03 11:01:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3559, 12833, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"اشکان","ReqNo":"495"}', NULL, '2016-06-03 11:03:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3561, 12915, '', '{"FileName":"4_562780667285340166.pdf","UserName":"محمد","ReqNo":"497"}', NULL, '2016-06-03 11:03:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3563, 12991, '', '{"FileName":"13613_2015_Article_59.pdf","UserName":"میثم","ReqNo":"499"}', NULL, '2016-06-03 11:03:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3565, 12833, '', '{"FileName":"4_524717984208388104.doc","UserName":"اشکان","ReqNo":"501"}', NULL, '2016-06-03 11:09:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3567, 13055, '', '{"FileName":"زیست‌شناسی.docx","UserName":"parviz","ReqNo":"509"}', NULL, '2016-06-03 11:13:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3569, 13325, '', '{"FileName":"magic_number_500_CA.pdf","UserName":"mohsen","ReqNo":"521"}', NULL, '2016-06-03 11:16:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3571, 13239, '', '{"FileName":"0b647508e61e8e5c4c60d33eb672df77.pdf","UserName":"مهدی","ReqNo":"531"}', NULL, '2016-06-03 11:22:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3573, 13593, '', '{"FileName":"opinion-paper-armed-conflict.pdf","UserName":"اعظم","ReqNo":"533"}', NULL, '2016-06-03 11:25:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3575, 9801, '', '{"FileName":"How to Burn More Fat and Build More Muscle While Playing Sports.pdf","UserName":"ایوب","ReqNo":"537"}', NULL, '2016-06-03 11:27:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3577, 13923, '', '{"FileName":"DISTRIBUTOR AGREEMENT.pdf","UserName":"Saeid","ReqNo":"539"}', NULL, '2016-06-03 11:31:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3579, 13991, '', '{"FileName":"مقدمه ساده.pdf","UserName":"hossein","ReqNo":"541"}', NULL, '2016-06-03 11:44:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3581, 14125, '', '{"FileName":"New PDF Document.pdf","UserName":"hojjat","ReqNo":"547"}', NULL, '2016-06-03 11:44:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3583, 14181, '', '{"FileName":"Making a Complaint or Criticism.docx","UserName":"غزاله","ReqNo":"551"}', NULL, '2016-06-03 11:45:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3585, 14411, '', '{"FileName":"kerio-operator-stepbystep-en-2.2.6-3042.docx","UserName":"مهدی","ReqNo":"563"}', NULL, '2016-06-03 11:45:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3587, 14411, '', '{"FileName":"kerio-operator-adminguide-en-2.4.7-5666-p1.docx","UserName":"مهدی","ReqNo":"565"}', NULL, '2016-06-03 11:59:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3589, 14463, '', '{"FileName":"Library_7p.pdf","UserName":"masoud","ReqNo":"567"}', NULL, '2016-06-03 12:00:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3591, 14465, '', '{"FileName":"history of organazation theory.pdf","UserName":"Ali","ReqNo":"569"}', NULL, '2016-06-03 12:02:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3593, 14563, '', '{"FileName":"Abstract.pdf","UserName":"مهدي","ReqNo":"603"}', NULL, '2016-06-03 12:02:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3595, 14679, '', '{"FileName":"A multi-phase approach for supply chain design with product life cycle considerations.pdf","UserName":"فرشاد","ReqNo":"605"}', NULL, '2016-06-03 12:03:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3597, 14693, '', '{"FileName":"4536-Instant-Messaging.pdf","UserName":"مائده","ReqNo":"607"}', NULL, '2016-06-03 12:03:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3599, 14807, '', '{"FileName":"tmp_12553-Emailing 10.1016_J.PMCJ.2014.09.006-A-utility-paradigm-for-IoT-The-sensing-Cloud-113867067","UserName":"محسن","ReqNo":"609"}', NULL, '2016-06-03 12:03:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3601, 14871, '', '{"FileName":"SCAME MANUAL JADID.pdf","UserName":"میثم","ReqNo":"611"}', NULL, '2016-06-03 12:03:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3603, 14923, '', '{"FileName":"10.1016_j.jcsr.2015.02.013-Effects-of-floor-slab-on-progressive-collapse-resistance-of-steel-moment-","UserName":"saeid","ReqNo":"613"}', NULL, '2016-06-03 12:03:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3605, 14923, '', '{"FileName":"10.1016_j.jcsr.2015.02.013-Effects-of-floor-slab-on-progressive-collapse-resistance-of-steel-moment-","UserName":"saeid","ReqNo":"615"}', NULL, '2016-06-03 12:03:42', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3607, 14563, '', '{"FileName":"In this paper.pdf","UserName":"مهدي","ReqNo":"617"}', NULL, '2016-06-03 12:03:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3609, 14807, '', '{"FileName":"tmp_8923-Emailing 10.1016_J.PMCJ.2014.09.006-A-utility-paradigm-for-IoT-The-sensing-Cloud1869178662.","UserName":"محسن","ReqNo":"619"}', NULL, '2016-06-03 12:03:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3611, 15029, '', '{"FileName":"[Elearnica] -Human_action_recognition_using_genetic_algorithms_and_convolutional_neural_.pdf","UserName":"balal","ReqNo":"621"}', NULL, '2016-06-03 12:03:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3613, 15029, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"balal","ReqNo":"623"}', NULL, '2016-06-03 12:03:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3615, 15059, '', '{"FileName":"houston2003.doc","UserName":"حمید","ReqNo":"625"}', NULL, '2016-06-03 12:04:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3617, 15125, '', '{"FileName":"ebook3101(www.takbook.com).pdf","UserName":"پویا","ReqNo":"627"}', NULL, '2016-06-03 12:06:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3619, 15129, '', '{"FileName":"seed priming a technique.pdf","UserName":"بهرام","ReqNo":"629"}', NULL, '2016-06-03 12:06:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3621, 15133, '', '{"FileName":"16_CybersecurityStrategyoftheEur - Copy.docx","UserName":"ایمان","ReqNo":"631"}', NULL, '2016-06-03 12:06:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3623, 14807, '', '{"FileName":"Mobiles.docx","UserName":"محسن","ReqNo":"633"}', NULL, '2016-06-03 12:07:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3625, 15161, '', '{"FileName":"IFCGEM+Brochure+sec+5-6.pdf","UserName":"Meysam","ReqNo":"635"}', NULL, '2016-06-03 12:07:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3627, 2081, '', '{"FileName":"30.docx","UserName":"alireza","ReqNo":"637"}', NULL, '2016-06-03 12:07:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3629, 2081, '', '{"FileName":"30.docx","UserName":"alireza","ReqNo":"639"}', NULL, '2016-06-03 12:08:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3631, 15291, '', '{"FileName":"DBSCAN(4).pdf","UserName":"ابوذر","ReqNo":"641"}', NULL, '2016-06-03 12:08:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3633, 15315, '', '{"FileName":"02-23 Omitiran_2.pdf","UserName":"حامد","ReqNo":"643"}', NULL, '2016-06-03 12:08:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3635, 15433, '', '{"FileName":"1-s2.0-S187704281502354X-main(1).pdf","UserName":"سمانه","ReqNo":"645"}', NULL, '2016-06-03 12:08:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3637, 15541, '', '{"FileName":"paper - Copy.doc","UserName":"najja","ReqNo":"647"}', NULL, '2016-06-03 12:09:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3639, 15603, '', '{"FileName":"4_294606392541577248.pdf","UserName":"farid","ReqNo":"649"}', NULL, '2016-06-03 12:10:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3641, 15091, '', '{"FileName":"[Reza_Hasmath]_Managing_Ethnic_Diversity_(Research(BookZZ.org).pdf","UserName":"mohsen","ReqNo":"651"}', NULL, '2016-06-03 12:13:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3643, 15641, '', '{"FileName":"مقاله 1.pdf","UserName":"مونا","ReqNo":"653"}', NULL, '2016-06-03 12:16:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3645, 15433, '', '{"FileName":"1-s2.0-S187704281502354X-main(1).pdf","UserName":"سمانه","ReqNo":"655"}', NULL, '2016-06-03 12:19:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3647, 15717, '', '{"FileName":"Tarjomeiran.com_ProjectFile_27862_swoJgVkBp0X7.pdf","UserName":"علی رضا","ReqNo":"657"}', NULL, '2016-06-03 12:19:02', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3649, 15717, '', '{"FileName":"Tarjomeiran.com_ProjectFile_20981_eimL7NXSWrHd.pdf","UserName":"علی رضا","ReqNo":"659"}', NULL, '2016-06-03 12:20:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3651, 15743, '', '{"FileName":"1-s2.0-S0720048X12000903-main.pdf","UserName":"elnaz","ReqNo":"661"}', NULL, '2016-06-03 12:21:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3653, 15743, '', '{"FileName":"1-s2.0-S0169500211004211-main.pdf","UserName":"elnaz","ReqNo":"663"}', NULL, '2016-06-03 12:23:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3655, 15785, '', '{"FileName":"New Doc 3.pdf","UserName":"کیوان","ReqNo":"665"}', NULL, '2016-06-03 12:23:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3657, 15785, '', '{"FileName":"New Doc 4.pdf","UserName":"کیوان","ReqNo":"667"}', NULL, '2016-06-03 12:23:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3659, 19, '', '{"FileName":"Tahlil Filter.pdf","UserName":"سید محمد","ReqNo":"669"}', NULL, '2016-06-03 12:24:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3661, 15717, '', '{"FileName":"Untitleddocument.pdf","UserName":"علی رضا","ReqNo":"671"}', NULL, '2016-06-03 12:25:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3663, 15907, '', '{"FileName":"DISTRIBUTION AGREEMENT INDUSTRY_GA.docx","UserName":"محمدرضا","ReqNo":"673"}', NULL, '2016-06-03 12:26:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3665, 15913, '', '{"FileName":"10.pdf","UserName":"علی","ReqNo":"675"}', NULL, '2016-06-03 12:27:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3667, 15917, '', '{"FileName":"pxc3880446.pdf","UserName":"فائزه","ReqNo":"677"}', NULL, '2016-06-03 12:29:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3669, 15917, '', '{"FileName":"D411012736.pdf","UserName":"فائزه","ReqNo":"679"}', NULL, '2016-06-03 12:32:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3671, 15917, '', '{"FileName":"IJRET20140313001.pdf","UserName":"فائزه","ReqNo":"681"}', NULL, '2016-06-03 12:33:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3673, 15717, '', '{"FileName":"11.PNG.docx","UserName":"علی رضا","ReqNo":"683"}', NULL, '2016-06-03 12:33:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3675, 15953, '', '{"FileName":"a2.pdf","UserName":"amir","ReqNo":"685"}', NULL, '2016-06-03 12:36:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3677, 16005, '', '{"FileName":"شadhd11-1.pdf","UserName":"وحید","ReqNo":"687"}', NULL, '2016-06-03 16:19:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3679, 16005, '', '{"FileName":"شadhd11-1.pdf","UserName":"وحید","ReqNo":"687"}', NULL, '2016-06-03 16:30:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3681, 16043, '', '{"FileName":"Renewable and Sustainable Energy Reviews Volume 59 issue 2016 [doi 10.1016_j.rser.2016.01.059] Shiva","UserName":"payam","ReqNo":"689"}', NULL, '2016-06-03 19:11:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3683, 16043, '', '{"FileName":"Renewable Energy Volume 80 issue 2015 [doi 10.1016_j.renene.2015.02.045] Maleki, Akbar; Ameri, Mehra","UserName":"payam","ReqNo":"691"}', NULL, '2016-06-03 19:13:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3685, 16043, '', '{"FileName":"Renewable Energy Volume 85 issue 2016 [doi 10.1016_j.renene.2015.07.007] Cervone, A.; Carbone, G.; S","UserName":"payam","ReqNo":"693"}', NULL, '2016-06-03 19:14:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3687, 16043, '', '{"FileName":"Renewable and Sustainable Energy Reviews Volume 59 issue 2016 [doi 10.1016_j.rser.2016.01.059] Shiva","UserName":"payam","ReqNo":"689"}', NULL, '2016-06-03 19:30:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3689, 16043, '', '{"FileName":"Renewable Energy Volume 80 issue 2015 [doi 10.1016_j.renene.2015.02.045] Maleki, Akbar; Ameri, Mehra","UserName":"payam","ReqNo":"691"}', NULL, '2016-06-03 19:30:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3691, 16043, '', '{"FileName":"Renewable Energy Volume 85 issue 2016 [doi 10.1016_j.renene.2015.07.007] Cervone, A.; Carbone, G.; S","UserName":"payam","ReqNo":"693"}', NULL, '2016-06-03 19:30:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3693, 16055, '', '{"FileName":"THE CREATIVE MIND.docx","UserName":"دریا","ReqNo":"695"}', NULL, '2016-06-03 21:04:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3695, 16055, '', '{"FileName":"THE CREATIVE MIND.docx","UserName":"دریا","ReqNo":"695"}', NULL, '2016-06-03 21:30:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3697, 16067, '', '{"FileName":"fasl-8.pdf","UserName":"محمود","ReqNo":"697"}', NULL, '2016-06-04 01:05:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3699, 16067, '', '{"FileName":"fasl-8.pdf","UserName":"محمود","ReqNo":"697"}', NULL, '2016-06-04 01:30:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3701, 13017, '', '{"FileName":"p72-nerur.pdf","UserName":"mehrnoosh","ReqNo":"503"}', NULL, '2016-06-05 12:05:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3703, 16195, '', '{"FileName":"EasyScaleModeling.pdf","UserName":"مرتضی","ReqNo":"699"}', NULL, '2016-06-05 13:19:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3705, 16195, '', '{"FileName":"make-your-own-sculpture-dough.pdf","UserName":"مرتضی","ReqNo":"701"}', NULL, '2016-06-05 13:21:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3707, 16195, '', '{"FileName":"EasyScaleModeling.pdf","UserName":"مرتضی","ReqNo":"699"}', NULL, '2016-06-05 13:30:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3709, 16195, '', '{"FileName":"make-your-own-sculpture-dough.pdf","UserName":"مرتضی","ReqNo":"701"}', NULL, '2016-06-05 13:30:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3711, 16195, '', '{"FileName":"CarvingManual.pdf","UserName":"مرتضی","ReqNo":"703"}', NULL, '2016-06-05 14:29:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3713, 16195, '', '{"FileName":"CraftsMakeOwnArtSupplies.pdf","UserName":"مرتضی","ReqNo":"705"}', NULL, '2016-06-05 14:30:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3715, 16195, '', '{"FileName":"CarvingManual.pdf","UserName":"مرتضی","ReqNo":"703"}', NULL, '2016-06-05 14:30:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3717, 16195, '', '{"FileName":"DontThrowOut.pdf","UserName":"مرتضی","ReqNo":"707"}', NULL, '2016-06-05 14:30:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3719, 16195, '', '{"FileName":"homemade_art _supplies.pdf","UserName":"مرتضی","ReqNo":"709"}', NULL, '2016-06-05 14:30:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3721, 16195, '', '{"FileName":"Homemade_Play_Dough.pdf","UserName":"مرتضی","ReqNo":"711"}', NULL, '2016-06-05 14:31:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3723, 16195, '', '{"FileName":"howto2.pdf","UserName":"مرتضی","ReqNo":"713"}', NULL, '2016-06-05 14:31:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3725, 16195, '', '{"FileName":"How-To-Paint-Ceramics.pdf","UserName":"مرتضی","ReqNo":"715"}', NULL, '2016-06-05 14:31:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3727, 16195, '', '{"FileName":"How-To-Use-Rubber-Stamps.pdf","UserName":"مرتضی","ReqNo":"717"}', NULL, '2016-06-05 14:32:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3729, 16195, '', '{"FileName":"HP1787_FS_12playdough_recipes.pdf","UserName":"مرتضی","ReqNo":"719"}', NULL, '2016-06-05 14:35:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3731, 16195, '', '{"FileName":"Lotta pom pom fact sheet.pdf","UserName":"مرتضی","ReqNo":"721"}', NULL, '2016-06-05 14:35:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3733, 16195, '', '{"FileName":"Make_your_own_rock.pdf","UserName":"مرتضی","ReqNo":"723"}', NULL, '2016-06-05 14:36:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3735, 16195, '', '{"FileName":"Making_Soap_and_Candles.pdf","UserName":"مرتضی","ReqNo":"725"}', NULL, '2016-06-05 14:36:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3737, 16195, '', '{"FileName":"Making-Your-Own-Casting-Slip-from-Existing-Slury.pdf","UserName":"مرتضی","ReqNo":"727"}', NULL, '2016-06-05 14:36:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3739, 16195, '', '{"FileName":"Making-Your-Own-Casting-Slip-from-Existing-Slury.pdf","UserName":"مرتضی","ReqNo":"729"}', NULL, '2016-06-05 14:36:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3741, 16195, '', '{"FileName":"model_making_guidelines.pdf","UserName":"مرتضی","ReqNo":"731"}', NULL, '2016-06-05 14:37:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3743, 16195, '', '{"FileName":"mold.making.for.glass.art.pdf","UserName":"مرتضی","ReqNo":"733"}', NULL, '2016-06-05 14:37:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3745, 16195, '', '{"FileName":"mold.pdf","UserName":"مرتضی","ReqNo":"735"}', NULL, '2016-06-05 14:37:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3747, 16195, '', '{"FileName":"Muffler-Packing-FAQ.pdf","UserName":"مرتضی","ReqNo":"737"}', NULL, '2016-06-05 14:38:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3749, 16195, '', '{"FileName":"paper.pdf","UserName":"مرتضی","ReqNo":"739"}', NULL, '2016-06-05 14:40:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3751, 16195, '', '{"FileName":"paperclay.pdf","UserName":"مرتضی","ReqNo":"741"}', NULL, '2016-06-05 14:40:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3753, 16195, '', '{"FileName":"plankingprojectbeginners.pdf","UserName":"مرتضی","ReqNo":"743"}', NULL, '2016-06-05 14:40:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3755, 16195, '', '{"FileName":"Play Dough.pdf","UserName":"مرتضی","ReqNo":"745"}', NULL, '2016-06-05 14:40:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3757, 16195, '', '{"FileName":"Playdough Recipe.pdf","UserName":"مرتضی","ReqNo":"747"}', NULL, '2016-06-05 14:40:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3759, 16195, '', '{"FileName":"POM POM GALAXY factsheet.pdf","UserName":"مرتضی","ReqNo":"749"}', NULL, '2016-06-05 14:41:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3761, 16195, '', '{"FileName":"pompom-1.pdf","UserName":"مرتضی","ReqNo":"751"}', NULL, '2016-06-05 14:41:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3763, 16195, '', '{"FileName":"PomPom-tutorial.pdf","UserName":"مرتضی","ReqNo":"753"}', NULL, '2016-06-05 14:43:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3765, 16195, '', '{"FileName":"quillpens.pdf","UserName":"مرتضی","ReqNo":"755"}', NULL, '2016-06-05 14:44:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3767, 16195, '', '{"FileName":"roads.pdf","UserName":"مرتضی","ReqNo":"757"}', NULL, '2016-06-05 14:44:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3769, 16195, '', '{"FileName":"Scented-Candles-Made-Simple.pdf","UserName":"مرتضی","ReqNo":"759"}', NULL, '2016-06-05 14:44:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3771, 16195, '', '{"FileName":"CraftsMakeOwnArtSupplies.pdf","UserName":"مرتضی","ReqNo":"705"}', NULL, '2016-06-05 15:00:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3773, 16195, '', '{"FileName":"DontThrowOut.pdf","UserName":"مرتضی","ReqNo":"707"}', NULL, '2016-06-05 15:01:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3775, 16195, '', '{"FileName":"homemade_art _supplies.pdf","UserName":"مرتضی","ReqNo":"709"}', NULL, '2016-06-05 15:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3777, 16195, '', '{"FileName":"Homemade_Play_Dough.pdf","UserName":"مرتضی","ReqNo":"711"}', NULL, '2016-06-05 15:01:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3779, 16195, '', '{"FileName":"howto2.pdf","UserName":"مرتضی","ReqNo":"713"}', NULL, '2016-06-05 15:05:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3781, 16195, '', '{"FileName":"How-To-Paint-Ceramics.pdf","UserName":"مرتضی","ReqNo":"715"}', NULL, '2016-06-05 15:06:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3783, 16195, '', '{"FileName":"How-To-Use-Rubber-Stamps.pdf","UserName":"مرتضی","ReqNo":"717"}', NULL, '2016-06-05 15:08:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3785, 16195, '', '{"FileName":"HP1787_FS_12playdough_recipes.pdf","UserName":"مرتضی","ReqNo":"719"}', NULL, '2016-06-05 15:10:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3787, 16195, '', '{"FileName":"Lotta pom pom fact sheet.pdf","UserName":"مرتضی","ReqNo":"721"}', NULL, '2016-06-05 15:11:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3789, 16195, '', '{"FileName":"Make_your_own_rock.pdf","UserName":"مرتضی","ReqNo":"723"}', NULL, '2016-06-05 15:12:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3791, 16201, '', '{"FileName":"ورد خام.docx","UserName":"ali","ReqNo":"761"}', NULL, '2016-06-05 15:27:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3793, 16195, '', '{"FileName":"Making_Soap_and_Candles.pdf","UserName":"مرتضی","ReqNo":"725"}', NULL, '2016-06-05 15:37:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3795, 16195, '', '{"FileName":"Making-Your-Own-Casting-Slip-from-Existing-Slury.pdf","UserName":"مرتضی","ReqNo":"727"}', NULL, '2016-06-05 15:38:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3797, 16195, '', '{"FileName":"Making-Your-Own-Casting-Slip-from-Existing-Slury.pdf","UserName":"مرتضی","ReqNo":"729"}', NULL, '2016-06-05 15:39:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3799, 16207, '', '{"FileName":"dentistbeh.pdf","UserName":"الهام","ReqNo":"763"}', NULL, '2016-06-05 15:42:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3801, 16195, '', '{"FileName":"model_making_guidelines.pdf","UserName":"مرتضی","ReqNo":"731"}', NULL, '2016-06-05 15:42:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3803, 16195, '', '{"FileName":"mold.pdf","UserName":"مرتضی","ReqNo":"735"}', NULL, '2016-06-05 15:47:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3805, 16195, '', '{"FileName":"Muffler-Packing-FAQ.pdf","UserName":"مرتضی","ReqNo":"737"}', NULL, '2016-06-05 15:47:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3807, 16195, '', '{"FileName":"paper.pdf","UserName":"مرتضی","ReqNo":"739"}', NULL, '2016-06-05 15:47:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3809, 16195, '', '{"FileName":"paperclay.pdf","UserName":"مرتضی","ReqNo":"741"}', NULL, '2016-06-05 15:47:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3811, 16195, '', '{"FileName":"plankingprojectbeginners.pdf","UserName":"مرتضی","ReqNo":"743"}', NULL, '2016-06-05 15:47:40', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3813, 16195, '', '{"FileName":"Play Dough.pdf","UserName":"مرتضی","ReqNo":"745"}', NULL, '2016-06-05 15:47:41', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3815, 16195, '', '{"FileName":"Playdough Recipe.pdf","UserName":"مرتضی","ReqNo":"747"}', NULL, '2016-06-05 15:47:43', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3817, 16195, '', '{"FileName":"POM POM GALAXY factsheet.pdf","UserName":"مرتضی","ReqNo":"749"}', NULL, '2016-06-05 15:47:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3819, 16195, '', '{"FileName":"pompom-1.pdf","UserName":"مرتضی","ReqNo":"751"}', NULL, '2016-06-05 15:47:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3821, 16195, '', '{"FileName":"PomPom-tutorial.pdf","UserName":"مرتضی","ReqNo":"753"}', NULL, '2016-06-05 15:47:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3823, 16195, '', '{"FileName":"quillpens.pdf","UserName":"مرتضی","ReqNo":"755"}', NULL, '2016-06-05 15:47:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3825, 16195, '', '{"FileName":"Scented-Candles-Made-Simple.pdf","UserName":"مرتضی","ReqNo":"759"}', NULL, '2016-06-05 15:47:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3827, 16195, '', '{"FileName":"mold.making.for.glass.art.pdf","UserName":"مرتضی","ReqNo":"733"}', NULL, '2016-06-05 15:48:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3829, 16195, '', '{"FileName":"mold.pdf","UserName":"مرتضی","ReqNo":"735"}', NULL, '2016-06-05 15:51:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3831, 16195, '', '{"FileName":"Muffler-Packing-FAQ.pdf","UserName":"مرتضی","ReqNo":"737"}', NULL, '2016-06-05 15:51:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3833, 16195, '', '{"FileName":"paper.pdf","UserName":"مرتضی","ReqNo":"739"}', NULL, '2016-06-05 15:51:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3835, 16195, '', '{"FileName":"paperclay.pdf","UserName":"مرتضی","ReqNo":"741"}', NULL, '2016-06-05 15:55:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3837, 16195, '', '{"FileName":"plankingprojectbeginners.pdf","UserName":"مرتضی","ReqNo":"743"}', NULL, '2016-06-05 15:55:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3839, 16195, '', '{"FileName":"Play Dough.pdf","UserName":"مرتضی","ReqNo":"745"}', NULL, '2016-06-05 15:55:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3841, 16195, '', '{"FileName":"Playdough Recipe.pdf","UserName":"مرتضی","ReqNo":"747"}', NULL, '2016-06-05 15:56:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3843, 16195, '', '{"FileName":"POM POM GALAXY factsheet.pdf","UserName":"مرتضی","ReqNo":"749"}', NULL, '2016-06-05 15:56:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3845, 16195, '', '{"FileName":"pompom-1.pdf","UserName":"مرتضی","ReqNo":"751"}', NULL, '2016-06-05 15:56:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(3847, 16195, '', '{"FileName":"PomPom-tutorial.pdf","UserName":"مرتضی","ReqNo":"753"}', NULL, '2016-06-05 15:56:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3849, 16195, '', '{"FileName":"quillpens.pdf","UserName":"مرتضی","ReqNo":"755"}', NULL, '2016-06-05 15:56:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3851, 16195, '', '{"FileName":"roads.pdf","UserName":"مرتضی","ReqNo":"757"}', NULL, '2016-06-05 15:56:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3853, 16195, '', '{"FileName":"Scented-Candles-Made-Simple.pdf","UserName":"مرتضی","ReqNo":"759"}', NULL, '2016-06-05 15:56:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3855, 16201, '', '{"FileName":"ورد خام.docx","UserName":"ali","ReqNo":"761"}', NULL, '2016-06-05 16:00:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3857, 16195, '', '{"FileName":"YiT0QwSB21.pdf","UserName":"مرتضی","ReqNo":"765"}', NULL, '2016-06-05 16:22:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3859, 16195, '', '{"FileName":"YarnPomPoms.pdf","UserName":"مرتضی","ReqNo":"767"}', NULL, '2016-06-05 16:23:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3861, 16195, '', '{"FileName":"y7_t2_hw.pdf","UserName":"مرتضی","ReqNo":"769"}', NULL, '2016-06-05 16:23:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3863, 16195, '', '{"FileName":"xmasideasebook.pdf","UserName":"مرتضی","ReqNo":"771"}', NULL, '2016-06-05 16:23:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3865, 16195, '', '{"FileName":"xmas_snowflake1.pdf","UserName":"مرتضی","ReqNo":"773"}', NULL, '2016-06-05 16:24:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3867, 16195, '', '{"FileName":"women_quiltsquare.pdf","UserName":"مرتضی","ReqNo":"775"}', NULL, '2016-06-05 16:24:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3869, 16195, '', '{"FileName":"whiskey-bottom-fredmag.pdf","UserName":"مرتضی","ReqNo":"777"}', NULL, '2016-06-05 16:24:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3871, 16195, '', '{"FileName":"wgh_fullbooklet.PDF","UserName":"مرتضی","ReqNo":"779"}', NULL, '2016-06-05 16:25:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3873, 16195, '', '{"FileName":"web_angpow.pdf","UserName":"مرتضی","ReqNo":"781"}', NULL, '2016-06-05 16:25:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3875, 16195, '', '{"FileName":"vajameeldesignacarpetf.pdf","UserName":"مرتضی","ReqNo":"783"}', NULL, '2016-06-05 16:25:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3877, 16195, '', '{"FileName":"v20n6_Basic_Beadmaking_Studio_Setting_Up.pdf","UserName":"مرتضی","ReqNo":"785"}', NULL, '2016-06-05 16:26:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3879, 16195, '', '{"FileName":"unique_homemade_gift_ideas.pdf","UserName":"مرتضی","ReqNo":"787"}', NULL, '2016-06-05 16:26:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3881, 16195, '', '{"FileName":"Unicorn Riders Horn Making Activity Sheet.pdf","UserName":"مرتضی","ReqNo":"789"}', NULL, '2016-06-05 16:26:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3883, 16195, '', '{"FileName":"Underglazes2012.pdf","UserName":"مرتضی","ReqNo":"791"}', NULL, '2016-06-05 16:26:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3885, 16195, '', '{"FileName":"UFOGlider.pdf","UserName":"مرتضی","ReqNo":"793"}', NULL, '2016-06-05 16:27:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3887, 16195, '', '{"FileName":"tt_magiccarpet.pdf","UserName":"مرتضی","ReqNo":"795"}', NULL, '2016-06-05 16:27:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3889, 16195, '', '{"FileName":"TotemPole-LessonPlan.pdf","UserName":"مرتضی","ReqNo":"797"}', NULL, '2016-06-05 16:27:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3891, 16195, '', '{"FileName":"The_Hogwild_Stuff_Guide_to_making_geocoins_10-5-06.pdf","UserName":"مرتضی","ReqNo":"799"}', NULL, '2016-06-05 16:29:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3893, 16195, '', '{"FileName":"Textbook Edited Version.pdf","UserName":"مرتضی","ReqNo":"801"}', NULL, '2016-06-05 16:29:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3895, 16195, '', '{"FileName":"TeacherManual.pdf","UserName":"مرتضی","ReqNo":"803"}', NULL, '2016-06-05 16:30:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3897, 16195, '', '{"FileName":"sw_make_your_own_paper.pdf","UserName":"مرتضی","ReqNo":"805"}', NULL, '2016-06-05 16:30:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3899, 16195, '', '{"FileName":"SunbeamCandles-Catalog-Fall2013-forEmail.pdf","UserName":"مرتضی","ReqNo":"807"}', NULL, '2016-06-05 16:30:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3901, 16195, '', '{"FileName":"sugarloaf.pdf","UserName":"مرتضی","ReqNo":"809"}', NULL, '2016-06-05 16:30:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3903, 16195, '', '{"FileName":"StringDollGang_ContestEntry_2012.pdf","UserName":"مرتضی","ReqNo":"811"}', NULL, '2016-06-05 16:30:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3905, 16195, '', '{"FileName":"Step-By-Step Cardmaking.pdf","UserName":"مرتضی","ReqNo":"813"}', NULL, '2016-06-05 16:31:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3907, 16195, '', '{"FileName":"sockdoll.pdf","UserName":"مرتضی","ReqNo":"815"}', NULL, '2016-06-05 16:32:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3909, 16195, '', '{"FileName":"SnakeSkinTutorial.pdf","UserName":"مرتضی","ReqNo":"817"}', NULL, '2016-06-05 16:32:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3911, 16195, '', '{"FileName":"Sculpture-Park-Discovery.pdf","UserName":"مرتضی","ReqNo":"819"}', NULL, '2016-06-05 16:32:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3913, 16195, '', '{"FileName":"sculpture.pdf","UserName":"مرتضی","ReqNo":"821"}', NULL, '2016-06-05 16:32:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3915, 16195, '', '{"FileName":"SCULPTURE TERMS.pdf","UserName":"مرتضی","ReqNo":"823"}', NULL, '2016-06-05 16:32:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3917, 16195, '', '{"FileName":"ScratchBuildingaModelShipgene.pdf","UserName":"مرتضی","ReqNo":"825"}', NULL, '2016-06-05 16:33:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3919, 16207, '', '{"FileName":"dentistbeh.pdf","UserName":"الهام","ReqNo":"763"}', NULL, '2016-06-05 16:33:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(3921, 16195, '', '{"FileName":"SCRAPING.pdf","UserName":"مرتضی","ReqNo":"827"}', NULL, '2016-06-05 16:33:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3923, 16195, '', '{"FileName":"sandcraft00maso2.pdf","UserName":"مرتضی","ReqNo":"829"}', NULL, '2016-06-05 16:33:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3925, 16195, '', '{"FileName":"RRFB_Craft_Book.pdf","UserName":"مرتضی","ReqNo":"831"}', NULL, '2016-06-05 16:34:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3927, 16195, '', '{"FileName":"reuse craft activity card - pom-poms and envelopes (pdf).pdf","UserName":"مرتضی","ReqNo":"833"}', NULL, '2016-06-05 16:35:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3929, 16195, '', '{"FileName":"resource_guide.pdf","UserName":"مرتضی","ReqNo":"835"}', NULL, '2016-06-05 16:35:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3931, 16195, '', '{"FileName":"Report.pdf","UserName":"مرتضی","ReqNo":"837"}', NULL, '2016-06-05 16:36:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3933, 16195, '', '{"FileName":"RC_Resources_Educators_Paper.pdf","UserName":"مرتضی","ReqNo":"839"}', NULL, '2016-06-05 16:36:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3935, 16195, '', '{"FileName":"RAMS-KAC-pompoms.pdf","UserName":"مرتضی","ReqNo":"841"}', NULL, '2016-06-05 16:37:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3937, 16195, '', '{"FileName":"QueenOfGreen-Green-cleaning-recipes.pdf","UserName":"مرتضی","ReqNo":"843"}', NULL, '2016-06-05 16:39:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3939, 16195, '', '{"FileName":"Queen-of-Green-candlemaking.pdf","UserName":"مرتضی","ReqNo":"845"}', NULL, '2016-06-05 16:39:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3941, 16195, '', '{"FileName":"QA-HandQuilting.pdf","UserName":"مرتضی","ReqNo":"847"}', NULL, '2016-06-05 16:39:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3943, 16195, '', '{"FileName":"printed-matter---homemade.pdf","UserName":"مرتضی","ReqNo":"849"}', NULL, '2016-06-05 16:40:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3945, 16195, '', '{"FileName":"potterywheels2.pdf","UserName":"مرتضی","ReqNo":"851"}', NULL, '2016-06-05 16:40:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3947, 16195, '', '{"FileName":"pop-up-template.pdf","UserName":"مرتضی","ReqNo":"853"}', NULL, '2016-06-05 16:40:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3949, 16195, '', '{"FileName":"popups.pdf","UserName":"مرتضی","ReqNo":"855"}', NULL, '2016-06-05 16:41:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3951, 16195, '', '{"FileName":"popup_final.pdf","UserName":"مرتضی","ReqNo":"857"}', NULL, '2016-06-05 16:41:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3953, 16195, '', '{"FileName":"pop_up.pdf","UserName":"مرتضی","ReqNo":"859"}', NULL, '2016-06-05 16:41:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3955, 16195, '', '{"FileName":"pongpompom.pdf","UserName":"مرتضی","ReqNo":"861"}', NULL, '2016-06-05 16:42:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3957, 16195, '', '{"FileName":"pom-pom-instructions (1).pdf","UserName":"مرتضی","ReqNo":"863"}', NULL, '2016-06-05 16:42:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3959, 16195, '', '{"FileName":"PompomBunny.pdf","UserName":"مرتضی","ReqNo":"865"}', NULL, '2016-06-05 16:43:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3961, 16195, '', '{"FileName":"pompom-1.pdf","UserName":"مرتضی","ReqNo":"867"}', NULL, '2016-06-05 16:44:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3963, 16195, '', '{"FileName":"pocketdecodermakeyourown.pdf","UserName":"مرتضی","ReqNo":"869"}', NULL, '2016-06-05 16:45:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3965, 16195, '', '{"FileName":"PDFHowtoMakeKorkerRibbonandBows.pdf","UserName":"مرتضی","ReqNo":"871"}', NULL, '2016-06-05 16:46:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3967, 16195, '', '{"FileName":"Papermaking Instructions.pdf","UserName":"مرتضی","ReqNo":"873"}', NULL, '2016-06-05 16:47:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3969, 16195, '', '{"FileName":"paper01.pdf","UserName":"مرتضی","ReqNo":"875"}', NULL, '2016-06-05 16:48:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3971, 16195, '', '{"FileName":"paper_making.pdf","UserName":"مرتضی","ReqNo":"877"}', NULL, '2016-06-05 16:48:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3973, 16195, '', '{"FileName":"Paper in Three Dimensions Origami, Pop-Ups, Sculpture, Baskets, Boxes, and More.pdf","UserName":"مرتضی","ReqNo":"879"}', NULL, '2016-06-05 16:56:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3975, 16195, '', '{"FileName":"ost_45.pdf","UserName":"مرتضی","ReqNo":"881"}', NULL, '2016-06-05 16:56:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3977, 16195, '', '{"FileName":"OneEyedJake.pdf","UserName":"مرتضی","ReqNo":"883"}', NULL, '2016-06-05 16:57:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3979, 16195, '', '{"FileName":"Olympics0001.pdf","UserName":"مرتضی","ReqNo":"885"}', NULL, '2016-06-05 16:57:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3981, 16195, '', '{"FileName":"OCR_GCSE_Art_and_Design_Themes_2014.pdf","UserName":"مرتضی","ReqNo":"887"}', NULL, '2016-06-05 16:57:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3983, 16195, '', '{"FileName":"nzdf_article.pdf","UserName":"مرتضی","ReqNo":"889"}', NULL, '2016-06-05 16:58:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3985, 16195, '', '{"FileName":"newspaper_pot.pdf","UserName":"مرتضی","ReqNo":"891"}', NULL, '2016-06-05 16:58:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3987, 16195, '', '{"FileName":"newsfile12761_1.pdf","UserName":"مرتضی","ReqNo":"893"}', NULL, '2016-06-05 16:58:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3989, 16195, '', '{"FileName":"NewDesignSpace_Sanders_01.pdf","UserName":"مرتضی","ReqNo":"895"}', NULL, '2016-06-05 16:59:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3991, 16195, '', '{"FileName":"nb11_balloon_pop.pdf","UserName":"مرتضی","ReqNo":"897"}', NULL, '2016-06-05 16:59:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3993, 16195, '', '{"FileName":"Natural Cork Craft FINAL.pdf","UserName":"مرتضی","ReqNo":"899"}', NULL, '2016-06-05 17:00:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3995, 16195, '', '{"FileName":"naghashi_sang[www.irpdf.com].pdf","UserName":"مرتضی","ReqNo":"901"}', NULL, '2016-06-05 17:00:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3997, 16195, '', '{"FileName":"MYO-CAJ_web.pdf","UserName":"مرتضی","ReqNo":"903"}', NULL, '2016-06-05 17:00:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(3999, 16195, '', '{"FileName":"Muffler-Packing-FAQ.pdf","UserName":"مرتضی","ReqNo":"905"}', NULL, '2016-06-05 17:01:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4001, 16195, '', '{"FileName":"YiT0QwSB21.pdf","UserName":"مرتضی","ReqNo":"765"}', NULL, '2016-06-05 17:01:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4003, 16195, '', '{"FileName":"MRRV2.pdf","UserName":"مرتضی","ReqNo":"907"}', NULL, '2016-06-05 17:01:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4005, 16195, '', '{"FileName":"YarnPomPoms.pdf","UserName":"مرتضی","ReqNo":"767"}', NULL, '2016-06-05 17:01:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4007, 16195, '', '{"FileName":"mosaicapple.pdf","UserName":"مرتضی","ReqNo":"909"}', NULL, '2016-06-05 17:02:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4009, 16195, '', '{"FileName":"y7_t2_hw.pdf","UserName":"مرتضی","ReqNo":"769"}', NULL, '2016-06-05 17:02:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4011, 16195, '', '{"FileName":"modeling-creativity.pdf","UserName":"مرتضی","ReqNo":"911"}', NULL, '2016-06-05 17:04:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4013, 16195, '', '{"FileName":"mlkdream.pdf","UserName":"مرتضی","ReqNo":"913"}', NULL, '2016-06-05 17:04:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4015, 16195, '', '{"FileName":"MathArt.pdf","UserName":"مرتضی","ReqNo":"915"}', NULL, '2016-06-05 17:05:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4017, 16195, '', '{"FileName":"mask.pdf","UserName":"مرتضی","ReqNo":"917"}', NULL, '2016-06-05 17:05:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4019, 16195, '', '{"FileName":"Maquette_Sculpting_Sonny_Sng.pdf","UserName":"مرتضی","ReqNo":"919"}', NULL, '2016-06-05 17:05:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4021, 16195, '', '{"FileName":"maquette.pdf","UserName":"مرتضی","ReqNo":"921"}', NULL, '2016-06-05 17:06:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4023, 16195, '', '{"FileName":"xmasideasebook.pdf","UserName":"مرتضی","ReqNo":"771"}', NULL, '2016-06-05 17:12:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4025, 16195, '', '{"FileName":"xmas_snowflake1.pdf","UserName":"مرتضی","ReqNo":"773"}', NULL, '2016-06-05 17:13:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4027, 16195, '', '{"FileName":"women_quiltsquare.pdf","UserName":"مرتضی","ReqNo":"775"}', NULL, '2016-06-05 17:14:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4029, 16195, '', '{"FileName":"whiskey-bottom-fredmag.pdf","UserName":"مرتضی","ReqNo":"777"}', NULL, '2016-06-05 17:16:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4031, 16195, '', '{"FileName":"wgh_fullbooklet.PDF","UserName":"مرتضی","ReqNo":"779"}', NULL, '2016-06-05 17:16:03', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4033, 16195, '', '{"FileName":"web_angpow.pdf","UserName":"مرتضی","ReqNo":"781"}', NULL, '2016-06-05 17:18:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4035, 16195, '', '{"FileName":"vajameeldesignacarpetf.pdf","UserName":"مرتضی","ReqNo":"783"}', NULL, '2016-06-05 17:19:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4037, 16195, '', '{"FileName":"v20n6_Basic_Beadmaking_Studio_Setting_Up.pdf","UserName":"مرتضی","ReqNo":"785"}', NULL, '2016-06-05 17:20:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4039, 16195, '', '{"FileName":"unique_homemade_gift_ideas.pdf","UserName":"مرتضی","ReqNo":"787"}', NULL, '2016-06-05 17:22:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4041, 16195, '', '{"FileName":"Unicorn Riders Horn Making Activity Sheet.pdf","UserName":"مرتضی","ReqNo":"789"}', NULL, '2016-06-05 17:24:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4043, 16195, '', '{"FileName":"Underglazes2012.pdf","UserName":"مرتضی","ReqNo":"791"}', NULL, '2016-06-05 17:28:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4045, 16195, '', '{"FileName":"UFOGlider.pdf","UserName":"مرتضی","ReqNo":"793"}', NULL, '2016-06-05 17:29:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4047, 16195, '', '{"FileName":"tt_magiccarpet.pdf","UserName":"مرتضی","ReqNo":"795"}', NULL, '2016-06-05 17:30:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4049, 16195, '', '{"FileName":"TotemPole-LessonPlan.pdf","UserName":"مرتضی","ReqNo":"797"}', NULL, '2016-06-05 17:32:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4051, 16195, '', '{"FileName":"The_Hogwild_Stuff_Guide_to_making_geocoins_10-5-06.pdf","UserName":"مرتضی","ReqNo":"799"}', NULL, '2016-06-05 17:35:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4053, 16227, '', '{"FileName":"YiT0QwSB21.pdf","UserName":"mortezaboorboori","ReqNo":"923"}', NULL, '2016-06-05 17:36:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4055, 16227, '', '{"FileName":"YarnPomPoms.pdf","UserName":"mortezaboorboori","ReqNo":"925"}', NULL, '2016-06-05 17:36:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4057, 16227, '', '{"FileName":"y7_t2_hw.pdf","UserName":"mortezaboorboori","ReqNo":"927"}', NULL, '2016-06-05 17:36:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4059, 16227, '', '{"FileName":"xmasideasebook.pdf","UserName":"mortezaboorboori","ReqNo":"929"}', NULL, '2016-06-05 17:36:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4061, 16227, '', '{"FileName":"xmasideasebook.pdf","UserName":"mortezaboorboori","ReqNo":"931"}', NULL, '2016-06-05 17:37:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4063, 16227, '', '{"FileName":"women_quiltsquare.pdf","UserName":"mortezaboorboori","ReqNo":"933"}', NULL, '2016-06-05 17:37:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4065, 16227, '', '{"FileName":"whiskey-bottom-fredmag.pdf","UserName":"mortezaboorboori","ReqNo":"935"}', NULL, '2016-06-05 17:37:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4067, 16227, '', '{"FileName":"wgh_fullbooklet.PDF","UserName":"mortezaboorboori","ReqNo":"937"}', NULL, '2016-06-05 17:38:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4069, 16227, '', '{"FileName":"web_angpow.pdf","UserName":"mortezaboorboori","ReqNo":"939"}', NULL, '2016-06-05 17:38:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4071, 16227, '', '{"FileName":"vajameeldesignacarpetf.pdf","UserName":"mortezaboorboori","ReqNo":"941"}', NULL, '2016-06-05 17:38:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4073, 16195, '', '{"FileName":"Textbook Edited Version.pdf","UserName":"مرتضی","ReqNo":"801"}', NULL, '2016-06-05 17:38:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4075, 16227, '', '{"FileName":"v20n6_Basic_Beadmaking_Studio_Setting_Up.pdf","UserName":"mortezaboorboori","ReqNo":"943"}', NULL, '2016-06-05 17:38:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4077, 16227, '', '{"FileName":"unique_homemade_gift_ideas.pdf","UserName":"mortezaboorboori","ReqNo":"945"}', NULL, '2016-06-05 17:38:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4079, 16227, '', '{"FileName":"Unicorn Riders Horn Making Activity Sheet.pdf","UserName":"mortezaboorboori","ReqNo":"947"}', NULL, '2016-06-05 17:39:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4081, 16227, '', '{"FileName":"Underglazes2012.pdf","UserName":"mortezaboorboori","ReqNo":"949"}', NULL, '2016-06-05 17:39:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4083, 16227, '', '{"FileName":"UFOGlider.pdf","UserName":"mortezaboorboori","ReqNo":"951"}', NULL, '2016-06-05 17:41:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4085, 16227, '', '{"FileName":"tweetsie_paper_blank12_b.pdf","UserName":"mortezaboorboori","ReqNo":"953"}', NULL, '2016-06-05 17:42:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4087, 16227, '', '{"FileName":"tt_magiccarpet.pdf","UserName":"mortezaboorboori","ReqNo":"955"}', NULL, '2016-06-05 17:42:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4089, 16227, '', '{"FileName":"TotemPole-LessonPlan.pdf","UserName":"mortezaboorboori","ReqNo":"957"}', NULL, '2016-06-05 17:42:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4091, 16227, '', '{"FileName":"TIPS307.pdf","UserName":"mortezaboorboori","ReqNo":"959"}', NULL, '2016-06-05 17:42:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4093, 16227, '', '{"FileName":"Tiger Model.pdf","UserName":"mortezaboorboori","ReqNo":"961"}', NULL, '2016-06-05 17:42:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4095, 16227, '', '{"FileName":"SCRAPING.pdf","UserName":"mortezaboorboori","ReqNo":"963"}', NULL, '2016-06-05 17:43:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4097, 16227, '', '{"FileName":"sandcraft00maso2.pdf","UserName":"mortezaboorboori","ReqNo":"965"}', NULL, '2016-06-05 17:45:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4099, 16227, '', '{"FileName":"resource_guide.pdf","UserName":"mortezaboorboori","ReqNo":"967"}', NULL, '2016-06-05 17:46:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4101, 16227, '', '{"FileName":"Report.pdf","UserName":"mortezaboorboori","ReqNo":"969"}', NULL, '2016-06-05 17:46:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4103, 16227, '', '{"FileName":"Recipes_for_Sensory_Experiences__1_.pdf","UserName":"mortezaboorboori","ReqNo":"971"}', NULL, '2016-06-05 17:47:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4105, 16227, '', '{"FileName":"RC_Resources_Educators_Paper.pdf","UserName":"mortezaboorboori","ReqNo":"973"}', NULL, '2016-06-05 17:47:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4107, 16227, '', '{"FileName":"RAMS-KAC-pompoms.pdf","UserName":"mortezaboorboori","ReqNo":"975"}', NULL, '2016-06-05 18:04:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4109, 16195, '', '{"FileName":"TeacherManual.pdf","UserName":"مرتضی","ReqNo":"803"}', NULL, '2016-06-05 18:05:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4111, 16195, '', '{"FileName":"sw_make_your_own_paper.pdf","UserName":"مرتضی","ReqNo":"805"}', NULL, '2016-06-05 18:07:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4113, 16227, '', '{"FileName":"QueenOfGreen-Green-cleaning-recipes.pdf","UserName":"mortezaboorboori","ReqNo":"977"}', NULL, '2016-06-05 18:08:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4115, 16227, '', '{"FileName":"Queen-of-Green-candlemaking.pdf","UserName":"mortezaboorboori","ReqNo":"979"}', NULL, '2016-06-05 18:08:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4117, 16227, '', '{"FileName":"QA-HandQuilting.pdf","UserName":"mortezaboorboori","ReqNo":"981"}', NULL, '2016-06-05 18:08:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4119, 16195, '', '{"FileName":"SunbeamCandles-Catalog-Fall2013-forEmail.pdf","UserName":"مرتضی","ReqNo":"807"}', NULL, '2016-06-05 18:10:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4121, 16195, '', '{"FileName":"sugarloaf.pdf","UserName":"مرتضی","ReqNo":"809"}', NULL, '2016-06-05 18:12:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4123, 16195, '', '{"FileName":"StringDollGang_ContestEntry_2012.pdf","UserName":"مرتضی","ReqNo":"811"}', NULL, '2016-06-05 18:13:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4125, 16195, '', '{"FileName":"Step-By-Step Cardmaking.pdf","UserName":"مرتضی","ReqNo":"813"}', NULL, '2016-06-05 18:13:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4127, 16195, '', '{"FileName":"sockdoll.pdf","UserName":"مرتضی","ReqNo":"815"}', NULL, '2016-06-05 18:14:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4129, 16195, '', '{"FileName":"SnakeSkinTutorial.pdf","UserName":"مرتضی","ReqNo":"817"}', NULL, '2016-06-05 18:15:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4131, 16195, '', '{"FileName":"Sculpture-Park-Discovery.pdf","UserName":"مرتضی","ReqNo":"819"}', NULL, '2016-06-05 18:18:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4133, 16195, '', '{"FileName":"sculpture.pdf","UserName":"مرتضی","ReqNo":"821"}', NULL, '2016-06-05 18:21:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4135, 16195, '', '{"FileName":"SCULPTURE TERMS.pdf","UserName":"مرتضی","ReqNo":"823"}', NULL, '2016-06-05 18:23:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4137, 16195, '', '{"FileName":"ScratchBuildingaModelShipgene.pdf","UserName":"مرتضی","ReqNo":"825"}', NULL, '2016-06-05 18:26:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4139, 16195, '', '{"FileName":"SCRAPING.pdf","UserName":"مرتضی","ReqNo":"827"}', NULL, '2016-06-05 18:28:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4141, 16195, '', '{"FileName":"sandcraft00maso2.pdf","UserName":"مرتضی","ReqNo":"829"}', NULL, '2016-06-05 18:34:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4143, 16195, '', '{"FileName":"RRFB_Craft_Book.pdf","UserName":"مرتضی","ReqNo":"831"}', NULL, '2016-06-05 18:36:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4145, 16195, '', '{"FileName":"reuse craft activity card - pom-poms and envelopes (pdf).pdf","UserName":"مرتضی","ReqNo":"833"}', NULL, '2016-06-05 18:36:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4147, 16195, '', '{"FileName":"resource_guide.pdf","UserName":"مرتضی","ReqNo":"835"}', NULL, '2016-06-05 18:37:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4149, 16195, '', '{"FileName":"Report.pdf","UserName":"مرتضی","ReqNo":"837"}', NULL, '2016-06-05 18:38:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4151, 16195, '', '{"FileName":"RC_Resources_Educators_Paper.pdf","UserName":"مرتضی","ReqNo":"839"}', NULL, '2016-06-05 18:38:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4153, 16195, '', '{"FileName":"RAMS-KAC-pompoms.pdf","UserName":"مرتضی","ReqNo":"841"}', NULL, '2016-06-05 18:38:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4155, 16195, '', '{"FileName":"QueenOfGreen-Green-cleaning-recipes.pdf","UserName":"مرتضی","ReqNo":"843"}', NULL, '2016-06-05 18:38:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4157, 16195, '', '{"FileName":"Queen-of-Green-candlemaking.pdf","UserName":"مرتضی","ReqNo":"845"}', NULL, '2016-06-05 18:38:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4159, 16195, '', '{"FileName":"QA-HandQuilting.pdf","UserName":"مرتضی","ReqNo":"847"}', NULL, '2016-06-05 18:43:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4161, 16195, '', '{"FileName":"printed-matter---homemade.pdf","UserName":"مرتضی","ReqNo":"849"}', NULL, '2016-06-05 18:43:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4163, 16195, '', '{"FileName":"potterywheels2.pdf","UserName":"مرتضی","ReqNo":"851"}', NULL, '2016-06-05 18:48:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4165, 16195, '', '{"FileName":"pop-up-template.pdf","UserName":"مرتضی","ReqNo":"853"}', NULL, '2016-06-05 18:49:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4167, 16195, '', '{"FileName":"popups.pdf","UserName":"مرتضی","ReqNo":"855"}', NULL, '2016-06-05 18:51:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4169, 16195, '', '{"FileName":"popup_final.pdf","UserName":"مرتضی","ReqNo":"857"}', NULL, '2016-06-05 18:51:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4171, 16323, '', '{"FileName":"Dynamic risk, accounting-based valuation and ﬁrm fundamentals.pdf","UserName":"محمد","ReqNo":"983"}', NULL, '2016-06-06 13:19:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4173, 16323, '', '{"FileName":"Dynamic risk, accounting-based valuation and ﬁrm fundamentals.pdf","UserName":"محمد","ReqNo":"983"}', NULL, '2016-06-06 14:00:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4175, 16341, '', '{"FileName":"MIS-Weeks8.pdf","UserName":"سعید","ReqNo":"985"}', NULL, '2016-06-06 17:13:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4177, 16341, '', '{"FileName":"MIS-Weeks9.pdf","UserName":"سعید","ReqNo":"987"}', NULL, '2016-06-06 17:15:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4179, 16341, '', '{"FileName":"MIS-Weeks8.pdf","UserName":"سعید","ReqNo":"985"}', NULL, '2016-06-06 18:00:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4181, 16341, '', '{"FileName":"MIS-Weeks9.pdf","UserName":"سعید","ReqNo":"987"}', NULL, '2016-06-06 18:00:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4183, 16005, '', '{"FileName":"adhd11-1.translated-1.doc","UserName":"وحید","ReqNo":"989"}', NULL, '2016-06-06 18:30:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4185, 16005, '', '{"FileName":"adhd11-1.translated-1.doc","UserName":"وحید","ReqNo":"989"}', NULL, '2016-06-06 18:31:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4187, 15641, '', '{"FileName":"Splitting Methods for Convex Clustering.pdf","UserName":"مونا","ReqNo":"991"}', NULL, '2016-06-06 19:00:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4189, 15641, '', '{"FileName":"Splitting Methods for Convex Clustering.pdf","UserName":"مونا","ReqNo":"991"}', NULL, '2016-06-06 20:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4191, 16005, '', '{"FileName":"adhd11-1.translated-1.translated.doc","UserName":"وحید","ReqNo":"993"}', NULL, '2016-06-06 20:13:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4193, 16005, '', '{"FileName":"adhd11-1.translated-1.translated.doc","UserName":"وحید","ReqNo":"993"}', NULL, '2016-06-06 20:15:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4195, 16341, '', '{"FileName":"MIS-Weeks3.pdf","UserName":"سعید","ReqNo":"995"}', NULL, '2016-06-06 22:14:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4197, 16341, '', '{"FileName":"MIS-Weeks3.pdf","UserName":"سعید","ReqNo":"995"}', NULL, '2016-06-06 22:15:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4199, 16341, '', '{"FileName":"MIS-Weeks4.pdf","UserName":"سعید","ReqNo":"997"}', NULL, '2016-06-06 22:15:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4201, 16341, '', '{"FileName":"MIS-Weeks5.pdf","UserName":"سعید","ReqNo":"999"}', NULL, '2016-06-06 22:15:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4203, 16341, '', '{"FileName":"MIS-Weeks6.pdf","UserName":"سعید","ReqNo":"1001"}', NULL, '2016-06-06 22:16:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4205, 16341, '', '{"FileName":"MIS-Weeks4.pdf","UserName":"سعید","ReqNo":"997"}', NULL, '2016-06-06 22:16:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4207, 16341, '', '{"FileName":"MIS-Weeks5.pdf","UserName":"سعید","ReqNo":"999"}', NULL, '2016-06-06 22:16:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4209, 16341, '', '{"FileName":"MIS-Weeks7.pdf","UserName":"سعید","ReqNo":"1003"}', NULL, '2016-06-06 22:16:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4211, 16341, '', '{"FileName":"MIS-Weeks6.pdf","UserName":"سعید","ReqNo":"1001"}', NULL, '2016-06-06 22:17:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4213, 16341, '', '{"FileName":"MIS-Weeks7.pdf","UserName":"سعید","ReqNo":"1003"}', NULL, '2016-06-06 22:17:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4215, 16423, '', '{"FileName":"Trinidad and Tobago1.pdf","UserName":"فاطمه","ReqNo":"1005"}', NULL, '2016-06-07 15:22:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4217, 16423, '', '{"FileName":"Trinidad and Tobago2.pdf","UserName":"فاطمه","ReqNo":"1007"}', NULL, '2016-06-07 15:25:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4219, 16423, '', '{"FileName":"Trinidad and Tobago1.pdf","UserName":"فاطمه","ReqNo":"1005"}', NULL, '2016-06-07 16:00:10', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4221, 16423, '', '{"FileName":"Trinidad and Tobago2.pdf","UserName":"فاطمه","ReqNo":"1007"}', NULL, '2016-06-07 16:00:15', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4223, 16341, '', '{"FileName":"MIS-Weeks1-2.pdf","UserName":"سعید","ReqNo":"1009"}', NULL, '2016-06-08 09:53:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4225, 16341, '', '{"FileName":"MIS-Weeks1-2.pdf","UserName":"سعید","ReqNo":"1009"}', NULL, '2016-06-08 10:00:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4227, 16511, '', '{"FileName":"9780415428668_The_Archaeology_of_Celtic_Art.pdf","UserName":"کامی","ReqNo":"1011"}', NULL, '2016-06-08 14:09:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4229, 16511, '', '{"FileName":"9781569762820_The_Paper_Boomerang_Book.pdf","UserName":"کامی","ReqNo":"1013"}', NULL, '2016-06-08 14:15:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4231, 16511, '', '{"FileName":"9780823016761_Knit_with_Beads.pdf","UserName":"کامی","ReqNo":"1015"}', NULL, '2016-06-08 14:22:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4233, 16511, '', '{"FileName":"9780415428668_The_Archaeology_of_Celtic_Art.pdf","UserName":"کامی","ReqNo":"1011"}', NULL, '2016-06-08 14:43:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4235, 16511, '', '{"FileName":"9781569762820_The_Paper_Boomerang_Book.pdf","UserName":"کامی","ReqNo":"1013"}', NULL, '2016-06-08 14:52:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4237, 16511, '', '{"FileName":"9780823016761_Knit_with_Beads.pdf","UserName":"کامی","ReqNo":"1015"}', NULL, '2016-06-08 14:52:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4239, 16511, '', '{"FileName":"9780811735797_Concrete_Crafts.pdf","UserName":"کامی","ReqNo":"1017"}', NULL, '2016-06-08 15:14:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4241, 16511, '', '{"FileName":"9780870407574_Paper_Magic.pdf","UserName":"کامی","ReqNo":"1019"}', NULL, '2016-06-08 15:20:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4243, 16511, '', '{"FileName":"9780811735797_Concrete_Crafts.pdf","UserName":"کامی","ReqNo":"1017"}', NULL, '2016-06-08 16:09:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4245, 16511, '', '{"FileName":"9780870407574_Paper_Magic.pdf","UserName":"کامی","ReqNo":"1019"}', NULL, '2016-06-08 16:09:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4247, 16511, '', '{"FileName":"9780892367580_The_Arts_of_Fire.pdf","UserName":"کامی","ReqNo":"1021"}', NULL, '2016-06-08 16:27:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4249, 16511, '', '{"FileName":"9780895562586_Amazing_Sculpture_You_Can_Do.pdf","UserName":"کامی","ReqNo":"1023"}', NULL, '2016-06-08 16:29:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4251, 16511, '', '{"FileName":"9780892367580_The_Arts_of_Fire.pdf","UserName":"کامی","ReqNo":"1021"}', NULL, '2016-06-08 16:30:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4253, 16511, '', '{"FileName":"9781111538910_Contemporary_Color.pdf","UserName":"کامی","ReqNo":"1025"}', NULL, '2016-06-08 16:31:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4255, 16511, '', '{"FileName":"9780895562586_Amazing_Sculpture_You_Can_Do.pdf","UserName":"کامی","ReqNo":"1023"}', NULL, '2016-06-08 16:36:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4257, 16511, '', '{"FileName":"9781461414087-c1.pdf","UserName":"کامی","ReqNo":"1027"}', NULL, '2016-06-08 16:39:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4259, 16511, '', '{"FileName":"9781580173667_The_Candlemaker\'s_Companion.pdf","UserName":"کامی","ReqNo":"1029"}', NULL, '2016-06-08 16:41:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4261, 16511, '', '{"FileName":"9781111538910_Contemporary_Color.pdf","UserName":"کامی","ReqNo":"1025"}', NULL, '2016-06-08 17:05:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4263, 16511, '', '{"FileName":"9781461414087-c1.pdf","UserName":"کامی","ReqNo":"1027"}', NULL, '2016-06-08 18:00:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4265, 16511, '', '{"FileName":"9781580173667_The_Candlemaker\'s_Companion.pdf","UserName":"کامی","ReqNo":"1029"}', NULL, '2016-06-08 18:03:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4267, 16511, '', '{"FileName":"9781596355873_Clean_and_Simple_Cards.pdf","UserName":"کامی","ReqNo":"1031"}', NULL, '2016-06-08 18:11:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4269, 16511, '', '{"FileName":"9781895688481_I_Can_Make_Nature_Crafts.pdf","UserName":"کامی","ReqNo":"1033"}', NULL, '2016-06-08 18:12:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4271, 16511, '', '{"FileName":"9784889960853_Polyhedron_Origami_for_Beginners.pdf","UserName":"کامی","ReqNo":"1035"}', NULL, '2016-06-08 18:18:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4273, 16511, '', '{"FileName":"9781596355873_Clean_and_Simple_Cards.pdf","UserName":"کامی","ReqNo":"1031"}', NULL, '2016-06-08 18:18:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4275, 16511, '', '{"FileName":"9781895688481_I_Can_Make_Nature_Crafts.pdf","UserName":"کامی","ReqNo":"1033"}', NULL, '2016-06-08 18:19:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4277, 16511, '', '{"FileName":"9784889960853_Polyhedron_Origami_for_Beginners.pdf","UserName":"کامی","ReqNo":"1035"}', NULL, '2016-06-08 18:19:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4279, 16511, '', '{"FileName":"9784889962055_Unit_Polyhedron_Origami.pdf","UserName":"کامی","ReqNo":"1037"}', NULL, '2016-06-08 18:22:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4281, 16511, '', '{"FileName":"9784889962055_Unit_Polyhedron_Origami.pdf","UserName":"کامی","ReqNo":"1037"}', NULL, '2016-06-08 18:23:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4283, 16511, '', '{"FileName":"Accordion-Trees-Template.pdf","UserName":"کامی","ReqNo":"1039"}', NULL, '2016-06-08 18:24:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4285, 16511, '', '{"FileName":"Accordion-Trees-Template.pdf","UserName":"کامی","ReqNo":"1039"}', NULL, '2016-06-08 18:25:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4287, 16511, '', '{"FileName":"art_and_craft_of_handmade_books.pdf","UserName":"کامی","ReqNo":"1041"}', NULL, '2016-06-08 18:28:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4289, 16511, '', '{"FileName":"art_and_craft_of_handmade_books.pdf","UserName":"کامی","ReqNo":"1041"}', NULL, '2016-06-08 18:29:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4291, 16511, '', '{"FileName":"Article_19.pdf","UserName":"کامی","ReqNo":"1043"}', NULL, '2016-06-08 18:33:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4293, 16511, '', '{"FileName":"ASC 2008-BIM Benefits.pdf","UserName":"کامی","ReqNo":"1045"}', NULL, '2016-06-08 18:33:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4295, 16511, '', '{"FileName":"Article_19.pdf","UserName":"کامی","ReqNo":"1043"}', NULL, '2016-06-08 18:34:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4297, 16511, '', '{"FileName":"ASC 2008-BIM Benefits.pdf","UserName":"کامی","ReqNo":"1045"}', NULL, '2016-06-08 18:34:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4299, 16511, '', '{"FileName":"dalypapercraft1.pdf","UserName":"کامی","ReqNo":"1047"}', NULL, '2016-06-08 18:35:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4301, 16511, '', '{"FileName":"DesignNoirPlacebo.pdf","UserName":"کامی","ReqNo":"1049"}', NULL, '2016-06-08 18:35:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4303, 16511, '', '{"FileName":"dalypapercraft1.pdf","UserName":"کامی","ReqNo":"1047"}', NULL, '2016-06-08 18:40:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4305, 16511, '', '{"FileName":"DesignNoirPlacebo.pdf","UserName":"کامی","ReqNo":"1049"}', NULL, '2016-06-08 18:40:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4307, 16511, '', '{"FileName":"EyeShapedIt.pdf","UserName":"کامی","ReqNo":"1051"}', NULL, '2016-06-08 18:48:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4309, 16511, '', '{"FileName":"EyeShapedIt.pdf","UserName":"کامی","ReqNo":"1051"}', NULL, '2016-06-08 18:50:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4311, 16511, '', '{"FileName":"Fish and Hat Origami.pdf","UserName":"کامی","ReqNo":"1053"}', NULL, '2016-06-08 18:52:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4313, 16511, '', '{"FileName":"FeatherInlays.pdf","UserName":"کامی","ReqNo":"1055"}', NULL, '2016-06-08 18:52:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4315, 16511, '', '{"FileName":"Gift_Box_Sashes.pdf","UserName":"کامی","ReqNo":"1057"}', NULL, '2016-06-08 18:53:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4317, 16511, '', '{"FileName":"Fish and Hat Origami.pdf","UserName":"کامی","ReqNo":"1053"}', NULL, '2016-06-08 18:54:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4319, 16511, '', '{"FileName":"FeatherInlays.pdf","UserName":"کامی","ReqNo":"1055"}', NULL, '2016-06-08 18:54:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4321, 16511, '', '{"FileName":"Gift_Box_Sashes.pdf","UserName":"کامی","ReqNo":"1057"}', NULL, '2016-06-08 18:55:08', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4323, 16511, '', '{"FileName":"Gift_Box_Sashes.pdf","UserName":"کامی","ReqNo":"1059"}', NULL, '2016-06-08 18:58:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4325, 16511, '', '{"FileName":"Gift_Box_Sashes.pdf","UserName":"کامی","ReqNo":"1059"}', NULL, '2016-06-08 18:59:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4327, 16511, '', '{"FileName":"HenryHat.pdf","UserName":"کامی","ReqNo":"1061"}', NULL, '2016-06-08 18:59:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4329, 16511, '', '{"FileName":"kassala.pdf","UserName":"کامی","ReqNo":"1063"}', NULL, '2016-06-08 19:30:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4331, 16511, '', '{"FileName":"Low-cost_3D_printing_screen.pdf","UserName":"کامی","ReqNo":"1065"}', NULL, '2016-06-08 19:47:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4333, 16511, '', '{"FileName":"HenryHat.pdf","UserName":"کامی","ReqNo":"1061"}', NULL, '2016-06-08 20:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4335, 16511, '', '{"FileName":"kassala.pdf","UserName":"کامی","ReqNo":"1063"}', NULL, '2016-06-08 20:00:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4337, 16511, '', '{"FileName":"Low-cost_3D_printing_screen.pdf","UserName":"کامی","ReqNo":"1065"}', NULL, '2016-06-08 20:29:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4339, 16511, '', '{"FileName":"Origami.pdf","UserName":"کامی","ReqNo":"1067"}', NULL, '2016-06-08 20:31:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4341, 16511, '', '{"FileName":"Origami.pdf","UserName":"کامی","ReqNo":"1067"}', NULL, '2016-06-08 20:34:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4343, 14385, '', '{"FileName":"MPQSF-40anddirections[1] (1) (2).doc","UserName":"نادیا","ReqNo":"1069"}', NULL, '2016-06-09 09:30:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4345, 14385, '', '{"FileName":"MPQSF-40anddirections[1] (1) (2).doc","UserName":"نادیا","ReqNo":"1071"}', NULL, '2016-06-09 09:31:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4347, 14385, '', '{"FileName":"MPQSF-40anddirections[1] (1) (2).doc","UserName":"نادیا","ReqNo":"1069"}', NULL, '2016-06-09 10:00:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4349, 14385, '', '{"FileName":"MPQSF-40anddirections[1] (1) (2).doc","UserName":"نادیا","ReqNo":"1071"}', NULL, '2016-06-09 10:00:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4351, 16627, '', '{"FileName":"Scanning Quick Guide.pdf","UserName":"سعید","ReqNo":"1073"}', NULL, '2016-06-09 14:03:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4353, 16627, '', '{"FileName":"Scanning Quick Guide.pdf","UserName":"سعید","ReqNo":"1073"}', NULL, '2016-06-09 14:03:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4355, 16713, '', '{"FileName":"Enhancing community detection using a network weighting strategy1.pdf","UserName":"ایرج","ReqNo":"1075"}', NULL, '2016-06-10 12:27:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4357, 16713, '', '{"FileName":"Enhancing community detection using a network weighting strategy1.pdf","UserName":"ایرج","ReqNo":"1075"}', NULL, '2016-06-10 12:28:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4359, 16739, '', '{"FileName":"Data Mining.docx","UserName":"مهریار","ReqNo":"1077"}', NULL, '2016-06-10 15:40:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4361, 16739, '', '{"FileName":"Data Mining.docx","UserName":"مهریار","ReqNo":"1077"}', NULL, '2016-06-10 16:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4363, 16741, '', '{"FileName":"12.docx","UserName":"زهرا","ReqNo":"1079"}', NULL, '2016-06-10 16:41:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4365, 16741, '', '{"FileName":"12.docx","UserName":"زهرا","ReqNo":"1079"}', NULL, '2016-06-10 16:42:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4367, 16803, '', '{"FileName":"yokoyama2015.pdf","UserName":"زهرا","ReqNo":"1081"}', NULL, '2016-06-11 13:09:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4369, 16803, '', '{"FileName":"yokoyama2015.pdf","UserName":"زهرا","ReqNo":"1081"}', NULL, '2016-06-11 14:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4371, 16819, '', '{"FileName":"34375-34375.pdf","UserName":"علی","ReqNo":"1083"}', NULL, '2016-06-11 14:47:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4373, 16819, '', '{"FileName":"34375-34375.pdf","UserName":"علی","ReqNo":"1083"}', NULL, '2016-06-11 14:48:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4375, 16913, '', '{"FileName":"tarjome.docx","UserName":"ولی","ReqNo":"1085"}', NULL, '2016-06-12 09:26:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4377, 16913, '', '{"FileName":"tarjome.docx","UserName":"ولی","ReqNo":"1085"}', NULL, '2016-06-12 10:00:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4379, 16915, '', '{"FileName":"Camera Lenses.docx","UserName":"amene","ReqNo":"1087"}', NULL, '2016-06-12 10:29:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4381, 16915, '', '{"FileName":"Camera Lenses.docx","UserName":"amene","ReqNo":"1087"}', NULL, '2016-06-12 10:30:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4383, 16931, '', '{"FileName":"Research About Optimization Of Campus Network Security   System.pdf","UserName":"مجید","ReqNo":"1089"}', NULL, '2016-06-12 11:45:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4385, 16931, '', '{"FileName":"Research About Optimization Of Campus Network Security   System.pdf","UserName":"مجید","ReqNo":"1089"}', NULL, '2016-06-12 12:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4387, 16915, '', '{"FileName":"Camera Lenses.docx","UserName":"amene","ReqNo":"1091"}', NULL, '2016-06-12 12:01:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4389, 16915, '', '{"FileName":"Camera Lenses.docx","UserName":"amene","ReqNo":"1091"}', NULL, '2016-06-12 12:02:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4391, 17077, '', '{"FileName":"4_893037448752267315.pdf","UserName":"mani","ReqNo":"1093"}', NULL, '2016-06-13 14:47:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4393, 17077, '', '{"FileName":"4_893037448752267315.pdf","UserName":"mani","ReqNo":"1093"}', NULL, '2016-06-13 14:52:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4395, 16005, '', '{"FileName":"4_500999057380999293.pdf","UserName":"وحید","ReqNo":"1095"}', NULL, '2016-06-13 15:48:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4397, 16005, '', '{"FileName":"4_500999057380999293.pdf","UserName":"وحید","ReqNo":"1095"}', NULL, '2016-06-13 16:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4399, 19, '', '{"FileName":"Estekhdam.odt","UserName":"سید محمد","ReqNo":"1097"}', NULL, '2016-06-14 18:47:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4401, 19, '', '{"FileName":"Estekhdam.odt","UserName":"سید محمد","ReqNo":"1097"}', NULL, '2016-06-14 18:48:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4403, 17345, '', '{"FileName":"0321863852.pdf","UserName":"احمد","ReqNo":"1099"}', NULL, '2016-06-15 23:51:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4405, 15641, '', '{"FileName":"314056_90731.pdf","UserName":"مونا","ReqNo":"1101"}', NULL, '2016-06-15 23:54:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4407, 17345, '', '{"FileName":"0321863852.pdf","UserName":"احمد","ReqNo":"1099"}', NULL, '2016-06-16 00:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4409, 15641, '', '{"FileName":"314056_90731.pdf","UserName":"مونا","ReqNo":"1101"}', NULL, '2016-06-16 00:00:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4411, 15641, '', '{"FileName":"Adaptive_dimension_reduction_for_clustering_high_d (1).pdf","UserName":"مونا","ReqNo":"1103"}', NULL, '2016-06-16 00:40:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4413, 15641, '', '{"FileName":"Adaptive_dimension_reduction_for_clustering_high_d (1).pdf","UserName":"مونا","ReqNo":"1103"}', NULL, '2016-06-16 00:41:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4415, 17365, '', '{"FileName":"پروژه استاد صافی.pdf","UserName":"محمد","ReqNo":"1105"}', NULL, '2016-06-16 06:00:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4417, 17365, '', '{"FileName":"پروژه استاد صافی.pdf","UserName":"محمد","ReqNo":"1105"}', NULL, '2016-06-16 06:01:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4419, 17385, '', '{"FileName":"Software_Requirement_Patterns.pdf","UserName":"علی","ReqNo":"1107"}', NULL, '2016-06-16 11:56:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4421, 17385, '', '{"FileName":"Software_Requirement_Patterns.pdf","UserName":"علی","ReqNo":"1109"}', NULL, '2016-06-16 12:11:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4423, 17385, '', '{"FileName":"mas wooldridge.pdf","UserName":"علی","ReqNo":"1111"}', NULL, '2016-06-16 12:15:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4425, 17385, '', '{"FileName":"Software_Requirement_Patterns.pdf","UserName":"علی","ReqNo":"1107"}', NULL, '2016-06-16 12:33:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4427, 17385, '', '{"FileName":"Software_Requirement_Patterns.pdf","UserName":"علی","ReqNo":"1109"}', NULL, '2016-06-16 13:07:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4429, 17385, '', '{"FileName":"mas wooldridge.pdf","UserName":"علی","ReqNo":"1111"}', NULL, '2016-06-16 13:40:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4431, 17437, '', '{"FileName":"leardeship theory 2.pdf","UserName":"ملیحه","ReqNo":"1113"}', NULL, '2016-06-16 18:01:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4433, 17437, '', '{"FileName":"leardeship theory 2.pdf","UserName":"ملیحه","ReqNo":"1113"}', NULL, '2016-06-16 18:02:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4435, 17437, '', '{"FileName":"leardeship theory 2.pdf","UserName":"ملیحه","ReqNo":"1115"}', NULL, '2016-06-16 18:12:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4437, 17437, '', '{"FileName":"leardeship theory 2.pdf","UserName":"ملیحه","ReqNo":"1115"}', NULL, '2016-06-16 18:13:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4439, 17437, '', '{"FileName":"leadership style 3.pdf","UserName":"ملیحه","ReqNo":"1117"}', NULL, '2016-06-16 20:11:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4441, 17437, '', '{"FileName":"leadership style 3.pdf","UserName":"ملیحه","ReqNo":"1117"}', NULL, '2016-06-16 20:12:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4443, 17437, '', '{"FileName":"leadership style 3.pdf","UserName":"ملیحه","ReqNo":"1119"}', NULL, '2016-06-16 21:23:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4445, 17437, '', '{"FileName":"leadership style 3.pdf","UserName":"ملیحه","ReqNo":"1119"}', NULL, '2016-06-16 22:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4447, 17437, '', '{"FileName":"leardeship theory 2.pdf","UserName":"ملیحه","ReqNo":"1121"}', NULL, '2016-06-16 22:42:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4449, 17437, '', '{"FileName":"leardeship theory 2.pdf","UserName":"ملیحه","ReqNo":"1121"}', NULL, '2016-06-16 22:43:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4451, 17481, '', '{"FileName":"1-s2.0-S0370269316301940-main.pdf","UserName":"سامان","ReqNo":"1123"}', NULL, '2016-06-17 00:40:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4453, 15641, '', '{"FileName":"1روش آماری آموزش برای طولی بالا بعدی.pdf","UserName":"مونا","ReqNo":"1125"}', NULL, '2016-06-17 05:05:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4455, 15641, '', '{"FileName":"2خوشه بندی داده های طولی.pdf","UserName":"مونا","ReqNo":"1127"}', NULL, '2016-06-17 05:05:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4457, 15641, '', '{"FileName":"تشخیص با نفوذ.pdf","UserName":"مونا","ReqNo":"1129"}', NULL, '2016-06-17 05:05:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4459, 15641, '', '{"FileName":"خوشه بندی جريان داده ها با استفاده از joinpoint.pdf","UserName":"مونا","ReqNo":"1131"}', NULL, '2016-06-17 05:06:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4461, 15641, '', '{"FileName":"خوشه بندی جريان داده ها با استفاده از joinpoint.pdf","UserName":"مونا","ReqNo":"1133"}', NULL, '2016-06-17 05:06:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4463, 15641, '', '{"FileName":"گروه روش خوشه بندی بر اساس.pdf","UserName":"مونا","ReqNo":"1135"}', NULL, '2016-06-17 05:06:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4465, 17503, '', '{"FileName":"lol.pdf","UserName":"محمد رضا","ReqNo":"1137"}', NULL, '2016-06-17 11:54:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4467, 17437, '', '{"FileName":"leadership style 3.pdf","UserName":"ملیحه","ReqNo":"1139"}', NULL, '2016-06-17 18:44:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4469, 17563, '', '{"FileName":"marjan.docx","UserName":"مرجان","ReqNo":"1141"}', NULL, '2016-06-17 22:33:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4471, 17481, '', '{"FileName":"1-s2.0-S0370269316301940-main.pdf","UserName":"سامان","ReqNo":"1143"}', NULL, '2016-06-18 11:12:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4473, 17609, '', '{"FileName":"New Microsoft Word Document.pdf","UserName":"yasaman","ReqNo":"1145"}', NULL, '2016-06-18 13:00:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4475, 17481, '', '{"FileName":"1-s2.0-S0370269316301940-main.pdf","UserName":"سامان","ReqNo":"1123"}', NULL, '2016-06-18 15:37:44', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4477, 15641, '', '{"FileName":"1روش آماری آموزش برای طولی بالا بعدی.pdf","UserName":"مونا","ReqNo":"1125"}', NULL, '2016-06-18 15:37:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4479, 15641, '', '{"FileName":"2خوشه بندی داده های طولی.pdf","UserName":"مونا","ReqNo":"1127"}', NULL, '2016-06-18 15:37:45', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4481, 15641, '', '{"FileName":"تشخیص با نفوذ.pdf","UserName":"مونا","ReqNo":"1129"}', NULL, '2016-06-18 15:37:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4483, 15641, '', '{"FileName":"خوشه بندی جريان داده ها با استفاده از joinpoint.pdf","UserName":"مونا","ReqNo":"1131"}', NULL, '2016-06-18 15:37:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4485, 15641, '', '{"FileName":"خوشه بندی جريان داده ها با استفاده از joinpoint.pdf","UserName":"مونا","ReqNo":"1133"}', NULL, '2016-06-18 15:37:49', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4487, 15641, '', '{"FileName":"گروه روش خوشه بندی بر اساس.pdf","UserName":"مونا","ReqNo":"1135"}', NULL, '2016-06-18 15:37:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4489, 17503, '', '{"FileName":"lol.pdf","UserName":"محمد رضا","ReqNo":"1137"}', NULL, '2016-06-18 15:37:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4491, 17437, '', '{"FileName":"leadership style 3.pdf","UserName":"ملیحه","ReqNo":"1139"}', NULL, '2016-06-18 15:37:51', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4493, 17563, '', '{"FileName":"marjan.docx","UserName":"مرجان","ReqNo":"1141"}', NULL, '2016-06-18 15:37:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4495, 17481, '', '{"FileName":"1-s2.0-S0370269316301940-main.pdf","UserName":"سامان","ReqNo":"1143"}', NULL, '2016-06-18 15:37:53', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4497, 17609, '', '{"FileName":"New Microsoft Word Document.pdf","UserName":"yasaman","ReqNo":"1145"}', NULL, '2016-06-18 15:38:14', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4499, 17661, '', '{"FileName":"DJHTP.pdf","UserName":"mahdi","ReqNo":"1147"}', NULL, '2016-06-19 06:55:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4501, 17661, '', '{"FileName":"DJHTP.pdf","UserName":"mahdi","ReqNo":"1147"}', NULL, '2016-06-19 06:59:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4503, 17751, '', '{"FileName":"A11ENGLISH.pdf","UserName":"حسین","ReqNo":"1149"}', NULL, '2016-06-20 08:35:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4505, 17751, '', '{"FileName":"A11ENGLISH.pdf","UserName":"حسین","ReqNo":"1151"}', NULL, '2016-06-20 08:36:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4507, 17751, '', '{"FileName":"A11ENGLISH.pdf","UserName":"حسین","ReqNo":"1149"}', NULL, '2016-06-20 08:36:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4509, 17751, '', '{"FileName":"A11ENGLISH.pdf","UserName":"حسین","ReqNo":"1151"}', NULL, '2016-06-20 08:37:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4511, 17751, '', '{"FileName":"A11Manual.pdf","UserName":"حسین","ReqNo":"1153"}', NULL, '2016-06-20 08:48:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4513, 17751, '', '{"FileName":"A11Manual.pdf","UserName":"حسین","ReqNo":"1153"}', NULL, '2016-06-20 08:49:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4515, 17751, '', '{"FileName":"A18Manual.pdf","UserName":"حسین","ReqNo":"1155"}', NULL, '2016-06-20 12:32:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4517, 17751, '', '{"FileName":"A18Manual.pdf","UserName":"حسین","ReqNo":"1155"}', NULL, '2016-06-20 12:33:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4519, 17799, '', '{"FileName":"P10011.pdf","UserName":"elahe","ReqNo":"1157"}', NULL, '2016-06-20 15:06:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4521, 17799, '', '{"FileName":"P10011.pdf","UserName":"elahe","ReqNo":"1157"}', NULL, '2016-06-20 16:00:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4523, 17805, '', '{"FileName":"1-s2.0-S1877042815053859-main.docx","UserName":"محمد","ReqNo":"1159"}', NULL, '2016-06-20 17:00:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4525, 17805, '', '{"FileName":"1-s2.0-S0301421514000251-main.docx","UserName":"محمد","ReqNo":"1161"}', NULL, '2016-06-20 17:04:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4527, 17805, '', '{"FileName":"1-s2.0-S1877042815053859-main.docx","UserName":"محمد","ReqNo":"1159"}', NULL, '2016-06-20 18:00:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4529, 17805, '', '{"FileName":"1-s2.0-S0301421514000251-main.docx","UserName":"محمد","ReqNo":"1161"}', NULL, '2016-06-20 18:00:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4531, 17799, '', '{"FileName":"P10011.pdf","UserName":"elahe","ReqNo":"1163"}', NULL, '2016-06-21 13:02:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4533, 17799, '', '{"FileName":"14042076.pdf","UserName":"elahe","ReqNo":"1165"}', NULL, '2016-06-21 13:13:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4535, 17799, '', '{"FileName":"Paper 16-A Survey on Resource Allocation Strategie.pdf","UserName":"elahe","ReqNo":"1167"}', NULL, '2016-06-21 13:13:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4537, 17799, '', '{"FileName":"ijcsit2015060413.pdf","UserName":"elahe","ReqNo":"1169"}', NULL, '2016-06-21 13:14:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4539, 17799, '', '{"FileName":"AntonPhDThesis2013.pdf","UserName":"elahe","ReqNo":"1171"}', NULL, '2016-06-21 13:14:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4541, 17799, '', '{"FileName":"P10011.pdf","UserName":"elahe","ReqNo":"1163"}', NULL, '2016-06-21 14:00:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4543, 17799, '', '{"FileName":"14042076.pdf","UserName":"elahe","ReqNo":"1165"}', NULL, '2016-06-21 14:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4545, 17799, '', '{"FileName":"Paper 16-A Survey on Resource Allocation Strategie.pdf","UserName":"elahe","ReqNo":"1167"}', NULL, '2016-06-21 14:01:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4547, 17799, '', '{"FileName":"ijcsit2015060413.pdf","UserName":"elahe","ReqNo":"1169"}', NULL, '2016-06-21 14:02:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4549, 17799, '', '{"FileName":"AntonPhDThesis2013.pdf","UserName":"elahe","ReqNo":"1171"}', NULL, '2016-06-21 14:30:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4551, 17905, '', '{"FileName":"همزمان با افزایش جمعیت.pdf","UserName":"مرجان","ReqNo":"1173"}', NULL, '2016-06-21 16:26:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4553, 17905, '', '{"FileName":"همزمان با افزایش جمعیت.pdf","UserName":"مرجان","ReqNo":"1173"}', NULL, '2016-06-21 16:27:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4555, 17907, '', '{"FileName":"4.pdf","UserName":"ali","ReqNo":"1175"}', NULL, '2016-06-21 16:32:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4557, 17907, '', '{"FileName":"2.pdf","UserName":"ali","ReqNo":"1177"}', NULL, '2016-06-21 16:32:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4559, 17907, '', '{"FileName":"3.pdf","UserName":"ali","ReqNo":"1179"}', NULL, '2016-06-21 16:33:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4561, 17907, '', '{"FileName":"4.pdf","UserName":"ali","ReqNo":"1175"}', NULL, '2016-06-21 16:33:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4563, 17907, '', '{"FileName":"19.pdf","UserName":"ali","ReqNo":"1181"}', NULL, '2016-06-21 16:33:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4565, 17907, '', '{"FileName":"2.pdf","UserName":"ali","ReqNo":"1177"}', NULL, '2016-06-21 16:33:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4567, 17907, '', '{"FileName":"20.pdf","UserName":"ali","ReqNo":"1183"}', NULL, '2016-06-21 16:33:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4569, 17907, '', '{"FileName":"3.pdf","UserName":"ali","ReqNo":"1179"}', NULL, '2016-06-21 16:34:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4571, 17907, '', '{"FileName":"اثربخشی الگوی طراحی.pdf","UserName":"ali","ReqNo":"1185"}', NULL, '2016-06-21 16:34:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4573, 17907, '', '{"FileName":"19.pdf","UserName":"ali","ReqNo":"1181"}', NULL, '2016-06-21 16:34:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4575, 17907, '', '{"FileName":"20.pdf","UserName":"ali","ReqNo":"1183"}', NULL, '2016-06-21 16:34:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4577, 17907, '', '{"FileName":"آزمایش تکرار در الگوی معماری.pdf","UserName":"ali","ReqNo":"1187"}', NULL, '2016-06-21 16:34:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4579, 17907, '', '{"FileName":"اثربخشی الگوی طراحی.pdf","UserName":"ali","ReqNo":"1185"}', NULL, '2016-06-21 16:35:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4581, 17907, '', '{"FileName":"آزمایش تکرار در الگوی معماری.pdf","UserName":"ali","ReqNo":"1187"}', NULL, '2016-06-21 16:35:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4583, 17907, '', '{"FileName":"الگوهای طراحی نرم افزار.pdf","UserName":"ali","ReqNo":"1189"}', NULL, '2016-06-21 16:36:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4585, 17907, '', '{"FileName":"الگوی طراحی.pdf","UserName":"ali","ReqNo":"1191"}', NULL, '2016-06-21 16:36:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4587, 17907, '', '{"FileName":"اندازه  گیری کیفیت طراحی.pdf","UserName":"ali","ReqNo":"1193"}', NULL, '2016-06-21 16:37:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4589, 17907, '', '{"FileName":"پیشرفت ها و چالش های مهندسی نرم افزار.pdf","UserName":"ali","ReqNo":"1195"}', NULL, '2016-06-21 16:37:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4591, 17907, '', '{"FileName":"الگوهای طراحی نرم افزار.pdf","UserName":"ali","ReqNo":"1189"}', NULL, '2016-06-21 16:37:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4593, 17907, '', '{"FileName":"الگوی طراحی.pdf","UserName":"ali","ReqNo":"1191"}', NULL, '2016-06-21 16:37:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4595, 17907, '', '{"FileName":"پیشرفت ها و چالش های مهندسی نرم افزار.pdf","UserName":"ali","ReqNo":"1197"}', NULL, '2016-06-21 16:37:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4597, 17907, '', '{"FileName":"اندازه  گیری کیفیت طراحی.pdf","UserName":"ali","ReqNo":"1193"}', NULL, '2016-06-21 16:38:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4599, 17907, '', '{"FileName":"پیشرفت ها و چالش های مهندسی نرم افزار.pdf","UserName":"ali","ReqNo":"1195"}', NULL, '2016-06-21 16:38:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4601, 17907, '', '{"FileName":"پیشرفت ها و چالش های مهندسی نرم افزار.pdf","UserName":"ali","ReqNo":"1197"}', NULL, '2016-06-21 16:38:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4603, 17907, '', '{"FileName":"تجزیه تحلیل داده های مهندسی.pdf","UserName":"ali","ReqNo":"1199"}', NULL, '2016-06-21 16:38:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4605, 17907, '', '{"FileName":"تشخیص الگوهای طراحی.pdf","UserName":"ali","ReqNo":"1201"}', NULL, '2016-06-21 16:38:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4607, 17907, '', '{"FileName":"روش برای استخراج معماری نرم افزار از کیفیت مورد نیاز.pdf","UserName":"ali","ReqNo":"1203"}', NULL, '2016-06-21 16:39:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4609, 17907, '', '{"FileName":"فاهیم برای قطعی مهندسی نرم.pdf","UserName":"ali","ReqNo":"1205"}', NULL, '2016-06-21 16:39:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4611, 17907, '', '{"FileName":"تجزیه تحلیل داده های مهندسی.pdf","UserName":"ali","ReqNo":"1199"}', NULL, '2016-06-21 16:39:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4613, 17905, '', '{"FileName":"در آغاز بکار تولید سنگ های مصنوعی در ایران.pdf","UserName":"مرجان","ReqNo":"1207"}', NULL, '2016-06-21 16:39:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4615, 17907, '', '{"FileName":"تشخیص الگوهای طراحی.pdf","UserName":"ali","ReqNo":"1201"}', NULL, '2016-06-21 16:39:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4617, 17907, '', '{"FileName":"معماری محور تکامل نرم افزار الگوهای.pdf","UserName":"ali","ReqNo":"1209"}', NULL, '2016-06-21 16:39:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4619, 17907, '', '{"FileName":"معماری نرم افزار.pdf","UserName":"ali","ReqNo":"1211"}', NULL, '2016-06-21 16:40:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4621, 17907, '', '{"FileName":"روش برای استخراج معماری نرم افزار از کیفیت مورد نیاز.pdf","UserName":"ali","ReqNo":"1203"}', NULL, '2016-06-21 16:40:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4623, 17907, '', '{"FileName":"فاهیم برای قطعی مهندسی نرم.pdf","UserName":"ali","ReqNo":"1205"}', NULL, '2016-06-21 16:40:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4625, 17905, '', '{"FileName":"در آغاز بکار تولید سنگ های مصنوعی در ایران.pdf","UserName":"مرجان","ReqNo":"1207"}', NULL, '2016-06-21 16:40:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4627, 17907, '', '{"FileName":"معماری محور تکامل نرم افزار الگوهای.pdf","UserName":"ali","ReqNo":"1209"}', NULL, '2016-06-21 16:40:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4629, 17907, '', '{"FileName":"معماری و طراحی.pdf","UserName":"ali","ReqNo":"1213"}', NULL, '2016-06-21 16:40:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4631, 17907, '', '{"FileName":"مقاله_الگوهاي_تحليل_در_مهندسي_نرم_افزار.pdf","UserName":"ali","ReqNo":"1215"}', NULL, '2016-06-21 16:40:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4633, 17907, '', '{"FileName":"نرم افزار معماری مبانی، نظریه و تمرین.pdf","UserName":"ali","ReqNo":"1217"}', NULL, '2016-06-21 16:41:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4635, 17907, '', '{"FileName":"معماری نرم افزار.pdf","UserName":"ali","ReqNo":"1211"}', NULL, '2016-06-21 16:41:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4637, 17907, '', '{"FileName":"معماری و طراحی.pdf","UserName":"ali","ReqNo":"1213"}', NULL, '2016-06-21 16:41:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4639, 17907, '', '{"FileName":"مقاله_الگوهاي_تحليل_در_مهندسي_نرم_افزار.pdf","UserName":"ali","ReqNo":"1215"}', NULL, '2016-06-21 16:41:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4641, 17907, '', '{"FileName":"11201343457.pdf","UserName":"ali","ReqNo":"1219"}', NULL, '2016-06-21 16:41:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4643, 17907, '', '{"FileName":"25313820901.pdf","UserName":"ali","ReqNo":"1221"}', NULL, '2016-06-21 16:41:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4645, 17907, '', '{"FileName":"61013900301.pdf","UserName":"ali","ReqNo":"1223"}', NULL, '2016-06-21 16:42:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4647, 17907, '', '{"FileName":"ASE.1998.732576_f157ef7efeefe8936e3c776ca33f47fd.pdf","UserName":"ali","ReqNo":"1225"}', NULL, '2016-06-21 16:42:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4649, 17907, '', '{"FileName":"نرم افزار معماری مبانی، نظریه و تمرین.pdf","UserName":"ali","ReqNo":"1217"}', NULL, '2016-06-21 16:42:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4651, 17907, '', '{"FileName":"11201343457.pdf","UserName":"ali","ReqNo":"1219"}', NULL, '2016-06-21 16:42:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4653, 17907, '', '{"FileName":"COMPSAC.2010.53_e6e5620cbfd70439baf9a0495e125b4c.pdf","UserName":"ali","ReqNo":"1227"}', NULL, '2016-06-21 16:42:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4655, 17907, '', '{"FileName":"25313820901.pdf","UserName":"ali","ReqNo":"1221"}', NULL, '2016-06-21 16:42:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4657, 17907, '', '{"FileName":"CSAE.2011.5952926_7e01ef189bf44df73cb7ef3fcbad178f.pdf","UserName":"ali","ReqNo":"1229"}', NULL, '2016-06-21 16:42:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4659, 17907, '', '{"FileName":"61013900301.pdf","UserName":"ali","ReqNo":"1223"}', NULL, '2016-06-21 16:43:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4661, 17907, '', '{"FileName":"ASE.1998.732576_f157ef7efeefe8936e3c776ca33f47fd.pdf","UserName":"ali","ReqNo":"1225"}', NULL, '2016-06-21 16:43:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4663, 17907, '', '{"FileName":"FIE.2013.6685156_ebcd5452a1d25267007f389ec4c438b6.pdf","UserName":"ali","ReqNo":"1231"}', NULL, '2016-06-21 16:43:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4665, 17907, '', '{"FileName":"ic.2012.0017_c717ae595b5941ee8c7c288d881f0a62.pdf","UserName":"ali","ReqNo":"1233"}', NULL, '2016-06-21 16:43:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4667, 17907, '', '{"FileName":"RCIS.2009.5089271_444c6980e96f0da202d43ce7f30e6531.pdf","UserName":"ali","ReqNo":"1235"}', NULL, '2016-06-21 16:44:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4669, 17907, '', '{"FileName":"RE.2012.6345834_417e9fd37c88e3c685aa75cc17e3e287.pdf","UserName":"ali","ReqNo":"1237"}', NULL, '2016-06-21 16:44:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4671, 17907, '', '{"FileName":"SE2-session10-93.1.29.pdf","UserName":"ali","ReqNo":"1239"}', NULL, '2016-06-21 16:44:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4673, 17907, '', '{"FileName":"SERA.2008.24_240b0b1a7262524fd4602de1f6ac556b.pdf","UserName":"ali","ReqNo":"1241"}', NULL, '2016-06-21 16:44:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4675, 17907, '', '{"FileName":"1.pdf","UserName":"ali","ReqNo":"1243"}', NULL, '2016-06-21 16:45:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4677, 17907, '', '{"FileName":"COMPSAC.2010.53_e6e5620cbfd70439baf9a0495e125b4c.pdf","UserName":"ali","ReqNo":"1227"}', NULL, '2016-06-21 16:45:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4679, 17905, '', '{"FileName":"ترموفرمینگ یا شکلدهی گرمایی پلاستیک ها چیست.pdf","UserName":"مرجان","ReqNo":"1245"}', NULL, '2016-06-21 16:46:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4681, 17905, '', '{"FileName":"فرم دهی به ورقهای گرما نرم توسط حرارت مستقیم یا القائی وتخلیه هوای مابین ورق وقالب وشکل پذیری ورق هم","UserName":"مرجان","ReqNo":"1247"}', NULL, '2016-06-21 16:47:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4683, 17907, '', '{"FileName":"CSAE.2011.5952926_7e01ef189bf44df73cb7ef3fcbad178f.pdf","UserName":"ali","ReqNo":"1229"}', NULL, '2016-06-21 16:47:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4685, 17905, '', '{"FileName":"یکی از مهمترین قسمت های هر برج خنک کننده چیزی نیست جز پکینگ.pdf","UserName":"مرجان","ReqNo":"1249"}', NULL, '2016-06-21 16:47:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4687, 17907, '', '{"FileName":"FIE.2013.6685156_ebcd5452a1d25267007f389ec4c438b6.pdf","UserName":"ali","ReqNo":"1231"}', NULL, '2016-06-21 16:49:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4689, 17907, '', '{"FileName":"ic.2012.0017_c717ae595b5941ee8c7c288d881f0a62.pdf","UserName":"ali","ReqNo":"1233"}', NULL, '2016-06-21 16:50:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4691, 17907, '', '{"FileName":"RCIS.2009.5089271_444c6980e96f0da202d43ce7f30e6531.pdf","UserName":"ali","ReqNo":"1235"}', NULL, '2016-06-21 16:52:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4693, 17907, '', '{"FileName":"RE.2012.6345834_417e9fd37c88e3c685aa75cc17e3e287.pdf","UserName":"ali","ReqNo":"1237"}', NULL, '2016-06-21 16:53:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4695, 17907, '', '{"FileName":"SE2-session10-93.1.29.pdf","UserName":"ali","ReqNo":"1239"}', NULL, '2016-06-21 16:53:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4697, 17907, '', '{"FileName":"SERA.2008.24_240b0b1a7262524fd4602de1f6ac556b.pdf","UserName":"ali","ReqNo":"1241"}', NULL, '2016-06-21 16:56:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4699, 17907, '', '{"FileName":"1.pdf","UserName":"ali","ReqNo":"1243"}', NULL, '2016-06-21 16:57:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4701, 17905, '', '{"FileName":"ترموفرمینگ یا شکلدهی گرمایی پلاستیک ها چیست.pdf","UserName":"مرجان","ReqNo":"1245"}', NULL, '2016-06-21 16:57:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4703, 17905, '', '{"FileName":"فرم دهی به ورقهای گرما نرم توسط حرارت مستقیم یا القائی وتخلیه هوای مابین ورق وقالب وشکل پذیری ورق هم","UserName":"مرجان","ReqNo":"1247"}', NULL, '2016-06-21 16:57:20', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4705, 17905, '', '{"FileName":"یکی از مهمترین قسمت های هر برج خنک کننده چیزی نیست جز پکینگ.pdf","UserName":"مرجان","ReqNo":"1249"}', NULL, '2016-06-21 16:57:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4707, 17915, '', '{"FileName":"ISCEE_6843(1).pdf","UserName":"nesa","ReqNo":"1251"}', NULL, '2016-06-21 17:52:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4709, 17915, '', '{"FileName":"ISCEE_6843(1).pdf","UserName":"nesa","ReqNo":"1251"}', NULL, '2016-06-21 18:00:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4711, 18007, '', '{"FileName":"Lane_D._Pederson 11.pdf","UserName":"reza","ReqNo":"1253"}', NULL, '2016-06-22 11:27:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4713, 18007, '', '{"FileName":"Lane_D._Pederson 11.pdf","UserName":"reza","ReqNo":"1253"}', NULL, '2016-06-22 12:00:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4715, 18037, '', '{"FileName":"The evolving role of strategic.pdf","UserName":"عارف","ReqNo":"1255"}', NULL, '2016-06-22 15:13:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4717, 18037, '', '{"FileName":"The evolving role of strategic.pdf","UserName":"عارف","ReqNo":"1255"}', NULL, '2016-06-22 16:02:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4719, 15641, '', '{"FileName":"خوشه بندی جريان داده ها با استفاده از joinpoint.pdf","UserName":"مونا","ReqNo":"1257"}', NULL, '2016-06-22 17:49:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4721, 15641, '', '{"FileName":"خوشه بندی جريان داده ها با استفاده از joinpoint.pdf","UserName":"مونا","ReqNo":"1257"}', NULL, '2016-06-22 18:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4723, 18037, '', '{"FileName":"The evolving role of strategic.translated.doc","UserName":"عارف","ReqNo":"1259"}', NULL, '2016-06-23 04:37:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4725, 18037, '', '{"FileName":"The evolving role of strategic.translated.doc","UserName":"عارف","ReqNo":"1259"}', NULL, '2016-06-23 04:38:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4727, 18129, '', '{"FileName":"roudbari Motor Cooling.pdf","UserName":"میثم","ReqNo":"1261"}', NULL, '2016-06-23 15:36:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4729, 18129, '', '{"FileName":"roudbari Motor Cooling.pdf","UserName":"میثم","ReqNo":"1261"}', NULL, '2016-06-23 16:00:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4731, 18145, '', '{"FileName":"ahmadddddddddddd.pdf","UserName":"احمد رضا","ReqNo":"1263"}', NULL, '2016-06-23 20:10:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4733, 18145, '', '{"FileName":"ahmadddddddddddd.pdf","UserName":"احمد رضا","ReqNo":"1263"}', NULL, '2016-06-23 20:11:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4735, 18205, '', '{"FileName":"7 - Copy.doc","UserName":"زاهد","ReqNo":"1265"}', NULL, '2016-06-24 16:23:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4737, 18205, '', '{"FileName":"7 - Copy.doc","UserName":"زاهد","ReqNo":"1265"}', NULL, '2016-06-24 16:24:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4739, 18215, '', '{"FileName":"cavbubdynam_Zamani.pdf","UserName":"متین","ReqNo":"1267"}', NULL, '2016-06-24 20:36:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4741, 18215, '', '{"FileName":"cavbubdynam_Zamani.pdf","UserName":"متین","ReqNo":"1267"}', NULL, '2016-06-24 20:37:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4743, 18251, '', '{"FileName":"Python Machine Learning.pdf","UserName":"محمدرضا","ReqNo":"1269"}', NULL, '2016-06-25 08:44:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4745, 18251, '', '{"FileName":"Python Machine Learning.pdf","UserName":"محمدرضا","ReqNo":"1269"}', NULL, '2016-06-25 09:18:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4747, 18257, '', '{"FileName":"bizhub-c452-c552-c652.pdf","UserName":"شهرام","ReqNo":"1271"}', NULL, '2016-06-25 10:35:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4749, 18257, '', '{"FileName":"bizhub-c452-c552-c652.pdf","UserName":"شهرام","ReqNo":"1271"}', NULL, '2016-06-25 10:36:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4751, 18287, '', '{"FileName":"article2.pdf","UserName":"محمد","ReqNo":"1273"}', NULL, '2016-06-25 13:03:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4753, 18287, '', '{"FileName":"article2.pdf","UserName":"محمد","ReqNo":"1275"}', NULL, '2016-06-25 13:11:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4755, 18287, '', '{"FileName":"article2.pdf","UserName":"محمد","ReqNo":"1273"}', NULL, '2016-06-25 14:05:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4757, 18287, '', '{"FileName":"article2.pdf","UserName":"محمد","ReqNo":"1275"}', NULL, '2016-06-25 14:11:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4759, 18329, '', '{"FileName":"1.pdf","UserName":"fatemeh","ReqNo":"1277"}', NULL, '2016-06-25 16:56:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4761, 18329, '', '{"FileName":"1.pdf","UserName":"fatemeh","ReqNo":"1277"}', NULL, '2016-06-25 16:57:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4763, 18329, '', '{"FileName":"mentors.pdf","UserName":"fatemeh","ReqNo":"1279"}', NULL, '2016-06-25 16:58:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4765, 18329, '', '{"FileName":"mentors.pdf","UserName":"fatemeh","ReqNo":"1279"}', NULL, '2016-06-25 16:59:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4767, 14385, '', '{"FileName":"International Journal of Intercultural Relations 38.docx","UserName":"نادیا","ReqNo":"1281"}', NULL, '2016-06-25 23:16:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4769, 14385, '', '{"FileName":"International Journal of Intercultural Relations 38.docx","UserName":"نادیا","ReqNo":"1281"}', NULL, '2016-06-26 00:00:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4771, 18393, '', '{"FileName":"CEHv9 Module 11 Hacking Webservers.pdf","UserName":"سعید","ReqNo":"1283"}', NULL, '2016-06-26 08:39:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4773, 18393, '', '{"FileName":"CEHv9 Module 11 Hacking Webservers.pdf","UserName":"سعید","ReqNo":"1283"}', NULL, '2016-06-26 08:40:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4775, 18433, '', '{"FileName":"1.pdf","UserName":"امیر","ReqNo":"1285"}', NULL, '2016-06-26 16:01:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4777, 18433, '', '{"FileName":"1.pdf","UserName":"امیر","ReqNo":"1285"}', NULL, '2016-06-26 16:02:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4779, 18433, '', '{"FileName":"2.pdf","UserName":"امیر","ReqNo":"1287"}', NULL, '2016-06-26 16:03:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4781, 18433, '', '{"FileName":"2.pdf","UserName":"امیر","ReqNo":"1287"}', NULL, '2016-06-26 16:05:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4783, 18477, '', '{"FileName":"1 so-fa.pdf","UserName":"علیرضا","ReqNo":"1289"}', NULL, '2016-06-26 23:52:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4785, 14385, '', '{"FileName":"5 Planning goals and learning outcomes.docx","UserName":"نادیا","ReqNo":"1291"}', NULL, '2016-06-26 23:56:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4787, 18477, '', '{"FileName":"1 so-fa.pdf","UserName":"علیرضا","ReqNo":"1289"}', NULL, '2016-06-27 00:01:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4789, 14385, '', '{"FileName":"5 Planning goals and learning outcomes.docx","UserName":"نادیا","ReqNo":"1291"}', NULL, '2016-06-27 00:03:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4791, 17805, '', '{"FileName":"1-s2.0-S2212567116300259-main.docx","UserName":"محمد","ReqNo":"1293"}', NULL, '2016-06-27 11:52:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4793, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1295"}', NULL, '2016-06-27 11:56:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4795, 17805, '', '{"FileName":"1-s2.0-S2212567116300259-main.docx","UserName":"محمد","ReqNo":"1293"}', NULL, '2016-06-27 12:00:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4797, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1295"}', NULL, '2016-06-27 12:00:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4799, 18597, '', '{"FileName":"wileye205ed72-d785-20141207114232.pdf","UserName":"farzad","ReqNo":"1297"}', NULL, '2016-06-27 16:48:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4801, 18597, '', '{"FileName":"wileye205ed72-d785-20141207114232.pdf","UserName":"farzad","ReqNo":"1297"}', NULL, '2016-06-27 16:49:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4803, 18609, '', '{"FileName":"3134.pdf","UserName":"روح الله","ReqNo":"1299"}', NULL, '2016-06-27 16:51:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4805, 18609, '', '{"FileName":"3134.pdf","UserName":"روح الله","ReqNo":"1299"}', NULL, '2016-06-27 16:52:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4807, 18631, '', '{"FileName":"چکیده.docx","UserName":"کبری","ReqNo":"1301"}', NULL, '2016-06-27 19:30:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4809, 18631, '', '{"FileName":"چکیده.docx","UserName":"کبری","ReqNo":"1301"}', NULL, '2016-06-27 20:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4811, 18393, '', '{"FileName":"CEHv9 Module 15 Hacking Mobile Platforms.pdf","UserName":"سعید","ReqNo":"1303"}', NULL, '2016-06-28 07:06:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4813, 18393, '', '{"FileName":"CEHv9 Module 15 Hacking Mobile Platforms.pdf","UserName":"سعید","ReqNo":"1303"}', NULL, '2016-06-28 08:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4815, 18665, '', '{"FileName":"wood paper final_tcm18-52110.pdf","UserName":"فرزین","ReqNo":"1305"}', NULL, '2016-06-28 09:22:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4817, 18665, '', '{"FileName":"wood paper final_tcm18-52110.pdf","UserName":"فرزین","ReqNo":"1305"}', NULL, '2016-06-28 10:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4819, 18701, '', '{"FileName":"hxrnx3.pdf","UserName":"حسین","ReqNo":"1307"}', NULL, '2016-06-28 12:36:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4821, 18701, '', '{"FileName":"hxrnx3.pdf","UserName":"حسین","ReqNo":"1307"}', NULL, '2016-06-28 13:06:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4823, 18731, '', '{"FileName":"TwoAsynchoronousActivityInSpikingNetworks.pdf","UserName":"نگار","ReqNo":"1309"}', NULL, '2016-06-28 15:03:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4825, 18731, '', '{"FileName":"TwoAsynchoronousActivityInSpikingNetworks.pdf","UserName":"نگار","ReqNo":"1309"}', NULL, '2016-06-28 16:00:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4827, 18821, '', '{"FileName":"10.1016_j.rser.2016.04.019-A-review-of-pumped-hydro-energy-storage-development-in-significant-intern","UserName":"محمد","ReqNo":"1311"}', NULL, '2016-06-29 11:32:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4829, 18821, '', '{"FileName":"10.1016_j.rser.2016.04.019-A-review-of-pumped-hydro-energy-storage-development-in-significant-intern","UserName":"محمد","ReqNo":"1311"}', NULL, '2016-06-29 12:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4831, 15717, '', '{"FileName":"رویا های قطار 1و2.docx","UserName":"علی رضا","ReqNo":"1313"}', NULL, '2016-06-30 06:01:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4833, 15717, '', '{"FileName":"رویا های قطار 1و2.docx","UserName":"علی رضا","ReqNo":"1313"}', NULL, '2016-06-30 06:03:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4835, 15717, '', '{"FileName":"رویا های قطار 1و2.docx","UserName":"علی رضا","ReqNo":"1315"}', NULL, '2016-06-30 06:04:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4837, 15717, '', '{"FileName":"رویا های قطار 1و2.docx","UserName":"علی رضا","ReqNo":"1315"}', NULL, '2016-06-30 06:06:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4839, 18893, '', '{"FileName":"2009-Cater-NetworkMarketing.pdf","UserName":"ahmad","ReqNo":"1317"}', NULL, '2016-06-30 06:47:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4841, 18893, '', '{"FileName":"1c416222-68de-4b6d-94aa-db5542ca8f7a.pdf","UserName":"ahmad","ReqNo":"1319"}', NULL, '2016-06-30 06:48:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4843, 18893, '', '{"FileName":"2009-Cater-NetworkMarketing.pdf","UserName":"ahmad","ReqNo":"1317"}', NULL, '2016-06-30 06:48:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4845, 18893, '', '{"FileName":"1c416222-68de-4b6d-94aa-db5542ca8f7a.pdf","UserName":"ahmad","ReqNo":"1319"}', NULL, '2016-06-30 06:49:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4847, 18927, '', '{"FileName":"Hillary Clinton.docx","UserName":"مسیب","ReqNo":"1321"}', NULL, '2016-06-30 17:15:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4849, 18927, '', '{"FileName":"Hillary Clinton.docx","UserName":"مسیب","ReqNo":"1321"}', NULL, '2016-06-30 18:00:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4851, 18953, '', '{"FileName":"[Elearnica] -The_Future_Ain_t_What_It_Used_to_Be__Strategic_Innovation_in_the_Global_Def.pdf","UserName":"کیوان","ReqNo":"1323"}', NULL, '2016-07-01 10:58:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4853, 18953, '', '{"FileName":"[Elearnica] -The_Future_Ain_t_What_It_Used_to_Be__Strategic_Innovation_in_the_Global_Def.pdf","UserName":"کیوان","ReqNo":"1323"}', NULL, '2016-07-01 10:59:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4855, 18845, '', '{"FileName":"111.docx","UserName":"علی","ReqNo":"1325"}', NULL, '2016-07-02 10:43:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4857, 19067, '', '{"FileName":"1qa.docx","UserName":"mortaza","ReqNo":"1327"}', NULL, '2016-07-02 17:23:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4859, 19131, '', '{"FileName":"Data-mining-of-social-networks-represented-as-graphs.pdf","UserName":"Ali Akbar","ReqNo":"1329"}', NULL, '2016-07-02 19:00:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4861, 19147, '', '{"FileName":"Data mining of social networks represented as graphs.docx","UserName":"Ali Akbar","ReqNo":"1331"}', NULL, '2016-07-02 22:25:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4863, 18845, '', '{"FileName":"111.docx","UserName":"علی","ReqNo":"1325"}', NULL, '2016-07-03 01:00:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4865, 19067, '', '{"FileName":"1qa.docx","UserName":"mortaza","ReqNo":"1327"}', NULL, '2016-07-03 01:00:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4867, 19131, '', '{"FileName":"Data-mining-of-social-networks-represented-as-graphs.pdf","UserName":"Ali Akbar","ReqNo":"1329"}', NULL, '2016-07-03 01:01:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4869, 19147, '', '{"FileName":"Data mining of social networks represented as graphs.docx","UserName":"Ali Akbar","ReqNo":"1331"}', NULL, '2016-07-03 01:02:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4871, 18845, '', '{"FileName":"111.docx","UserName":"علی","ReqNo":"1333"}', NULL, '2016-07-03 02:53:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4873, 19191, '', '{"FileName":"0040.pdf","UserName":"حمیده","ReqNo":"1335"}', NULL, '2016-07-03 11:41:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4875, 19067, '', '{"FileName":"Internet of Things2-new-fixed.doc","UserName":"mortaza","ReqNo":"1337"}', NULL, '2016-07-03 18:15:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4877, 19313, '', '{"FileName":"بررسی تاثیر آشنایی مدیران اجرایی با مفاهیم علم مدیریت بر موفقیت دستگاه.doc","UserName":"علی اصغر","ReqNo":"1339"}', NULL, '2016-07-04 10:01:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4879, 19515, '', '{"FileName":"Applying K-Means Clustering Algorithm.pdf","UserName":"مجتبی","ReqNo":"1341"}', NULL, '2016-07-06 18:06:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4881, 19515, '', '{"FileName":"Applying K-Means Clustering Algorithm.pdf","UserName":"مجتبی","ReqNo":"1343"}', NULL, '2016-07-06 18:10:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4883, 19515, '', '{"FileName":"Application of data mining Diabetes health care in young.pdf","UserName":"مجتبی","ReqNo":"1345"}', NULL, '2016-07-06 18:12:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4885, 14051, '', '{"FileName":"Available online at www.docx","UserName":"سپهر","ReqNo":"1347"}', NULL, '2016-07-06 19:20:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4887, 19553, '', '{"FileName":"1-s2.0مکانهای اضطراری-main.pdf","UserName":"صادق","ReqNo":"1349"}', NULL, '2016-07-07 10:46:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4889, 19515, '', '{"FileName":"The Application of Oracle Data Mining Technology in Classification of.pdf","UserName":"مجتبی","ReqNo":"1351"}', NULL, '2016-07-07 12:12:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4891, 19573, '', '{"FileName":"www.FreePaper.us_981647922230931841.pdf","UserName":"dadbeh","ReqNo":"1353"}', NULL, '2016-07-07 13:34:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4893, 19581, '', '{"FileName":"RBAC.pdf","UserName":"فاطمه","ReqNo":"1355"}', NULL, '2016-07-07 15:10:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4895, 19591, '', '{"FileName":"magale dovom.pdf","UserName":"مسعود","ReqNo":"1357"}', NULL, '2016-07-07 18:39:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4897, 19067, '', '{"FileName":"1.docx","UserName":"mortaza","ReqNo":"1359"}', NULL, '2016-07-07 22:40:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4899, 19067, '', '{"FileName":"2.docx","UserName":"mortaza","ReqNo":"1361"}', NULL, '2016-07-07 22:40:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4901, 19067, '', '{"FileName":"3.docx","UserName":"mortaza","ReqNo":"1363"}', NULL, '2016-07-07 22:40:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4903, 19067, '', '{"FileName":"4.docx","UserName":"mortaza","ReqNo":"1365"}', NULL, '2016-07-07 22:40:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4905, 19067, '', '{"FileName":"5.docx","UserName":"mortaza","ReqNo":"1367"}', NULL, '2016-07-07 22:40:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4907, 19771, '', '{"FileName":"چکیده.docx","UserName":"وحید","ReqNo":"1369"}', NULL, '2016-07-09 14:31:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4909, 19801, '', '{"FileName":"tendlite-manual-spring-2014.pdf","UserName":"Ahmad","ReqNo":"1371"}', NULL, '2016-07-09 19:05:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4911, 19855, '', '{"FileName":"SP_2079_10.1007-2Fs10700-014-9192-2.pdf","UserName":"saleh","ReqNo":"1373"}', NULL, '2016-07-10 11:57:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4913, 14385, '', '{"FileName":"Cross.docx","UserName":"نادیا","ReqNo":"1375"}', NULL, '2016-07-12 16:08:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4915, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1377"}', NULL, '2016-07-12 17:36:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4917, 19591, '', '{"FileName":"magale aval.pdf","UserName":"مسعود","ReqNo":"1379"}', NULL, '2016-07-13 09:20:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4919, 20113, '', '{"FileName":"فصل2 رساله.doc","UserName":"مصطفی","ReqNo":"1381"}', NULL, '2016-07-13 10:07:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4921, 20113, '', '{"FileName":"آقای سلگی.pdf","UserName":"مصطفی","ReqNo":"1383"}', NULL, '2016-07-13 10:13:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4923, 14385, '', '{"FileName":"Cross.docx","UserName":"نادیا","ReqNo":"1385"}', NULL, '2016-07-13 22:27:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4925, 14385, '', '{"FileName":"Cross.docx","UserName":"نادیا","ReqNo":"1387"}', NULL, '2016-07-13 22:28:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(4927, 18845, '', '{"FileName":"111.docx","UserName":"علی","ReqNo":"1333"}', NULL, '2016-07-13 23:55:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4929, 19191, '', '{"FileName":"0040.pdf","UserName":"حمیده","ReqNo":"1335"}', NULL, '2016-07-13 23:55:56', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4931, 19067, '', '{"FileName":"Internet of Things2-new-fixed.doc","UserName":"mortaza","ReqNo":"1337"}', NULL, '2016-07-13 23:56:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4933, 19313, '', '{"FileName":"بررسی تاثیر آشنایی مدیران اجرایی با مفاهیم علم مدیریت بر موفقیت دستگاه.doc","UserName":"علی اصغر","ReqNo":"1339"}', NULL, '2016-07-13 23:56:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4935, 19515, '', '{"FileName":"Applying K-Means Clustering Algorithm.pdf","UserName":"مجتبی","ReqNo":"1341"}', NULL, '2016-07-13 23:56:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4937, 19515, '', '{"FileName":"Applying K-Means Clustering Algorithm.pdf","UserName":"مجتبی","ReqNo":"1343"}', NULL, '2016-07-13 23:56:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4939, 19515, '', '{"FileName":"Application of data mining Diabetes health care in young.pdf","UserName":"مجتبی","ReqNo":"1345"}', NULL, '2016-07-13 23:56:34', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4941, 14051, '', '{"FileName":"Available online at www.docx","UserName":"سپهر","ReqNo":"1347"}', NULL, '2016-07-13 23:56:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4943, 19553, '', '{"FileName":"1-s2.0مکانهای اضطراری-main.pdf","UserName":"صادق","ReqNo":"1349"}', NULL, '2016-07-13 23:56:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4945, 19515, '', '{"FileName":"The Application of Oracle Data Mining Technology in Classification of.pdf","UserName":"مجتبی","ReqNo":"1351"}', NULL, '2016-07-13 23:56:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4947, 19573, '', '{"FileName":"www.FreePaper.us_981647922230931841.pdf","UserName":"dadbeh","ReqNo":"1353"}', NULL, '2016-07-14 00:01:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4949, 18845, '', '{"FileName":"111.docx","UserName":"علی","ReqNo":"1333"}', NULL, '2016-07-14 00:05:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4951, 19191, '', '{"FileName":"0040.pdf","UserName":"حمیده","ReqNo":"1335"}', NULL, '2016-07-14 00:05:16', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4953, 19067, '', '{"FileName":"Internet of Things2-new-fixed.doc","UserName":"mortaza","ReqNo":"1337"}', NULL, '2016-07-14 00:06:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4955, 19313, '', '{"FileName":"بررسی تاثیر آشنایی مدیران اجرایی با مفاهیم علم مدیریت بر موفقیت دستگاه.doc","UserName":"علی اصغر","ReqNo":"1339"}', NULL, '2016-07-14 00:06:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4957, 19515, '', '{"FileName":"Applying K-Means Clustering Algorithm.pdf","UserName":"مجتبی","ReqNo":"1341"}', NULL, '2016-07-14 00:06:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4959, 19515, '', '{"FileName":"Applying K-Means Clustering Algorithm.pdf","UserName":"مجتبی","ReqNo":"1343"}', NULL, '2016-07-14 00:06:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4961, 19515, '', '{"FileName":"Application of data mining Diabetes health care in young.pdf","UserName":"مجتبی","ReqNo":"1345"}', NULL, '2016-07-14 00:06:58', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4963, 14051, '', '{"FileName":"Available online at www.docx","UserName":"سپهر","ReqNo":"1347"}', NULL, '2016-07-14 00:07:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4965, 19553, '', '{"FileName":"1-s2.0مکانهای اضطراری-main.pdf","UserName":"صادق","ReqNo":"1349"}', NULL, '2016-07-14 00:07:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4967, 19515, '', '{"FileName":"The Application of Oracle Data Mining Technology in Classification of.pdf","UserName":"مجتبی","ReqNo":"1351"}', NULL, '2016-07-14 00:09:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4969, 19573, '', '{"FileName":"www.FreePaper.us_981647922230931841.pdf","UserName":"dadbeh","ReqNo":"1353"}', NULL, '2016-07-14 00:17:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4971, 19581, '', '{"FileName":"RBAC.pdf","UserName":"فاطمه","ReqNo":"1355"}', NULL, '2016-07-14 00:17:38', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(4973, 19591, '', '{"FileName":"magale dovom.pdf","UserName":"مسعود","ReqNo":"1357"}', NULL, '2016-07-14 00:21:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4975, 19067, '', '{"FileName":"1.docx","UserName":"mortaza","ReqNo":"1359"}', NULL, '2016-07-14 00:23:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4977, 19067, '', '{"FileName":"2.docx","UserName":"mortaza","ReqNo":"1361"}', NULL, '2016-07-14 00:25:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4979, 19067, '', '{"FileName":"3.docx","UserName":"mortaza","ReqNo":"1363"}', NULL, '2016-07-14 00:27:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4981, 19067, '', '{"FileName":"4.docx","UserName":"mortaza","ReqNo":"1365"}', NULL, '2016-07-14 00:28:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4983, 19067, '', '{"FileName":"5.docx","UserName":"mortaza","ReqNo":"1367"}', NULL, '2016-07-14 00:32:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4985, 19771, '', '{"FileName":"چکیده.docx","UserName":"وحید","ReqNo":"1369"}', NULL, '2016-07-14 00:32:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4987, 19801, '', '{"FileName":"tendlite-manual-spring-2014.pdf","UserName":"Ahmad","ReqNo":"1371"}', NULL, '2016-07-14 00:36:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4989, 19855, '', '{"FileName":"SP_2079_10.1007-2Fs10700-014-9192-2.pdf","UserName":"saleh","ReqNo":"1373"}', NULL, '2016-07-14 00:39:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4991, 14385, '', '{"FileName":"Cross.docx","UserName":"نادیا","ReqNo":"1375"}', NULL, '2016-07-14 00:40:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4993, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1377"}', NULL, '2016-07-14 00:40:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4995, 19591, '', '{"FileName":"magale aval.pdf","UserName":"مسعود","ReqNo":"1379"}', NULL, '2016-07-14 00:46:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4997, 20113, '', '{"FileName":"فصل2 رساله.doc","UserName":"مصطفی","ReqNo":"1381"}', NULL, '2016-07-14 00:51:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(4999, 20113, '', '{"FileName":"آقای سلگی.pdf","UserName":"مصطفی","ReqNo":"1383"}', NULL, '2016-07-14 00:51:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5001, 14385, '', '{"FileName":"Cross.docx","UserName":"نادیا","ReqNo":"1385"}', NULL, '2016-07-14 00:52:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5003, 14385, '', '{"FileName":"Cross.docx","UserName":"نادیا","ReqNo":"1387"}', NULL, '2016-07-14 00:53:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5005, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1389"}', NULL, '2016-07-14 03:20:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5007, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1391"}', NULL, '2016-07-14 03:23:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5009, 20199, '', '{"FileName":"WSN20101100008_62970559 (1).pdf","UserName":"mostafa","ReqNo":"1393"}', NULL, '2016-07-14 12:33:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5011, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1389"}', NULL, '2016-07-15 01:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5013, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1391"}', NULL, '2016-07-15 01:00:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5015, 20199, '', '{"FileName":"WSN20101100008_62970559 (1).pdf","UserName":"mostafa","ReqNo":"1393"}', NULL, '2016-07-15 01:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5017, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1395"}', NULL, '2016-07-15 17:13:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5019, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1397"}', NULL, '2016-07-15 17:14:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5021, 17805, '', '{"FileName":"1-s2.0-S2212567116300259-main.docx","UserName":"محمد","ReqNo":"1399"}', NULL, '2016-07-15 17:14:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5023, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1401"}', NULL, '2016-07-15 17:28:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5025, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1395"}', NULL, '2016-07-16 01:00:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5027, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1397"}', NULL, '2016-07-16 01:00:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5029, 17805, '', '{"FileName":"1-s2.0-S2212567116300259-main.docx","UserName":"محمد","ReqNo":"1399"}', NULL, '2016-07-16 01:00:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5031, 17805, '', '{"FileName":"Intellectual capital disclosures and corporate governance_ An empirical examination.docx","UserName":"محمد","ReqNo":"1401"}', NULL, '2016-07-16 01:04:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5033, 20361, '', '{"FileName":"ukrain.haghi.1.2.1395.doc","UserName":"سیدرضا","ReqNo":"1403"}', NULL, '2016-07-16 22:47:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5035, 20361, '', '{"FileName":"ukrain.haghi.1.2.1395.doc","UserName":"سیدرضا","ReqNo":"1403"}', NULL, '2016-07-17 01:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5037, 20585, '', '{"FileName":"Lit_BiomechPropsSkin_2.pdf","UserName":"Sa","ReqNo":"1405"}', NULL, '2016-07-18 20:51:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5039, 20585, '', '{"FileName":"PDIA-30-21579.pdf","UserName":"Sa","ReqNo":"1407"}', NULL, '2016-07-18 20:51:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5041, 20585, '', '{"FileName":"PDIA-30-21579.pdf","UserName":"Sa","ReqNo":"1409"}', NULL, '2016-07-18 20:51:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5043, 20585, '', '{"FileName":"wound.2011.0321.pdf","UserName":"Sa","ReqNo":"1411"}', NULL, '2016-07-18 20:52:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5045, 20585, '', '{"FileName":"Agache1980.pdf","UserName":"Sa","ReqNo":"1413"}', NULL, '2016-07-18 20:52:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5047, 20585, '', '{"FileName":"Modelling methods for In Vitro biomechanical properties of the skin_ A review - Springer.pdf","UserName":"Sa","ReqNo":"1415"}', NULL, '2016-07-18 20:52:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5049, 20585, '', '{"FileName":"JM-JMBBM-27-Orig.pdf","UserName":"Sa","ReqNo":"1417"}', NULL, '2016-07-18 20:57:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5051, 20585, '', '{"FileName":"DermaLab Single parameter USB - Cortex Technology ApS.pdf","UserName":"Sa","ReqNo":"1419"}', NULL, '2016-07-18 20:57:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5053, 20585, '', '{"FileName":"DermaScan skin ultrasound - Cortex Technology ApS.pdf","UserName":"Sa","ReqNo":"1421"}', NULL, '2016-07-18 20:57:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5055, 20585, '', '{"FileName":"Efficacy.pdf","UserName":"Sa","ReqNo":"1423"}', NULL, '2016-07-18 20:57:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5057, 20585, '', '{"FileName":"Multi parameter skin testing.pdf","UserName":"Sa","ReqNo":"1425"}', NULL, '2016-07-18 20:58:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5059, 20585, '', '{"FileName":"Skin color - Cortex Technology ApS.pdf","UserName":"Sa","ReqNo":"1427"}', NULL, '2016-07-18 20:59:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5061, 20585, '', '{"FileName":"Dia-stron Limited - Skin Instruments.pdf","UserName":"Sa","ReqNo":"1429"}', NULL, '2016-07-18 20:59:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5063, 20585, '', '{"FileName":"Torsional_Ballistometer_2.pdf","UserName":"Sa","ReqNo":"1431"}', NULL, '2016-07-18 20:59:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5065, 20585, '', '{"FileName":"A ballistometer for the study of the plasto-elastic properties of skin.pdf","UserName":"Sa","ReqNo":"1433"}', NULL, '2016-07-18 20:59:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5067, 20585, '', '{"FileName":"10.1.1.526.818.pdf","UserName":"Sa","ReqNo":"1435"}', NULL, '2016-07-18 20:59:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5069, 20585, '', '{"FileName":"Z0cux34Tue13100.pdf","UserName":"Sa","ReqNo":"1437"}', NULL, '2016-07-18 21:00:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5071, 20585, '', '{"FileName":"Study Equipment _ Stephens & Associates.pdf","UserName":"Sa","ReqNo":"1439"}', NULL, '2016-07-18 21:00:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5073, 20585, '', '{"FileName":"Lit_BiomechPropsSkin_2.pdf","UserName":"Sa","ReqNo":"1405"}', NULL, '2016-07-19 01:00:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5075, 20585, '', '{"FileName":"PDIA-30-21579.pdf","UserName":"Sa","ReqNo":"1407"}', NULL, '2016-07-19 01:00:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5077, 20585, '', '{"FileName":"PDIA-30-21579.pdf","UserName":"Sa","ReqNo":"1409"}', NULL, '2016-07-19 01:00:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5079, 20585, '', '{"FileName":"wound.2011.0321.pdf","UserName":"Sa","ReqNo":"1411"}', NULL, '2016-07-19 01:00:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5081, 20585, '', '{"FileName":"Agache1980.pdf","UserName":"Sa","ReqNo":"1413"}', NULL, '2016-07-19 01:01:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5083, 20585, '', '{"FileName":"Modelling methods for In Vitro biomechanical properties of the skin_ A review - Springer.pdf","UserName":"Sa","ReqNo":"1415"}', NULL, '2016-07-19 01:01:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5085, 20585, '', '{"FileName":"JM-JMBBM-27-Orig.pdf","UserName":"Sa","ReqNo":"1417"}', NULL, '2016-07-19 01:01:16', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5087, 20585, '', '{"FileName":"DermaLab Single parameter USB - Cortex Technology ApS.pdf","UserName":"Sa","ReqNo":"1419"}', NULL, '2016-07-19 01:01:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5089, 20585, '', '{"FileName":"DermaScan skin ultrasound - Cortex Technology ApS.pdf","UserName":"Sa","ReqNo":"1421"}', NULL, '2016-07-19 01:01:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5091, 20585, '', '{"FileName":"Efficacy.pdf","UserName":"Sa","ReqNo":"1423"}', NULL, '2016-07-19 01:01:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5093, 20585, '', '{"FileName":"Multi parameter skin testing.pdf","UserName":"Sa","ReqNo":"1425"}', NULL, '2016-07-19 01:01:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5095, 20585, '', '{"FileName":"Skin color - Cortex Technology ApS.pdf","UserName":"Sa","ReqNo":"1427"}', NULL, '2016-07-19 01:01:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5097, 20585, '', '{"FileName":"Dia-stron Limited - Skin Instruments.pdf","UserName":"Sa","ReqNo":"1429"}', NULL, '2016-07-19 01:01:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5099, 20585, '', '{"FileName":"Torsional_Ballistometer_2.pdf","UserName":"Sa","ReqNo":"1431"}', NULL, '2016-07-19 01:01:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5101, 20585, '', '{"FileName":"A ballistometer for the study of the plasto-elastic properties of skin.pdf","UserName":"Sa","ReqNo":"1433"}', NULL, '2016-07-19 01:01:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5103, 20585, '', '{"FileName":"10.1.1.526.818.pdf","UserName":"Sa","ReqNo":"1435"}', NULL, '2016-07-19 01:01:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5105, 20585, '', '{"FileName":"Z0cux34Tue13100.pdf","UserName":"Sa","ReqNo":"1437"}', NULL, '2016-07-19 01:02:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5107, 20585, '', '{"FileName":"Study Equipment _ Stephens & Associates.pdf","UserName":"Sa","ReqNo":"1439"}', NULL, '2016-07-19 01:02:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5109, 3539, '', '{"FileName":"Impact of High PV Penetration on Voltage Profiles in Residential Neighborhoods.docx","UserName":"Fatemeh","ReqNo":"1441"}', NULL, '2016-07-19 10:26:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5111, 3539, '', '{"FileName":"Impact of High PV Penetration on Voltage Profiles in Residential Neighborhoods.docx","UserName":"Fatemeh","ReqNo":"1441"}', NULL, '2016-07-20 01:00:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5113, 20747, '', '{"FileName":"Seifi.pdf","UserName":"سيف اله","ReqNo":"1443"}', NULL, '2016-07-20 13:30:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5115, 20747, '', '{"FileName":"Seifi.pdf","UserName":"سيف اله","ReqNo":"1443"}', NULL, '2016-07-21 01:00:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5117, 20819, '', '{"FileName":"inp.pdf","UserName":"حامد","ReqNo":"1445"}', NULL, '2016-07-21 10:52:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5119, 20819, '', '{"FileName":"inp.pdf","UserName":"حامد","ReqNo":"1445"}', NULL, '2016-07-22 01:12:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5121, 20747, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"سيف اله","ReqNo":"1447"}', NULL, '2016-07-22 16:42:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5123, 20919, '', '{"FileName":"iot.pdf","UserName":"فریده","ReqNo":"1449"}', NULL, '2016-07-22 19:23:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5125, 20919, '', '{"FileName":"iot.pdf","UserName":"فریده","ReqNo":"1451"}', NULL, '2016-07-23 00:23:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5127, 20747, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"سيف اله","ReqNo":"1447"}', NULL, '2016-07-23 01:00:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5129, 20919, '', '{"FileName":"iot.pdf","UserName":"فریده","ReqNo":"1449"}', NULL, '2016-07-23 01:00:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5131, 20919, '', '{"FileName":"iot.pdf","UserName":"فریده","ReqNo":"1451"}', NULL, '2016-07-23 01:00:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5133, 21021, '', '{"FileName":"963365.pdf","UserName":"عبداله","ReqNo":"1453"}', NULL, '2016-07-24 08:18:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5135, 21021, '', '{"FileName":"963365.pdf","UserName":"عبداله","ReqNo":"1453"}', NULL, '2016-07-25 01:29:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5137, 18821, '', '{"FileName":"Abstract.docx","UserName":"محمد","ReqNo":"1455"}', NULL, '2016-07-25 11:49:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5139, 18821, '', '{"FileName":"Abstract.docx","UserName":"محمد","ReqNo":"1455"}', NULL, '2016-07-26 01:00:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5141, 21213, '', '{"FileName":"10.1016_j.rser.2014.10.014-A-review-of-solar-powered-absorption-systems.pdf","UserName":"هادی","ReqNo":"1457"}', NULL, '2016-07-26 12:41:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5143, 21261, '', '{"FileName":"The_Role_Of_Forensic_Accounting_In_Mitigating_Financial_Crimes.pdf","UserName":"امیرعلی","ReqNo":"1459"}', NULL, '2016-07-26 18:13:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5145, 21213, '', '{"FileName":"10.1016_j.rser.2014.10.014-A-review-of-solar-powered-absorption-systems.pdf","UserName":"هادی","ReqNo":"1457"}', NULL, '2016-07-27 01:00:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5147, 21261, '', '{"FileName":"The_Role_Of_Forensic_Accounting_In_Mitigating_Financial_Crimes.pdf","UserName":"امیرعلی","ReqNo":"1459"}', NULL, '2016-07-27 01:01:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5149, 21301, '', '{"FileName":"A-Machine-Learning-Based-Web-Spam-Filtering-Approach.pdf","UserName":"جواد","ReqNo":"1461"}', NULL, '2016-07-27 10:14:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5151, 21301, '', '{"FileName":"A-Machine-Learning-Based-Web-Spam-Filtering-Approach.pdf","UserName":"جواد","ReqNo":"1461"}', NULL, '2016-07-28 01:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5153, 21403, '', '{"FileName":"Mehler-Bicher6ICRO.pdf","UserName":"عصمت","ReqNo":"1463"}', NULL, '2016-07-29 18:01:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5155, 21333, '', '{"FileName":"let.docx","UserName":"yasser","ReqNo":"1465"}', NULL, '2016-07-29 22:19:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5157, 21333, '', '{"FileName":"let.docx","UserName":"yasser","ReqNo":"1467"}', NULL, '2016-07-29 22:28:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5159, 21403, '', '{"FileName":"Mehler-Bicher6ICRO.pdf","UserName":"عصمت","ReqNo":"1463"}', NULL, '2016-07-30 01:00:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5161, 21333, '', '{"FileName":"let.docx","UserName":"yasser","ReqNo":"1465"}', NULL, '2016-07-30 01:00:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5163, 21333, '', '{"FileName":"let.docx","UserName":"yasser","ReqNo":"1467"}', NULL, '2016-07-30 01:00:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5165, 21443, '', '{"FileName":"اولویت بندی.docx","UserName":"sahar","ReqNo":"1469"}', NULL, '2016-07-30 20:01:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5167, 21451, '', '{"FileName":"1.pdf","UserName":"مریم","ReqNo":"1471"}', NULL, '2016-07-30 20:41:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5169, 21451, '', '{"FileName":"2.pdf","UserName":"مریم","ReqNo":"1473"}', NULL, '2016-07-30 20:43:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5171, 21451, '', '{"FileName":"3.pdf","UserName":"مریم","ReqNo":"1475"}', NULL, '2016-07-30 20:47:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5173, 21443, '', '{"FileName":"اولویت بندی.docx","UserName":"sahar","ReqNo":"1469"}', NULL, '2016-07-31 01:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5175, 21451, '', '{"FileName":"1.pdf","UserName":"مریم","ReqNo":"1471"}', NULL, '2016-07-31 01:00:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5177, 21451, '', '{"FileName":"2.pdf","UserName":"مریم","ReqNo":"1473"}', NULL, '2016-07-31 01:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5179, 21451, '', '{"FileName":"3.pdf","UserName":"مریم","ReqNo":"1475"}', NULL, '2016-07-31 01:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5181, 21461, '', '{"FileName":"akyildiz06.pdf","UserName":"احمد","ReqNo":"1477"}', NULL, '2016-07-31 01:35:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5183, 21475, '', '{"FileName":"1-s2.0-S1877050913008831-main.pdf","UserName":"نفیسه","ReqNo":"1479"}', NULL, '2016-07-31 10:29:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5185, 21443, '', '{"FileName":"مچوریتی مدل.pdf","UserName":"sahar","ReqNo":"1481"}', NULL, '2016-07-31 12:09:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5187, 21443, '', '{"FileName":"ahp1.pdf","UserName":"sahar","ReqNo":"1483"}', NULL, '2016-07-31 12:14:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5189, 21485, '', '{"FileName":"arzyabi 1-s2.0-S0957417411003551-main.pdf","UserName":"جواد","ReqNo":"1485"}', NULL, '2016-07-31 18:43:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5191, 21461, '', '{"FileName":"akyildiz06.pdf","UserName":"احمد","ReqNo":"1477"}', NULL, '2016-08-01 01:05:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5193, 21475, '', '{"FileName":"1-s2.0-S1877050913008831-main.pdf","UserName":"نفیسه","ReqNo":"1479"}', NULL, '2016-08-01 01:07:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5195, 21443, '', '{"FileName":"مچوریتی مدل.pdf","UserName":"sahar","ReqNo":"1481"}', NULL, '2016-08-01 01:09:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5197, 21443, '', '{"FileName":"ahp1.pdf","UserName":"sahar","ReqNo":"1483"}', NULL, '2016-08-01 01:11:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5199, 21485, '', '{"FileName":"arzyabi 1-s2.0-S0957417411003551-main.pdf","UserName":"جواد","ReqNo":"1485"}', NULL, '2016-08-01 01:15:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5201, 21443, '', '{"FileName":"الویت بندی.docx","UserName":"sahar","ReqNo":"1487"}', NULL, '2016-08-01 10:29:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5203, 21443, '', '{"FileName":"الویت بندی.docx","UserName":"sahar","ReqNo":"1487"}', NULL, '2016-08-02 01:00:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5205, 21655, '', '{"FileName":"Test 1.docx","UserName":"محرم","ReqNo":"1489"}', NULL, '2016-08-04 15:36:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5207, 21659, '', '{"FileName":"blackmirror3.pdf","UserName":"علی","ReqNo":"1491"}', NULL, '2016-08-04 15:38:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5209, 21655, '', '{"FileName":"Test 1.docx","UserName":"محرم","ReqNo":"1489"}', NULL, '2016-08-05 01:00:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5211, 21659, '', '{"FileName":"blackmirror3.pdf","UserName":"علی","ReqNo":"1491"}', NULL, '2016-08-05 01:01:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5213, 21909, '', '{"FileName":"Risk Assessment inPractice _ 2012_COSO ERM.pdf","UserName":"ابوالفضل","ReqNo":"1493"}', NULL, '2016-08-09 19:27:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5215, 21909, '', '{"FileName":"Risk Assessment inPractice _ 2012_COSO ERM.pdf","UserName":"ابوالفضل","ReqNo":"1493"}', NULL, '2016-08-10 01:02:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5217, 22029, '', '{"FileName":"MFL67840201.pdf","UserName":"مسلم","ReqNo":"1495"}', NULL, '2016-08-12 02:52:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5219, 22029, '', '{"FileName":"MFL67840201.pdf","UserName":"مسلم","ReqNo":"1495"}', NULL, '2016-08-13 01:06:37', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5221, 22163, '', '{"FileName":"GSM.pdf","UserName":"nima","ReqNo":"1497"}', NULL, '2016-08-13 16:09:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5223, 22163, '', '{"FileName":"GSM.pdf","UserName":"nima","ReqNo":"1497"}', NULL, '2016-08-14 01:00:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5225, 22201, '', '{"FileName":"02_whole.pdf","UserName":"وحید","ReqNo":"1499"}', NULL, '2016-08-14 13:12:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5227, 22233, '', '{"FileName":"5 Strategic Decision Selection Using Hesitant fuzzy.pdf","UserName":"هدی","ReqNo":"1501"}', NULL, '2016-08-14 21:47:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5229, 22201, '', '{"FileName":"02_whole.pdf","UserName":"وحید","ReqNo":"1499"}', NULL, '2016-08-15 01:00:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5231, 22251, '', '{"FileName":"4_479156842283401317.pdf","UserName":"yas","ReqNo":"1503"}', NULL, '2016-08-15 01:18:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5233, 22251, '', '{"FileName":"4_479156842283401316.pdf","UserName":"yas","ReqNo":"1505"}', NULL, '2016-08-15 01:19:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5235, 22273, '', '{"FileName":"SSRN-id1011230.pdf","UserName":"مینا","ReqNo":"1507"}', NULL, '2016-08-15 11:43:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5237, 22295, '', '{"FileName":"Analysis on lightning triggering long transmission tethers of high altitude wind energy system.pdf","UserName":"آرش","ReqNo":"1509"}', NULL, '2016-08-15 16:45:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5239, 22321, '', '{"FileName":"RPA_StrategicPlan.pdf","UserName":"هادی","ReqNo":"1511"}', NULL, '2016-08-16 00:18:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5241, 22329, '', '{"FileName":"WorldWithoutCancer.pdf","UserName":"amir","ReqNo":"1513"}', NULL, '2016-08-16 01:55:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5243, 22333, '', '{"FileName":"[Stephen_Mumford]_David_Armstrong_(Philosophy_Now)(BookZZ.org).pdf","UserName":"کیا","ReqNo":"1515"}', NULL, '2016-08-16 07:06:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5245, 22333, '', '{"FileName":"(4) Armstrong_Universals.pdf","UserName":"کیا","ReqNo":"1517"}', NULL, '2016-08-16 07:18:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5247, 22333, '', '{"FileName":"kiya.docx","UserName":"کیا","ReqNo":"1519"}', NULL, '2016-08-16 07:19:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5249, 22333, '', '{"FileName":"kiya.docx","UserName":"کیا","ReqNo":"1521"}', NULL, '2016-08-16 07:20:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5251, 22333, '', '{"FileName":"Armstrongs_Truth_and_Truthmakers.pdf","UserName":"کیا","ReqNo":"1523"}', NULL, '2016-08-16 07:21:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5253, 22333, '', '{"FileName":"a theory of universals vol 2 universals and scientific realism.pdf","UserName":"کیا","ReqNo":"1525"}', NULL, '2016-08-16 07:22:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5255, 22333, '', '{"FileName":"[D._M._Armstrong]_Truth_and_Truthmakers_(Cambridge(BookFi.org).pdf","UserName":"کیا","ReqNo":"1527"}', NULL, '2016-08-16 07:24:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5257, 22333, '', '{"FileName":"D._M._Armstrong_A_World_of_States_of_Affairs__1997.pdf","UserName":"کیا","ReqNo":"1529"}', NULL, '2016-08-16 07:30:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5259, 22329, '', '{"FileName":"WorldWithoutCancer.pdf","UserName":"amir","ReqNo":"1531"}', NULL, '2016-08-17 14:01:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5261, 21655, '', '{"FileName":"Item.docx","UserName":"محرم","ReqNo":"1533"}', NULL, '2016-08-20 10:19:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5263, 22233, '', '{"FileName":"5 Strategic Decision Selection Using Hesitant fuzzy.pdf","UserName":"هدی","ReqNo":"1501"}', NULL, '2016-08-21 16:18:11', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5265, 22251, '', '{"FileName":"4_479156842283401317.pdf","UserName":"yas","ReqNo":"1503"}', NULL, '2016-08-21 16:26:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5267, 22251, '', '{"FileName":"4_479156842283401316.pdf","UserName":"yas","ReqNo":"1505"}', NULL, '2016-08-21 16:26:28', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5269, 22273, '', '{"FileName":"SSRN-id1011230.pdf","UserName":"مینا","ReqNo":"1507"}', NULL, '2016-08-21 16:40:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5271, 22295, '', '{"FileName":"Analysis on lightning triggering long transmission tethers of high altitude wind energy system.pdf","UserName":"آرش","ReqNo":"1509"}', NULL, '2016-08-21 16:44:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5273, 22321, '', '{"FileName":"RPA_StrategicPlan.pdf","UserName":"هادی","ReqNo":"1511"}', NULL, '2016-08-21 16:52:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5275, 22329, '', '{"FileName":"WorldWithoutCancer.pdf","UserName":"amir","ReqNo":"1513"}', NULL, '2016-08-21 17:25:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5277, 22333, '', '{"FileName":"[Stephen_Mumford]_David_Armstrong_(Philosophy_Now)(BookZZ.org).pdf","UserName":"کیا","ReqNo":"1515"}', NULL, '2016-08-21 17:50:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5279, 22333, '', '{"FileName":"(4) Armstrong_Universals.pdf","UserName":"کیا","ReqNo":"1517"}', NULL, '2016-08-21 17:52:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5281, 22333, '', '{"FileName":"kiya.docx","UserName":"کیا","ReqNo":"1519"}', NULL, '2016-08-21 17:53:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5283, 22333, '', '{"FileName":"kiya.docx","UserName":"کیا","ReqNo":"1521"}', NULL, '2016-08-21 17:54:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5285, 22333, '', '{"FileName":"Armstrongs_Truth_and_Truthmakers.pdf","UserName":"کیا","ReqNo":"1523"}', NULL, '2016-08-21 17:56:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5287, 22333, '', '{"FileName":"a theory of universals vol 2 universals and scientific realism.pdf","UserName":"کیا","ReqNo":"1525"}', NULL, '2016-08-21 17:57:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5289, 22333, '', '{"FileName":"[D._M._Armstrong]_Truth_and_Truthmakers_(Cambridge(BookFi.org).pdf","UserName":"کیا","ReqNo":"1527"}', NULL, '2016-08-21 18:08:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5291, 22333, '', '{"FileName":"D._M._Armstrong_A_World_of_States_of_Affairs__1997.pdf","UserName":"کیا","ReqNo":"1529"}', NULL, '2016-08-21 18:08:54', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5293, 22329, '', '{"FileName":"WorldWithoutCancer.pdf","UserName":"amir","ReqNo":"1531"}', NULL, '2016-08-21 18:33:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5295, 21655, '', '{"FileName":"Item.docx","UserName":"محرم","ReqNo":"1533"}', NULL, '2016-08-21 18:34:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5297, 22789, '', '{"FileName":"طبق آنالیز های صورت گرفته بار اول با بار دوم از کیفیت یکسان برخوردار نیست.docx","UserName":"mahmood","ReqNo":"1535"}', NULL, '2016-08-22 14:41:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5299, 21451, '', '{"FileName":"1.pdf","UserName":"مریم","ReqNo":"1537"}', NULL, '2016-08-22 21:52:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5301, 21451, '', '{"FileName":"2.pdf","UserName":"مریم","ReqNo":"1539"}', NULL, '2016-08-22 21:53:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5303, 21451, '', '{"FileName":"3.pdf","UserName":"مریم","ReqNo":"1541"}', NULL, '2016-08-22 21:55:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5305, 22789, '', '{"FileName":"طبق آنالیز های صورت گرفته بار اول با بار دوم از کیفیت یکسان برخوردار نیست.docx","UserName":"mahmood","ReqNo":"1535"}', NULL, '2016-08-23 01:00:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5307, 21451, '', '{"FileName":"1.pdf","UserName":"مریم","ReqNo":"1537"}', NULL, '2016-08-23 01:00:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5309, 21451, '', '{"FileName":"2.pdf","UserName":"مریم","ReqNo":"1539"}', NULL, '2016-08-23 01:00:35', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5311, 21451, '', '{"FileName":"3.pdf","UserName":"مریم","ReqNo":"1541"}', NULL, '2016-08-23 01:00:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5313, 22835, '', '{"FileName":"چکیده.docx","UserName":"مهسا","ReqNo":"1543"}', NULL, '2016-08-23 15:04:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5315, 22835, '', '{"FileName":"چکیده.docx","UserName":"مهسا","ReqNo":"1545"}', NULL, '2016-08-23 15:04:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5317, 22835, '', '{"FileName":"چکیده.docx","UserName":"مهسا","ReqNo":"1547"}', NULL, '2016-08-23 15:05:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5319, 22835, '', '{"FileName":"چکیده.docx","UserName":"مهسا","ReqNo":"1543"}', NULL, '2016-08-24 01:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5321, 22835, '', '{"FileName":"چکیده.docx","UserName":"مهسا","ReqNo":"1545"}', NULL, '2016-08-24 01:00:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5323, 22835, '', '{"FileName":"چکیده.docx","UserName":"مهسا","ReqNo":"1547"}', NULL, '2016-08-24 01:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5325, 22997, '', '{"FileName":"Abstract.doc","UserName":"زهرا","ReqNo":"1549"}', NULL, '2016-08-24 20:22:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5327, 22997, '', '{"FileName":"Abstract.doc","UserName":"زهرا","ReqNo":"1549"}', NULL, '2016-08-25 01:00:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5329, 23039, '', '{"FileName":"Understanding Strategic Planning and the Formulation and Implementation of Strategic Plans as a Way ","UserName":"محمد","ReqNo":"1551"}', NULL, '2016-08-25 11:07:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5331, 23039, '', '{"FileName":"strategic_planning (1).doc","UserName":"محمد","ReqNo":"1553"}', NULL, '2016-08-25 11:08:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5333, 23039, '', '{"FileName":"Understanding Strategic Planning and the Formulation and Implementation of Strategic Plans as a Way ","UserName":"محمد","ReqNo":"1551"}', NULL, '2016-08-26 01:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5335, 23039, '', '{"FileName":"strategic_planning (1).doc","UserName":"محمد","ReqNo":"1553"}', NULL, '2016-08-26 01:00:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5337, 22997, '', '{"FileName":"1.doc","UserName":"زهرا","ReqNo":"1555"}', NULL, '2016-08-26 14:37:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5339, 18007, '', '{"FileName":"[Elearnica] -Elsevier__Article_Locator.pdf","UserName":"reza","ReqNo":"1557"}', NULL, '2016-08-26 22:30:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5341, 22997, '', '{"FileName":"1.doc","UserName":"زهرا","ReqNo":"1555"}', NULL, '2016-08-27 01:00:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5343, 18007, '', '{"FileName":"[Elearnica] -Elsevier__Article_Locator.pdf","UserName":"reza","ReqNo":"1557"}', NULL, '2016-08-27 01:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5345, 23181, '', '{"FileName":"06912980.pdf","UserName":"fama","ReqNo":"1559"}', NULL, '2016-08-27 03:18:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5347, 23181, '', '{"FileName":"Ant Colony Optimization-based Adaptive.pdf","UserName":"fama","ReqNo":"1561"}', NULL, '2016-08-27 03:24:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5349, 23181, '', '{"FileName":"FreeRider Non local Adaptive.pdf","UserName":"fama","ReqNo":"1563"}', NULL, '2016-08-27 03:28:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5351, 23181, '', '{"FileName":"Research Challenges on 2-D and 3-D Network-on-Chip IEEE_06726874.pdf","UserName":"fama","ReqNo":"1565"}', NULL, '2016-08-27 03:28:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5353, 23181, '', '{"FileName":"Low-overhead Routing Algorithm for 3D Network-on-Chip.pdf","UserName":"fama","ReqNo":"1567"}', NULL, '2016-08-27 03:28:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5355, 23183, '', '{"FileName":"IRJET-V3I4394.pdf","UserName":"ابراهیم","ReqNo":"1569"}', NULL, '2016-08-27 05:36:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5357, 23211, '', '{"FileName":"پرسشنامه خودم 123.pdf","UserName":"فرشاد","ReqNo":"1571"}', NULL, '2016-08-27 19:30:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5359, 23181, '', '{"FileName":"06912980.pdf","UserName":"fama","ReqNo":"1559"}', NULL, '2016-08-28 01:00:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5361, 23181, '', '{"FileName":"Ant Colony Optimization-based Adaptive.pdf","UserName":"fama","ReqNo":"1561"}', NULL, '2016-08-28 01:00:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5363, 23181, '', '{"FileName":"FreeRider Non local Adaptive.pdf","UserName":"fama","ReqNo":"1563"}', NULL, '2016-08-28 01:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5365, 23181, '', '{"FileName":"Research Challenges on 2-D and 3-D Network-on-Chip IEEE_06726874.pdf","UserName":"fama","ReqNo":"1565"}', NULL, '2016-08-28 01:01:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5367, 23181, '', '{"FileName":"Low-overhead Routing Algorithm for 3D Network-on-Chip.pdf","UserName":"fama","ReqNo":"1567"}', NULL, '2016-08-28 01:01:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5369, 23183, '', '{"FileName":"IRJET-V3I4394.pdf","UserName":"ابراهیم","ReqNo":"1569"}', NULL, '2016-08-28 01:02:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5371, 23211, '', '{"FileName":"پرسشنامه خودم 123.pdf","UserName":"فرشاد","ReqNo":"1571"}', NULL, '2016-08-28 01:03:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5373, 23263, '', '{"FileName":"bonn, 2001.pdf","UserName":"مهدی","ReqNo":"1573"}', NULL, '2016-08-28 12:58:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5375, 23263, '', '{"FileName":"bonn, 2001.pdf","UserName":"مهدی","ReqNo":"1573"}', NULL, '2016-08-29 01:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5377, 23357, '', '{"FileName":"Untitled-1.pdf","UserName":"علی","ReqNo":"1575"}', NULL, '2016-08-29 10:19:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5379, 23357, '', '{"FileName":"Untitled-1.pdf","UserName":"علی","ReqNo":"1575"}', NULL, '2016-08-30 01:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5381, 2157, '', '{"FileName":"sr505.pdf","UserName":"محمد سعید","ReqNo":"1577"}', NULL, '2016-08-30 23:36:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5383, 2157, '', '{"FileName":"sr505.pdf","UserName":"محمد سعید","ReqNo":"1577"}', NULL, '2016-08-31 01:00:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5385, 23575, '', '{"FileName":"رزومه.pdf","UserName":"علي","ReqNo":"1579"}', NULL, '2016-08-31 15:00:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5387, 23575, '', '{"FileName":"رزومه.pdf","UserName":"علي","ReqNo":"1579"}', NULL, '2016-09-01 01:00:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5389, 23651, '', '{"FileName":"Internet_of_Things_a_New_Application_for_Intellige.pdf","UserName":"ali","ReqNo":"1581"}', NULL, '2016-09-01 23:44:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5391, 23651, '', '{"FileName":"Internet_of_Things_a_New_Application_for_Intellige.pdf","UserName":"ali","ReqNo":"1581"}', NULL, '2016-09-02 01:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5393, 23689, '', '{"FileName":"paper 18.pdf","UserName":"مجید","ReqNo":"1583"}', NULL, '2016-09-02 12:15:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5395, 23689, '', '{"FileName":"paper 14.pdf","UserName":"مجید","ReqNo":"1585"}', NULL, '2016-09-02 14:22:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5397, 23709, '', '{"FileName":"Mm.pdf","UserName":"اورانوس","ReqNo":"1587"}', NULL, '2016-09-02 20:29:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5399, 23689, '', '{"FileName":"paper 18.pdf","UserName":"مجید","ReqNo":"1583"}', NULL, '2016-09-03 01:00:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5401, 23689, '', '{"FileName":"paper 14.pdf","UserName":"مجید","ReqNo":"1585"}', NULL, '2016-09-03 01:01:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5403, 23709, '', '{"FileName":"Mm.pdf","UserName":"اورانوس","ReqNo":"1587"}', NULL, '2016-09-03 01:12:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5405, 23831, '', '{"FileName":"AP2 Accounting for Goodwill.pdf","UserName":"atena","ReqNo":"1589"}', NULL, '2016-09-04 10:45:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5407, 23831, '', '{"FileName":"AP2 Accounting for Goodwill.pdf","UserName":"atena","ReqNo":"1589"}', NULL, '2016-09-05 01:00:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5409, 20747, '', '{"FileName":"Seifi.pdf","UserName":"سيف اله","ReqNo":"1591"}', NULL, '2016-09-05 12:28:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5411, 23831, '', '{"FileName":"AP2 Accounting for Goodwill (2).pdf","UserName":"atena","ReqNo":"1593"}', NULL, '2016-09-05 18:21:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5413, 20747, '', '{"FileName":"Seifi.pdf","UserName":"سيف اله","ReqNo":"1591"}', NULL, '2016-09-06 01:01:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5415, 23831, '', '{"FileName":"AP2 Accounting for Goodwill (2).pdf","UserName":"atena","ReqNo":"1593"}', NULL, '2016-09-06 01:01:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5417, 23039, '', '{"FileName":"Theory-of-Change-Tool-for-Strategic-Planning-Report-on-Early-Experiences_2.pdf","UserName":"محمد","ReqNo":"1595"}', NULL, '2016-09-09 07:44:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5419, 23039, '', '{"FileName":"5803-23175-1-PB.pdf","UserName":"محمد","ReqNo":"1597"}', NULL, '2016-09-09 07:45:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5421, 23039, '', '{"FileName":"5803-23175-1-PB.pdf","UserName":"محمد","ReqNo":"1599"}', NULL, '2016-09-09 07:45:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5423, 23039, '', '{"FileName":"Theory-of-Change-Tool-for-Strategic-Planning-Report-on-Early-Experiences_2.pdf","UserName":"محمد","ReqNo":"1595"}', NULL, '2016-09-10 01:00:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5425, 23039, '', '{"FileName":"5803-23175-1-PB.pdf","UserName":"محمد","ReqNo":"1597"}', NULL, '2016-09-10 01:01:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5427, 23039, '', '{"FileName":"5803-23175-1-PB.pdf","UserName":"محمد","ReqNo":"1599"}', NULL, '2016-09-10 01:01:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5429, 24391, '', '{"FileName":"ch.docx","UserName":"cafe","ReqNo":"1601"}', NULL, '2016-09-11 23:27:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5431, 24391, '', '{"FileName":"ch.docx","UserName":"cafe","ReqNo":"1601"}', NULL, '2016-09-12 01:00:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5433, 24403, '', '{"FileName":"10.1049el.2010.2888_4d70124f50128de67780a68c3482a701.pdf","UserName":"امیر","ReqNo":"1603"}', NULL, '2016-09-12 09:14:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5435, 24403, '', '{"FileName":"لبه خطی با رجلت.pdf","UserName":"امیر","ReqNo":"1605"}', NULL, '2016-09-12 09:16:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5437, 24403, '', '{"FileName":"10.1049el.2010.2888_4d70124f50128de67780a68c3482a701.pdf","UserName":"امیر","ReqNo":"1603"}', NULL, '2016-09-13 01:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5439, 24403, '', '{"FileName":"لبه خطی با رجلت.pdf","UserName":"امیر","ReqNo":"1605"}', NULL, '2016-09-13 01:00:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5441, 24647, '', '{"FileName":"ABSTRACT OF DISSERTATION.docx","UserName":"هادی","ReqNo":"1607"}', NULL, '2016-09-15 18:29:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5443, 24647, '', '{"FileName":"ABSTRACT OF DISSERTATION.docx","UserName":"هادی","ReqNo":"1607"}', NULL, '2016-09-16 01:00:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5445, 25001, '', '{"FileName":"Marketing_13_Promoting_Products.pdf","UserName":"لیلا","ReqNo":"1609"}', NULL, '2016-09-21 18:50:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5447, 25015, '', '{"FileName":"3333333333.docx","UserName":"Reza","ReqNo":"1611"}', NULL, '2016-09-21 18:53:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5449, 25001, '', '{"FileName":"Marketing_13_Promoting_Products.pdf","UserName":"لیلا","ReqNo":"1609"}', NULL, '2016-09-22 01:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5451, 25015, '', '{"FileName":"3333333333.docx","UserName":"Reza","ReqNo":"1611"}', NULL, '2016-09-22 01:00:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5453, 14385, '', '{"FileName":"chap 21-JH.docx","UserName":"نادیا","ReqNo":"1613"}', NULL, '2016-09-22 11:43:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5455, 25077, '', '{"FileName":"رزومه.docx","UserName":"آگرین","ReqNo":"1615"}', NULL, '2016-09-22 22:45:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5457, 14385, '', '{"FileName":"chap 21-JH.docx","UserName":"نادیا","ReqNo":"1613"}', NULL, '2016-09-23 01:00:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5459, 25077, '', '{"FileName":"رزومه.docx","UserName":"آگرین","ReqNo":"1615"}', NULL, '2016-09-23 01:00:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5461, 25103, '', '{"FileName":"Occupancy Models.pdf","UserName":"تمنا","ReqNo":"1617"}', NULL, '2016-09-23 07:34:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5463, 25103, '', '{"FileName":"PRESENCE_examples_and_exercises.pdf عالی نرم افزار (1).pdf","UserName":"تمنا","ReqNo":"1619"}', NULL, '2016-09-23 07:35:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5465, 25105, '', '{"FileName":"Paper-SH.docx","UserName":"اسماعیل","ReqNo":"1621"}', NULL, '2016-09-23 09:32:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5467, 25107, '', '{"FileName":"T1.docx","UserName":"Nasim","ReqNo":"1623"}', NULL, '2016-09-23 10:49:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5469, 25119, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"محسن","ReqNo":"1625"}', NULL, '2016-09-23 12:16:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5471, 25145, '', '{"FileName":"Durkin, R. 14.pdf","UserName":"فرهاد","ReqNo":"1627"}', NULL, '2016-09-24 00:56:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5473, 25145, '', '{"FileName":"Dialnet-ElaboratingDialogueInCommunitiesOfInquiry-5278637.pdf","UserName":"فرهاد","ReqNo":"1629"}', NULL, '2016-09-24 00:58:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5475, 25103, '', '{"FileName":"Occupancy Models.pdf","UserName":"تمنا","ReqNo":"1617"}', NULL, '2016-09-24 01:00:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5477, 25103, '', '{"FileName":"PRESENCE_examples_and_exercises.pdf عالی نرم افزار (1).pdf","UserName":"تمنا","ReqNo":"1619"}', NULL, '2016-09-24 01:00:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5479, 25105, '', '{"FileName":"Paper-SH.docx","UserName":"اسماعیل","ReqNo":"1621"}', NULL, '2016-09-24 01:01:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5481, 25107, '', '{"FileName":"T1.docx","UserName":"Nasim","ReqNo":"1623"}', NULL, '2016-09-24 01:01:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5483, 25119, '', '{"FileName":"New Microsoft Word Document.docx","UserName":"محسن","ReqNo":"1625"}', NULL, '2016-09-24 01:01:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5485, 25145, '', '{"FileName":"Durkin, R. 14.pdf","UserName":"فرهاد","ReqNo":"1627"}', NULL, '2016-09-24 01:04:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5487, 25145, '', '{"FileName":"Dialnet-ElaboratingDialogueInCommunitiesOfInquiry-5278637.pdf","UserName":"فرهاد","ReqNo":"1629"}', NULL, '2016-09-24 01:06:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5489, 25251, '', '{"FileName":"seo.pdf","UserName":"علی","ReqNo":"1631"}', NULL, '2016-09-25 17:34:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5491, 25251, '', '{"FileName":"seo.pdf","UserName":"علی","ReqNo":"1631"}', NULL, '2016-09-26 01:00:09', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5493, 25421, '', '{"FileName":"M 1 Mirzaii.pdf","UserName":"داود","ReqNo":"1633"}', NULL, '2016-09-28 10:23:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5495, 25421, '', '{"FileName":"M 1 Mirzaii.pdf","UserName":"داود","ReqNo":"1633"}', NULL, '2016-09-29 01:02:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5497, 25553, '', '{"FileName":"مقاله غروی (1).docx","UserName":"behrooz","ReqNo":"1635"}', NULL, '2016-09-29 22:47:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5499, 25559, '', '{"FileName":"How many days has it been since the last Google Penguin Update.docx","UserName":"ali","ReqNo":"1637"}', NULL, '2016-09-29 23:58:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5501, 25553, '', '{"FileName":"مقاله غروی (1).docx","UserName":"behrooz","ReqNo":"1635"}', NULL, '2016-09-30 01:00:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5503, 25559, '', '{"FileName":"How many days has it been since the last Google Penguin Update.docx","UserName":"ali","ReqNo":"1637"}', NULL, '2016-09-30 01:00:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5505, 25575, '', '{"FileName":"Access Control and Authentication in the Internet of Things Environment_2016.PDF","UserName":"احمد","ReqNo":"1639"}', NULL, '2016-09-30 10:23:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5507, 25595, '', '{"FileName":"afkarnews.docx","UserName":"امید","ReqNo":"1641"}', NULL, '2016-09-30 16:26:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5509, 25575, '', '{"FileName":"Access Control and Authentication in the Internet of Things Environment_2016.PDF","UserName":"احمد","ReqNo":"1639"}', NULL, '2016-10-01 01:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5511, 25595, '', '{"FileName":"afkarnews.docx","UserName":"امید","ReqNo":"1641"}', NULL, '2016-10-01 01:00:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5513, 25663, '', '{"FileName":"New Microsoft Word Document (2).docx","UserName":"مسعود","ReqNo":"1643"}', NULL, '2016-10-01 11:50:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5515, 25663, '', '{"FileName":"New Microsoft Word Document (2).docx","UserName":"مسعود","ReqNo":"1643"}', NULL, '2016-10-02 01:01:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5517, 25745, '', '{"FileName":"New Doc 3.pdf","UserName":"محسن","ReqNo":"1645"}', NULL, '2016-10-02 10:16:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5519, 3539, '', '{"FileName":"Paper_AMultiAgentBasedHierarchicalControlSystemForDERsManagementInIslandedMicroGrid.pdf","UserName":"Fatemeh","ReqNo":"1647"}', NULL, '2016-10-02 13:39:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5521, 3539, '', '{"FileName":"Paper_AMultiAgentBasedHierarchicalControlSystemForDERsManagementInIslandedMicroGrid.pdf","UserName":"Fatemeh","ReqNo":"1649"}', NULL, '2016-10-02 14:04:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5523, 25745, '', '{"FileName":"New Doc 3.pdf","UserName":"محسن","ReqNo":"1645"}', NULL, '2016-10-03 01:00:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5525, 3539, '', '{"FileName":"Paper_AMultiAgentBasedHierarchicalControlSystemForDERsManagementInIslandedMicroGrid.pdf","UserName":"Fatemeh","ReqNo":"1647"}', NULL, '2016-10-03 01:00:10', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5527, 3539, '', '{"FileName":"Paper_AMultiAgentBasedHierarchicalControlSystemForDERsManagementInIslandedMicroGrid.pdf","UserName":"Fatemeh","ReqNo":"1649"}', NULL, '2016-10-03 01:00:10', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5529, 25871, '', '{"FileName":"2015-elsevier-4-Vehicular-delay-tolerant-networks-VDTNs.pdf","UserName":"majidreza","ReqNo":"1651"}', NULL, '2016-10-03 16:43:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5531, 25871, '', '{"FileName":"2016-springer-Neighbor-stability-based-VANET-clustering-for-urban-vehicular-environments.pdf","UserName":"majidreza","ReqNo":"1653"}', NULL, '2016-10-03 16:52:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5533, 25889, '', '{"FileName":"www.FreePaper.us_3-s2.0-b978184569765550007x-main.pdf","UserName":"پیام","ReqNo":"1655"}', NULL, '2016-10-03 17:44:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5535, 25941, '', '{"FileName":"aaa.pdf","UserName":"farzad","ReqNo":"1657"}', NULL, '2016-10-03 22:38:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5537, 25871, '', '{"FileName":"2015-elsevier-4-Vehicular-delay-tolerant-networks-VDTNs.pdf","UserName":"majidreza","ReqNo":"1651"}', NULL, '2016-10-04 01:00:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5539, 25871, '', '{"FileName":"2016-springer-Neighbor-stability-based-VANET-clustering-for-urban-vehicular-environments.pdf","UserName":"majidreza","ReqNo":"1653"}', NULL, '2016-10-04 01:00:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5541, 25889, '', '{"FileName":"www.FreePaper.us_3-s2.0-b978184569765550007x-main.pdf","UserName":"پیام","ReqNo":"1655"}', NULL, '2016-10-04 01:01:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5543, 25941, '', '{"FileName":"aaa.pdf","UserName":"farzad","ReqNo":"1657"}', NULL, '2016-10-04 01:02:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5545, 25953, '', '{"FileName":"Microsoft Word - Print_Version.pdf","UserName":"ali","ReqNo":"1659"}', NULL, '2016-10-04 10:58:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5547, 25967, '', '{"FileName":"-Clim Temp Medic Health N1146 2014 LA.pdf","UserName":"mitra","ReqNo":"1661"}', NULL, '2016-10-04 13:00:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5549, 25967, '', '{"FileName":"IDENTIFICATION AND ANALYSIS OF THE NORTH ATLANTIC BLOCKINGS_2.pdf","UserName":"mitra","ReqNo":"1663"}', NULL, '2016-10-04 13:06:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5551, 25953, '', '{"FileName":"Microsoft Word - Print_Version.pdf","UserName":"ali","ReqNo":"1659"}', NULL, '2016-10-05 01:06:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5553, 25967, '', '{"FileName":"-Clim Temp Medic Health N1146 2014 LA.pdf","UserName":"mitra","ReqNo":"1661"}', NULL, '2016-10-05 01:09:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5555, 25967, '', '{"FileName":"IDENTIFICATION AND ANALYSIS OF THE NORTH ATLANTIC BLOCKINGS_2.pdf","UserName":"mitra","ReqNo":"1663"}', NULL, '2016-10-05 01:11:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5557, 26059, '', '{"FileName":"4_546071586646524020.pdf","UserName":"مجتبی","ReqNo":"1665"}', NULL, '2016-10-05 11:31:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5559, 26061, '', '{"FileName":"ATPOLAT.pdf","UserName":"محسن","ReqNo":"1667"}', NULL, '2016-10-05 12:33:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5561, 26085, '', '{"FileName":"بررسی عوامل اجتماعی هوشمند دریک شهر.pdf","UserName":"فاطمه","ReqNo":"1669"}', NULL, '2016-10-05 16:27:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5563, 26059, '', '{"FileName":"4_546071586646524020.pdf","UserName":"مجتبی","ReqNo":"1665"}', NULL, '2016-10-06 01:00:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5565, 26061, '', '{"FileName":"ATPOLAT.pdf","UserName":"محسن","ReqNo":"1667"}', NULL, '2016-10-06 01:00:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5567, 26085, '', '{"FileName":"بررسی عوامل اجتماعی هوشمند دریک شهر.pdf","UserName":"فاطمه","ReqNo":"1669"}', NULL, '2016-10-06 01:01:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5569, 26159, '', '{"FileName":"(Wiley Blackwell Companions to Religion) Robert A.   Segal-The Blackwell Companion to the Study of  ","UserName":"Zohreh","ReqNo":"1671"}', NULL, '2016-10-06 11:40:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5571, 26159, '', '{"FileName":"(Wiley Blackwell Companions to Religion) Robert A.   Segal-The Blackwell Companion to the Study of  ","UserName":"Zohreh","ReqNo":"1671"}', NULL, '2016-10-07 01:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5573, 26343, '', '{"FileName":"implementing_public_policy.pdf","UserName":"یاسر","ReqNo":"1673"}', NULL, '2016-10-07 18:27:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5575, 26343, '', '{"FileName":"implementing_public_policy.pdf","UserName":"یاسر","ReqNo":"1673"}', NULL, '2016-10-08 01:13:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5577, 26419, '', '{"FileName":"03062015.pdf","UserName":"narges","ReqNo":"1675"}', NULL, '2016-10-08 12:30:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5579, 26419, '', '{"FileName":"ccopارزیابی ریسک.pdf","UserName":"narges","ReqNo":"1677"}', NULL, '2016-10-08 12:43:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5581, 26419, '', '{"FileName":"چالش های جغرافیایی دانشمندان.pdf","UserName":"narges","ReqNo":"1679"}', NULL, '2016-10-08 12:44:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5583, 26419, '', '{"FileName":"ارزیابی ریسک های فنی.pdf","UserName":"narges","ReqNo":"1681"}', NULL, '2016-10-08 12:45:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5585, 26419, '', '{"FileName":"Land-use-conflict-risk-assessment-LUCRA-guide.pdf","UserName":"narges","ReqNo":"1683"}', NULL, '2016-10-08 12:45:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5587, 26419, '', '{"FileName":"ارزیابی خطرفرسایش خاک دراروپا.pdf","UserName":"narges","ReqNo":"1685"}', NULL, '2016-10-08 12:45:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5589, 26419, '', '{"FileName":"ارزیابی ریسک بیابانزایی وبرنامه های مدیریت.pdf","UserName":"narges","ReqNo":"1687"}', NULL, '2016-10-08 12:46:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5591, 26419, '', '{"FileName":"روش شبکه بندی برای ارزیابی زمین شناسی.pdf","UserName":"narges","ReqNo":"1689"}', NULL, '2016-10-08 12:47:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5593, 26423, '', '{"FileName":"general system theory.pdf","UserName":"شادی","ReqNo":"1691"}', NULL, '2016-10-08 13:40:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5595, 14671, '', '{"FileName":"MinMax_k-means.pdf","UserName":"مهدی","ReqNo":"1693"}', NULL, '2016-10-08 21:42:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5597, 26419, '', '{"FileName":"03062015.pdf","UserName":"narges","ReqNo":"1675"}', NULL, '2016-10-09 01:01:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5599, 26419, '', '{"FileName":"ccopارزیابی ریسک.pdf","UserName":"narges","ReqNo":"1677"}', NULL, '2016-10-09 01:05:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5601, 26419, '', '{"FileName":"چالش های جغرافیایی دانشمندان.pdf","UserName":"narges","ReqNo":"1679"}', NULL, '2016-10-09 01:08:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5603, 26419, '', '{"FileName":"ارزیابی ریسک های فنی.pdf","UserName":"narges","ReqNo":"1681"}', NULL, '2016-10-09 01:09:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5605, 26419, '', '{"FileName":"Land-use-conflict-risk-assessment-LUCRA-guide.pdf","UserName":"narges","ReqNo":"1683"}', NULL, '2016-10-09 01:11:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5607, 26419, '', '{"FileName":"ارزیابی خطرفرسایش خاک دراروپا.pdf","UserName":"narges","ReqNo":"1685"}', NULL, '2016-10-09 01:15:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5609, 26419, '', '{"FileName":"ارزیابی ریسک بیابانزایی وبرنامه های مدیریت.pdf","UserName":"narges","ReqNo":"1687"}', NULL, '2016-10-09 01:17:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5611, 26419, '', '{"FileName":"روش شبکه بندی برای ارزیابی زمین شناسی.pdf","UserName":"narges","ReqNo":"1689"}', NULL, '2016-10-09 01:18:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5613, 26423, '', '{"FileName":"general system theory.pdf","UserName":"شادی","ReqNo":"1691"}', NULL, '2016-10-09 01:19:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5615, 14671, '', '{"FileName":"MinMax_k-means.pdf","UserName":"مهدی","ReqNo":"1693"}', NULL, '2016-10-09 01:21:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5617, 26507, '', '{"FileName":"TPWRS.2012.2232682_9da74c97a15509017d661cf7af18f721.pdf","UserName":"مهدی","ReqNo":"1695"}', NULL, '2016-10-09 13:05:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5619, 26547, '', '{"FileName":"ch2-el-65.pdf","UserName":"مینو","ReqNo":"1697"}', NULL, '2016-10-09 22:15:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5621, 26507, '', '{"FileName":"TPWRS.2012.2232682_9da74c97a15509017d661cf7af18f721.pdf","UserName":"مهدی","ReqNo":"1695"}', NULL, '2016-10-10 01:00:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5623, 26547, '', '{"FileName":"ch2-el-65.pdf","UserName":"مینو","ReqNo":"1697"}', NULL, '2016-10-10 01:01:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5625, 26569, '', '{"FileName":"obeydi.doc","UserName":"نیما","ReqNo":"1699"}', NULL, '2016-10-10 09:41:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5627, 26569, '', '{"FileName":"obeydi.doc","UserName":"نیما","ReqNo":"1699"}', NULL, '2016-10-11 01:00:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5629, 26419, '', '{"FileName":"The Risk Management Programing for Urban Systems.pdf","UserName":"narges","ReqNo":"1701"}', NULL, '2016-10-11 10:19:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5631, 26419, '', '{"FileName":"ارزیابی ریسک آلودگی حوضه آبریز گلستان با استفاده از مدل WRASTIC.pdf","UserName":"narges","ReqNo":"1703"}', NULL, '2016-10-11 10:20:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5633, 26419, '', '{"FileName":"روش شبکه بندی برای ارزیابی زمین شناسی.pdf","UserName":"narges","ReqNo":"1705"}', NULL, '2016-10-11 10:22:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5635, 26419, '', '{"FileName":"هیدرولوژی وارزیابی خطرسیل.pdf","UserName":"narges","ReqNo":"1707"}', NULL, '2016-10-11 10:23:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5637, 26419, '', '{"FileName":"ارزیابی ریسک زمین لغزش2002.pdf","UserName":"narges","ReqNo":"1709"}', NULL, '2016-10-11 10:24:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5639, 26419, '', '{"FileName":"ارزیابی ریسک تغییرات اب وهوا.pdf","UserName":"narges","ReqNo":"1711"}', NULL, '2016-10-11 10:25:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5641, 26419, '', '{"FileName":"ارزیابی ریسک بهداشت زمین.pdf","UserName":"narges","ReqNo":"1713"}', NULL, '2016-10-11 10:26:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5643, 26419, '', '{"FileName":"tartakovsky-2013-assessment.pdf","UserName":"narges","ReqNo":"1715"}', NULL, '2016-10-11 10:28:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5645, 26419, '', '{"FileName":"USU_Manual_of_Field_Safety_AppendD.pdf","UserName":"narges","ReqNo":"1717"}', NULL, '2016-10-11 10:29:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5647, 26419, '', '{"FileName":"Quantitative-rock-fall-hazard-and-risk-assessment-for-Yosemite-Valley-April-2012.pdf","UserName":"narges","ReqNo":"1719"}', NULL, '2016-10-11 10:31:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5649, 26419, '', '{"FileName":"2013-y17Gx3-ADPC-Safer_Cities_26.pdf","UserName":"narges","ReqNo":"1721"}', NULL, '2016-10-11 10:33:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5651, 26419, '', '{"FileName":"48588_unisdrannualreport2015evs.pdf","UserName":"narges","ReqNo":"1723"}', NULL, '2016-10-11 10:36:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5653, 26419, '', '{"FileName":"ارزیابی ریسک برای موجودات اصلاح شده.pdf","UserName":"narges","ReqNo":"1725"}', NULL, '2016-10-11 10:38:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5655, 26639, '', '{"FileName":"ISIA.pdf","UserName":"ali","ReqNo":"1727"}', NULL, '2016-10-11 15:43:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5657, 26639, '', '{"FileName":"s63.pdf","UserName":"ali","ReqNo":"1729"}', NULL, '2016-10-11 15:44:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5659, 26681, '', '{"FileName":"WHAT_IS_STRATEGY0.pdf","UserName":"سید","ReqNo":"1731"}', NULL, '2016-10-11 21:08:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5661, 26685, '', '{"FileName":"transitions_theory__middle_range_and_situation_specific_theories_in_nursing_.pdf","UserName":"سید محمد","ReqNo":"1733"}', NULL, '2016-10-11 21:42:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5663, 26419, '', '{"FileName":"The Risk Management Programing for Urban Systems.pdf","UserName":"narges","ReqNo":"1701"}', NULL, '2016-10-12 01:00:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5665, 26419, '', '{"FileName":"ارزیابی ریسک آلودگی حوضه آبریز گلستان با استفاده از مدل WRASTIC.pdf","UserName":"narges","ReqNo":"1703"}', NULL, '2016-10-12 01:00:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5667, 26419, '', '{"FileName":"روش شبکه بندی برای ارزیابی زمین شناسی.pdf","UserName":"narges","ReqNo":"1705"}', NULL, '2016-10-12 01:00:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5669, 26419, '', '{"FileName":"هیدرولوژی وارزیابی خطرسیل.pdf","UserName":"narges","ReqNo":"1707"}', NULL, '2016-10-12 01:00:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5671, 26419, '', '{"FileName":"ارزیابی ریسک زمین لغزش2002.pdf","UserName":"narges","ReqNo":"1709"}', NULL, '2016-10-12 01:01:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5673, 26419, '', '{"FileName":"ارزیابی ریسک تغییرات اب وهوا.pdf","UserName":"narges","ReqNo":"1711"}', NULL, '2016-10-12 01:01:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5675, 26419, '', '{"FileName":"ارزیابی ریسک بهداشت زمین.pdf","UserName":"narges","ReqNo":"1713"}', NULL, '2016-10-12 01:25:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5677, 26419, '', '{"FileName":"tartakovsky-2013-assessment.pdf","UserName":"narges","ReqNo":"1715"}', NULL, '2016-10-12 01:28:18', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5679, 26419, '', '{"FileName":"USU_Manual_of_Field_Safety_AppendD.pdf","UserName":"narges","ReqNo":"1717"}', NULL, '2016-10-12 01:31:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5681, 26419, '', '{"FileName":"Quantitative-rock-fall-hazard-and-risk-assessment-for-Yosemite-Valley-April-2012.pdf","UserName":"narges","ReqNo":"1719"}', NULL, '2016-10-12 01:38:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5683, 26419, '', '{"FileName":"2013-y17Gx3-ADPC-Safer_Cities_26.pdf","UserName":"narges","ReqNo":"1721"}', NULL, '2016-10-12 01:41:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5685, 26419, '', '{"FileName":"48588_unisdrannualreport2015evs.pdf","UserName":"narges","ReqNo":"1723"}', NULL, '2016-10-12 01:52:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5687, 26419, '', '{"FileName":"ارزیابی ریسک برای موجودات اصلاح شده.pdf","UserName":"narges","ReqNo":"1725"}', NULL, '2016-10-12 01:55:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5689, 26639, '', '{"FileName":"ISIA.pdf","UserName":"ali","ReqNo":"1727"}', NULL, '2016-10-12 01:56:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5691, 26639, '', '{"FileName":"s63.pdf","UserName":"ali","ReqNo":"1729"}', NULL, '2016-10-12 02:06:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5693, 26681, '', '{"FileName":"WHAT_IS_STRATEGY0.pdf","UserName":"سید","ReqNo":"1731"}', NULL, '2016-10-12 02:10:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5695, 26685, '', '{"FileName":"transitions_theory__middle_range_and_situation_specific_theories_in_nursing_.pdf","UserName":"سید محمد","ReqNo":"1733"}', NULL, '2016-10-12 02:12:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5697, 26861, '', '{"FileName":"1.docx","UserName":"roya","ReqNo":"1735"}', NULL, '2016-10-13 15:27:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5699, 26881, '', '{"FileName":"[Elearnica] -Organizational_Strategy__Structure__and_Process_ (2).pdf","UserName":"علی","ReqNo":"1737"}', NULL, '2016-10-13 22:37:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5701, 26861, '', '{"FileName":"1.docx","UserName":"roya","ReqNo":"1735"}', NULL, '2016-10-14 01:00:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5703, 26881, '', '{"FileName":"[Elearnica] -Organizational_Strategy__Structure__and_Process_ (2).pdf","UserName":"علی","ReqNo":"1737"}', NULL, '2016-10-14 01:00:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5705, 26881, '', '{"FileName":"[Elearnica] -Organizational_Strategy__Structure__and_Process_ (2).pdf","UserName":"علی","ReqNo":"1739"}', NULL, '2016-10-14 20:26:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5707, 26881, '', '{"FileName":"[Elearnica] -Organizational_Strategy__Structure__and_Process_ (2).pdf","UserName":"علی","ReqNo":"1739"}', NULL, '2016-10-15 01:00:37', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5709, 27003, '', '{"FileName":"8-Tax_reforms_and_stock_return_volatility__The_case_of_Japan.pdf","UserName":"سارا","ReqNo":"1741"}', NULL, '2016-10-16 22:04:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5711, 27003, '', '{"FileName":"8-Tax_reforms_and_stock_return_volatility__The_case_of_Japan.pdf","UserName":"سارا","ReqNo":"1741"}', NULL, '2016-10-17 01:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5713, 27117, '', '{"FileName":"learned+helplessness1 زبان تخصصی.docx","UserName":"زهره","ReqNo":"1743"}', NULL, '2016-10-17 09:58:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5715, 27147, '', '{"FileName":"Understanding the Management Skills Pyramid.docx","UserName":"پرستو","ReqNo":"1745"}', NULL, '2016-10-17 16:28:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5717, 27117, '', '{"FileName":"learned+helplessness1 زبان تخصصی.docx","UserName":"زهره","ReqNo":"1743"}', NULL, '2016-10-18 01:00:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5719, 27147, '', '{"FileName":"Understanding the Management Skills Pyramid.docx","UserName":"پرستو","ReqNo":"1745"}', NULL, '2016-10-18 01:00:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5721, 27797, '', '{"FileName":"4_589987558528647300.pdf","UserName":"الهه","ReqNo":"1747"}', NULL, '2016-10-22 00:25:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5723, 27797, '', '{"FileName":"4_589987558528647300.pdf","UserName":"الهه","ReqNo":"1747"}', NULL, '2016-10-22 01:02:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5725, 26323, '', '{"FileName":"1053-russian-federation-oemv-verchenmurbaisk-area-otradninsk-area-psc.pdf","UserName":"milad","ReqNo":"1749"}', NULL, '2016-10-22 10:49:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5727, 31975, '', '{"FileName":"SSRN-id1600945.pdf","UserName":"رضا","ReqNo":"1751"}', NULL, '2016-10-22 16:29:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5729, 31983, '', '{"FileName":"پی.pdf","UserName":"علی","ReqNo":"1753"}', NULL, '2016-10-22 18:22:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5731, 31983, '', '{"FileName":"پی.pdf","UserName":"علی","ReqNo":"1755"}', NULL, '2016-10-22 18:24:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5733, 31993, '', '{"FileName":"Untitled.docx","UserName":"سهیل","ReqNo":"1757"}', NULL, '2016-10-22 20:40:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5735, 26323, '', '{"FileName":"1053-russian-federation-oemv-verchenmurbaisk-area-otradninsk-area-psc.pdf","UserName":"milad","ReqNo":"1749"}', NULL, '2016-10-23 01:09:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5737, 31975, '', '{"FileName":"SSRN-id1600945.pdf","UserName":"رضا","ReqNo":"1751"}', NULL, '2016-10-23 01:14:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5739, 31983, '', '{"FileName":"پی.pdf","UserName":"علی","ReqNo":"1753"}', NULL, '2016-10-23 01:14:14', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5741, 31983, '', '{"FileName":"پی.pdf","UserName":"علی","ReqNo":"1755"}', NULL, '2016-10-23 01:14:14', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5743, 31993, '', '{"FileName":"Untitled.docx","UserName":"سهیل","ReqNo":"1757"}', NULL, '2016-10-23 01:15:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5745, 32045, '', '{"FileName":"finance for engineering code 31.pdf","UserName":"پیام","ReqNo":"1759"}', NULL, '2016-10-23 11:36:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5747, 32035, '', '{"FileName":"4_786426138443383102.pdf","UserName":"موسی","ReqNo":"1761"}', NULL, '2016-10-23 12:45:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5749, 32045, '', '{"FileName":"finance for engineering code 31.pdf","UserName":"پیام","ReqNo":"1759"}', NULL, '2016-10-24 01:01:26', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5751, 32035, '', '{"FileName":"4_786426138443383102.pdf","UserName":"موسی","ReqNo":"1761"}', NULL, '2016-10-24 01:09:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5753, 25871, '', '{"FileName":"vanet_survey.pdf","UserName":"majidreza","ReqNo":"1763"}', NULL, '2016-10-24 22:33:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5755, 25871, '', '{"FileName":"vanet_survey.pdf","UserName":"majidreza","ReqNo":"1763"}', NULL, '2016-10-25 01:01:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5757, 23039, '', '{"FileName":"22pdf.PDF","UserName":"محمد","ReqNo":"1765"}', NULL, '2016-10-26 14:55:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5759, 23039, '', '{"FileName":"22pdf.PDF","UserName":"محمد","ReqNo":"1765"}', NULL, '2016-10-27 01:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5761, 32447, '', '{"FileName":"Nanorobotics - Current Approaches and Techniques.pdf","UserName":"milad","ReqNo":"1767"}', NULL, '2016-10-27 21:11:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5763, 32447, '', '{"FileName":"Nanorobotics - Current Approaches and Techniques.pdf","UserName":"milad","ReqNo":"1767"}', NULL, '2016-10-28 01:01:22', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5765, 32519, '', '{"FileName":"android.pdf","UserName":"وحید","ReqNo":"1769"}', NULL, '2016-10-29 10:08:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5767, 32519, '', '{"FileName":"4_704311758222787000.pdf","UserName":"وحید","ReqNo":"1771"}', NULL, '2016-10-29 10:15:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5769, 32519, '', '{"FileName":"4_400491436765614722.pdf","UserName":"وحید","ReqNo":"1773"}', NULL, '2016-10-29 10:17:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5771, 24389, '', '{"FileName":"using genetic programming and simulation to learn how to dynamically adapt the number of cards in re","UserName":"یونس","ReqNo":"1775"}', NULL, '2016-10-29 21:22:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5773, 32519, '', '{"FileName":"android.pdf","UserName":"وحید","ReqNo":"1769"}', NULL, '2016-10-30 01:01:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5775, 32519, '', '{"FileName":"4_704311758222787000.pdf","UserName":"وحید","ReqNo":"1771"}', NULL, '2016-10-30 01:01:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5777, 32519, '', '{"FileName":"4_400491436765614722.pdf","UserName":"وحید","ReqNo":"1773"}', NULL, '2016-10-30 01:33:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5779, 24389, '', '{"FileName":"using genetic programming and simulation to learn how to dynamically adapt the number of cards in re","UserName":"یونس","ReqNo":"1775"}', NULL, '2016-10-30 01:33:43', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5781, 32625, '', '{"FileName":"2016. The relationship between children-\'s motor  proficiency and health-related fitness (1).pdf","UserName":"Fereshte","ReqNo":"1777"}', NULL, '2016-10-30 16:56:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5783, 32631, '', '{"FileName":"[Elearnica] -Targeting_allergenic_fungi_in_agricultural_environments_aids_the_identifica.pdf","UserName":"لیلا","ReqNo":"1779"}', NULL, '2016-10-30 19:17:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5785, 32631, '', '{"FileName":"[Elearnica] -Ploidy_variation_as_an_adaptive_mechanism_in_human_pathogenic_fungi.pdf","UserName":"لیلا","ReqNo":"1781"}', NULL, '2016-10-30 19:18:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5787, 32625, '', '{"FileName":"2016. The relationship between children-\'s motor  proficiency and health-related fitness (1).pdf","UserName":"Fereshte","ReqNo":"1777"}', NULL, '2016-10-31 01:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5789, 32631, '', '{"FileName":"[Elearnica] -Targeting_allergenic_fungi_in_agricultural_environments_aids_the_identifica.pdf","UserName":"لیلا","ReqNo":"1779"}', NULL, '2016-10-31 01:00:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5791, 32631, '', '{"FileName":"[Elearnica] -Ploidy_variation_as_an_adaptive_mechanism_in_human_pathogenic_fungi.pdf","UserName":"لیلا","ReqNo":"1781"}', NULL, '2016-10-31 01:01:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5793, 32045, '', '{"FileName":"finance for engineering code 3.pdf","UserName":"پیام","ReqNo":"1783"}', NULL, '2016-10-31 18:13:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5795, 32721, '', '{"FileName":"Doc1.docx","UserName":"javad","ReqNo":"1785"}', NULL, '2016-10-31 19:22:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5797, 32755, '', '{"FileName":"DeXpression.docx","UserName":"مهتا","ReqNo":"1787"}', NULL, '2016-11-01 00:36:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5799, 32755, '', '{"FileName":"2016-تشخیص خودکار حالات چهره با استفاده از شبکه های عصبی کانولوشنی عمیق.pdf","UserName":"مهتا","ReqNo":"1789"}', NULL, '2016-11-01 00:39:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5801, 32045, '', '{"FileName":"finance for engineering code 3.pdf","UserName":"پیام","ReqNo":"1783"}', NULL, '2016-11-01 01:01:47', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5803, 32721, '', '{"FileName":"Doc1.docx","UserName":"javad","ReqNo":"1785"}', NULL, '2016-11-01 01:02:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5805, 32755, '', '{"FileName":"DeXpression.docx","UserName":"مهتا","ReqNo":"1787"}', NULL, '2016-11-01 01:03:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5807, 32755, '', '{"FileName":"2016-تشخیص خودکار حالات چهره با استفاده از شبکه های عصبی کانولوشنی عمیق.pdf","UserName":"مهتا","ReqNo":"1789"}', NULL, '2016-11-01 01:06:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5809, 32721, '', '{"FileName":"Doc2.pdf","UserName":"javad","ReqNo":"1791"}', NULL, '2016-11-01 10:54:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5811, 26085, '', '{"FileName":"[Elearnica] -Targeting_allergenic_fungi_in_agricultural_environments_aids_the_identifica.pdf","UserName":"فاطمه","ReqNo":"1793"}', NULL, '2016-11-01 11:31:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5813, 26085, '', '{"FileName":"[Elearnica] -Ploidy_variation_as_an_adaptive_mechanism_in_human_pathogenic_fungi.pdf","UserName":"فاطمه","ReqNo":"1795"}', NULL, '2016-11-01 11:33:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5815, 32721, '', '{"FileName":"Doc2.pdf","UserName":"javad","ReqNo":"1791"}', NULL, '2016-11-02 01:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5817, 26085, '', '{"FileName":"[Elearnica] -Targeting_allergenic_fungi_in_agricultural_environments_aids_the_identifica.pdf","UserName":"فاطمه","ReqNo":"1793"}', NULL, '2016-11-02 01:00:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5819, 26085, '', '{"FileName":"[Elearnica] -Ploidy_variation_as_an_adaptive_mechanism_in_human_pathogenic_fungi.pdf","UserName":"فاطمه","ReqNo":"1795"}', NULL, '2016-11-02 01:00:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5821, 32721, '', '{"FileName":"Doc2.pdf","UserName":"javad","ReqNo":"1791"}', NULL, '2017-04-17 10:47:57', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5823, 32721, '', '{"FileName":"Doc2.pdf","UserName":"javad","ReqNo":"1791"}', NULL, '2017-04-17 10:54:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5825, 58571, '', '{"FileName":"NFC Technology.pdf","UserName":"یونس","ReqNo":"2267"}', NULL, '2017-04-17 10:56:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5827, 388, '', '{"FileName":"1409.0473.pdf","UserName":"بهروز","ReqNo":"2269"}', NULL, '2017-04-17 10:59:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5829, 58571, '', '{"FileName":"NFC Technology.pdf","UserName":"یونس","ReqNo":"2267"}', NULL, '2017-04-17 11:00:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5831, 58571, '', '{"FileName":"Modeling AND Simulation of NFC logical layer Peer-To-Peer.pdf","UserName":"یونس","ReqNo":"2271"}', NULL, '2017-04-17 11:00:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5833, 388, '', '{"FileName":"1409.0473.pdf","UserName":"بهروز","ReqNo":"2269"}', NULL, '2017-04-17 11:00:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5835, 58571, '', '{"FileName":"Modeling AND Simulation of NFC logical layer Peer-To-Peer.pdf","UserName":"یونس","ReqNo":"2271"}', NULL, '2017-04-18 01:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5837, 58717, '', '{"FileName":"Integrated information and the cost of capital.pdf","UserName":"Rasoul","ReqNo":"2273"}', NULL, '2017-04-18 15:59:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5839, 58717, '', '{"FileName":"Integrated information and the cost of capital.pdf","UserName":"Rasoul","ReqNo":"2273"}', NULL, '2017-04-19 01:05:05', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5841, 59027, '', '{"FileName":"Epistemicity_in_voseo_and_tuteo_negative.pdf","UserName":"سردار","ReqNo":"2275"}', NULL, '2017-04-21 12:24:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5843, 59067, '', '{"FileName":"zhang2016.pdf","UserName":"صادق","ReqNo":"2277"}', NULL, '2017-04-21 21:14:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5845, 59065, '', '{"FileName":"107.docx","UserName":"مهدی","ReqNo":"2279"}', NULL, '2017-04-21 21:20:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5847, 59027, '', '{"FileName":"Epistemicity_in_voseo_and_tuteo_negative.pdf","UserName":"سردار","ReqNo":"2275"}', NULL, '2017-04-22 01:01:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5849, 59067, '', '{"FileName":"zhang2016.pdf","UserName":"صادق","ReqNo":"2277"}', NULL, '2017-04-22 01:01:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5851, 59065, '', '{"FileName":"107.docx","UserName":"مهدی","ReqNo":"2279"}', NULL, '2017-04-22 01:01:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5853, 59137, '', '{"FileName":"Effects of e-CRM on customer–bank relationship quality and outcomes_ The case of Thailand.pdf","UserName":"public","ReqNo":"2281"}', NULL, '2017-04-22 11:57:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5855, 59167, '', '{"FileName":"DCOSS.2013.78_4625614774c0b9301b6ff07f8c91f5c0.pdf","UserName":"hamed","ReqNo":"2283"}', NULL, '2017-04-22 17:29:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5857, 59137, '', '{"FileName":"Effects of e-CRM on customer–bank relationship quality and outcomes_ The case of Thailand.pdf","UserName":"public","ReqNo":"2281"}', NULL, '2017-04-23 01:00:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5859, 59167, '', '{"FileName":"DCOSS.2013.78_4625614774c0b9301b6ff07f8c91f5c0.pdf","UserName":"hamed","ReqNo":"2283"}', NULL, '2017-04-23 01:01:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5861, 59247, '', '{"FileName":"DRUG DELIVERY.pdf","UserName":"حنانه","ReqNo":"2285"}', NULL, '2017-04-23 11:51:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5863, 59277, '', '{"FileName":"2-1.pdf","UserName":"احد","ReqNo":"2287"}', NULL, '2017-04-23 18:04:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5865, 59277, '', '{"FileName":"Sociology of education-text for   class -Esfand 95.docx","UserName":"احد","ReqNo":"2289"}', NULL, '2017-04-23 18:08:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5867, 59247, '', '{"FileName":"DRUG DELIVERY.pdf","UserName":"حنانه","ReqNo":"2285"}', NULL, '2017-04-24 01:00:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5869, 59277, '', '{"FileName":"2-1.pdf","UserName":"احد","ReqNo":"2287"}', NULL, '2017-04-24 01:01:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5871, 59277, '', '{"FileName":"Sociology of education-text for   class -Esfand 95.docx","UserName":"احد","ReqNo":"2289"}', NULL, '2017-04-24 01:01:16', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5873, 59351, '', '{"FileName":"ترجمه زبان  254 ۲۴-آوریل-۲۰۱۷ ۱۴-۱۷-۱۸.pdf","UserName":"علی","ReqNo":"2291"}', NULL, '2017-04-24 15:21:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5875, 59351, '', '{"FileName":"ترجمه زبان  254 ۲۴-آوریل-۲۰۱۷ ۱۴-۱۷-۱۸.pdf","UserName":"علی","ReqNo":"2291"}', NULL, '2017-04-25 01:00:30', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5877, 59407, '', '{"FileName":"مقاله.pdf","UserName":"پرویز","ReqNo":"2293"}', NULL, '2017-04-25 09:36:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5879, 59307, '', '{"FileName":"1-s2.0-S1045235416300260-main.pdf","UserName":"اسفندیار","ReqNo":"2295"}', NULL, '2017-04-25 15:44:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5881, 59507, '', '{"FileName":"sh.pdf","UserName":"نیما","ReqNo":"2297"}', NULL, '2017-04-25 16:50:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5883, 35139, '', '{"FileName":"Routing optimization for multi-type containerships.pdf","UserName":"میلاد","ReqNo":"2299"}', NULL, '2017-04-25 22:43:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5885, 59407, '', '{"FileName":"مقاله.pdf","UserName":"پرویز","ReqNo":"2293"}', NULL, '2017-04-26 01:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5887, 59307, '', '{"FileName":"1-s2.0-S1045235416300260-main.pdf","UserName":"اسفندیار","ReqNo":"2295"}', NULL, '2017-04-26 01:00:43', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5889, 59507, '', '{"FileName":"sh.pdf","UserName":"نیما","ReqNo":"2297"}', NULL, '2017-04-26 01:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5891, 35139, '', '{"FileName":"Routing optimization for multi-type containerships.pdf","UserName":"میلاد","ReqNo":"2299"}', NULL, '2017-04-26 01:01:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5893, 59573, '', '{"FileName":"9.docx","UserName":"fatemeh","ReqNo":"2301"}', NULL, '2017-04-26 05:38:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5895, 59573, '', '{"FileName":"8.docx","UserName":"fatemeh","ReqNo":"2303"}', NULL, '2017-04-26 05:43:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5897, 59573, '', '{"FileName":"11 Security.docx","UserName":"fatemeh","ReqNo":"2305"}', NULL, '2017-04-26 05:43:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5899, 59573, '', '{"FileName":"13Simultaneous optimization of performance.docx","UserName":"fatemeh","ReqNo":"2307"}', NULL, '2017-04-26 05:43:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5901, 59573, '', '{"FileName":"18.docx","UserName":"fatemeh","ReqNo":"2309"}', NULL, '2017-04-26 05:43:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5903, 59573, '', '{"FileName":"22 Chasing Gaps between Bursts.docx","UserName":"fatemeh","ReqNo":"2311"}', NULL, '2017-04-26 05:43:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5905, 59573, '', '{"FileName":"22 Chasing Gaps between Bursts.docx","UserName":"fatemeh","ReqNo":"2313"}', NULL, '2017-04-26 05:44:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5907, 59137, '', '{"FileName":"[Elearnica] -The_impact_of_cost__technology_acceptance_and_employees__satisfaction_on_th_2.pdf","UserName":"public","ReqNo":"2315"}', NULL, '2017-04-26 08:43:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5909, 59609, '', '{"FileName":"Planning_human_resource_requirements.pdf","UserName":"مریم","ReqNo":"2317"}', NULL, '2017-04-26 12:37:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5911, 59573, '', '{"FileName":"9.docx","UserName":"fatemeh","ReqNo":"2301"}', NULL, '2017-04-26 15:57:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5913, 59573, '', '{"FileName":"8.docx","UserName":"fatemeh","ReqNo":"2303"}', NULL, '2017-04-26 15:58:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5915, 59573, '', '{"FileName":"11 Security.docx","UserName":"fatemeh","ReqNo":"2305"}', NULL, '2017-04-26 15:58:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5917, 59573, '', '{"FileName":"13Simultaneous optimization of performance.docx","UserName":"fatemeh","ReqNo":"2307"}', NULL, '2017-04-26 15:58:14', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5919, 59573, '', '{"FileName":"18.docx","UserName":"fatemeh","ReqNo":"2309"}', NULL, '2017-04-26 15:58:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5921, 59573, '', '{"FileName":"22 Chasing Gaps between Bursts.docx","UserName":"fatemeh","ReqNo":"2311"}', NULL, '2017-04-26 15:58:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5923, 59573, '', '{"FileName":"22 Chasing Gaps between Bursts.docx","UserName":"fatemeh","ReqNo":"2313"}', NULL, '2017-04-26 15:58:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5925, 59137, '', '{"FileName":"[Elearnica] -The_impact_of_cost__technology_acceptance_and_employees__satisfaction_on_th_2.pdf","UserName":"public","ReqNo":"2315"}', NULL, '2017-04-26 16:00:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5927, 59609, '', '{"FileName":"Planning_human_resource_requirements.pdf","UserName":"مریم","ReqNo":"2317"}', NULL, '2017-04-26 16:06:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5929, 59651, '', '{"FileName":"1396.pdf","UserName":"حمید","ReqNo":"2319"}', NULL, '2017-04-26 18:32:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5931, 59705, '', '{"FileName":"Jowett_Propaganda_and_Persuasion_2012.pdf","UserName":"رضا","ReqNo":"2321"}', NULL, '2017-04-27 00:36:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5933, 59651, '', '{"FileName":"1396.pdf","UserName":"حمید","ReqNo":"2319"}', NULL, '2017-04-27 01:00:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(5935, 59705, '', '{"FileName":"Jowett_Propaganda_and_Persuasion_2012.pdf","UserName":"رضا","ReqNo":"2321"}', NULL, '2017-04-27 01:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5937, 59727, '', '{"FileName":"strategy-and-the-internet-porter-hbr-2001.pdf","UserName":"manijeh","ReqNo":"2323"}', NULL, '2017-04-27 10:10:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5939, 59727, '', '{"FileName":"HBR_on_Strategy.pdf","UserName":"manijeh","ReqNo":"2325"}', NULL, '2017-04-27 10:18:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5941, 59739, '', '{"FileName":"THE-DEAD.pdf","UserName":"مهسا","ReqNo":"2327"}', NULL, '2017-04-27 11:54:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5943, 59739, '', '{"FileName":"WHO.pdf","UserName":"مهسا","ReqNo":"2329"}', NULL, '2017-04-27 12:14:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5945, 59739, '', '{"FileName":"WHO.pdf","UserName":"مهسا","ReqNo":"2331"}', NULL, '2017-04-27 12:38:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5947, 50847, '', '{"FileName":"optex_d2rf-d2gf_series_brochure.pdf","UserName":"ازاد","ReqNo":"2333"}', NULL, '2017-04-27 19:26:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5949, 59813, '', '{"FileName":"مسئولیت کیفری .docx","UserName":"سیدمهدی","ReqNo":"2335"}', NULL, '2017-04-27 20:04:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5951, 59065, '', '{"FileName":"Article_11_A_Theoretical_Discussion_of_Internal_Audit_Effectiveness.pdf","UserName":"مهدی","ReqNo":"2337"}', NULL, '2017-04-27 22:13:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5953, 59727, '', '{"FileName":"strategy-and-the-internet-porter-hbr-2001.pdf","UserName":"manijeh","ReqNo":"2323"}', NULL, '2017-04-28 01:01:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5955, 59727, '', '{"FileName":"HBR_on_Strategy.pdf","UserName":"manijeh","ReqNo":"2325"}', NULL, '2017-04-28 01:30:07', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5957, 59739, '', '{"FileName":"THE-DEAD.pdf","UserName":"مهسا","ReqNo":"2327"}', NULL, '2017-04-28 01:36:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5959, 59739, '', '{"FileName":"WHO.pdf","UserName":"مهسا","ReqNo":"2329"}', NULL, '2017-04-28 01:37:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5961, 59739, '', '{"FileName":"WHO.pdf","UserName":"مهسا","ReqNo":"2331"}', NULL, '2017-04-28 01:39:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5963, 50847, '', '{"FileName":"optex_d2rf-d2gf_series_brochure.pdf","UserName":"ازاد","ReqNo":"2333"}', NULL, '2017-04-28 01:39:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5965, 59813, '', '{"FileName":"مسئولیت کیفری .docx","UserName":"سیدمهدی","ReqNo":"2335"}', NULL, '2017-04-28 01:39:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5967, 59065, '', '{"FileName":"Article_11_A_Theoretical_Discussion_of_Internal_Audit_Effectiveness.pdf","UserName":"مهدی","ReqNo":"2337"}', NULL, '2017-04-28 01:41:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(5969, 21451, '', '{"FileName":"5b09c1fa6bc7ed9c8c9365c5a71c49ca_32741.pdf","UserName":"مریم","ReqNo":"2339"}', NULL, '2017-04-28 05:33:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5971, 21451, '', '{"FileName":"8d0b82d685efec3fb38e90cdb77e51f9_32732.pdf","UserName":"مریم","ReqNo":"2341"}', NULL, '2017-04-28 05:34:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5973, 21451, '', '{"FileName":"341dc7260c4e3f9ec437abb395289c01_32740.pdf","UserName":"مریم","ReqNo":"2343"}', NULL, '2017-04-28 05:35:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5975, 21451, '', '{"FileName":"1408.3967.pdf","UserName":"مریم","ReqNo":"2345"}', NULL, '2017-04-28 05:36:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5977, 21451, '', '{"FileName":"1410.1037.pdf","UserName":"مریم","ReqNo":"2347"}', NULL, '2017-04-28 05:36:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5979, 21451, '', '{"FileName":"1501.07304.pdf","UserName":"مریم","ReqNo":"2349"}', NULL, '2017-04-28 05:37:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5981, 21451, '', '{"FileName":"1506.02897.pdf","UserName":"مریم","ReqNo":"2351"}', NULL, '2017-04-28 05:37:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5983, 21451, '', '{"FileName":"1506.03799.pdf","UserName":"مریم","ReqNo":"2353"}', NULL, '2017-04-28 05:38:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5985, 21451, '', '{"FileName":"1509.05366.pdf","UserName":"مریم","ReqNo":"2355"}', NULL, '2017-04-28 05:39:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5987, 21451, '', '{"FileName":"1511.01186.pdf","UserName":"مریم","ReqNo":"2357"}', NULL, '2017-04-28 05:40:01', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5989, 21451, '', '{"FileName":"1601.03805.pdf","UserName":"مریم","ReqNo":"2359"}', NULL, '2017-04-28 05:40:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5991, 21451, '', '{"FileName":"1603.01249.pdf","UserName":"مریم","ReqNo":"2361"}', NULL, '2017-04-28 05:41:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5993, 21451, '', '{"FileName":"1603.07388.pdf","UserName":"مریم","ReqNo":"2363"}', NULL, '2017-04-28 05:42:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5995, 21451, '', '{"FileName":"1606.02909.pdf","UserName":"مریم","ReqNo":"2365"}', NULL, '2017-04-28 05:42:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5997, 21451, '', '{"FileName":"1610.01854.pdf","UserName":"مریم","ReqNo":"2367"}', NULL, '2017-04-28 05:43:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(5999, 21451, '', '{"FileName":"1611.00851.pdf","UserName":"مریم","ReqNo":"2369"}', NULL, '2017-04-28 05:43:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6001, 21451, '', '{"FileName":"1611.01751.pdf","UserName":"مریم","ReqNo":"2371"}', NULL, '2017-04-28 05:45:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6003, 21451, '', '{"FileName":"1611.04135.pdf","UserName":"مریم","ReqNo":"2373"}', NULL, '2017-04-28 05:45:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6005, 21451, '', '{"FileName":"1612.02572.pdf","UserName":"مریم","ReqNo":"2375"}', NULL, '2017-04-28 05:46:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6007, 21451, '', '{"FileName":"1612.04904.pdf","UserName":"مریم","ReqNo":"2377"}', NULL, '2017-04-28 05:46:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6009, 21451, '', '{"FileName":"1701.00008.pdf","UserName":"مریم","ReqNo":"2379"}', NULL, '2017-04-28 05:47:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6011, 21451, '', '{"FileName":"1701.01876.pdf","UserName":"مریم","ReqNo":"2381"}', NULL, '2017-04-28 05:48:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6013, 21451, '', '{"FileName":"1702.04280.pdf","UserName":"مریم","ReqNo":"2383"}', NULL, '2017-04-28 05:50:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6015, 21451, '', '{"FileName":"1702.04471.pdf","UserName":"مریم","ReqNo":"2385"}', NULL, '2017-04-28 05:51:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6017, 21451, '', '{"FileName":"1703.10580.pdf","UserName":"مریم","ReqNo":"2387"}', NULL, '2017-04-28 05:51:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6019, 21451, '', '{"FileName":"1704.05838.pdf","UserName":"مریم","ReqNo":"2389"}', NULL, '2017-04-28 05:52:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6021, 21451, '', '{"FileName":"101016jpatrec201506006.pdf","UserName":"مریم","ReqNo":"2391"}', NULL, '2017-04-28 05:52:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6023, 21451, '', '{"FileName":"67813893007.pdf","UserName":"مریم","ReqNo":"2393"}', NULL, '2017-04-28 05:53:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6025, 21451, '', '{"FileName":"a9cbb17b37cfea4d12a6aa3fd145c852_32739.pdf","UserName":"مریم","ReqNo":"2395"}', NULL, '2017-04-28 05:54:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6027, 21451, '', '{"FileName":"Age-Synthesis-and-Estimation-via-Faces.pdf","UserName":"مریم","ReqNo":"2397"}', NULL, '2017-04-28 05:55:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6029, 21451, '', '{"FileName":"art-3A10.1186-2Fs13640-016-0151-4.pdf","UserName":"مریم","ReqNo":"2399"}', NULL, '2017-04-28 05:55:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6031, 21451, '', '{"FileName":"CIVILICA_ThesisInquiry.pdf","UserName":"مریم","ReqNo":"2401"}', NULL, '2017-04-28 05:56:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6033, 21451, '', '{"FileName":"e0f8a36595cfa479c444743242e3362b_32735.pdf","UserName":"مریم","ReqNo":"2403"}', NULL, '2017-04-28 05:57:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6035, 21451, '', '{"FileName":"f08f68d3db8d354b2b926e5e194eee82_32742.pdf","UserName":"مریم","ReqNo":"2405"}', NULL, '2017-04-28 05:58:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6037, 21451, '', '{"FileName":"Huerta_PRLETTERS-D-15-00102R2.pdf","UserName":"مریم","ReqNo":"2407"}', NULL, '2017-04-28 05:58:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6039, 21451, '', '{"FileName":"Khr.pdf","UserName":"مریم","ReqNo":"2409"}', NULL, '2017-04-28 05:59:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6041, 59887, '', '{"FileName":"4_6041686725271486482.pdf","UserName":"نیما","ReqNo":"2411"}', NULL, '2017-04-28 14:27:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6043, 59909, '', '{"FileName":"asliA_review_on_ultra_high_performance.pdf","UserName":"سورنا","ReqNo":"2413"}', NULL, '2017-04-28 19:16:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6045, 21451, '', '{"FileName":"5b09c1fa6bc7ed9c8c9365c5a71c49ca_32741.pdf","UserName":"مریم","ReqNo":"2339"}', NULL, '2017-04-29 03:00:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6047, 21451, '', '{"FileName":"8d0b82d685efec3fb38e90cdb77e51f9_32732.pdf","UserName":"مریم","ReqNo":"2341"}', NULL, '2017-04-29 03:01:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6049, 21451, '', '{"FileName":"341dc7260c4e3f9ec437abb395289c01_32740.pdf","UserName":"مریم","ReqNo":"2343"}', NULL, '2017-04-29 03:02:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6051, 21451, '', '{"FileName":"1408.3967.pdf","UserName":"مریم","ReqNo":"2345"}', NULL, '2017-04-29 03:04:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6053, 21451, '', '{"FileName":"1410.1037.pdf","UserName":"مریم","ReqNo":"2347"}', NULL, '2017-04-29 03:11:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6055, 21451, '', '{"FileName":"1501.07304.pdf","UserName":"مریم","ReqNo":"2349"}', NULL, '2017-04-29 03:15:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6057, 21451, '', '{"FileName":"1506.02897.pdf","UserName":"مریم","ReqNo":"2351"}', NULL, '2017-04-29 03:17:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6059, 21451, '', '{"FileName":"1506.03799.pdf","UserName":"مریم","ReqNo":"2353"}', NULL, '2017-04-29 03:19:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6061, 21451, '', '{"FileName":"1509.05366.pdf","UserName":"مریم","ReqNo":"2355"}', NULL, '2017-04-29 03:22:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6063, 21451, '', '{"FileName":"1511.01186.pdf","UserName":"مریم","ReqNo":"2357"}', NULL, '2017-04-29 03:24:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6065, 21451, '', '{"FileName":"1601.03805.pdf","UserName":"مریم","ReqNo":"2359"}', NULL, '2017-04-29 03:24:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6067, 21451, '', '{"FileName":"1603.01249.pdf","UserName":"مریم","ReqNo":"2361"}', NULL, '2017-04-29 03:26:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6069, 21451, '', '{"FileName":"1603.07388.pdf","UserName":"مریم","ReqNo":"2363"}', NULL, '2017-04-29 03:28:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6071, 21451, '', '{"FileName":"1606.02909.pdf","UserName":"مریم","ReqNo":"2365"}', NULL, '2017-04-29 03:29:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6073, 21451, '', '{"FileName":"1610.01854.pdf","UserName":"مریم","ReqNo":"2367"}', NULL, '2017-04-29 03:29:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6075, 21451, '', '{"FileName":"1611.00851.pdf","UserName":"مریم","ReqNo":"2369"}', NULL, '2017-04-29 03:30:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6077, 21451, '', '{"FileName":"1611.01751.pdf","UserName":"مریم","ReqNo":"2371"}', NULL, '2017-04-29 03:30:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6079, 21451, '', '{"FileName":"1611.04135.pdf","UserName":"مریم","ReqNo":"2373"}', NULL, '2017-04-29 03:30:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6081, 21451, '', '{"FileName":"1612.02572.pdf","UserName":"مریم","ReqNo":"2375"}', NULL, '2017-04-29 03:31:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6083, 21451, '', '{"FileName":"1612.04904.pdf","UserName":"مریم","ReqNo":"2377"}', NULL, '2017-04-29 03:31:12', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6085, 21451, '', '{"FileName":"1701.00008.pdf","UserName":"مریم","ReqNo":"2379"}', NULL, '2017-04-29 03:31:41', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6087, 21451, '', '{"FileName":"1701.01876.pdf","UserName":"مریم","ReqNo":"2381"}', NULL, '2017-04-29 03:31:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6089, 21451, '', '{"FileName":"1702.04280.pdf","UserName":"مریم","ReqNo":"2383"}', NULL, '2017-04-29 03:32:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6091, 21451, '', '{"FileName":"1702.04471.pdf","UserName":"مریم","ReqNo":"2385"}', NULL, '2017-04-29 03:32:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6093, 21451, '', '{"FileName":"1703.10580.pdf","UserName":"مریم","ReqNo":"2387"}', NULL, '2017-04-29 03:32:38', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6095, 21451, '', '{"FileName":"1704.05838.pdf","UserName":"مریم","ReqNo":"2389"}', NULL, '2017-04-29 03:32:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6097, 21451, '', '{"FileName":"101016jpatrec201506006.pdf","UserName":"مریم","ReqNo":"2391"}', NULL, '2017-04-29 03:35:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6099, 21451, '', '{"FileName":"67813893007.pdf","UserName":"مریم","ReqNo":"2393"}', NULL, '2017-04-29 03:35:04', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6101, 21451, '', '{"FileName":"a9cbb17b37cfea4d12a6aa3fd145c852_32739.pdf","UserName":"مریم","ReqNo":"2395"}', NULL, '2017-04-29 03:36:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6103, 21451, '', '{"FileName":"Age-Synthesis-and-Estimation-via-Faces.pdf","UserName":"مریم","ReqNo":"2397"}', NULL, '2017-04-29 03:41:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6105, 21451, '', '{"FileName":"art-3A10.1186-2Fs13640-016-0151-4.pdf","UserName":"مریم","ReqNo":"2399"}', NULL, '2017-04-29 03:44:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6107, 21451, '', '{"FileName":"CIVILICA_ThesisInquiry.pdf","UserName":"مریم","ReqNo":"2401"}', NULL, '2017-04-29 03:44:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6109, 21451, '', '{"FileName":"e0f8a36595cfa479c444743242e3362b_32735.pdf","UserName":"مریم","ReqNo":"2403"}', NULL, '2017-04-29 03:47:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6111, 21451, '', '{"FileName":"f08f68d3db8d354b2b926e5e194eee82_32742.pdf","UserName":"مریم","ReqNo":"2405"}', NULL, '2017-04-29 03:49:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6113, 21451, '', '{"FileName":"Huerta_PRLETTERS-D-15-00102R2.pdf","UserName":"مریم","ReqNo":"2407"}', NULL, '2017-04-29 03:50:01', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6115, 21451, '', '{"FileName":"Khr.pdf","UserName":"مریم","ReqNo":"2409"}', NULL, '2017-04-29 03:51:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6117, 59887, '', '{"FileName":"4_6041686725271486482.pdf","UserName":"نیما","ReqNo":"2411"}', NULL, '2017-04-29 04:11:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6119, 59909, '', '{"FileName":"asliA_review_on_ultra_high_performance.pdf","UserName":"سورنا","ReqNo":"2413"}', NULL, '2017-04-29 04:13:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6121, 537, '', '{"FileName":"SCADA.pdf","UserName":"نیلوفر","ReqNo":"2415"}', NULL, '2017-04-29 12:58:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6123, 59137, '', '{"FileName":"مقاله بیس دو_1.pdf","UserName":"public","ReqNo":"2417"}', NULL, '2017-04-29 15:04:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6125, 59137, '', '{"FileName":"مقاله بیس سه_2.pdf","UserName":"public","ReqNo":"2419"}', NULL, '2017-04-29 15:04:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6127, 59137, '', '{"FileName":"مقاله بیس یک.pdf","UserName":"public","ReqNo":"2421"}', NULL, '2017-04-29 15:05:02', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6129, 60085, '', '{"FileName":"Essentials-of-Hypnosis-2015.pdf","UserName":"جواد","ReqNo":"2423"}', NULL, '2017-04-29 23:22:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6131, 537, '', '{"FileName":"SCADA.pdf","UserName":"نیلوفر","ReqNo":"2415"}', NULL, '2017-04-30 03:00:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6133, 59137, '', '{"FileName":"مقاله بیس دو_1.pdf","UserName":"public","ReqNo":"2417"}', NULL, '2017-04-30 03:01:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6135, 59137, '', '{"FileName":"مقاله بیس سه_2.pdf","UserName":"public","ReqNo":"2419"}', NULL, '2017-04-30 03:01:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6137, 59137, '', '{"FileName":"مقاله بیس یک.pdf","UserName":"public","ReqNo":"2421"}', NULL, '2017-04-30 03:02:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6139, 60085, '', '{"FileName":"Essentials-of-Hypnosis-2015.pdf","UserName":"جواد","ReqNo":"2423"}', NULL, '2017-04-30 03:02:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6141, 60117, '', '{"FileName":"COMPLETE-TETRANSLATION.docx","UserName":"رضا","ReqNo":"2425"}', NULL, '2017-04-30 04:48:10', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6143, 60221, '', '{"FileName":"customer relations_english.pdf","UserName":"رضا","ReqNo":"2427"}', NULL, '2017-05-01 01:50:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6145, 60117, '', '{"FileName":"COMPLETE-TETRANSLATION.docx","UserName":"رضا","ReqNo":"2425"}', NULL, '2017-05-01 03:01:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6147, 60221, '', '{"FileName":"customer relations_english.pdf","UserName":"رضا","ReqNo":"2427"}', NULL, '2017-05-01 03:02:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6149, 60283, '', '{"FileName":"ERP-159-181.pdf","UserName":"احسان","ReqNo":"2429"}', NULL, '2017-05-01 09:39:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6151, 59137, '', '{"FileName":"جدید - یک.pdf","UserName":"public","ReqNo":"2431"}', NULL, '2017-05-01 14:10:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6153, 59137, '', '{"FileName":"جدید - دو.pdf","UserName":"public","ReqNo":"2433"}', NULL, '2017-05-01 14:10:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6155, 60283, '', '{"FileName":"ERP-159-181.pdf","UserName":"احسان","ReqNo":"2429"}', NULL, '2017-05-02 03:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6157, 59137, '', '{"FileName":"جدید - یک.pdf","UserName":"public","ReqNo":"2431"}', NULL, '2017-05-02 03:01:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6159, 59137, '', '{"FileName":"جدید - دو.pdf","UserName":"public","ReqNo":"2433"}', NULL, '2017-05-02 03:02:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6161, 60457, '', '{"FileName":"10.1016_j.camwa.2013.08.012-Adopting-genetic-algorithms-for-technical-analysis-and-portfolio-managem","UserName":"امید","ReqNo":"2435"}', NULL, '2017-05-02 11:59:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6163, 60509, '', '{"FileName":"maghale varzesh.pdf","UserName":"hooman","ReqNo":"2437"}', NULL, '2017-05-02 18:04:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6165, 60283, '', '{"FileName":"ERP-6.pdf","UserName":"احسان","ReqNo":"2439"}', NULL, '2017-05-02 19:31:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6167, 60457, '', '{"FileName":"10.1016_j.camwa.2013.08.012-Adopting-genetic-algorithms-for-technical-analysis-and-portfolio-managem","UserName":"امید","ReqNo":"2435"}', NULL, '2017-05-03 03:00:31', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6169, 60509, '', '{"FileName":"maghale varzesh.pdf","UserName":"hooman","ReqNo":"2437"}', NULL, '2017-05-03 03:02:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6171, 60283, '', '{"FileName":"ERP-6.pdf","UserName":"احسان","ReqNo":"2439"}', NULL, '2017-05-03 03:08:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6173, 2057, '', '{"FileName":"Business-Intelligence-Turban bigdeli.pdf","UserName":"محمد موسی","ReqNo":"2441"}', NULL, '2017-05-04 11:20:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6175, 60679, '', '{"FileName":"A novel approach for intelligent distribution of data warehouses.pdf","UserName":"mahdi","ReqNo":"2443"}', NULL, '2017-05-04 11:47:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6177, 60731, '', '{"FileName":"C1(3).pdf","UserName":"امین","ReqNo":"2445"}', NULL, '2017-05-04 18:36:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6179, 60731, '', '{"FileName":"C1(3).pdf","UserName":"امین","ReqNo":"2447"}', NULL, '2017-05-04 21:32:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6181, 2057, '', '{"FileName":"Business-Intelligence-Turban bigdeli.pdf","UserName":"محمد موسی","ReqNo":"2441"}', NULL, '2017-05-05 03:01:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6183, 60679, '', '{"FileName":"A novel approach for intelligent distribution of data warehouses.pdf","UserName":"mahdi","ReqNo":"2443"}', NULL, '2017-05-05 03:01:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6185, 60731, '', '{"FileName":"C1(3).pdf","UserName":"امین","ReqNo":"2445"}', NULL, '2017-05-05 03:06:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6187, 60731, '', '{"FileName":"C1(3).pdf","UserName":"امین","ReqNo":"2447"}', NULL, '2017-05-05 03:12:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6189, 59257, '', '{"FileName":"Crime.pdf","UserName":"بردیا","ReqNo":"2449"}', NULL, '2017-05-05 17:21:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6191, 60887, '', '{"FileName":"1-s2.0-S1877042816311570-main.pdf","UserName":"Mostafa","ReqNo":"2451"}', NULL, '2017-05-05 20:27:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6193, 59257, '', '{"FileName":"Crime.pdf","UserName":"بردیا","ReqNo":"2449"}', NULL, '2017-05-06 03:01:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6195, 60887, '', '{"FileName":"1-s2.0-S1877042816311570-main.pdf","UserName":"Mostafa","ReqNo":"2451"}', NULL, '2017-05-06 03:01:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6197, 59257, '', '{"FileName":"tarjomyar (1).pdf","UserName":"بردیا","ReqNo":"2453"}', NULL, '2017-05-06 14:52:53', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6199, 60991, '', '{"FileName":"4_6050887988328530254.pdf","UserName":"میلاد","ReqNo":"2455"}', NULL, '2017-05-06 15:28:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6201, 61047, '', '{"FileName":"1-s2.0-S2305050016301622-main.pdf","UserName":"sara","ReqNo":"2457"}', NULL, '2017-05-06 22:10:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6203, 61047, '', '{"FileName":"1-s2.0-S1043661816314025-main.pdf","UserName":"sara","ReqNo":"2459"}', NULL, '2017-05-06 22:49:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6205, 61047, '', '{"FileName":"1-s2.0-S1471492216302264-main.pdf","UserName":"sara","ReqNo":"2461"}', NULL, '2017-05-06 22:55:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6207, 59257, '', '{"FileName":"tarjomyar (1).pdf","UserName":"بردیا","ReqNo":"2453"}', NULL, '2017-05-07 03:00:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6209, 60991, '', '{"FileName":"4_6050887988328530254.pdf","UserName":"میلاد","ReqNo":"2455"}', NULL, '2017-05-07 03:01:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6211, 61047, '', '{"FileName":"1-s2.0-S2305050016301622-main.pdf","UserName":"sara","ReqNo":"2457"}', NULL, '2017-05-07 03:01:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6213, 61047, '', '{"FileName":"1-s2.0-S1043661816314025-main.pdf","UserName":"sara","ReqNo":"2459"}', NULL, '2017-05-07 03:02:22', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6215, 61047, '', '{"FileName":"1-s2.0-S1471492216302264-main.pdf","UserName":"sara","ReqNo":"2461"}', NULL, '2017-05-07 03:02:23', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6217, 61081, '', '{"FileName":"قرارداد.docx","UserName":"محمدرضا","ReqNo":"2463"}', NULL, '2017-05-07 09:49:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6219, 59727, '', '{"FileName":"Chapter 6 (2).pdf","UserName":"manijeh","ReqNo":"2465"}', NULL, '2017-05-07 12:53:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6221, 61081, '', '{"FileName":"قرارداد.docx","UserName":"محمدرضا","ReqNo":"2463"}', NULL, '2017-05-08 03:00:39', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6223, 59727, '', '{"FileName":"Chapter 6 (2).pdf","UserName":"manijeh","ReqNo":"2465"}', NULL, '2017-05-08 03:01:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6225, 61217, '', '{"FileName":"ICCSIT.2010.5563674_11a7e6f7474fce3a7bb5fdc0eeb61375 (3).pdf","UserName":"nasim","ReqNo":"2467"}', NULL, '2017-05-08 11:37:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6227, 61217, '', '{"FileName":"ICCSIT.2010.5563674_11a7e6f7474fce3a7bb5fdc0eeb61375 (3).pdf","UserName":"nasim","ReqNo":"2469"}', NULL, '2017-05-08 11:41:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6229, 61219, '', '{"FileName":"ارائه.pdf","UserName":"علی","ReqNo":"2471"}', NULL, '2017-05-08 12:32:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6231, 61229, '', '{"FileName":"trust iot 2015.pdf","UserName":"معین","ReqNo":"2473"}', NULL, '2017-05-08 13:39:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6233, 61217, '', '{"FileName":"ICCSIT.2010.5563674_11a7e6f7474fce3a7bb5fdc0eeb61375 (3).pdf","UserName":"nasim","ReqNo":"2467"}', NULL, '2017-05-09 03:00:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6235, 61217, '', '{"FileName":"ICCSIT.2010.5563674_11a7e6f7474fce3a7bb5fdc0eeb61375 (3).pdf","UserName":"nasim","ReqNo":"2469"}', NULL, '2017-05-09 03:00:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6237, 61219, '', '{"FileName":"ارائه.pdf","UserName":"علی","ReqNo":"2471"}', NULL, '2017-05-09 03:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6239, 61229, '', '{"FileName":"trust iot 2015.pdf","UserName":"معین","ReqNo":"2473"}', NULL, '2017-05-09 03:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6241, 61343, '', '{"FileName":"4-1.pdf","UserName":"مهري","ReqNo":"2475"}', NULL, '2017-05-09 11:02:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6243, 61343, '', '{"FileName":"4-1.pdf","UserName":"مهري","ReqNo":"2475"}', NULL, '2017-05-10 03:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6245, 61517, '', '{"FileName":"Arc Project (82).pdf","UserName":"sadegh","ReqNo":"2477"}', NULL, '2017-05-10 21:18:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6247, 61517, '', '{"FileName":"Arc Project (82).pdf","UserName":"sadegh","ReqNo":"2477"}', NULL, '2017-05-11 03:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6249, 61603, '', '{"FileName":"1-s2.0-S0304383513005910-main.pdf","UserName":"امیر","ReqNo":"2479"}', NULL, '2017-05-11 18:20:57', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6251, 61603, '', '{"FileName":"1-s2.0-S0304383513005910-main.pdf","UserName":"امیر","ReqNo":"2479"}', NULL, '2017-05-12 03:00:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6253, 61655, '', '{"FileName":"علی اصغر شریفی.pdf","UserName":"علی اصغر","ReqNo":"2481"}', NULL, '2017-05-12 11:26:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6255, 61699, '', '{"FileName":"07296665.pdf","UserName":"sweet","ReqNo":"2483"}', NULL, '2017-05-12 17:15:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6257, 61655, '', '{"FileName":"علی اصغر شریفی.pdf","UserName":"علی اصغر","ReqNo":"2481"}', NULL, '2017-05-13 03:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6259, 61699, '', '{"FileName":"07296665.pdf","UserName":"sweet","ReqNo":"2483"}', NULL, '2017-05-13 03:00:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6261, 61799, '', '{"FileName":"arnheim.pdf","UserName":"مازيار","ReqNo":"2485"}', NULL, '2017-05-13 11:53:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6263, 61827, '', '{"FileName":"4_5810083637491662980.pdf","UserName":"نرگس","ReqNo":"2487"}', NULL, '2017-05-13 15:14:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6265, 61799, '', '{"FileName":"arnheim.pdf","UserName":"مازيار","ReqNo":"2485"}', NULL, '2017-05-14 03:00:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6267, 61827, '', '{"FileName":"4_5810083637491662980.pdf","UserName":"نرگس","ReqNo":"2487"}', NULL, '2017-05-14 03:00:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6269, 62037, '', '{"FileName":"AI-WSN- Adaptive and Intelligent Wireless Sensor Network - ScienceDirect.pdf","UserName":"مهسا","ReqNo":"2489"}', NULL, '2017-05-14 18:40:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6271, 62037, '', '{"FileName":"AI-WSN- Adaptive and Intelligent Wireless Sensor Network - ScienceDirect.pdf","UserName":"مهسا","ReqNo":"2489"}', NULL, '2017-05-15 03:00:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6273, 61799, '', '{"FileName":"j.patcog.2008.05.026.pdf","UserName":"مازيار","ReqNo":"2491"}', NULL, '2017-05-15 11:52:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6275, 19, '', '{"FileName":"EB-179-TA-418.14-.C48-2006-THEORY-OF-PLASTICITY.pdf","UserName":"سید محمد","ReqNo":"2493"}', NULL, '2017-05-15 16:48:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6277, 19, '', '{"FileName":"BasicCivilEngineeringBookbyBhavikatti.pdf","UserName":"سید محمد","ReqNo":"2495"}', NULL, '2017-05-15 16:49:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6279, 62145, '', '{"FileName":"hossini2222222222222.pdf","UserName":"masoud","ReqNo":"2497"}', NULL, '2017-05-15 19:06:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6281, 62045, '', '{"FileName":"متن انگلیسی.docx","UserName":"آرش","ReqNo":"2499"}', NULL, '2017-05-15 22:43:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6283, 61799, '', '{"FileName":"j.patcog.2008.05.026.pdf","UserName":"مازيار","ReqNo":"2491"}', NULL, '2017-05-16 03:00:53', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6285, 19, '', '{"FileName":"EB-179-TA-418.14-.C48-2006-THEORY-OF-PLASTICITY.pdf","UserName":"سید محمد","ReqNo":"2493"}', NULL, '2017-05-16 03:03:25', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6287, 62215, '', '{"FileName":"tmp_13872-4_2873270328602133601606795959.pdf","UserName":"فاطمه","ReqNo":"2501"}', NULL, '2017-05-16 07:25:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6289, 62215, '', '{"FileName":"Part1-1.pdf","UserName":"فاطمه","ReqNo":"2503"}', NULL, '2017-05-16 10:39:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6291, 62233, '', '{"FileName":"ترجمه.docx","UserName":"hamed","ReqNo":"2505"}', NULL, '2017-05-16 11:12:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6293, 61827, '', '{"FileName":"Data Security Challenges.pdf","UserName":"نرگس","ReqNo":"2507"}', NULL, '2017-05-17 01:15:48', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6295, 61827, '', '{"FileName":"Data Storage Security.pdf","UserName":"نرگس","ReqNo":"2509"}', NULL, '2017-05-17 01:21:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6297, 61827, '', '{"FileName":"Security Algorithms.pdf","UserName":"نرگس","ReqNo":"2511"}', NULL, '2017-05-17 01:23:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6299, 19, '', '{"FileName":"BasicCivilEngineeringBookbyBhavikatti.pdf","UserName":"سید محمد","ReqNo":"2495"}', NULL, '2017-05-17 03:11:17', 1, 'OFR.Ok', b'0', b'1', b'1', 'N'),
	(6301, 62145, '', '{"FileName":"hossini2222222222222.pdf","UserName":"masoud","ReqNo":"2497"}', NULL, '2017-05-17 03:13:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6303, 62045, '', '{"FileName":"متن انگلیسی.docx","UserName":"آرش","ReqNo":"2499"}', NULL, '2017-05-17 03:15:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6305, 62215, '', '{"FileName":"tmp_13872-4_2873270328602133601606795959.pdf","UserName":"فاطمه","ReqNo":"2501"}', NULL, '2017-05-17 03:15:32', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6307, 62215, '', '{"FileName":"Part1-1.pdf","UserName":"فاطمه","ReqNo":"2503"}', NULL, '2017-05-17 03:20:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6309, 62233, '', '{"FileName":"ترجمه.docx","UserName":"hamed","ReqNo":"2505"}', NULL, '2017-05-17 03:29:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6311, 61827, '', '{"FileName":"Data Security Challenges.pdf","UserName":"نرگس","ReqNo":"2507"}', NULL, '2017-05-17 03:31:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6313, 61827, '', '{"FileName":"Data Storage Security.pdf","UserName":"نرگس","ReqNo":"2509"}', NULL, '2017-05-17 03:34:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6315, 61827, '', '{"FileName":"Security Algorithms.pdf","UserName":"نرگس","ReqNo":"2511"}', NULL, '2017-05-17 03:36:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6317, 62395, '', '{"FileName":"fff.pdf","UserName":"فرامرز","ReqNo":"2513"}', NULL, '2017-05-17 17:16:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6319, 62395, '', '{"FileName":"fff.pdf","UserName":"فرامرز","ReqNo":"2515"}', NULL, '2017-05-17 17:18:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6321, 62395, '', '{"FileName":"fff.pdf","UserName":"فرامرز","ReqNo":"2513"}', NULL, '2017-05-18 03:00:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6323, 62395, '', '{"FileName":"fff.pdf","UserName":"فرامرز","ReqNo":"2515"}', NULL, '2017-05-18 03:00:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6325, 62499, '', '{"FileName":"kasayi tarjome.pdf","UserName":"behnoosh","ReqNo":"2517"}', NULL, '2017-05-19 10:13:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6327, 62519, '', '{"FileName":"Optimal Coordination of Directional Overcurrent Relays.docx","UserName":"وحید","ReqNo":"2519"}', NULL, '2017-05-19 16:11:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6329, 62499, '', '{"FileName":"kasayi tarjome.pdf","UserName":"behnoosh","ReqNo":"2517"}', NULL, '2017-05-20 03:00:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6331, 62519, '', '{"FileName":"Optimal Coordination of Directional Overcurrent Relays.docx","UserName":"وحید","ReqNo":"2519"}', NULL, '2017-05-20 03:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6333, 62551, '', '{"FileName":"fusing insar_ monitoring.pdf","UserName":"محسن","ReqNo":"2521"}', NULL, '2017-05-20 12:57:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6335, 62559, '', '{"FileName":"244756e.pdf","UserName":"محمد حسین","ReqNo":"2523"}', NULL, '2017-05-20 14:05:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6337, 55773, '', '{"FileName":"Agarose-chitosan-C18-film-micro-solid-phase-extraction-combined-with-high-performance-liquid-chromat","UserName":"محمد","ReqNo":"2525"}', NULL, '2017-05-20 23:49:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6339, 62551, '', '{"FileName":"fusing insar_ monitoring.pdf","UserName":"محسن","ReqNo":"2521"}', NULL, '2017-05-21 03:02:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6341, 62559, '', '{"FileName":"244756e.pdf","UserName":"محمد حسین","ReqNo":"2523"}', NULL, '2017-05-21 03:18:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6343, 55773, '', '{"FileName":"Agarose-chitosan-C18-film-micro-solid-phase-extraction-combined-with-high-performance-liquid-chromat","UserName":"محمد","ReqNo":"2525"}', NULL, '2017-05-21 03:18:24', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6345, 62653, '', '{"FileName":"1-s2.0-S1875389213001090-main.pdf","UserName":"jahan","ReqNo":"2527"}', NULL, '2017-05-21 11:33:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6347, 61657, '', '{"FileName":"daneshyari.com_4743083.pdf","UserName":"habel","ReqNo":"2529"}', NULL, '2017-05-21 22:35:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6349, 62733, '', '{"FileName":"jung2016.pdf","UserName":"مریم","ReqNo":"2531"}', NULL, '2017-05-21 22:51:27', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6351, 62653, '', '{"FileName":"1-s2.0-S1875389213001090-main.pdf","UserName":"jahan","ReqNo":"2527"}', NULL, '2017-05-22 03:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6353, 61657, '', '{"FileName":"daneshyari.com_4743083.pdf","UserName":"habel","ReqNo":"2529"}', NULL, '2017-05-22 03:01:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6355, 62733, '', '{"FileName":"jung2016.pdf","UserName":"مریم","ReqNo":"2531"}', NULL, '2017-05-22 03:01:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6357, 62771, '', '{"FileName":"01-05-.pdf","UserName":"جواد","ReqNo":"2533"}', NULL, '2017-05-22 10:32:15', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6359, 62779, '', '{"FileName":"10.4028_www.scientific.net_amm.214.644_vps8-1.pdf","UserName":"محمد","ReqNo":"2535"}', NULL, '2017-05-22 12:38:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6361, 61799, '', '{"FileName":"OhtsukaBrewer92.pdf","UserName":"مازيار","ReqNo":"2537"}', NULL, '2017-05-22 13:40:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6363, 62843, '', '{"FileName":"BOOK_Chapter_Measuring_Agricultural.pdf","UserName":"ابولی","ReqNo":"2539"}', NULL, '2017-05-22 22:49:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6365, 62843, '', '{"FileName":"elearnica_ir_Empirical_evaluatio.pdf","UserName":"ابولی","ReqNo":"2541"}', NULL, '2017-05-22 22:54:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6367, 62843, '', '{"FileName":"1-s2.0-S2452315116000096-main.pdf","UserName":"ابولی","ReqNo":"2543"}', NULL, '2017-05-22 22:55:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6369, 62843, '', '{"FileName":"hhhh.pdf","UserName":"ابولی","ReqNo":"2545"}', NULL, '2017-05-22 22:55:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6371, 62843, '', '{"FileName":"1-s2.0-S2213297X15000178-main.pdf","UserName":"ابولی","ReqNo":"2547"}', NULL, '2017-05-22 22:56:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6373, 62843, '', '{"FileName":"1-s2.0-S0014292108001116-main.pdf","UserName":"ابولی","ReqNo":"2549"}', NULL, '2017-05-22 22:56:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6375, 62843, '', '{"FileName":"a.pdf","UserName":"ابولی","ReqNo":"2551"}', NULL, '2017-05-22 22:56:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6377, 62843, '', '{"FileName":"b.pdf","UserName":"ابولی","ReqNo":"2553"}', NULL, '2017-05-22 22:57:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6379, 62843, '', '{"FileName":"c.pdf","UserName":"ابولی","ReqNo":"2555"}', NULL, '2017-05-22 22:57:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6381, 62771, '', '{"FileName":"01-05-.pdf","UserName":"جواد","ReqNo":"2533"}', NULL, '2017-05-23 03:01:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6383, 62779, '', '{"FileName":"10.4028_www.scientific.net_amm.214.644_vps8-1.pdf","UserName":"محمد","ReqNo":"2535"}', NULL, '2017-05-23 03:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6385, 61799, '', '{"FileName":"OhtsukaBrewer92.pdf","UserName":"مازيار","ReqNo":"2537"}', NULL, '2017-05-23 03:01:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6387, 62843, '', '{"FileName":"BOOK_Chapter_Measuring_Agricultural.pdf","UserName":"ابولی","ReqNo":"2539"}', NULL, '2017-05-23 03:02:12', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6389, 62843, '', '{"FileName":"elearnica_ir_Empirical_evaluatio.pdf","UserName":"ابولی","ReqNo":"2541"}', NULL, '2017-05-23 03:02:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6391, 62843, '', '{"FileName":"1-s2.0-S2452315116000096-main.pdf","UserName":"ابولی","ReqNo":"2543"}', NULL, '2017-05-23 03:05:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6393, 62843, '', '{"FileName":"hhhh.pdf","UserName":"ابولی","ReqNo":"2545"}', NULL, '2017-05-23 03:07:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6395, 62843, '', '{"FileName":"1-s2.0-S2213297X15000178-main.pdf","UserName":"ابولی","ReqNo":"2547"}', NULL, '2017-05-23 03:09:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6397, 62843, '', '{"FileName":"1-s2.0-S0014292108001116-main.pdf","UserName":"ابولی","ReqNo":"2549"}', NULL, '2017-05-23 03:10:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6399, 62843, '', '{"FileName":"a.pdf","UserName":"ابولی","ReqNo":"2551"}', NULL, '2017-05-23 03:10:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6401, 62843, '', '{"FileName":"b.pdf","UserName":"ابولی","ReqNo":"2553"}', NULL, '2017-05-23 03:11:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6403, 62843, '', '{"FileName":"c.pdf","UserName":"ابولی","ReqNo":"2555"}', NULL, '2017-05-23 03:12:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6405, 62903, '', '{"FileName":"art-direct-2015-10-Expert system for medicine diagnosis-asl.pdf","UserName":"فائزه","ReqNo":"2557"}', NULL, '2017-05-23 14:45:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6407, 62923, '', '{"FileName":"4.pdf","UserName":"sina","ReqNo":"2559"}', NULL, '2017-05-23 17:27:11', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6409, 2057, '', '{"FileName":"1 (2).pdf","UserName":"محمد موسی","ReqNo":"2561"}', NULL, '2017-05-24 00:15:07', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6411, 2057, '', '{"FileName":"1 (9).pdf","UserName":"محمد موسی","ReqNo":"2563"}', NULL, '2017-05-24 00:15:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6413, 2057, '', '{"FileName":"1 (10).pdf","UserName":"محمد موسی","ReqNo":"2565"}', NULL, '2017-05-24 00:16:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6415, 2057, '', '{"FileName":"1 (13).pdf","UserName":"محمد موسی","ReqNo":"2567"}', NULL, '2017-05-24 00:16:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6417, 2057, '', '{"FileName":"1 (14).pdf","UserName":"محمد موسی","ReqNo":"2569"}', NULL, '2017-05-24 00:17:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6419, 2057, '', '{"FileName":"1 (1).pdf","UserName":"محمد موسی","ReqNo":"2571"}', NULL, '2017-05-24 00:19:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6421, 2057, '', '{"FileName":"1 (6).pdf","UserName":"محمد موسی","ReqNo":"2573"}', NULL, '2017-05-24 00:20:25', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6423, 2057, '', '{"FileName":"1 (7).pdf","UserName":"محمد موسی","ReqNo":"2575"}', NULL, '2017-05-24 00:22:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6425, 62903, '', '{"FileName":"art-direct-2015-10-Expert system for medicine diagnosis-asl.pdf","UserName":"فائزه","ReqNo":"2557"}', NULL, '2017-05-24 03:00:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6427, 62923, '', '{"FileName":"4.pdf","UserName":"sina","ReqNo":"2559"}', NULL, '2017-05-24 03:01:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6429, 2057, '', '{"FileName":"1 (2).pdf","UserName":"محمد موسی","ReqNo":"2561"}', NULL, '2017-05-24 03:01:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6431, 2057, '', '{"FileName":"1 (9).pdf","UserName":"محمد موسی","ReqNo":"2563"}', NULL, '2017-05-24 03:01:48', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6433, 2057, '', '{"FileName":"1 (10).pdf","UserName":"محمد موسی","ReqNo":"2565"}', NULL, '2017-05-24 03:02:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6435, 2057, '', '{"FileName":"1 (13).pdf","UserName":"محمد موسی","ReqNo":"2567"}', NULL, '2017-05-24 03:02:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6437, 2057, '', '{"FileName":"1 (14).pdf","UserName":"محمد موسی","ReqNo":"2569"}', NULL, '2017-05-24 03:05:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6439, 2057, '', '{"FileName":"1 (1).pdf","UserName":"محمد موسی","ReqNo":"2571"}', NULL, '2017-05-24 03:08:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6441, 2057, '', '{"FileName":"1 (6).pdf","UserName":"محمد موسی","ReqNo":"2573"}', NULL, '2017-05-24 03:11:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6443, 2057, '', '{"FileName":"1 (7).pdf","UserName":"محمد موسی","ReqNo":"2575"}', NULL, '2017-05-24 03:17:11', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6445, 61047, '', '{"FileName":"1-s2.0-S2305050016301622-main.pdf","UserName":"sara","ReqNo":"2577"}', NULL, '2017-05-24 09:54:35', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6447, 63007, '', '{"FileName":"4.pdf","UserName":"sherhood","ReqNo":"2579"}', NULL, '2017-05-24 14:45:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6449, 63011, '', '{"FileName":"Chapter 1-2.pdf","UserName":"مجید","ReqNo":"2581"}', NULL, '2017-05-24 14:50:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6451, 63007, '', '{"FileName":"31.pdf","UserName":"sherhood","ReqNo":"2583"}', NULL, '2017-05-24 14:52:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6453, 63021, '', '{"FileName":"10.1037_fsh0000012_ayzq_2.pdf","UserName":"babak","ReqNo":"2585"}', NULL, '2017-05-24 15:50:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6455, 63023, '', '{"FileName":"واکنش های چند جزیی با استفاده از مایعات یونی (1).pdf","UserName":"فرید","ReqNo":"2587"}', NULL, '2017-05-24 17:38:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6457, 58553, '', '{"FileName":"نقش حسابداری در یکپارچگی پس از  ادغام.pdf","UserName":"امیرحسین","ReqNo":"2589"}', NULL, '2017-05-24 23:14:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6459, 61047, '', '{"FileName":"1-s2.0-S2305050016301622-main.pdf","UserName":"sara","ReqNo":"2577"}', NULL, '2017-05-25 03:01:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6461, 63007, '', '{"FileName":"4.pdf","UserName":"sherhood","ReqNo":"2579"}', NULL, '2017-05-25 03:05:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6463, 63011, '', '{"FileName":"Chapter 1-2.pdf","UserName":"مجید","ReqNo":"2581"}', NULL, '2017-05-25 03:07:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6465, 63007, '', '{"FileName":"31.pdf","UserName":"sherhood","ReqNo":"2583"}', NULL, '2017-05-25 03:09:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6467, 63021, '', '{"FileName":"10.1037_fsh0000012_ayzq_2.pdf","UserName":"babak","ReqNo":"2585"}', NULL, '2017-05-25 03:11:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6469, 63023, '', '{"FileName":"واکنش های چند جزیی با استفاده از مایعات یونی (1).pdf","UserName":"فرید","ReqNo":"2587"}', NULL, '2017-05-25 03:21:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6471, 58553, '', '{"FileName":"نقش حسابداری در یکپارچگی پس از  ادغام.pdf","UserName":"امیرحسین","ReqNo":"2589"}', NULL, '2017-05-25 03:21:13', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6473, 63127, '', '{"FileName":"2009-98 Conceptual framework for e-Maintenance Illustration by e-Maintenance technologies and platfo","UserName":"سید دانیال","ReqNo":"2591"}', NULL, '2017-05-25 10:34:17', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6475, 63231, '', '{"FileName":"ترجمه1.pdf","UserName":"azad","ReqNo":"2593"}', NULL, '2017-05-25 17:36:20', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6477, 63281, '', '{"FileName":"befudjulpp.docx","UserName":"نرگس","ReqNo":"2595"}', NULL, '2017-05-25 23:03:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6479, 63289, '', '{"FileName":"6-beiranvand-2016-DM.docx","UserName":"مهدی","ReqNo":"2597"}', NULL, '2017-05-25 23:20:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6481, 63127, '', '{"FileName":"2009-98 Conceptual framework for e-Maintenance Illustration by e-Maintenance technologies and platfo","UserName":"سید دانیال","ReqNo":"2591"}', NULL, '2017-05-26 03:00:34', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6483, 63231, '', '{"FileName":"ترجمه1.pdf","UserName":"azad","ReqNo":"2593"}', NULL, '2017-05-26 03:00:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6485, 63281, '', '{"FileName":"befudjulpp.docx","UserName":"نرگس","ReqNo":"2595"}', NULL, '2017-05-26 03:36:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6487, 63289, '', '{"FileName":"6-beiranvand-2016-DM.docx","UserName":"مهدی","ReqNo":"2597"}', NULL, '2017-05-26 03:36:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6489, 63371, '', '{"FileName":"[Human Affairs] Educating for profit educating global citizenship.pdf","UserName":"مژگان","ReqNo":"2599"}', NULL, '2017-05-26 15:22:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6491, 63389, '', '{"FileName":"baumeister2015.pdf","UserName":"بهناز","ReqNo":"2601"}', NULL, '2017-05-26 19:21:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6493, 63389, '', '{"FileName":"baumeister2015.pdf","UserName":"بهناز","ReqNo":"2603"}', NULL, '2017-05-26 19:23:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6495, 63371, '', '{"FileName":"[Human Affairs] Educating for profit educating global citizenship.pdf","UserName":"مژگان","ReqNo":"2599"}', NULL, '2017-05-27 03:00:47', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6497, 63389, '', '{"FileName":"baumeister2015.pdf","UserName":"بهناز","ReqNo":"2601"}', NULL, '2017-05-27 03:01:15', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6499, 63389, '', '{"FileName":"baumeister2015.pdf","UserName":"بهناز","ReqNo":"2603"}', NULL, '2017-05-27 03:01:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6501, 63555, '', '{"FileName":"wu2017.pdf","UserName":"nader","ReqNo":"2605"}', NULL, '2017-05-28 06:46:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6503, 63561, '', '{"FileName":"Douvere_Ehler_MSP_stepbystep_2009.pdf","UserName":"sajjad","ReqNo":"2607"}', NULL, '2017-05-28 09:27:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6505, 63561, '', '{"FileName":"MARINE SPATIAL۲.pdf","UserName":"sajjad","ReqNo":"2609"}', NULL, '2017-05-28 10:13:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6507, 63625, '', '{"FileName":"Discriminant model for online viral marketing influencing consumers.pdf","UserName":"مهدی","ReqNo":"2611"}', NULL, '2017-05-28 17:29:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6509, 63625, '', '{"FileName":"Social and attitudinal determinants of viral marketing dynamics.pdf","UserName":"مهدی","ReqNo":"2613"}', NULL, '2017-05-28 17:29:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6511, 63625, '', '{"FileName":"The success of viral ads- Social and attitudinal predictors.pdf","UserName":"مهدی","ReqNo":"2615"}', NULL, '2017-05-28 17:29:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6513, 63613, '', '{"FileName":"1.docx","UserName":"فربود","ReqNo":"2617"}', NULL, '2017-05-28 17:34:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6515, 63627, '', '{"FileName":"10.1109@INDICON.2016.7838939.docx","UserName":"mokhtar","ReqNo":"2619"}', NULL, '2017-05-28 17:53:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6517, 63627, '', '{"FileName":"10.1109@INDICON.2016.7838939.pdf","UserName":"mokhtar","ReqNo":"2621"}', NULL, '2017-05-28 17:53:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6519, 63613, '', '{"FileName":"1.docx","UserName":"فربود","ReqNo":"2623"}', NULL, '2017-05-28 22:03:42', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6521, 63555, '', '{"FileName":"wu2017.pdf","UserName":"nader","ReqNo":"2605"}', NULL, '2017-05-29 03:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6523, 63561, '', '{"FileName":"Douvere_Ehler_MSP_stepbystep_2009.pdf","UserName":"sajjad","ReqNo":"2607"}', NULL, '2017-05-29 03:02:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6525, 63561, '', '{"FileName":"MARINE SPATIAL۲.pdf","UserName":"sajjad","ReqNo":"2609"}', NULL, '2017-05-29 03:02:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6527, 63625, '', '{"FileName":"Discriminant model for online viral marketing influencing consumers.pdf","UserName":"مهدی","ReqNo":"2611"}', NULL, '2017-05-29 03:03:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6529, 63625, '', '{"FileName":"Social and attitudinal determinants of viral marketing dynamics.pdf","UserName":"مهدی","ReqNo":"2613"}', NULL, '2017-05-29 03:04:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6531, 63625, '', '{"FileName":"The success of viral ads- Social and attitudinal predictors.pdf","UserName":"مهدی","ReqNo":"2615"}', NULL, '2017-05-29 03:07:06', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6533, 63613, '', '{"FileName":"1.docx","UserName":"فربود","ReqNo":"2617"}', NULL, '2017-05-29 03:07:29', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6535, 63627, '', '{"FileName":"10.1109@INDICON.2016.7838939.docx","UserName":"mokhtar","ReqNo":"2619"}', NULL, '2017-05-29 03:07:34', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6537, 63627, '', '{"FileName":"10.1109@INDICON.2016.7838939.pdf","UserName":"mokhtar","ReqNo":"2621"}', NULL, '2017-05-29 03:08:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6539, 63613, '', '{"FileName":"1.docx","UserName":"فربود","ReqNo":"2623"}', NULL, '2017-05-29 03:09:10', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6541, 48437, '', '{"FileName":"LaurenGowdy2009.pdf","UserName":"Mohammad","ReqNo":"2625"}', NULL, '2017-05-29 10:23:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6543, 63749, '', '{"FileName":"Information Systems and systems Development.docx","UserName":"mousa","ReqNo":"2627"}', NULL, '2017-05-29 10:47:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6545, 63749, '', '{"FileName":"strategic.docx","UserName":"mousa","ReqNo":"2629"}', NULL, '2017-05-29 10:47:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6547, 63771, '', '{"FileName":"PI(wzxd20170522)final.pdf","UserName":"ali","ReqNo":"2631"}', NULL, '2017-05-29 15:11:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6549, 63771, '', '{"FileName":"RFQ500C Zipper Bag Making Machine.pdf","UserName":"ali","ReqNo":"2633"}', NULL, '2017-05-29 15:11:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6551, 63771, '', '{"FileName":"SJ-50 High Speed.pdf","UserName":"ali","ReqNo":"2635"}', NULL, '2017-05-29 15:11:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6553, 63795, '', '{"FileName":"h-10_1016_S0142_694X0300013_9_Focus.pdf","UserName":"hadi","ReqNo":"2637"}', NULL, '2017-05-29 17:58:45', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6555, 63613, '', '{"FileName":"1.pdf.docx","UserName":"فربود","ReqNo":"2639"}', NULL, '2017-05-29 19:14:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6557, 48437, '', '{"FileName":"LaurenGowdy2009.pdf","UserName":"Mohammad","ReqNo":"2625"}', NULL, '2017-05-30 03:01:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6559, 63749, '', '{"FileName":"Information Systems and systems Development.docx","UserName":"mousa","ReqNo":"2627"}', NULL, '2017-05-30 03:01:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6561, 63749, '', '{"FileName":"strategic.docx","UserName":"mousa","ReqNo":"2629"}', NULL, '2017-05-30 03:01:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6563, 63771, '', '{"FileName":"PI(wzxd20170522)final.pdf","UserName":"ali","ReqNo":"2631"}', NULL, '2017-05-30 03:01:48', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6565, 63771, '', '{"FileName":"RFQ500C Zipper Bag Making Machine.pdf","UserName":"ali","ReqNo":"2633"}', NULL, '2017-05-30 03:01:54', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6567, 63771, '', '{"FileName":"SJ-50 High Speed.pdf","UserName":"ali","ReqNo":"2635"}', NULL, '2017-05-30 03:02:00', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6569, 63795, '', '{"FileName":"h-10_1016_S0142_694X0300013_9_Focus.pdf","UserName":"hadi","ReqNo":"2637"}', NULL, '2017-05-30 03:02:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6571, 63613, '', '{"FileName":"1.pdf.docx","UserName":"فربود","ReqNo":"2639"}', NULL, '2017-05-30 03:02:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6573, 63795, '', '{"FileName":"1_archive.pdf","UserName":"hadi","ReqNo":"2641"}', NULL, '2017-05-30 22:31:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6575, 63795, '', '{"FileName":"1_archive.pdf","UserName":"hadi","ReqNo":"2641"}', NULL, '2017-05-31 03:03:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6577, 64017, '', '{"FileName":"AR catagories.docx","UserName":"علی","ReqNo":"2643"}', NULL, '2017-05-31 12:07:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6579, 64017, '', '{"FileName":"AR catagories.docx","UserName":"علی","ReqNo":"2643"}', NULL, '2017-06-01 03:00:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6581, 64123, '', '{"FileName":"Game-Changer A Case Study of SocialMedia.pdf","UserName":"ali","ReqNo":"2645"}', NULL, '2017-06-01 09:17:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6583, 64155, '', '{"FileName":"171 - 180.pdf","UserName":"امیرحسین","ReqNo":"2647"}', NULL, '2017-06-01 11:26:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6585, 64157, '', '{"FileName":"Udp111(1).pdf","UserName":"mahsa","ReqNo":"2649"}', NULL, '2017-06-01 11:27:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6587, 64157, '', '{"FileName":"Udp111(1).pdf","UserName":"mahsa","ReqNo":"2651"}', NULL, '2017-06-01 11:31:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6589, 64165, '', '{"FileName":"An effective test program that incorporates the automation of software testing has a.docx","UserName":"مصطفی","ReqNo":"2653"}', NULL, '2017-06-01 11:54:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6591, 64123, '', '{"FileName":"Game-Changer A Case Study of SocialMedia.pdf","UserName":"ali","ReqNo":"2645"}', NULL, '2017-06-02 03:00:57', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6593, 64155, '', '{"FileName":"171 - 180.pdf","UserName":"امیرحسین","ReqNo":"2647"}', NULL, '2017-06-02 03:01:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6595, 64157, '', '{"FileName":"Udp111(1).pdf","UserName":"mahsa","ReqNo":"2649"}', NULL, '2017-06-02 03:01:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6597, 64157, '', '{"FileName":"Udp111(1).pdf","UserName":"mahsa","ReqNo":"2651"}', NULL, '2017-06-02 03:01:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6599, 64165, '', '{"FileName":"An effective test program that incorporates the automation of software testing has a.docx","UserName":"مصطفی","ReqNo":"2653"}', NULL, '2017-06-02 03:03:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6601, 64365, '', '{"FileName":"198precis.pdf","UserName":"ramin63","ReqNo":"2655"}', NULL, '2017-06-03 01:16:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6603, 64365, '', '{"FileName":"198precis.pdf","UserName":"ramin63","ReqNo":"2655"}', NULL, '2017-06-03 03:00:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6605, 64383, '', '{"FileName":"ترجمه1.pdf","UserName":"azad","ReqNo":"2657"}', NULL, '2017-06-03 08:49:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6607, 34481, '', '{"FileName":"translate.docx","UserName":"علیرضا","ReqNo":"2659"}', NULL, '2017-06-03 09:33:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6609, 64393, '', '{"FileName":"2276-6746-1-PB.pdf","UserName":"محمدحسن","ReqNo":"2661"}', NULL, '2017-06-03 11:38:03', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6611, 64443, '', '{"FileName":"2-Classical Encryption 33(1).pdf","UserName":"alireza","ReqNo":"2663"}', NULL, '2017-06-03 18:09:12', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6613, 64443, '', '{"FileName":"2-Classical Encryption 33(1).pdf","UserName":"alireza","ReqNo":"2665"}', NULL, '2017-06-03 18:20:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6615, 64443, '', '{"FileName":"content-based-rec-systems.pdf","UserName":"alireza","ReqNo":"2667"}', NULL, '2017-06-03 18:21:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6617, 64443, '', '{"FileName":"Big_Data_3.pdf","UserName":"alireza","ReqNo":"2669"}', NULL, '2017-06-03 19:14:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6619, 64443, '', '{"FileName":"Big_Data_4.pdf","UserName":"alireza","ReqNo":"2671"}', NULL, '2017-06-03 19:15:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6621, 64443, '', '{"FileName":"content-based-rec-systems.pdf","UserName":"alireza","ReqNo":"2673"}', NULL, '2017-06-03 19:17:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6623, 64445, '', '{"FileName":"tarjome.docx","UserName":"اسد","ReqNo":"2675"}', NULL, '2017-06-04 00:21:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6625, 64445, '', '{"FileName":"tarjome.docx","UserName":"اسد","ReqNo":"2677"}', NULL, '2017-06-04 00:41:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6627, 64383, '', '{"FileName":"ترجمه1.pdf","UserName":"azad","ReqNo":"2657"}', NULL, '2017-06-04 03:01:19', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6629, 34481, '', '{"FileName":"translate.docx","UserName":"علیرضا","ReqNo":"2659"}', NULL, '2017-06-04 03:01:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6631, 64393, '', '{"FileName":"2276-6746-1-PB.pdf","UserName":"محمدحسن","ReqNo":"2661"}', NULL, '2017-06-04 03:04:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6633, 64443, '', '{"FileName":"2-Classical Encryption 33(1).pdf","UserName":"alireza","ReqNo":"2663"}', NULL, '2017-06-04 03:06:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6635, 64443, '', '{"FileName":"2-Classical Encryption 33(1).pdf","UserName":"alireza","ReqNo":"2665"}', NULL, '2017-06-04 03:07:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6637, 64443, '', '{"FileName":"content-based-rec-systems.pdf","UserName":"alireza","ReqNo":"2667"}', NULL, '2017-06-04 03:08:40', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6639, 64443, '', '{"FileName":"Big_Data_3.pdf","UserName":"alireza","ReqNo":"2669"}', NULL, '2017-06-04 03:09:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6641, 64443, '', '{"FileName":"Big_Data_4.pdf","UserName":"alireza","ReqNo":"2671"}', NULL, '2017-06-04 03:11:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6643, 64443, '', '{"FileName":"content-based-rec-systems.pdf","UserName":"alireza","ReqNo":"2673"}', NULL, '2017-06-04 03:12:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6645, 64445, '', '{"FileName":"tarjome.docx","UserName":"اسد","ReqNo":"2675"}', NULL, '2017-06-04 03:14:24', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6647, 64445, '', '{"FileName":"tarjome.docx","UserName":"اسد","ReqNo":"2677"}', NULL, '2017-06-04 03:15:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6649, 64481, '', '{"FileName":"s.pdf","UserName":"رضا","ReqNo":"2679"}', NULL, '2017-06-04 09:03:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6651, 64521, '', '{"FileName":"1-s2.0-S0892687514004257-main.pdf","UserName":"سعید","ReqNo":"2681"}', NULL, '2017-06-04 14:44:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6653, 64573, '', '{"FileName":"IEEE_01027041.pdf","UserName":"danial","ReqNo":"2683"}', NULL, '2017-06-04 23:19:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6655, 64573, '', '{"FileName":"10.154392015F002.pdf","UserName":"danial","ReqNo":"2685"}', NULL, '2017-06-04 23:21:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6657, 64481, '', '{"FileName":"s.pdf","UserName":"رضا","ReqNo":"2679"}', NULL, '2017-06-05 03:00:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6659, 64521, '', '{"FileName":"1-s2.0-S0892687514004257-main.pdf","UserName":"سعید","ReqNo":"2681"}', NULL, '2017-06-05 03:01:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6661, 64573, '', '{"FileName":"IEEE_01027041.pdf","UserName":"danial","ReqNo":"2683"}', NULL, '2017-06-05 03:01:17', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6663, 64573, '', '{"FileName":"10.154392015F002.pdf","UserName":"danial","ReqNo":"2685"}', NULL, '2017-06-05 03:01:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6665, 64639, '', '{"FileName":"131(3).pdf","UserName":"حسن","ReqNo":"2687"}', NULL, '2017-06-05 14:39:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6667, 64647, '', '{"FileName":"A New Dimension of Health Care Systematic Review.doc","UserName":"محمد جواد","ReqNo":"2689"}', NULL, '2017-06-05 16:10:29', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6669, 64647, '', '{"FileName":"Social_Media_and_Mobile_Apps_for.doc","UserName":"محمد جواد","ReqNo":"2691"}', NULL, '2017-06-05 16:10:43', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6671, 64639, '', '{"FileName":"131(3).pdf","UserName":"حسن","ReqNo":"2687"}', NULL, '2017-06-06 03:00:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6673, 64647, '', '{"FileName":"A New Dimension of Health Care Systematic Review.doc","UserName":"محمد جواد","ReqNo":"2689"}', NULL, '2017-06-06 03:02:28', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6675, 64647, '', '{"FileName":"Social_Media_and_Mobile_Apps_for.doc","UserName":"محمد جواد","ReqNo":"2691"}', NULL, '2017-06-06 03:03:44', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6677, 64699, '', '{"FileName":"Using fuzzy logic for diagnosis of technical condition of power circuit breakers.pdf","UserName":"اشکان","ReqNo":"2693"}', NULL, '2017-06-06 07:50:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6679, 64699, '', '{"FileName":"Aspects about the monitoring and diagnosis of high voltage circuit breakers_2.pdf","UserName":"اشکان","ReqNo":"2695"}', NULL, '2017-06-06 08:00:26', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6681, 64699, '', '{"FileName":"CB modeling.pdf","UserName":"اشکان","ReqNo":"2697"}', NULL, '2017-06-06 08:01:04', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6683, 64699, '', '{"FileName":"Embedded system and software application for monitoring and diagnosis of high voltage circuit breake","UserName":"اشکان","ReqNo":"2699"}', NULL, '2017-06-06 08:01:18', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6685, 64699, '', '{"FileName":"failure rate analysis.pdf","UserName":"اشکان","ReqNo":"2701"}', NULL, '2017-06-06 08:01:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6687, 64725, '', '{"FileName":"LVDP_Guidelines_052815.pdf","UserName":"شکوه","ReqNo":"2703"}', NULL, '2017-06-06 13:09:06', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6689, 64699, '', '{"FileName":"Using fuzzy logic for diagnosis of technical condition of power circuit breakers.pdf","UserName":"اشکان","ReqNo":"2693"}', NULL, '2017-06-07 03:00:51', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6691, 64699, '', '{"FileName":"Aspects about the monitoring and diagnosis of high voltage circuit breakers_2.pdf","UserName":"اشکان","ReqNo":"2695"}', NULL, '2017-06-07 03:01:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6693, 64699, '', '{"FileName":"CB modeling.pdf","UserName":"اشکان","ReqNo":"2697"}', NULL, '2017-06-07 03:01:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6695, 64699, '', '{"FileName":"Embedded system and software application for monitoring and diagnosis of high voltage circuit breake","UserName":"اشکان","ReqNo":"2699"}', NULL, '2017-06-07 03:01:20', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6697, 64699, '', '{"FileName":"failure rate analysis.pdf","UserName":"اشکان","ReqNo":"2701"}', NULL, '2017-06-07 03:01:34', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6699, 64725, '', '{"FileName":"LVDP_Guidelines_052815.pdf","UserName":"شکوه","ReqNo":"2703"}', NULL, '2017-06-07 03:18:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6701, 64881, '', '{"FileName":"mmm.pdf","UserName":"حسین","ReqNo":"2705"}', NULL, '2017-06-07 23:33:14', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6703, 64881, '', '{"FileName":"mmm.pdf","UserName":"حسین","ReqNo":"2705"}', NULL, '2017-06-08 03:01:03', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6705, 64957, '', '{"FileName":"Doc1.pdf","UserName":"مونا","ReqNo":"2707"}', NULL, '2017-06-08 17:03:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6707, 64957, '', '{"FileName":"Doc1.pdf","UserName":"مونا","ReqNo":"2707"}', NULL, '2017-06-09 03:00:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6709, 53383, '', '{"FileName":"Chapter 7. 237-253.pdf","UserName":"مهدی","ReqNo":"2709"}', NULL, '2017-06-09 10:38:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6711, 65027, '', '{"FileName":"video_3.pdf","UserName":"omid","ReqNo":"2711"}', NULL, '2017-06-09 14:17:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6713, 53383, '', '{"FileName":"Chapter 7. 237-253.pdf","UserName":"مهدی","ReqNo":"2709"}', NULL, '2017-06-10 03:00:49', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6715, 65027, '', '{"FileName":"video_3.pdf","UserName":"omid","ReqNo":"2711"}', NULL, '2017-06-10 03:01:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6717, 65083, '', '{"FileName":"Untitled-1.pdf","UserName":"alireza","ReqNo":"2713"}', NULL, '2017-06-10 09:52:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6719, 65083, '', '{"FileName":"Untitled-2.pdf","UserName":"alireza","ReqNo":"2715"}', NULL, '2017-06-10 09:53:59', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6721, 65083, '', '{"FileName":"Untitled-3.pdf","UserName":"alireza","ReqNo":"2717"}', NULL, '2017-06-10 09:54:09', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6723, 65083, '', '{"FileName":"Untitled-4.pdf","UserName":"alireza","ReqNo":"2719"}', NULL, '2017-06-10 09:54:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6725, 65083, '', '{"FileName":"Untitled-5.pdf","UserName":"alireza","ReqNo":"2721"}', NULL, '2017-06-10 09:54:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6727, 64699, '', '{"FileName":"Corrosion Studies on Gold-Plated Electrical Contacts.pdf","UserName":"اشکان","ReqNo":"2723"}', NULL, '2017-06-10 16:44:16', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6729, 64443, '', '{"FileName":"Big_Data_2.pdf","UserName":"alireza","ReqNo":"2725"}', NULL, '2017-06-10 19:41:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6731, 64443, '', '{"FileName":"85.pdf","UserName":"alireza","ReqNo":"2727"}', NULL, '2017-06-10 19:44:39', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6733, 64443, '', '{"FileName":"87.pdf","UserName":"alireza","ReqNo":"2729"}', NULL, '2017-06-10 19:44:51', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6735, 64443, '', '{"FileName":"89.pdf","UserName":"alireza","ReqNo":"2731"}', NULL, '2017-06-10 19:45:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6737, 53383, '', '{"FileName":"100000000.pdf","UserName":"مهدی","ReqNo":"2733"}', NULL, '2017-06-11 02:28:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6739, 65083, '', '{"FileName":"Untitled-1.pdf","UserName":"alireza","ReqNo":"2713"}', NULL, '2017-06-11 03:00:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6741, 65083, '', '{"FileName":"Untitled-2.pdf","UserName":"alireza","ReqNo":"2715"}', NULL, '2017-06-11 03:00:45', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6743, 65083, '', '{"FileName":"Untitled-3.pdf","UserName":"alireza","ReqNo":"2717"}', NULL, '2017-06-11 03:00:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6745, 65083, '', '{"FileName":"Untitled-4.pdf","UserName":"alireza","ReqNo":"2719"}', NULL, '2017-06-11 03:00:52', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6747, 65083, '', '{"FileName":"Untitled-5.pdf","UserName":"alireza","ReqNo":"2721"}', NULL, '2017-06-11 03:00:55', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6749, 64699, '', '{"FileName":"Corrosion Studies on Gold-Plated Electrical Contacts.pdf","UserName":"اشکان","ReqNo":"2723"}', NULL, '2017-06-11 03:01:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6751, 64443, '', '{"FileName":"Big_Data_2.pdf","UserName":"alireza","ReqNo":"2725"}', NULL, '2017-06-11 03:01:21', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6753, 64443, '', '{"FileName":"85.pdf","UserName":"alireza","ReqNo":"2727"}', NULL, '2017-06-11 03:01:26', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6755, 64443, '', '{"FileName":"87.pdf","UserName":"alireza","ReqNo":"2729"}', NULL, '2017-06-11 03:01:30', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6757, 64443, '', '{"FileName":"89.pdf","UserName":"alireza","ReqNo":"2731"}', NULL, '2017-06-11 03:01:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6759, 53383, '', '{"FileName":"100000000.pdf","UserName":"مهدی","ReqNo":"2733"}', NULL, '2017-06-11 03:01:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6761, 65083, '', '{"FileName":"Untitled-5.pdf","UserName":"alireza","ReqNo":"2735"}', NULL, '2017-06-11 07:57:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6763, 65083, '', '{"FileName":"Untitled-1.pdf","UserName":"alireza","ReqNo":"2737"}', NULL, '2017-06-11 07:58:22', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6765, 65083, '', '{"FileName":"Untitled-4.pdf","UserName":"alireza","ReqNo":"2739"}', NULL, '2017-06-11 07:58:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6767, 65239, '', '{"FileName":"منابع (2).docx","UserName":"احمد","ReqNo":"2741"}', NULL, '2017-06-11 08:35:33', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6769, 65267, '', '{"FileName":"A_study_on_current_business_performance.pdf","UserName":"مجتبی","ReqNo":"2743"}', NULL, '2017-06-11 15:49:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6771, 64647, '', '{"FileName":"MobiFish_A_lightweight_anti_phishing.pdf","UserName":"محمد جواد","ReqNo":"2745"}', NULL, '2017-06-12 01:00:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6773, 64647, '', '{"FileName":"Phishing_detection_based_Associative.pdf","UserName":"محمد جواد","ReqNo":"2747"}', NULL, '2017-06-12 01:00:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6775, 65083, '', '{"FileName":"Untitled-5.pdf","UserName":"alireza","ReqNo":"2735"}', NULL, '2017-06-12 03:00:39', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6777, 65083, '', '{"FileName":"Untitled-1.pdf","UserName":"alireza","ReqNo":"2737"}', NULL, '2017-06-12 03:00:43', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6779, 65083, '', '{"FileName":"Untitled-4.pdf","UserName":"alireza","ReqNo":"2739"}', NULL, '2017-06-12 03:00:46', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6781, 65239, '', '{"FileName":"منابع (2).docx","UserName":"احمد","ReqNo":"2741"}', NULL, '2017-06-12 03:02:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6783, 65267, '', '{"FileName":"A_study_on_current_business_performance.pdf","UserName":"مجتبی","ReqNo":"2743"}', NULL, '2017-06-12 03:02:32', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6785, 64647, '', '{"FileName":"MobiFish_A_lightweight_anti_phishing.pdf","UserName":"محمد جواد","ReqNo":"2745"}', NULL, '2017-06-12 03:02:36', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6787, 64647, '', '{"FileName":"Phishing_detection_based_Associative.pdf","UserName":"محمد جواد","ReqNo":"2747"}', NULL, '2017-06-12 03:04:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6789, 65345, '', '{"FileName":"K_T_Chau_Electric_Vehicle_Machines.pdf","UserName":"محمدرضا","ReqNo":"2749"}', NULL, '2017-06-12 12:09:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6791, 63007, '', '{"FileName":"4.docx","UserName":"sherhood","ReqNo":"2751"}', NULL, '2017-06-12 21:34:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6793, 63007, '', '{"FileName":"31.docx","UserName":"sherhood","ReqNo":"2753"}', NULL, '2017-06-12 22:01:58', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6795, 2057, '', '{"FileName":"SCM - Part 2.pdf","UserName":"محمد موسی","ReqNo":"2755"}', NULL, '2017-06-13 01:58:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6797, 65345, '', '{"FileName":"K_T_Chau_Electric_Vehicle_Machines.pdf","UserName":"محمدرضا","ReqNo":"2749"}', NULL, '2017-06-13 03:00:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6799, 65479, '', '{"FileName":"Family and Friends 3 - Readers (Pinocchio).pdf","UserName":"morteza","ReqNo":"2757"}', NULL, '2017-06-14 00:39:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6801, 65521, '', '{"FileName":"4.pdf","UserName":"مینا","ReqNo":"2759"}', NULL, '2017-06-14 14:36:13', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6803, 65565, '', '{"FileName":"mm.docx","UserName":"یگانه","ReqNo":"2761"}', NULL, '2017-06-15 03:57:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6805, 63007, '', '{"FileName":"4.doc","UserName":"sherhood","ReqNo":"2763"}', NULL, '2017-06-15 21:17:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6807, 63007, '', '{"FileName":"31.docx","UserName":"sherhood","ReqNo":"2765"}', NULL, '2017-06-15 21:18:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6809, 65565, '', '{"FileName":"t.docx","UserName":"یگانه","ReqNo":"2767"}', NULL, '2017-06-16 00:31:08', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6811, 65727, '', '{"FileName":"mandatory....pdf","UserName":"شیوا","ReqNo":"2769"}', NULL, '2017-06-16 18:11:40', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6813, 20919, '', '{"FileName":"مقاله بیس.pdf","UserName":"فریده","ReqNo":"2771"}', NULL, '2017-06-17 09:29:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6815, 65815, '', '{"FileName":"10 ST-SC.pdf","UserName":"hosein","ReqNo":"2773"}', NULL, '2017-06-17 12:26:49', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6817, 65855, '', '{"FileName":"دمیری فصل اول و دوم.pdf","UserName":"sarah1365","ReqNo":"2775"}', NULL, '2017-06-17 23:12:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6819, 65855, '', '{"FileName":"دمیری فصل اول و دوم.pdf","UserName":"sarah1365","ReqNo":"2777"}', NULL, '2017-06-17 23:17:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6821, 65929, '', '{"FileName":"critical study.pdf","UserName":"جواد","ReqNo":"2779"}', NULL, '2017-06-18 17:24:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6823, 65947, '', '{"FileName":"4_5870480206342389846.docx","UserName":"حمید رضا","ReqNo":"2781"}', NULL, '2017-06-18 23:04:28', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6825, 65947, '', '{"FileName":"4_5956462165958328351.doc","UserName":"حمید رضا","ReqNo":"2783"}', NULL, '2017-06-18 23:05:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6827, 65947, '', '{"FileName":"po.doc","UserName":"حمید رضا","ReqNo":"2785"}', NULL, '2017-06-18 23:21:23', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6829, 63007, '', '{"FileName":"4.docx","UserName":"sherhood","ReqNo":"2751"}', NULL, '2017-06-19 08:54:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6831, 63007, '', '{"FileName":"31.docx","UserName":"sherhood","ReqNo":"2753"}', NULL, '2017-06-19 08:54:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6833, 2057, '', '{"FileName":"SCM - Part 2.pdf","UserName":"محمد موسی","ReqNo":"2755"}', NULL, '2017-06-19 13:53:33', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6835, 2057, '', '{"FileName":"SCM - Part 2.pdf","UserName":"محمد موسی","ReqNo":"2755"}', NULL, '2017-06-19 13:55:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6837, 65479, '', '{"FileName":"Family and Friends 3 - Readers (Pinocchio).pdf","UserName":"morteza","ReqNo":"2757"}', NULL, '2017-06-19 13:55:05', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6839, 65521, '', '{"FileName":"4.pdf","UserName":"مینا","ReqNo":"2759"}', NULL, '2017-06-19 13:55:16', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6841, 65565, '', '{"FileName":"mm.docx","UserName":"یگانه","ReqNo":"2761"}', NULL, '2017-06-19 13:55:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6843, 63007, '', '{"FileName":"4.doc","UserName":"sherhood","ReqNo":"2763"}', NULL, '2017-06-19 13:55:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6845, 63007, '', '{"FileName":"31.docx","UserName":"sherhood","ReqNo":"2765"}', NULL, '2017-06-19 13:56:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6847, 65565, '', '{"FileName":"t.docx","UserName":"یگانه","ReqNo":"2767"}', NULL, '2017-06-19 13:57:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6849, 65727, '', '{"FileName":"mandatory....pdf","UserName":"شیوا","ReqNo":"2769"}', NULL, '2017-06-19 14:00:46', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6851, 20919, '', '{"FileName":"مقاله بیس.pdf","UserName":"فریده","ReqNo":"2771"}', NULL, '2017-06-19 14:00:50', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6853, 65815, '', '{"FileName":"10 ST-SC.pdf","UserName":"hosein","ReqNo":"2773"}', NULL, '2017-06-19 14:03:02', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6855, 65855, '', '{"FileName":"دمیری فصل اول و دوم.pdf","UserName":"sarah1365","ReqNo":"2775"}', NULL, '2017-06-19 14:12:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6857, 65855, '', '{"FileName":"دمیری فصل اول و دوم.pdf","UserName":"sarah1365","ReqNo":"2777"}', NULL, '2017-06-19 14:25:09', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6859, 65929, '', '{"FileName":"critical study.pdf","UserName":"جواد","ReqNo":"2779"}', NULL, '2017-06-19 14:28:31', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6861, 65947, '', '{"FileName":"4_5870480206342389846.docx","UserName":"حمید رضا","ReqNo":"2781"}', NULL, '2017-06-19 14:30:35', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6863, 65947, '', '{"FileName":"4_5956462165958328351.doc","UserName":"حمید رضا","ReqNo":"2783"}', NULL, '2017-06-19 14:31:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6865, 65947, '', '{"FileName":"po.doc","UserName":"حمید رضا","ReqNo":"2785"}', NULL, '2017-06-19 14:32:58', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6867, 66055, '', '{"FileName":"10_1016_j_soildyn_2013_02_011_Slope.pdf","UserName":"میلاد","ReqNo":"2787"}', NULL, '2017-06-19 20:18:56', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6869, 66063, '', '{"FileName":"277.pdf","UserName":"مجید","ReqNo":"2789"}', NULL, '2017-06-19 22:33:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6871, 66055, '', '{"FileName":"10_1016_j_soildyn_2013_02_011_Slope.pdf","UserName":"میلاد","ReqNo":"2787"}', NULL, '2017-06-20 03:01:10', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6873, 66063, '', '{"FileName":"277.pdf","UserName":"مجید","ReqNo":"2789"}', NULL, '2017-06-20 03:01:27', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6875, 66147, '', '{"FileName":"test.doc","UserName":"علی","ReqNo":"2791"}', NULL, '2017-06-20 23:57:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6877, 66147, '', '{"FileName":"test.doc","UserName":"علی","ReqNo":"2791"}', NULL, '2017-06-21 03:00:42', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6879, 66157, '', '{"FileName":"ترجمه.pdf","UserName":"مصطفی","ReqNo":"2793"}', NULL, '2017-06-21 06:43:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6881, 65815, '', '{"FileName":"10 ST-SC.pdf","UserName":"hosein","ReqNo":"2795"}', NULL, '2017-06-21 09:46:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6883, 65815, '', '{"FileName":"علي.pdf","UserName":"hosein","ReqNo":"2797"}', NULL, '2017-06-21 12:45:34', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6885, 66241, '', '{"FileName":"video_10.pdf","UserName":"میثم","ReqNo":"2799"}', NULL, '2017-06-22 02:49:24', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6887, 66157, '', '{"FileName":"ترجمه.pdf","UserName":"مصطفی","ReqNo":"2793"}', NULL, '2017-06-22 03:00:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6889, 65815, '', '{"FileName":"10 ST-SC.pdf","UserName":"hosein","ReqNo":"2795"}', NULL, '2017-06-22 03:01:08', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6891, 65815, '', '{"FileName":"علي.pdf","UserName":"hosein","ReqNo":"2797"}', NULL, '2017-06-22 03:01:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6893, 66241, '', '{"FileName":"video_10.pdf","UserName":"میثم","ReqNo":"2799"}', NULL, '2017-06-22 03:01:52', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6895, 66243, '', '{"FileName":"10.1016@j.ress.2017.03.005.pdf","UserName":"احمد","ReqNo":"2801"}', NULL, '2017-06-22 03:08:44', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6897, 66243, '', '{"FileName":"j.ijmst.2017.01.022_4b2bd0fce195c5a55d792d0af4869725.pdf","UserName":"احمد","ReqNo":"2803"}', NULL, '2017-06-22 03:12:21', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6899, 66243, '', '{"FileName":"10.1016@j.ress.2017.03.005.pdf","UserName":"احمد","ReqNo":"2801"}', NULL, '2017-06-23 03:01:18', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6901, 66243, '', '{"FileName":"j.ijmst.2017.01.022_4b2bd0fce195c5a55d792d0af4869725.pdf","UserName":"احمد","ReqNo":"2803"}', NULL, '2017-06-23 03:01:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6903, 66307, '', '{"FileName":"HijazinMuaz_2012_01_17.docx","UserName":"mohsen","ReqNo":"2805"}', NULL, '2017-06-23 05:26:50', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6905, 66307, '', '{"FileName":"HijazinMuaz_2012_01_17.docx","UserName":"mohsen","ReqNo":"2805"}', NULL, '2017-06-24 03:00:55', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6907, 66407, '', '{"FileName":"acer_software_license_information.pdf","UserName":"امیر","ReqNo":"2807"}', NULL, '2017-06-24 04:47:52', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6909, 66407, '', '{"FileName":"acer_software_license_information.pdf","UserName":"امیر","ReqNo":"2809"}', NULL, '2017-06-24 04:51:46', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6911, 66407, '', '{"FileName":"acer_software_license_information.pdf","UserName":"امیر","ReqNo":"2807"}', NULL, '2017-06-25 03:00:50', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6913, 66407, '', '{"FileName":"acer_software_license_information.pdf","UserName":"امیر","ReqNo":"2809"}', NULL, '2017-06-25 03:01:01', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6915, 20919, '', '{"FileName":"مقاله بیس.pdf","UserName":"فریده","ReqNo":"2811"}', NULL, '2017-06-25 10:33:30', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6917, 20919, '', '{"FileName":"مقاله بیس.pdf","UserName":"فریده","ReqNo":"2811"}', NULL, '2017-06-26 03:00:56', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6919, 66755, '', '{"FileName":"4_238319467442996062.pdf","UserName":"محسن","ReqNo":"2813"}', NULL, '2017-06-28 21:20:54', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6921, 66755, '', '{"FileName":"4_238319467442996062.pdf","UserName":"محسن","ReqNo":"2813"}', NULL, '2017-06-29 03:01:29', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6923, 66865, '', '{"FileName":"زبان مد.pdf","UserName":"Mahrad","ReqNo":"2815"}', NULL, '2017-06-29 19:39:00', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6925, 66865, '', '{"FileName":"The three S.pdf","UserName":"Mahrad","ReqNo":"2817"}', NULL, '2017-06-29 19:42:32', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6927, 66865, '', '{"FileName":"زبان مد.pdf","UserName":"Mahrad","ReqNo":"2815"}', NULL, '2017-06-30 03:03:04', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6929, 66865, '', '{"FileName":"The three S.pdf","UserName":"Mahrad","ReqNo":"2817"}', NULL, '2017-06-30 03:03:13', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6931, 67197, '', '{"FileName":"The_Vertical_Axis_of_Community_Organization.pdf","UserName":"محمد","ReqNo":"2819"}', NULL, '2017-07-03 13:10:05', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6933, 67197, '', '{"FileName":"The_Vertical_Axis_of_Community_Organization.pdf","UserName":"محمد","ReqNo":"2819"}', NULL, '2017-07-04 03:00:25', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6935, 67315, '', '{"FileName":"Malgorzata_Warmuzek,_Magorzata_Warmuzek_1396-4-8-11-43.pdf","UserName":"Haj Akbar","ReqNo":"2821"}', NULL, '2017-07-04 12:26:55', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6937, 67315, '', '{"FileName":"Malgorzata_Warmuzek,_Magorzata_Warmuzek_1396-4-8-11-43.pdf","UserName":"Haj Akbar","ReqNo":"2821"}', NULL, '2017-07-05 03:02:36', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6939, 67423, '', '{"FileName":"5GOperatorSurvey2016-Final03-16-17.pdf","UserName":"شهرام","ReqNo":"2823"}', NULL, '2017-07-05 15:51:36', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6941, 67423, '', '{"FileName":"5GOperatorSurvey2016-Final03-16-17.pdf","UserName":"شهرام","ReqNo":"2823"}', NULL, '2017-07-06 03:00:23', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6943, 67523, '', '{"FileName":"Inter Jour of Production Economics.Vol 105.Iss 2.A19.pdf","UserName":"نسرین","ReqNo":"2825"}', NULL, '2017-07-06 20:55:37', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6945, 67525, '', '{"FileName":"چکیده.docx","UserName":"فریبا","ReqNo":"2827"}', NULL, '2017-07-06 21:39:38', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6947, 67523, '', '{"FileName":"Inter Jour of Production Economics.Vol 105.Iss 2.A19.pdf","UserName":"نسرین","ReqNo":"2825"}', NULL, '2017-07-07 03:00:06', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6949, 67525, '', '{"FileName":"چکیده.docx","UserName":"فریبا","ReqNo":"2827"}', NULL, '2017-07-07 03:00:20', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6951, 67603, '', '{"FileName":"The Terrible Two - Mac Barnett.pdf","UserName":"محمد","ReqNo":"2829"}', NULL, '2017-07-08 02:02:47', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6953, 67603, '', '{"FileName":"The Terrible Two - Mac Barnett.pdf","UserName":"محمد","ReqNo":"2829"}', NULL, '2017-07-08 03:00:07', 1, 'OFR.Error', b'0', b'1', b'0', 'N'),
	(6955, 67615, '', '{"FileName":"Teaching_with_Technology_Remediating_the.pdf","UserName":"پریسا","ReqNo":"2831"}', NULL, '2017-07-08 10:13:41', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6957, 67615, '', '{"FileName":"Teaching_with_Technology_Remediating_the.pdf","UserName":"پریسا","ReqNo":"2833"}', NULL, '2017-07-08 14:58:31', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6959, 67651, '', '{"FileName":"10.1016_j.compag.2016.04.013-A-GIS-based-Logic-Scoring-of-Preference-method-for-evaluation-of-land-c","UserName":"حسن","ReqNo":"2835"}', NULL, '2017-07-08 18:10:19', 1, 'OFR.New', b'0', b'1', b'0', 'N'),
	(6961, 67615, '', '{"FileName":"Teaching_with_Technology_Remediating_the.pdf","UserName":"پریسا","ReqNo":"2831"}', NULL, '2017-07-09 03:00:33', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6963, 67615, '', '{"FileName":"Teaching_with_Technology_Remediating_the.pdf","UserName":"پریسا","ReqNo":"2833"}', NULL, '2017-07-09 03:00:59', 1, 'OFR.Ok', b'0', b'1', b'0', 'N'),
	(6965, 67651, '', '{"FileName":"10.1016_j.compag.2016.04.013-A-GIS-based-Logic-Scoring-of-Preference-method-for-evaluation-of-land-c","UserName":"حسن","ReqNo":"2835"}', NULL, '2017-07-09 03:00:59', 1, 'OFR.Error', b'0', b'1', b'0', 'N');
/*!40000 ALTER TABLE `tblPosts` ENABLE KEYS */;

-- Dumping structure for table TargomanBlog.tblPostTemplates
DROP TABLE IF EXISTS `tblPostTemplates`;
CREATE TABLE IF NOT EXISTS `tblPostTemplates` (
  `ptlID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ptlCode` varchar(50) NOT NULL,
  `ptlLanguage` varchar(2) NOT NULL,
  `ptlTitleTemplate` varchar(500) NOT NULL,
  `ptlBodyTemplate` text NOT NULL,
  PRIMARY KEY (`ptlID`),
  UNIQUE KEY `ptlPurpose_ptlLanguage` (`ptlCode`,`ptlLanguage`),
  KEY `ptlLanguage` (`ptlLanguage`),
  KEY `ptlPurpose` (`ptlCode`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- Dumping data for table TargomanBlog.tblPostTemplates: ~3 rows (approximately)
/*!40000 ALTER TABLE `tblPostTemplates` DISABLE KEYS */;
INSERT INTO `tblPostTemplates` (`ptlID`, `ptlCode`, `ptlLanguage`, `ptlTitleTemplate`, `ptlBodyTemplate`) VALUES
	(1, 'OFR.New', 'fa', 'درخواست ترجمه %@%FileName%@% ثبت شد.', '%@%UserName%@% عزیز درخواست شما برای ترجمه فایل با نام  %@%FileName%@% ثبت شد. نتیجه ترجمه این فایل در کمتر از ۲۴ ساعت آینده ارایه خواهد شد. کد درخواست شما %@%ReqNo%@% است.\r\n لطفا در صورت وجود مشکل یا تاخیر در پاسخگویی مشکل را با مراجعه به [صفحه گزارش خطا](http://bug.targoman.com) و با ذکر کد درخواست اعلام فرمایید تا در اسرع وقت رسیدگی شود. \r\nبا توجه به کیفیت ترجمه ماشینی که نیازمند بازبینی و ویرایش مجدد است پیشنهاد می‌کنیم فرایند ترجمه خود را در سامانه [ترجمیار](http://tarjomyar.ir) پیش برید. ترجمیار به عنوان یک دستیار ترجمه هوشمند می‌تواند فرایند ترجمه را برای شما ساده و آسان کند. '),
	(3, 'OFR.Error', 'fa', 'درخواست ترجمه %@%FileName%@% با مشکل مواجه شد.', '%@%UserName%@% عزیز متاسفانه امکان تبدیل و ترجمه فایل   %@%FileName%@% با کد درخواست %@%ReqNo%@%   وجود ندارد. دلیل این موضوع می تواند نوع فایل ورودی و عدم توانایی سیستم برای استخراج متن از درون آن باشد. '),
	(5, 'OFR.Ok', 'fa', 'ترجمه %@%FileName%@%آماده دریافت می‌باشد.', '%@%UserName%@% عزیز، فایل ارسالی با نام    %@%FileName%@%  ترجمه شده و آماده دریافت می باشد. برای دریافت فایل ترجمه شده دکمه زیر را فشار دهید\r\n\r\n  با توجه به مشکلاتی که در استخراج متن از اسناد وجود دارد ممکن است این فایل به صورت کامل ترجمه نشده باشد. اگر مشکل مهمی در ترجمه فایل مشاهده می‌کنید که امکان برطرف کردن آن میسر است آن را با مراجعه به [صفحه گزارش خطا](http://bug.targoman.com) و با ذکر کد درخواست ( %@%ReqNo%@%) اعلام فرمایید تا در اسرع وقت رسیدگی شود. \r\n\r\nلازم به توجه است که فایل شما توسط ماشین ترجمه شده و طبیعتاً از کیفیت بسیار خوبی برخوردار نیست. شما می‌توانید با  استفاده از سامانه [ترجمیار](http://tarjomyar.ir) متنی با کیفیت بالا تولید کنید. ترجمیار به عنوان یک دستیار مترجم هوشمند به شما کمک می‌کند تا خودتان با سرعت بالا و هزینه پایین متن را ترجمه کنید. \r\n\r\n - **[ دریافت فایل ترجمه شده](javascript::API::downloadTranslatedFile )**\r\n');
/*!40000 ALTER TABLE `tblPostTemplates` ENABLE KEYS */;

-- Dumping structure for procedure TargomanBlog.sp_CREATE_addPostComment
DROP PROCEDURE IF EXISTS `sp_CREATE_addPostComment`;
DELIMITER //
CREATE PROCEDURE `sp_CREATE_addPostComment`(
	IN `iPostID` BIGINT UNSIGNED,
	IN `iInReplyTo` BIGINT UNSIGNED,
	IN `iText` VARCHAR(5000),
	IN `iUserID` INT UNSIGNED

,
	IN `iUserFullName` VARCHAR(100)


)
BEGIN
	DECLARE PostID BIGINT UNSIGNED;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION 
	BEGIN
		ROLLBACK;
		RESIGNAL;
	END;
	
	SET PostID = IF(NOT ISNULL(iPostID), iPostID, (SELECT tblComments.cmt_bpsID FROM tblComments WHERE tblComments.cmtID = iInReplyTo));

	START TRANSACTION;
	   INSERT INTO tblComments
	       SET tblComments.cmt_bpsID = PostID,
	           tblComments.cmtInReplyTo_cmtID = iInReplyTo,
	           tblComments.cmtText = iText,
	           tblComments.cmtWrittenBy = iUserID,
	           tblComments.cmtWrittenByFullName = iUserFullName;
	           
		UPDATE tblPosts
			SET tblPosts.bpsHasComment = 1
			WHERE tblPosts.bpsID = iPostID;

		IF NOT ISNULL(iInReplyTo) THEN
			UPDATE tblComments
				SET tblComments.cmtHasReply = 1
				WHERE tblComments.cmtID = iInReplyTo;
		END IF;
	COMMIT;

END//
DELIMITER ;

-- Dumping structure for procedure TargomanBlog.sp_READ_listCommentReplies
DROP PROCEDURE IF EXISTS `sp_READ_listCommentReplies`;
DELIMITER //
CREATE PROCEDURE `sp_READ_listCommentReplies`(
	IN `iUserID` INT UNSIGNED,
	IN `icmtID` BIGINT UNSIGNED,
	IN `iStartFrom` BIGINT,
	IN `iCount` BIGINT
)
BEGIN
	SELECT SQL_CALC_FOUND_ROWS
		  InnerTable.cmtID,
		  InnerTable.cmtInReplyTo_cmtID,
	     InnerTable.cmtText,
	     InnerTable.cmtWrittenOn,
	     InnerTable.cmtWrittenByFullName,
        InnerTable.cmtHasReply,
  	     InnerTable.cmtCanReply,
	     (SELECT COALESCE(tblCommentsLikes.cmlOpinion, 0)
	         FROM tblCommentsLikes
	         WHERE tblCommentsLikes.cml_cmtID = InnerTable.cmtID
	           AND tblCommentsLikes.cml_usrID = iUserID
	     )AS cmtOldOpinion,
	     (SELECT COALESCE(SUM(CASE WHEN tblCommentsLikes.cmlOpinion = 1  THEN 1 ELSE 0 END), 0)
	         FROM tblCommentsLikes
	         WHERE tblCommentsLikes.cml_cmtID = InnerTable.cmtID) AS cmtLikes, 
	     (SELECT COALESCE(SUM(CASE WHEN tblCommentsLikes.cmlOpinion = -1 THEN 1 ELSE 0 END), 0)
	         FROM tblCommentsLikes
	         WHERE tblCommentsLikes.cml_cmtID = InnerTable.cmtID) AS cmtDisLikes,
	     InnerTable.cmtStatus
	 FROM (SELECT tblComments.cmtID,
			        tblComments.cmtText,
			        tblComments.cmtInReplyTo_cmtID,
			        tblComments.cmtWrittenOn,
			        tblComments.cmtWrittenByFullName,
			        tblComments.cmtHasReply,
			        tblComments.cmtCanReply,
			        tblComments.cmtStatus
			     FROM tblComments
			     WHERE tblComments.cmtInReplyTo_cmtID = icmtID
			       AND (tblComments.cmtStatus = 'P'
			            OR (tblComments.cmtStatus !='R' AND tblComments.cmtWrittenBy = iUserID))
			     ORDER BY
			         tblComments.cmtWrittenOn DESC
			     LIMIT iStartFrom, iCount
			) AS InnerTable;
END//
DELIMITER ;

-- Dumping structure for procedure TargomanBlog.sp_READ_listComments
DROP PROCEDURE IF EXISTS `sp_READ_listComments`;
DELIMITER //
CREATE PROCEDURE `sp_READ_listComments`(
	IN `iUserID` INT UNSIGNED,
	IN `ibpsID` BIGINT UNSIGNED,
	IN `iStartFrom` BIGINT,
	IN `iCount` BIGINT
)
BEGIN
	SELECT 
		  InnerTable.cmtID,
	     NULL AS cmtInReplyTo_cmtID,
	     InnerTable.cmtText,
	     InnerTable.cmtWrittenOn,
	     InnerTable.cmtWrittenByFullName,
	     InnerTable.cmtHasReply,
	     InnerTable.cmtCanReply,
	     (SELECT COALESCE(tblCommentsLikes.cmlOpinion, 0)
	         FROM tblCommentsLikes
	         WHERE tblCommentsLikes.cml_cmtID = InnerTable.cmtID
	           AND tblCommentsLikes.cml_usrID = iUserID
	     )AS cmtOldOpinion,
	     (SELECT COALESCE(SUM(CASE WHEN tblCommentsLikes.cmlOpinion = 1  THEN 1 ELSE 0 END), 0)
	         FROM tblCommentsLikes
	         WHERE tblCommentsLikes.cml_cmtID = InnerTable.cmtID) AS cmtLikes, 
	     (SELECT COALESCE(SUM(CASE WHEN tblCommentsLikes.cmlOpinion = -1 THEN 1 ELSE 0 END), 0)
	         FROM tblCommentsLikes
	         WHERE tblCommentsLikes.cml_cmtID = InnerTable.cmtID) AS cmtDisLikes,
	     InnerTable.cmtStatus
	 FROM (SELECT tblComments.cmtID,
			        tblComments.cmtText,
			        tblComments.cmtWrittenOn,
			        tblComments.cmtWrittenByFullName,
			        tblComments.cmtHasReply,
			        tblComments.cmtCanReply,
			        tblComments.cmtStatus
			     FROM tblComments
			     WHERE tblComments.cmt_bpsID = ibpsID
			   	 AND ISNULL(tblComments.cmtInReplyTo_cmtID)
			       AND (tblComments.cmtStatus = 'P'
			            OR (tblComments.cmtStatus !='R' AND tblComments.cmtWrittenBy = iUserID))
			     ORDER BY
			         tblComments.cmtWrittenOn DESC
			     LIMIT iStartFrom, iCount
			) AS InnerTable;

END//
DELIMITER ;

-- Dumping structure for procedure TargomanBlog.sp_READ_listPosts
DROP PROCEDURE IF EXISTS `sp_READ_listPosts`;
DELIMITER //
CREATE PROCEDURE `sp_READ_listPosts`(
	IN `iUserID` INT UNSIGNED,
	IN `iPostID` BIGINT UNSIGNED,
	IN `iCategoryID` INT,
	IN `iStartDate` DATE,
	IN `iEndDate` DATE,
	IN `iStartFrom` BIGINT,
	IN `iCount` BIGINT,
	IN `iPreferedLanguage` CHAR(2)

)
BEGIN
	SELECT 
	     InnerTable.bpsID,
	     InnerTable.bpsTitle,
	     InnerTable.bpsMarkdown,
	     InnerTable.bpsInsertionDate,
	     InnerTable.bps_ptlCode,
	     InnerTable.bps_catID,
	     InnerTable.catName,
	     InnerTable.catPersianName,
	     InnerTable.bpsShowOnTop,
	     InnerTable.bpsHasComment,
	     InnerTable.bpsCommentsAllowed,
	     (SELECT COUNT(1)
	         FROM tblPostRead
	         WHERE tblPostRead.bpr_bpsID = InnerTable.bpsID) AS bpsVisits,
	      InnerTable.bprOpinion AS bpsOldUserOpinion,
	     (SELECT  COALESCE(SUM(CASE WHEN tblPostRead.bprOpinion = 1 THEN 1 ELSE 0 END), 0) 
	         FROM tblPostRead
	         WHERE tblPostRead.bpr_bpsID = InnerTable.bpsID) AS bpsLikes,
	     (SELECT  COALESCE(SUM(CASE WHEN tblPostRead.bprOpinion = -1 THEN 1 ELSE 0 END), 0) 
	         FROM tblPostRead
	         WHERE tblPostRead.bpr_bpsID = InnerTable.bpsID) AS bpsDisLikes,
	     (SELECT COALESCE(COUNT(1), 0)
		  		FROM tblComments
		  		WHERE tblComments.cmt_bpsID = InnerTable.bpsID
				  AND tblComments.cmtStatus = 'P') AS bpsPublishedsComments,
	     (SELECT COALESCE(COUNT(1), 0)
		  		FROM tblComments
		  		WHERE tblComments.cmt_bpsID = InnerTable.bpsID
				  AND tblComments.cmtStatus = 'N') AS bpsWaitingComments,
	     IF(ISNULL(InnerTable.bprDate), InnerTable.bpsStatus, 'R') AS bpsStatus
	 FROM (SELECT tblPosts.bpsID,
			        tblPosts.bpsTitle,
			        tblPosts.bpsMarkdown,
			        tblPosts.bpsInsertionDate,
			        tblPosts.bps_catID,
			        tblCategories.catName,
			        tblCategories.catPersianName,
			        tblPosts.bps_ptlCode,
			        tblPosts.bpsShowOnTop,
			        tblPosts.bpsStatus,
			        tblPosts.bpsHasComment,
			        tblPosts.bpsCommentsAllowed,
			        tblPostRead.bprOpinion,
			        tblPostRead.bprDate
			     FROM tblPosts
			         JOIN tblCategories
			             ON tblPosts.bps_catID = tblCategories.catID
			         LEFT JOIN tblPostRead
			         	ON tblPostRead.bpr_bpsID = tblPosts.bpsID AND tblPostRead.bprReadBy_usrID = iUserID
			     WHERE (ISNULL(iPostID) OR tblPosts.bpsID = iPostID)
				    AND (ISNULL(iCategoryID) OR tblPosts.bps_catID = iCategoryID)
                AND (ISNULL(tblPosts.bpsTarget_usrID) OR tblPosts.bpsTarget_usrID  = iUserID)
					 AND tblPosts.bpsStatus NOT IN ('D','R')
			       AND (ISNULL(tblPosts.bpsLanguage) OR tblPosts.bpsLanguage = iPreferedLanguage)
			       AND (ISNULL(iStartDate) OR tblPosts.bpsInsertionDate >= iStartDate)
			       AND (ISNULL(iEndDate) OR tblPosts.bpsInsertionDate <= iEndDate)
			       AND tblPosts.bpsStatus != 'R'
			     ORDER BY
			         tblPosts.bpsShowOnTop DESC,
			         tblPosts.bpsInsertionDate DESC
			     LIMIT iStartFrom, iCount
			) AS InnerTable;
			
END//
DELIMITER ;


-- Dumping database structure for Ticketing
DROP DATABASE IF EXISTS `Ticketing`;
CREATE DATABASE IF NOT EXISTS `Ticketing` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `Ticketing`;

-- Dumping structure for table Ticketing.tblActionLogs
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

-- Dumping data for table Ticketing.tblActionLogs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActionLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActionLogs` ENABLE KEYS */;

-- Dumping structure for table Ticketing.tblTicketRead
DROP TABLE IF EXISTS `tblTicketRead`;
CREATE TABLE IF NOT EXISTS `tblTicketRead` (
  `tkr_tktID` bigint(20) unsigned NOT NULL,
  `tkrBy_usrID` int(10) unsigned NOT NULL,
  `tkrDateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`tkr_tktID`,`tkrBy_usrID`),
  KEY `bcrDateTime` (`tkrDateTime`),
  CONSTRAINT `FK__tblTickets` FOREIGN KEY (`tkr_tktID`) REFERENCES `tblTickets` (`tktid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Ticketing.tblTicketRead: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblTicketRead` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblTicketRead` ENABLE KEYS */;

-- Dumping structure for table Ticketing.tblTickets
DROP TABLE IF EXISTS `tblTickets`;
CREATE TABLE IF NOT EXISTS `tblTickets` (
  `tktID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `tktCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `tktTarget_usrID` int(10) unsigned DEFAULT NULL,
  `tkt_svcID` int(10) unsigned DEFAULT NULL,
  `tktInReply_tktID` bigint(20) unsigned DEFAULT NULL,
  `tktType` char(1) NOT NULL DEFAULT 'M' COMMENT 'B: Bug, C: Critical, S: Suggestion, M: Message, R: Reply, G: Broadcast',
  `tktTitle` varchar(100) NOT NULL,
  `tktBodyMarkdown` text NOT NULL,
  `tktHasAttachment` bit(1) NOT NULL DEFAULT b'0',
  `tktCreatedBy_usrID` int(10) unsigned NOT NULL,
  `tktUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `tktStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, R: Removed',
  PRIMARY KEY (`tktID`),
  KEY `tktTime` (`tktCreationDateTime`),
  KEY `tktType` (`tktType`),
  KEY `tktStatus` (`tktStatus`),
  KEY `tktCreatedBy_usrID` (`tktCreatedBy_usrID`),
  KEY `tktUpdatedBy_usrID` (`tktUpdatedBy_usrID`),
  KEY `tktTarget_usrID` (`tktTarget_usrID`),
  KEY `tkt_svcID` (`tkt_svcID`),
  KEY `FK_tblTickets_tblTickets` (`tktInReply_tktID`),
  KEY `tktHasAttachment` (`tktHasAttachment`),
  CONSTRAINT `FK_tblTickets_tblTickets` FOREIGN KEY (`tktInReply_tktID`) REFERENCES `tblTickets` (`tktid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Ticketing.tblTickets: ~2 rows (approximately)
/*!40000 ALTER TABLE `tblTickets` DISABLE KEYS */;
INSERT INTO `tblTickets` (`tktID`, `tktCreationDateTime`, `tktTarget_usrID`, `tkt_svcID`, `tktInReply_tktID`, `tktType`, `tktTitle`, `tktBodyMarkdown`, `tktHasAttachment`, `tktCreatedBy_usrID`, `tktUpdatedBy_usrID`, `tktStatus`) VALUES
	(1, '2020-01-09 19:09:25', 100, 1, NULL, 'M', '12', 'nmg\r\n---\r\n*this* is a sample _word_ for.', b'0', 1, NULL, 'N'),
	(2, '2020-01-09 15:40:58', NULL, NULL, NULL, 'G', '556', 'fghf', b'0', 2, NULL, 'N'),
	(3, '2020-01-09 15:41:17', NULL, NULL, NULL, 'B', '655', '55775', b'0', 100, NULL, 'N');
/*!40000 ALTER TABLE `tblTickets` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
