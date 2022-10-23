/* Migration File: m20221018_160643_Interfaces_Accounting_add_duration_and_life_dates.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountSaleables`
    ADD COLUMN `slbDurationDays` INT UNSIGNED NULL DEFAULT NULL AFTER `slbVoucherTemplate`,
    ADD COLUMN `slbStartAtFirstUse` BIT NOT NULL DEFAULT 0 AFTER `slbDurationDays`;

ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasBeginAt` DATETIME NULL DEFAULT NULL AFTER `uasPrefered`,
    ADD COLUMN `uasEndAt` DATETIME NULL DEFAULT NULL AFTER `uasBeginAt`,
    ADD COLUMN `uasSplittedFrom_uasID` BIGINT UNSIGNED NULL DEFAULT NULL AFTER `uasEndAt`;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasSplittedFrom_uasID` `uasSplittedFrom_uasID` BIGINT(20) UNSIGNED NULL DEFAULT NULL AFTER `uasPrefered`,
    ADD COLUMN `uasDurationDays` INT UNSIGNED NULL DEFAULT NULL AFTER `uasSplittedFrom_uasID`;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasOrderDateTime` `uasCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP AFTER `_InvalidatedAt`,
    DROP INDEX `uasPurchaseDate`,
    ADD INDEX `uasPurchaseDate` (`uasCreationDateTime`) USING BTREE;

ALTER TABLE `tblAccountSaleables`
    ADD COLUMN `slbValidFromHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `slbDurationDays`,
    ADD COLUMN `slbValidToHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `slbValidFromHour`;

ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasValidFromDate` DATE NULL DEFAULT NULL AFTER `uasDurationDays`,
    ADD COLUMN `uasValidToDate` DATE NULL DEFAULT NULL AFTER `uasValidFromDate`,
    ADD COLUMN `uasValidFromHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `uasValidToDate`,
    ADD COLUMN `uasValidToHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `uasValidFromHour`;

ALTER TABLE `tblAccountUserAssets`
    DROP COLUMN `uasBeginAt`,
    DROP COLUMN `uasEndAt`;

ALTER TABLE `tblAccountProducts`
    DROP COLUMN `prdValidFromDate`,
    DROP COLUMN `prdValidToDate`;

ALTER TABLE `tblAccountProducts`
    CHANGE COLUMN `prdValidFromHour` `prdCanUseFromHour` TINYINT(3) NULL DEFAULT NULL AFTER `prdDesc`,
    CHANGE COLUMN `prdValidToHour` `prdCanUseToHour` TINYINT(3) NULL DEFAULT NULL AFTER `prdCanUseFromHour`,
    DROP INDEX `prdValidFromTime`,
    ADD INDEX `prdCanUseFromHour` (`prdCanUseFromHour`) USING BTREE,
    DROP INDEX `prdValidToTime`,
    ADD INDEX `prdCanUseToHour` (`prdCanUseToHour`) USING BTREE;

ALTER TABLE `tblAccountProducts`
    DROP COLUMN `prdCanUseFromHour`,
    DROP COLUMN `prdCanUseToHour`;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbValidFromHour` `slbCanUseFromHour` TINYINT(3) UNSIGNED NULL DEFAULT NULL AFTER `slbDurationDays`,
    CHANGE COLUMN `slbValidToHour` `slbCanUseToHour` TINYINT(3) UNSIGNED NULL DEFAULT NULL AFTER `slbCanUseFromHour`;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasValidFromDate` `uasCanUseFromDate` DATE NULL DEFAULT NULL AFTER `uasDurationDays`,
    CHANGE COLUMN `uasValidToDate` `uasCanUseToDate` DATE NULL DEFAULT NULL AFTER `uasCanUseFromDate`,
    CHANGE COLUMN `uasValidFromHour` `uasCanUseFromHour` TINYINT(3) UNSIGNED NULL DEFAULT NULL AFTER `uasCanUseToDate`,
    CHANGE COLUMN `uasValidToHour` `uasCanUseToHour` TINYINT(3) UNSIGNED NULL DEFAULT NULL AFTER `uasCanUseFromHour`;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbAvailableFromDate` `slbSaleFromDate` DATETIME NULL AFTER `slbType`,
    CHANGE COLUMN `slbAvailableToDate` `slbSaleFromDate` DATETIME NULL DEFAULT NULL AFTER `slbSaleFromDate`,
    DROP INDEX `slbAvailableToDate`,
    ADD INDEX `slbSaleToDate` (`slbSaleToDate`) USING BTREE,
    DROP INDEX `slbAvailableFromDate`,
    ADD INDEX `slbSaleFromDate` (`slbSaleFromDate`) USING BTREE;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasPrefered` `uasPrefered` BIT(1) NOT NULL DEFAULT 0 AFTER `uasDiscountAmount`;
