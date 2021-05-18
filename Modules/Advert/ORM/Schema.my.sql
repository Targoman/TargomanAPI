/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

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
  CONSTRAINT `FK_tblAccountUsage_tblAccountUserPackages` FOREIGN KEY (`usg_aupID`) REFERENCES `tblAccountUserPackages` (`aupid`) ON UPDATE CASCADE
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

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
