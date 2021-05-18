/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

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
  `tktBase_tktID` bigint(20) unsigned DEFAULT NULL,
  `tktType` char(1) NOT NULL DEFAULT 'M' COMMENT 'B: Bug, C: Critical, S: Suggestion, M: Message, R: Reply, G: Broadcast',
  `tktScore` tinyint(3) unsigned DEFAULT NULL,
  `tktTitle` varchar(100) NOT NULL,
  `tktBodyMarkdown` text NOT NULL,
  `tktAttachments` json DEFAULT NULL,
  `tktCreatedBy_usrID` int(10) unsigned NOT NULL,
  `tktUpdatedBy_usrID` int(10) unsigned DEFAULT NULL,
  `tktStatus` char(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, R: Removed, C: Closed',
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
  CONSTRAINT `FK_tblTickets_tblTickets` FOREIGN KEY (`tktInReply_tktID`) REFERENCES `tblTickets` (`tktid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tblTickets_tblTickets_2` FOREIGN KEY (`tktBase_tktID`) REFERENCES `tblTickets` (`tktid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table Ticketing.tblTickets: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblTickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblTickets` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
