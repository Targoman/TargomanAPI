/* Migration File: m20221018_160643_Interfaces_Accounting_add_duration_and_life_dates.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountSaleables`
    ADD COLUMN `slbDurationDays` INT UNSIGNED NULL DEFAULT NULL AFTER `slbVoucherTemplate`,
    ADD COLUMN `slbStartAtFirstUse` BIT NULL DEFAULT NULL AFTER `slbDurationDays`;

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

