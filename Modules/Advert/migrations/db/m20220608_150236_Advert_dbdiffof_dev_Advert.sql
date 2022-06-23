/* Migration File: m20220608_150236_Advert_dbdiffof_dev_Advert.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountCoupons`
    CHANGE COLUMN `cpnCode` `cpnCode` VARCHAR(32) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `cpnID`,
    CHANGE COLUMN `cpnPrimaryCount` `cpnPrimaryCount` INT(10) UNSIGNED NOT NULL AFTER `cpnCode`,
    CHANGE COLUMN `cpnTotalMaxAmount` `cpnTotalMaxAmount` INT(10) UNSIGNED NOT NULL AFTER `cpnPrimaryCount`
;

ALTER TABLE `tblAccountCoupons`
    CHANGE COLUMN `cpnAmountType` `cpnAmountType` CHAR(1) NOT NULL DEFAULT '%' COMMENT '%:Percent, $:Amount, Z:Free' COLLATE 'utf8mb4_general_ci' AFTER `cpnAmount`,
    CHANGE COLUMN `cpnStatus` `cpnStatus` CHAR(50) NOT NULL DEFAULT 'A' COMMENT 'A:Active, R:Removed' COLLATE 'utf8mb4_general_ci' AFTER `cpnTotalUsedAmount`
;

ALTER TABLE `tblAccountProducts`
    CHANGE COLUMN `prdPrivs` `prdPrivs` JSON NULL DEFAULT NULL AFTER `prdValidToHour`,
    CHANGE COLUMN `prdVAT` `prdVAT` DOUBLE NULL DEFAULT NULL AFTER `prdPrivs`,
    CHANGE COLUMN `prdInStockQty` `prdInStockQty` DOUBLE NULL DEFAULT NULL AFTER `prdVAT`,
    CHANGE COLUMN `prdOrderedQty` `prdOrderedQty` DOUBLE NULL DEFAULT NULL AFTER `prdInStockQty`,
    CHANGE COLUMN `prdReturnedQty` `prdReturnedQty` DOUBLE NULL DEFAULT NULL AFTER `prdOrderedQty`,
    CHANGE COLUMN `prdType` `prdExType` CHAR(1) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `prdReturnedQty`,
    CHANGE COLUMN `prd_locID` `prdEx_locID` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `prdExType`,
    CHANGE COLUMN `prdDuration` `prdExDuration` INT(10) NULL DEFAULT '-1' AFTER `prdEx_locID`,
    CHANGE COLUMN `prdShowPerDay` `prdExShowPerDay` INT(10) NULL DEFAULT '-1' AFTER `prdExDuration`,
    CHANGE COLUMN `prdShowTotal` `prdExShowTotal` INT(10) NULL DEFAULT '-1' AFTER `prdExShowPerDay`,
    CHANGE COLUMN `prdClicksPerDay` `prdExClicksPerDay` INT(10) NULL DEFAULT '-1' AFTER `prdExShowTotal`,
    CHANGE COLUMN `prdClicksPerMonth` `prdExClicksPerMonth` INT(10) NULL DEFAULT '-1' AFTER `prdExClicksPerDay`,
    CHANGE COLUMN `prdClicksTotal` `prdExClicksTotal` BIGINT(19) NULL DEFAULT '-1' AFTER `prdExClicksPerMonth`,
    DROP INDEX `FK_tblAccountProducts_tblLocations`,
    ADD INDEX `FK_tblAccountProducts_tblLocations` (`prdEx_locID`) USING BTREE
;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbShowPerDay` `slbExShowPerDay` INT(10) NOT NULL DEFAULT '0' AFTER `slbVoucherTemplate`,
    CHANGE COLUMN `slbShowTotal` `slbExShowTotal` INT(10) NOT NULL DEFAULT '0' AFTER `slbExShowPerDay`,
    CHANGE COLUMN `slbClicksPerDay` `slbExClicksPerDay` INT(10) NOT NULL DEFAULT '0' AFTER `slbExShowTotal`,
    CHANGE COLUMN `slbClicksPerMonth` `slbExClicksPerMonth` INT(10) NOT NULL DEFAULT '0' AFTER `slbExClicksPerDay`,
    CHANGE COLUMN `slbClicksTotal` `slbExClicksTotal` INT(10) NOT NULL DEFAULT '0' AFTER `slbExClicksPerMonth`
;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasDays` `uasExDays` MEDIUMINT(7) NULL DEFAULT '0' AFTER `uasOrderDateTime`,
    CHANGE COLUMN `uasDayShow` `uasExDayShow` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `uasExDays`,
    CHANGE COLUMN `uasTotalShow` `uasExTotalShow` BIGINT(20) UNSIGNED NULL DEFAULT '0' AFTER `uasExDayShow`,
    CHANGE COLUMN `uasDayClicks` `uasExDayClicks` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `uasExTotalShow`,
    CHANGE COLUMN `uasMonthClicks` `uasExMonthClicks` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `uasExDayClicks`,
    CHANGE COLUMN `uasTotalClicks` `uasExTotalClicks` BIGINT(20) UNSIGNED NULL DEFAULT '0' AFTER `uasExMonthClicks`
;

ALTER TABLE `tblAccountAssetUsage`
    CHANGE COLUMN `usgRemainingDays` `usgExRemainingDays` SMALLINT(5) NOT NULL DEFAULT '0' AFTER `usg_uasID`,
    CHANGE COLUMN `usgDayShow` `usgExDayShow` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgExRemainingDays`,
    CHANGE COLUMN `usgTotalShow` `usgExTotalShow` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgExDayShow`,
    CHANGE COLUMN `usgDayClicks` `usgExDayClicks` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgExTotalShow`,
    CHANGE COLUMN `usgMonthClicks` `usgExMonthClicks` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgExDayClicks`,
    CHANGE COLUMN `usgTotalClicks` `usgExTotalClicks` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgExMonthClicks`
;

ALTER TABLE `tblAccountProducts`
	ADD COLUMN `prdQtyIsDecimal` BIT NOT NULL DEFAULT 0 AFTER `prdVAT`
;

ALTER TABLE `tblAccountUserAssets`
	ADD COLUMN `uasVoucherItemInfo` JSON NULL AFTER `uasVoucherItemUUID`
;

DROP PROCEDURE IF EXISTS `spSaleable_unReserve`;
DELIMITER ;;
CREATE PROCEDURE `spSaleable_unReserve`(
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
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spSaleable_unReserve`;
DELIMITER ;;
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
END;;
DELIMITER ;

ALTER TABLE `tblAccountProducts`
	CHANGE COLUMN `prdStatus` `prdStatus` CHAR(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed' COLLATE 'utf8mb4_general_ci' AFTER `prdReturnedQty`,
	CHANGE COLUMN `_InvalidatedAt` `_InvalidatedAt` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `prdStatus`,
	CHANGE COLUMN `prdCreationDateTime` `prdCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP AFTER `_InvalidatedAt`,
	CHANGE COLUMN `prdCreatedBy_usrID` `prdCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL AFTER `prdCreationDateTime`,
	CHANGE COLUMN `prdUpdatedBy_usrID` `prdUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL AFTER `prdCreatedBy_usrID`
;

ALTER TABLE `tblAccountSaleables`
	CHANGE COLUMN `slbStatus` `slbStatus` CHAR(1) NOT NULL DEFAULT 'A' COMMENT 'A: Active, R: Removed' COLLATE 'utf8mb4_general_ci' AFTER `slbVoucherTemplate`,
	CHANGE COLUMN `_InvalidatedAt` `_InvalidatedAt` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `slbStatus`,
	CHANGE COLUMN `slbCreationDateTime` `slbCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP AFTER `_InvalidatedAt`,
	CHANGE COLUMN `slbCreatedBy_usrID` `slbCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL AFTER `slbCreationDateTime`,
	CHANGE COLUMN `slbUpdatedBy_usrID` `slbUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL AFTER `slbCreatedBy_usrID`
;

ALTER TABLE `tblAccountUserAssets`
	CHANGE COLUMN `uasStatus` `uasStatus` CHAR(1) NOT NULL DEFAULT 'P' COMMENT 'P: Pending, A: Active, R: Removed, B: Blocked' COLLATE 'utf8mb4_general_ci' AFTER `uasOrderDateTime`,
	CHANGE COLUMN `_InvalidatedAt` `_InvalidatedAt` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `uasStatus`,
	CHANGE COLUMN `uasUpdatedBy_usrID` `uasUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL AFTER `_InvalidatedAt`
;
