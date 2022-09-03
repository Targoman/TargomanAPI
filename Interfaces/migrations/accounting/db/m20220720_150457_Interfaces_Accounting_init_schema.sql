/* Migration File: m20220720_150457_Interfaces_Accounting_init_schema.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE TABLE IF NOT EXISTS `tblAccountAssetUsage` (
  `usg_uasID` bigint unsigned NOT NULL,
  PRIMARY KEY (`usg_uasID`) USING BTREE,
  KEY `usg_uasID` (`usg_uasID`) USING BTREE,
  CONSTRAINT `FK_tblAccountAssetUsage_tblAccountUserAssets` FOREIGN KEY (`usg_uasID`) REFERENCES `tblAccountUserAssets` (`uasID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblAccountCoupons` (
  `cpnID` int unsigned NOT NULL AUTO_INCREMENT,
  `cpnCode` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `cpnPrimaryCount` int unsigned NOT NULL,
  `cpnTotalMaxAmount` int unsigned NOT NULL,
  `cpnPerUserMaxCount` int unsigned DEFAULT NULL,
  `cpnPerUserMaxAmount` int unsigned DEFAULT NULL,
  `cpnValidFrom` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cpnExpiryTime` datetime DEFAULT NULL,
  `cpnAmount` int unsigned NOT NULL,
  `cpnAmountType` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '%' COMMENT '%:Percent, $:Amount, Z:Free',
  `cpnMaxAmount` int unsigned DEFAULT NULL,
  `cpnSaleableBasedMultiplier` json NOT NULL,
  `cpnTotalUsedCount` int unsigned NOT NULL DEFAULT '0',
  `cpnTotalUsedAmount` int unsigned NOT NULL DEFAULT '0',
  `cpnStatus` char(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'A' COMMENT 'A:Active, R:Removed',
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

CREATE TABLE IF NOT EXISTS `tblAccountProducts` (
  `prdID` int unsigned NOT NULL AUTO_INCREMENT,
  `prdCode` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prdName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `prdDesc` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `prdValidFromDate` date DEFAULT NULL,
  `prdValidToDate` date DEFAULT NULL,
  `prdValidFromHour` tinyint DEFAULT NULL,
  `prdValidToHour` tinyint DEFAULT NULL,
  `prdPrivs` json DEFAULT NULL,
  `prdVAT` double DEFAULT NULL,
  `prd_untID` smallint unsigned NOT NULL,
  `prdQtyIsDecimal` bit(1) NOT NULL DEFAULT b'0',
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
  KEY `prdValidFromTime` (`prdValidFromHour`) USING BTREE,
  KEY `prdValidToTime` (`prdValidToHour`) USING BTREE,
  KEY `FK_tblAccountProducts_tblAccountUnits` (`prd_untID`),
  CONSTRAINT `FK_tblAccountProducts_tblAccountUnits` FOREIGN KEY (`prd_untID`) REFERENCES `tblAccountUnits` (`untID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblAccountProducts_translate` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `pid` int unsigned NOT NULL,
  `language` varchar(6) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `prdName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `prdDesc` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `pid_language` (`pid`,`language`),
  KEY `pid` (`pid`),
  KEY `language` (`language`),
  CONSTRAINT `FK_tblAccountProducts_translate_tblAccountProducts` FOREIGN KEY (`pid`) REFERENCES `tblAccountProducts` (`prdID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblAccountSaleables` (
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

CREATE TABLE IF NOT EXISTS `tblAccountSaleables_translate` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `pid` int unsigned NOT NULL,
  `language` varchar(6) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `slbDesc` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE KEY `pid_language` (`pid`,`language`) USING BTREE,
  KEY `pid` (`pid`) USING BTREE,
  KEY `language` (`language`) USING BTREE,
  CONSTRAINT `FK_tblAccountSaleables_translate_tblAccountSaleables` FOREIGN KEY (`pid`) REFERENCES `tblAccountSaleables` (`slbID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblAccountUnits` (
  `untID` smallint unsigned NOT NULL DEFAULT '0',
  `untName` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`untID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS `tblAccountUnits_translate` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `pid` smallint unsigned NOT NULL,
  `language` varchar(6) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `untName` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE KEY `pid_language` (`pid`,`language`) USING BTREE,
  KEY `pid` (`pid`) USING BTREE,
  KEY `language` (`language`) USING BTREE,
  CONSTRAINT `FK_tblAccountUnits_translate_tblAccountUnits` FOREIGN KEY (`pid`) REFERENCES `tblAccountUnits` (`untID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

INSERT IGNORE INTO tblAccountUnits(untID, untName)
    VALUES(1, 'Piece')
;

CREATE TABLE IF NOT EXISTS `tblAccountUserAssets` (
  `uasID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `uas_usrID` bigint unsigned NOT NULL,
  `uas_slbID` int unsigned NOT NULL,
  `uasQty` double unsigned NOT NULL,
  `uas_vchID` bigint unsigned DEFAULT NULL,
  `uasVoucherItemUUID` char(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `uasVoucherItemInfo` json DEFAULT NULL,
  `uas_cpnID` int unsigned DEFAULT NULL,
  `uasDiscountAmount` int unsigned DEFAULT NULL,
  `uasPrefered` bit(1) DEFAULT NULL,
  `uasOrderDateTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
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

DROP PROCEDURE IF EXISTS `spCoupon_DecreaseStats`;
DELIMITER //
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
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spCoupon_IncreaseStats`;
DELIMITER //
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
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spSaleable_Reserve`;
DELIMITER //
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
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spSaleable_unReserve`;
DELIMITER //
CREATE PROCEDURE `spSaleable_unReserve`(
    IN `iSaleableID` INT UNSIGNED,
    IN `iUserID` BIGINT UNSIGNED,
    IN `iQty` DOUBLE
)
BEGIN
    UPDATE tblAccountSaleables
       SET slbOrderedQty = IFNULL(slbOrderedQty, 0) - iQty
         , slbUpdatedBy_usrID = iUserID
     WHERE slbID = iSaleableID
    ;

    UPDATE tblAccountProducts
INNER JOIN tblAccountSaleables
        ON tblAccountSaleables.slb_prdID = tblAccountProducts.prdID
       SET prdOrderedQty = IFNULL(prdOrderedQty, 0) - iQty
         , prdUpdatedBy_usrID = iUserID
     WHERE slbID = iSaleableID
    ;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spUserAsset_SetAsPrefered`;
DELIMITER //
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
       AND tblAccountUserAssets.uasID != iUASID
    ;

    UPDATE tblAccountUserAssets
       SET tblAccountUserAssets.uasPrefered = TRUE,
           tblAccountUserAssets.uasUpdatedBy_usrID = iUserID
     WHERE tblAccountUserAssets.uas_usrID = PackageUser
       AND tblAccountUserAssets.uasID = iUASID
    ;

    COMMIT;

END//
DELIMITER ;

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
