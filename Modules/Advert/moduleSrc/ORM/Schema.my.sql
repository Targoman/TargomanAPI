
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

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `Advert` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `Advert`;
DROP TABLE IF EXISTS `tblAccountAssetUsage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAccountAssetUsage` (
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
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblAccountCoupons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAccountCoupons` (
  `cpnID` int unsigned NOT NULL AUTO_INCREMENT,
  `cpnCode` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '0',
  `cpnPrimaryCount` int unsigned NOT NULL DEFAULT '1',
  `cpnTotalMaxAmount` int unsigned NOT NULL DEFAULT '1',
  `cpnPerUserMaxCount` int unsigned DEFAULT NULL,
  `cpnPerUserMaxAmount` int unsigned DEFAULT NULL,
  `cpnValidFrom` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cpnExpiryTime` datetime DEFAULT NULL,
  `cpnAmount` int unsigned NOT NULL,
  `cpnAmountType` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT '%' COMMENT '%: Percent, $:Amount, Z: Free',
  `cpnMaxAmount` int unsigned DEFAULT NULL,
  `cpnSaleableBasedMultiplier` json NOT NULL,
  `cpnTotalUsedCount` int unsigned NOT NULL DEFAULT '0',
  `cpnTotalUsedAmount` int unsigned NOT NULL DEFAULT '0',
  `cpnStatus` char(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `cpnCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cpnCreatedBy_usrID` bigint unsigned NOT NULL,
  `cpnUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`cpnID`) USING BTREE,
  UNIQUE KEY `cpnCode__InvalidatedAt` (`cpnCode`,`_InvalidatedAt`),
  KEY `cpnExpiryTime` (`cpnExpiryTime`) USING BTREE,
  KEY `cpnCreatedBy_usrID` (`cpnCreatedBy_usrID`) USING BTREE,
  KEY `cpnCreationDateTime` (`cpnCreationDateTime`) USING BTREE,
  KEY `cpnUpdatedBy_usrID` (`cpnUpdatedBy_usrID`) USING BTREE,
  KEY `cpnStatus` (`cpnStatus`) USING BTREE,
  KEY `cpnValidFrom` (`cpnValidFrom`) USING BTREE,
  KEY `cpnType` (`cpnAmountType`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblAccountProducts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAccountProducts` (
  `prdID` int unsigned NOT NULL AUTO_INCREMENT,
  `prdCode` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prdName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prdDesc` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prdValidFromDate` date DEFAULT NULL,
  `prdValidToDate` date DEFAULT NULL,
  `prdValidFromHour` tinyint DEFAULT NULL,
  `prdValidToHour` tinyint DEFAULT NULL,
  `prdType` char(1) COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prd_locID` int unsigned DEFAULT NULL,
  `prdDuration` int DEFAULT '-1',
  `prdShowPerDay` int DEFAULT '-1',
  `prdShowTotal` int DEFAULT '-1',
  `prdClicksPerDay` int DEFAULT '-1',
  `prdClicksPerMonth` int DEFAULT '-1',
  `prdClicksTotal` bigint DEFAULT '-1',
  `prdPrivs` json DEFAULT NULL,
  `prdVAT` double DEFAULT NULL,
  `prdInStockQty` double DEFAULT NULL,
  `prdOrderedQty` double DEFAULT NULL,
  `prdReturnedQty` double DEFAULT NULL,
  `prdStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `prdCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `prdCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `prdUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`prdID`) USING BTREE,
  UNIQUE KEY `prdCode__InvalidatedAt` (`prdCode`,`_InvalidatedAt`),
  KEY `prdCreationDateTime` (`prdCreationDateTime`) USING BTREE,
  KEY `prdStatus` (`prdStatus`) USING BTREE,
  KEY `prdValidFrom` (`prdValidFromDate`) USING BTREE,
  KEY `prdValidTo` (`prdValidToDate`) USING BTREE,
  KEY `prdCreatedBy_usrID` (`prdCreatedBy_usrID`) USING BTREE,
  KEY `prdUpdatedBy_usrID` (`prdUpdatedBy_usrID`) USING BTREE,
  KEY `FK_tblAccountProducts_tblLocations` (`prd_locID`),
  KEY `prdValidFromTime` (`prdValidFromHour`) USING BTREE,
  KEY `prdValidToTime` (`prdValidToHour`) USING BTREE,
  CONSTRAINT `FK_tblAccountProducts_tblLocations` FOREIGN KEY (`prd_locID`) REFERENCES `tblLocations` (`locID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblAccountSaleables`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAccountSaleables` (
  `slbID` int unsigned NOT NULL AUTO_INCREMENT,
  `slb_prdID` int unsigned NOT NULL,
  `slbCode` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbDesc` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'N' COMMENT 'N: Normal, S: Special',
  `slbAvailableFromDate` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `slbAvailableToDate` datetime DEFAULT NULL,
  `slbPrivs` json DEFAULT NULL,
  `slbBasePrice` int unsigned NOT NULL,
  `slbAdditives` json DEFAULT NULL,
  `slbProductCount` int DEFAULT NULL COMMENT 'what is this?',
  `slbMaxSaleCountPerUser` int DEFAULT NULL,
  `slbInStockQty` double DEFAULT NULL,
  `slbOrderedQty` double DEFAULT NULL,
  `slbReturnedQty` double DEFAULT NULL,
  `slbVoucherTemplate` text CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbShowPerDay` int NOT NULL DEFAULT '0',
  `slbShowTotal` int NOT NULL DEFAULT '0',
  `slbClicksPerDay` int NOT NULL DEFAULT '0',
  `slbClicksPerMonth` int NOT NULL DEFAULT '0',
  `slbClicksTotal` int NOT NULL DEFAULT '0',
  `slbStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `slbCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `slbCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `slbUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`slbID`) USING BTREE,
  UNIQUE KEY `slbCode__InvalidatedAt` (`slbCode`,`_InvalidatedAt`),
  KEY `slbCreationDateTime` (`slbCreationDateTime`) USING BTREE,
  KEY `slbStatus` (`slbStatus`) USING BTREE,
  KEY `slbCreatedBy_usrID` (`slbCreatedBy_usrID`) USING BTREE,
  KEY `slbUpdatedBy_usrID` (`slbUpdatedBy_usrID`) USING BTREE,
  KEY `slbType` (`slbType`) USING BTREE,
  KEY `slbAvailableToDate` (`slbAvailableToDate`),
  KEY `slbAvailableFromDate` (`slbAvailableFromDate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblAccountUserAssets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblAccountUserAssets` (
  `uasID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `uas_usrID` bigint unsigned NOT NULL,
  `uas_slbID` int unsigned NOT NULL,
  `uasQty` double unsigned NOT NULL,
  `uas_vchID` bigint unsigned DEFAULT NULL,
  `uasVoucherItemUUID` char(50) COLLATE utf8mb4_general_ci NOT NULL,
  `uas_cpnID` int unsigned DEFAULT NULL,
  `uasDiscountAmount` int unsigned DEFAULT NULL,
  `uasPrefered` bit(1) DEFAULT NULL,
  `uasOrderDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `uasDays` mediumint DEFAULT '0',
  `uasDayShow` int unsigned DEFAULT '0',
  `uasTotalShow` bigint unsigned DEFAULT '0',
  `uasDayClicks` int unsigned DEFAULT '0',
  `uasMonthClicks` int unsigned DEFAULT '0',
  `uasTotalClicks` bigint unsigned DEFAULT '0',
  `uasStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'P' COMMENT 'P: Pending, A: Active, R: Removed, B: Blocked',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `uasUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`uasID`) USING BTREE,
  UNIQUE KEY `uas_usrID_uasVoucherItemUUID__InvalidatedAt` (`uas_usrID`,`uasVoucherItemUUID`,`_InvalidatedAt`),
  KEY `uasStatus` (`uasStatus`) USING BTREE,
  KEY `uas_usrID` (`uas_usrID`) USING BTREE,
  KEY `uasUpdatedBy_usrID` (`uasUpdatedBy_usrID`) USING BTREE,
  KEY `uas_invID` (`uas_vchID`) USING BTREE,
  KEY `uasPurchaseDate` (`uasOrderDateTime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblActionLogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblActionLogs` (
  `atlID` bigint NOT NULL AUTO_INCREMENT,
  `atlBy_usrID` bigint unsigned DEFAULT NULL,
  `atlInsertionDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `atlType` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `atlDescription` json DEFAULT NULL,
  PRIMARY KEY (`atlID`),
  KEY `atlType` (`atlType`),
  KEY `atlInsertionDateTime` (`atlInsertionDateTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblActiveAds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblActiveAds` (
  `act_binID` int unsigned NOT NULL,
  `act_locID` int unsigned NOT NULL,
  `actOrder` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `actOnKeyword` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  KEY `FK_tblActiveAds_tblBin` (`act_binID`),
  KEY `FK_tblActiveAds_tblLocations` (`act_locID`),
  KEY `actOrder` (`actOrder`),
  KEY `actOnKeyword` (`actOnKeyword`),
  CONSTRAINT `FK_tblActiveAds_tblBin` FOREIGN KEY (`act_binID`) REFERENCES `tblBin` (`binid`) ON UPDATE CASCADE,
  CONSTRAINT `FK_tblActiveAds_tblLocations` FOREIGN KEY (`act_locID`) REFERENCES `tblLocations` (`locid`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblBanners`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblBanners` (
  `bnr_binID` int unsigned NOT NULL,
  `bnrImage` text CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `bnrSize` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT 'Check c++ code it is too complex',
  `bnrUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  KEY `bnrSize` (`bnrSize`),
  KEY `FK_tblBanners_tblBin` (`bnr_binID`),
  KEY `bnrUpdatedBy_usrID` (`bnrUpdatedBy_usrID`),
  CONSTRAINT `FK_tblBanners_tblBin` FOREIGN KEY (`bnr_binID`) REFERENCES `tblBin` (`binid`) ON DELETE CASCADE ON UPDATE CASCADE
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
/*!50032 DROP TRIGGER IF EXISTS tblBanners_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017*/ /*!50003 TRIGGER `tblBanners_after_update` AFTER UPDATE ON `tblBanners` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";
  
  IF NEW.bnrImage           != OLD.bnrImage           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("bnrImage", OLD.bnrImage)); END IF;
  IF NEW.bnrSize            != OLD.bnrSize            THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("bnrSize", OLD.bnrSize)); END IF;
  
  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.bnrUpdatedBy_usrID,
         tblActionLogs.atlType = "tblBanners-Updated",
         tblActionLogs.atlDescription = Changes;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblBin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblBin` (
  `binID` int unsigned NOT NULL AUTO_INCREMENT,
  `binType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'T' COMMENT 'T: Text, I:Image',
  `binTitle` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `binDesc` varchar(400) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `binPrettyURL` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `binURL` varchar(400) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `binShown` bigint NOT NULL DEFAULT '0',
  `binClicks` bigint NOT NULL DEFAULT '0',
  `binStatus` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'P' COMMENT 'P:Pending, A:Active, B:Blocked, R:Removed',
  `binCreationDateTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `binCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `binUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`binID`),
  KEY `advStatus` (`binStatus`),
  KEY `adbType` (`binType`),
  KEY `adbShown` (`binShown`),
  KEY `adbCreationDateTime` (`binCreationDateTime`),
  KEY `adbUpdatedBy_usrID` (`binUpdatedBy_usrID`),
  KEY `adbClicks` (`binClicks`),
  KEY `binCreatedBy_usrID` (`binCreatedBy_usrID`)
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
/*!50032 DROP TRIGGER IF EXISTS tblBin_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017*/ /*!50003 TRIGGER `tblBin_after_update` AFTER UPDATE ON `tblBin` FOR EACH ROW BEGIN
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
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblClicks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblClicks` (
  `clkID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `clk_binID` int unsigned NOT NULL,
  `clk_locID` int unsigned NOT NULL,
  `clkDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `clkIP` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `clkDevice` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clkScreenSize` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clkOS` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `clkBrowser` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
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
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblLocations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblLocations` (
  `locID` int unsigned NOT NULL AUTO_INCREMENT,
  `locURL` varchar(200) COLLATE utf8mb4_general_ci NOT NULL,
  `locPlaceCode` char(3) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '',
  `locStatus` char(1) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed',
  `_InvalidatedAt` int unsigned NOT NULL DEFAULT '0',
  `locCreationDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `locCreatedBy_usrID` bigint unsigned DEFAULT NULL,
  `locUpdatedBy_usrID` bigint unsigned DEFAULT NULL,
  PRIMARY KEY (`locID`),
  UNIQUE KEY `locURL_locPlaceCode__InvalidatedAt` (`locURL`,`locPlaceCode`,`_InvalidatedAt`),
  KEY `locURL` (`locURL`),
  KEY `locStatus` (`locStatus`),
  KEY `locPlaceCode` (`locPlaceCode`),
  KEY `locCreator_usrID` (`locCreatedBy_usrID`),
  KEY `locCreationDateTime` (`locCreationDateTime`),
  KEY `locUpdater_usrID` (`locUpdatedBy_usrID`)
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
/*!50032 DROP TRIGGER IF EXISTS tblLocations_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017*/ /*!50003 TRIGGER `tblLocations_after_update` AFTER UPDATE ON `tblLocations` FOR EACH ROW BEGIN
  DECLARE Changes  JSON DEFAULT "{}";
  
  IF NEW.locURL          != OLD.locURL         THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("locURL", OLD.locURL)); END IF;
  IF NEW.locPlaceCode    != OLD.locPlaceCode   THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("locPlaceCode", OLD.locPlaceCode)); END IF;
  IF NEW.locStatus       != OLD.locStatus      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("locStatus", OLD.locStatus)); END IF;
  
  INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = NEW.locUpdatedBy_usrID,
         tblActionLogs.atlType = "locStatus-Updated",
         tblActionLogs.atlDescription = Changes;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
DROP TABLE IF EXISTS `tblMigrations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblMigrations` (
  `migName` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `migAppliedAt` datetime NOT NULL,
  PRIMARY KEY (`migName`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `tblProps`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tblProps` (
  `prp_binID` int unsigned NOT NULL,
  `prp_locID` int unsigned NOT NULL,
  `prpOrder` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT 'F: First, S: Second, T: Third, H: HugeReq, R: Normal',
  `prpKeyword` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
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
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS tblProps_after_update */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017*/ /*!50003 TRIGGER `tblProps_after_update` AFTER UPDATE ON `tblProps` FOR EACH ROW BEGIN
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
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spCoupon_DecreaseStats` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE PROCEDURE `spCoupon_DecreaseStats`(
	IN `iDiscountID` INT UNSIGNED,
	IN `iTotalUsedCount` INT UNSIGNED,
	IN `iTotalUsedAmount` INT UNSIGNED
)
BEGIN
    UPDATE tblAccountCoupons
       SET cpnTotalUsedCount = IFNULL(cpnTotalUsedCount, 0) - iTotalUsedCount
         , cpnTotalUsedAmount = IFNULL(cpnTotalUsedAmount, 0) - iTotalUsedAmount
     WHERE cpnID = iDiscountID
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spCoupon_IncreaseStats` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE PROCEDURE `spCoupon_IncreaseStats`(
	IN `iDiscountID` INT UNSIGNED,
	IN `iTotalUsedCount` INT UNSIGNED,
	IN `iTotalUsedAmount` INT UNSIGNED
)
BEGIN
    UPDATE tblAccountCoupons
       SET cpnTotalUsedCount = IFNULL(cpnTotalUsedCount, 0) + iTotalUsedCount
         , cpnTotalUsedAmount = IFNULL(cpnTotalUsedAmount, 0) + iTotalUsedAmount
     WHERE cpnID = iDiscountID
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spSaleable_Reserve` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE PROCEDURE `spSaleable_Reserve`(
	IN `iSaleableID` INT UNSIGNED,
	IN `iUserID` BIGINT UNSIGNED,
	IN `iQty` DOUBLE
)
BEGIN
        UPDATE tblAccountSaleables
           SET slbOrderedQty = IFNULL(slbOrderedQty, 0) + iQty
             , slbUpdatedBy_usrID = iUserID
         WHERE slbID = iSaleableID
    ;

        UPDATE tblAccountProducts
    INNER JOIN tblAccountSaleables
            ON tblAccountSaleables.slb_prdID = tblAccountProducts.prdID
           SET prdOrderedQty = IFNULL(prdOrderedQty, 0) + iQty
             , prdUpdatedBy_usrID = iUserID
         WHERE slbID = iSaleableID
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spSaleable_unReserve` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE PROCEDURE `spSaleable_unReserve`(
	IN `iSaleableID` INT UNSIGNED,
	IN `iUserID` BIGINT UNSIGNED,
	IN `iQty` DOUBLE
)
BEGIN
        UPDATE tblAccountSaleables
           SET slbReturnedQty = IFNULL(slbReturnedQty, 0) + iQty
             , slbUpdatedBy_usrID = iUserID
         WHERE slbID = iSaleableID
    ;

        UPDATE tblAccountProducts
    INNER JOIN tblAccountSaleables
            ON tblAccountSaleables.slb_prdID = tblAccountProducts.prdID
           SET prdReturnedQty = IFNULL(prdReturnedQty, 0) + iQty
             , prdUpdatedBy_usrID = iUserID
         WHERE slbID = iSaleableID
    ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spUserAsset_SetAsPrefered` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE PROCEDURE `spUserAsset_SetAsPrefered`(
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

