-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               8.0.25 - MySQL Community Server - GPL
-- Server OS:                    Linux
-- HeidiSQL Version:             11.2.0.6213
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- Dumping database structure for Advert
DROP DATABASE IF EXISTS `Advert`;
CREATE DATABASE IF NOT EXISTS `Advert` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `Advert`;

-- Dumping structure for table Advert.tblAccountAssetUsage
DROP TABLE IF EXISTS `tblAccountAssetUsage`;
CREATE TABLE IF NOT EXISTS `tblAccountAssetUsage` (
  `usg_uasID` bigint unsigned NOT NULL,
  `usgRemainingDays` smallint NOT NULL DEFAULT '0',
  `usgDayShow` bigint unsigned NOT NULL DEFAULT '0',
  `usgTotalShow` bigint unsigned NOT NULL DEFAULT '0',
  `usgDayClicks` int unsigned NOT NULL DEFAULT '0',
  `usgMonthClicks` int unsigned NOT NULL DEFAULT '0',
  `usgTotalClicks` bigint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`usg_uasID`) USING BTREE,
  KEY `usg_uasID` (`usg_uasID`) USING BTREE,
  CONSTRAINT `FK_tblAccountAssetUsage_tblAccountUserAssets` FOREIGN KEY (`usg_uasID`) REFERENCES `tblAccountUserAssets` (`uasID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblAccountAssetUsage: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountAssetUsage` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountAssetUsage` ENABLE KEYS */;

-- Dumping structure for table Advert.tblAccountCoupons
DROP TABLE IF EXISTS `tblAccountCoupons`;
CREATE TABLE IF NOT EXISTS `tblAccountCoupons` (
  `cpnID` int unsigned NOT NULL AUTO_INCREMENT,
  `cpnCode` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '0',
  `cpnType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '%' COMMENT '%: Percent, $:Amount, Z: Free',
  `cpnPackageBasedAmount` json NOT NULL,
  `cpnValidFrom` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cpnExpiryTime` datetime DEFAULT NULL,
  `cpnPrimaryCount` int unsigned NOT NULL DEFAULT '1',
  `cpnUsedCount` int unsigned NOT NULL DEFAULT '0',
  `cpnCreatedBy_usrID` bigint unsigned NOT NULL,
  `cpnCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cpnUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `cpnStatus` char(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL COMMENT 'A: Active, Removed',
  PRIMARY KEY (`cpnID`) USING BTREE,
  UNIQUE KEY `cpnCode` (`cpnCode`) USING BTREE,
  KEY `cpnType` (`cpnType`) USING BTREE,
  KEY `cpnExpiryTime` (`cpnExpiryTime`) USING BTREE,
  KEY `cpnCreatedBy_usrID` (`cpnCreatedBy_usrID`) USING BTREE,
  KEY `cpnCreationDateTime` (`cpnCreationDateTime`) USING BTREE,
  KEY `cpnUpdatedBy_usrID` (`cpnUpdatedBy_usrID`) USING BTREE,
  KEY `cpnStatus` (`cpnStatus`) USING BTREE,
  KEY `cpnValidFrom` (`cpnValidFrom`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblAccountCoupons: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountCoupons` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountCoupons` ENABLE KEYS */;

-- Dumping structure for table Advert.tblAccountSaleables
DROP TABLE IF EXISTS `tblAccountSaleables`;
CREATE TABLE IF NOT EXISTS `tblAccountSaleables` (
  `slbID` int unsigned NOT NULL AUTO_INCREMENT,
  `slbCode` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: Normal, S: Special',
  `slbValidFromDate` date DEFAULT NULL,
  `slbValidToDate` date DEFAULT NULL,
  `slbValidFromTime` tinyint DEFAULT NULL,
  `slbValidToTime` tinyint DEFAULT NULL,
  `slb_locID` int unsigned NOT NULL,
  `slbDuration` int NOT NULL DEFAULT '-1',
  `slbShowPerDay` int NOT NULL DEFAULT '-1',
  `slbShowTotal` int NOT NULL DEFAULT '-1',
  `slbClicksPerDay` int NOT NULL DEFAULT '-1',
  `slbClicksPerMonth` int NOT NULL DEFAULT '-1',
  `slbClicksTotal` bigint NOT NULL DEFAULT '-1',
  `slbPrivs` json DEFAULT NULL,
  `slbPrice` int unsigned NOT NULL,
  `slbCanBePurchasedSince` datetime DEFAULT CURRENT_TIMESTAMP,
  `slbNotAvailableSince` datetime DEFAULT NULL,
  `slbInvoiceTemplate` text CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbCreatedBy_usrID` bigint unsigned NOT NULL,
  `slbCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `slbUpdatedBy_usrID` bigint unsigned NOT NULL,
  `slbStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`slbID`) USING BTREE,
  UNIQUE KEY `slbCode` (`slbCode`,`slbStatus`) USING BTREE,
  KEY `slbCreationDateTime` (`slbCreationDateTime`) USING BTREE,
  KEY `slbStatus` (`slbStatus`) USING BTREE,
  KEY `slbValidFrom` (`slbValidFromDate`) USING BTREE,
  KEY `slbValidTo` (`slbValidToDate`) USING BTREE,
  KEY `slbCreatedBy_usrID` (`slbCreatedBy_usrID`) USING BTREE,
  KEY `slbUpdatedBy_usrID` (`slbUpdatedBy_usrID`) USING BTREE,
  KEY `slbValidFromTime` (`slbValidFromTime`) USING BTREE,
  KEY `slbValidToTime` (`slbValidToTime`) USING BTREE,
  KEY `slbType` (`slbType`) USING BTREE,
  KEY `slbCanBePurchasedFrom` (`slbCanBePurchasedSince`) USING BTREE,
  KEY `slbNotAvailableSince` (`slbNotAvailableSince`) USING BTREE,
  KEY `FK_tblAccountSaleables_tblLocations` (`slb_locID`) USING BTREE,
  CONSTRAINT `FK_tblAccountSaleables_tblLocations` FOREIGN KEY (`slb_locID`) REFERENCES `tblLocations` (`locID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblAccountSaleables: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountSaleables` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountSaleables` ENABLE KEYS */;

-- Dumping structure for table Advert.tblAccountUserAssets
DROP TABLE IF EXISTS `tblAccountUserAssets`;
CREATE TABLE IF NOT EXISTS `tblAccountUserAssets` (
  `uasID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `uas_usrID` bigint unsigned NOT NULL,
  `uas_slbID` int unsigned NOT NULL,
  `uasPrefered` bit(1) DEFAULT NULL,
  `uasPurchaseRequestDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `uasPaymentDateTime` datetime DEFAULT NULL,
  `uas_vchID` bigint unsigned NOT NULL,
  `uasUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `uasStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'P' COMMENT 'P: Pending, A: Active, R: Removed, B: Blocked',
  PRIMARY KEY (`uasID`) USING BTREE,
  UNIQUE KEY `uas_usrID_uas_slbID_uasPrefered` (`uas_usrID`,`uas_slbID`,`uasPrefered`,`uasStatus`) USING BTREE,
  KEY `uasStatus` (`uasStatus`) USING BTREE,
  KEY `uas_usrID` (`uas_usrID`) USING BTREE,
  KEY `uasPurchaseDate` (`uasPurchaseRequestDateTime`) USING BTREE,
  KEY `uasPaymentDataTime` (`uasPaymentDateTime`) USING BTREE,
  KEY `uasUpdatedBy_usrID` (`uasUpdatedBy_usrID`) USING BTREE,
  KEY `uas_invID` (`uas_vchID`) USING BTREE,
  KEY `FK_tblAccountUserAssets_tblAccountSaleables` (`uas_slbID`) USING BTREE,
  CONSTRAINT `FK_tblAccountUserAssets_tblAccountSaleables` FOREIGN KEY (`uas_slbID`) REFERENCES `tblAccountSaleables` (`slbID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblAccountUserAssets: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblAccountUserAssets` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblAccountUserAssets` ENABLE KEYS */;

-- Dumping structure for table Advert.tblActionLogs
DROP TABLE IF EXISTS `tblActionLogs`;
CREATE TABLE IF NOT EXISTS `tblActionLogs` (
  `atlID` bigint NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` bigint unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblActionLogs: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActionLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActionLogs` ENABLE KEYS */;

-- Dumping structure for table Advert.tblActiveAds
DROP TABLE IF EXISTS `tblActiveAds`;
CREATE TABLE IF NOT EXISTS `tblActiveAds` (
  `act_binID` int unsigned NOT NULL,
  `act_locID` int unsigned NOT NULL,
  `actOrder` char(1) COLLATE utf8mb4_general_ci NOT NULL,
  `actOnKeyword` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  KEY `FK_tblActiveAds_tblBin` (`act_binID`),
  KEY `FK_tblActiveAds_tblLocations` (`act_locID`),
  KEY `actOrder` (`actOrder`),
  KEY `actOnKeyword` (`actOnKeyword`),
  CONSTRAINT `FK_tblActiveAds_tblBin` FOREIGN KEY (`act_binID`) REFERENCES `tblBin` (`binid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblActiveAds_tblLocations` FOREIGN KEY (`act_locID`) REFERENCES `tblLocations` (`locid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblActiveAds: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblActiveAds` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblActiveAds` ENABLE KEYS */;

-- Dumping structure for table Advert.tblBanners
DROP TABLE IF EXISTS `tblBanners`;
CREATE TABLE IF NOT EXISTS `tblBanners` (
  `bnr_binID` int unsigned NOT NULL,
  `bnrImage` text COLLATE utf8mb4_general_ci NOT NULL,
  `bnrSize` char(1) COLLATE utf8mb4_general_ci NOT NULL COMMENT 'Check c++ code it is too complex',
  `bnrUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  KEY `bnrSize` (`bnrSize`),
  KEY `FK_tblBanners_tblBin` (`bnr_binID`),
  KEY `bnrUpdatedBy_usrID` (`bnrUpdatedBy_usrID`),
  CONSTRAINT `FK_tblBanners_tblBin` FOREIGN KEY (`bnr_binID`) REFERENCES `tblBin` (`binid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblBanners: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblBanners` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblBanners` ENABLE KEYS */;

-- Dumping structure for table Advert.tblBin
DROP TABLE IF EXISTS `tblBin`;
CREATE TABLE IF NOT EXISTS `tblBin` (
  `binID` int unsigned NOT NULL AUTO_INCREMENT,
  `binType` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'T' COMMENT 'T: Text, I:Image',
  `binTitle` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `binDesc` varchar(400) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `binPrettyURL` varchar(100) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `binURL` varchar(400) COLLATE utf8mb4_general_ci NOT NULL,
  `binShown` bigint NOT NULL DEFAULT '0',
  `binClicks` bigint NOT NULL DEFAULT '0',
  `binCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `binCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `binUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `binStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'P' COMMENT 'P:Pending, A:Active, B:Blocked, R:Removed',
  PRIMARY KEY (`binID`),
  KEY `advStatus` (`binStatus`),
  KEY `adbType` (`binType`),
  KEY `adbShown` (`binShown`),
  KEY `adbCreationDateTime` (`binCreationDateTime`),
  KEY `adbUpdatedBy_usrID` (`binUpdatedBy_usrID`),
  KEY `adbClicks` (`binClicks`),
  KEY `binCreatedBy_usrID` (`binCreatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblBin: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblBin` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblBin` ENABLE KEYS */;

-- Dumping structure for table Advert.tblClicks
DROP TABLE IF EXISTS `tblClicks`;
CREATE TABLE IF NOT EXISTS `tblClicks` (
  `clkID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `clk_binID` int unsigned NOT NULL,
  `clk_locID` int unsigned NOT NULL,
  `clkDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `clkIP` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `clkDevice` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clkScreenSize` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clkOS` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clkBrowser` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`clkID`),
  KEY `clkDevice` (`clkDevice`),
  KEY `clkScreenSize` (`clkScreenSize`),
  KEY `clkOS` (`clkOS`),
  KEY `clkBrowser` (`clkBrowser`),
  KEY `clkIP` (`clkIP`),
  KEY `FK_tblClicks_tblBin` (`clk_binID`),
  KEY `FK_tblClicks_tblLocations` (`clk_locID`),
  KEY `clkDateTime` (`clkDateTime`),
  CONSTRAINT `FK_tblClicks_tblBin` FOREIGN KEY (`clk_binID`) REFERENCES `tblBin` (`binID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblClicks_tblLocations` FOREIGN KEY (`clk_locID`) REFERENCES `tblLocations` (`locid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblClicks: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblClicks` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblClicks` ENABLE KEYS */;

-- Dumping structure for table Advert.tblLocations
DROP TABLE IF EXISTS `tblLocations`;
CREATE TABLE IF NOT EXISTS `tblLocations` (
  `locID` int unsigned NOT NULL AUTO_INCREMENT,
  `locURL` varchar(200) COLLATE utf8mb4_general_ci NOT NULL,
  `locPlaceCode` char(3) COLLATE utf8mb4_general_ci NOT NULL,
  `locCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `locCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `locUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  `locStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  PRIMARY KEY (`locID`),
  KEY `locURL` (`locURL`),
  KEY `locStatus` (`locStatus`),
  KEY `locPlaceCode` (`locPlaceCode`),
  KEY `locCreator_usrID` (`locCreatedBy_usrID`),
  KEY `locCreationDateTime` (`locCreationDateTime`),
  KEY `locUpdater_usrID` (`locUpdatedBy_usrID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblLocations: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblLocations` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblLocations` ENABLE KEYS */;

-- Dumping structure for table Advert.tblProps
DROP TABLE IF EXISTS `tblProps`;
CREATE TABLE IF NOT EXISTS `tblProps` (
  `prp_binID` int unsigned NOT NULL,
  `prp_locID` int unsigned NOT NULL,
  `prpOrder` char(1) COLLATE utf8mb4_general_ci NOT NULL COMMENT 'F: First, S: Second, T: Third, H: HugeReq, R: Normal',
  `prpKeyword` varchar(50) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prpStartDate` date NOT NULL,
  `prpEndDate` date NOT NULL,
  `prpCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `prpCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `prpUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
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
  CONSTRAINT `FK_tblProps_tblBin` FOREIGN KEY (`prp_binID`) REFERENCES `tblBin` (`binID`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblProps_tblLocations` FOREIGN KEY (`prp_locID`) REFERENCES `tblLocations` (`locID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- Dumping data for table Advert.tblProps: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblProps` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblProps` ENABLE KEYS */;

-- Dumping structure for procedure Advert.sp_UPDATE_setAsPrefered
DROP PROCEDURE IF EXISTS `sp_UPDATE_setAsPrefered`;
DELIMITER //
CREATE PROCEDURE `sp_UPDATE_setAsPrefered`(
	IN `iUserID` BIGINT UNSIGNED,
	IN `iUASID` BIGINT UNSIGNED
)
BEGIN
  DECLARE PackageUser BIGINT UNSIGNED;

  SELECT tblAccountUserAssets.uas_usrID INTO PackageUser
    FROM tblAccountUserAssets
   WHERE tblAccountUserAssets.uasID = iAUPID;

  IF PackageUser IS NULL THEN
		SIGNAL SQLSTATE '45403'
	      SET MESSAGE_TEXT = "403:UserPackage Not Found";
  END IF;

  START TRANSACTION;
    UPDATE tblAccountUserAssets
       SET tblAccountUserAssets.uasPrefered = NULL,
           tblAccountUserAssets.uasUpdatedBy_usrID = iUserID
     WHERE tblAccountUserAssets.uas_usrID = PackageUser
       AND tblAccountUserAssets.uasID != iUASID;

    UPDATE tblAccountUserAssets
       SET tblAccountUserAssets.uasPrefered = TRUE,
           tblAccountUserAssets.uasUpdatedBy_usrID = iUserID
     WHERE tblAccountUserAssets.uas_usrID = PackageUser
       AND tblAccountUserAssets.uasID = iUASID;
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
