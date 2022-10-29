/* Migration File: m20221018_160643_Interfaces_Accounting_add_duration_and_life_dates.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountProducts`
    CHANGE COLUMN `prdValidFromDate` `prdValidFromDate` DATETIME NULL DEFAULT NULL AFTER `prdDesc`,
    CHANGE COLUMN `prdValidToDate` `prdValidToDate` DATETIME NULL DEFAULT NULL AFTER `prdValidFromDate`,
    CHANGE COLUMN `prdValidFromHour` `prdValidFromHour` TINYINT(3) UNSIGNED NULL DEFAULT NULL AFTER `prdValidToDate`,
    CHANGE COLUMN `prdValidToHour` `prdValidToHour` TINYINT(3) UNSIGNED NULL DEFAULT NULL AFTER `prdValidFromHour`,
    ADD COLUMN `prdDurationMinutes` MEDIUMINT UNSIGNED NULL DEFAULT NULL AFTER `prdValidToHour`;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbAvailableFromDate` `slbAvailableFromDate` DATETIME NULL AFTER `slbType`,
    ADD COLUMN `slbValidFromDate` DATETIME NULL DEFAULT NULL AFTER `slbVoucherTemplate`,
    ADD COLUMN `slbValidToDate` DATETIME NULL DEFAULT NULL AFTER `slbValidFromDate`,
    ADD COLUMN `slbValidFromHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `slbValidToDate`,
    ADD COLUMN `slbValidToHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `slbValidFromHour`,
    ADD COLUMN `slbDurationMinutes` MEDIUMINT UNSIGNED NULL DEFAULT NULL AFTER `slbValidToHour`,
    ADD COLUMN `slbStartAtFirstUse` BIT NOT NULL DEFAULT 0 AFTER `slbDurationMinutes`;

ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasValidFromDate` DATETIME NULL DEFAULT NULL AFTER `uasPrefered`,
    ADD COLUMN `uasValidToDate` DATETIME NULL DEFAULT NULL AFTER `uasValidFromDate`,
    ADD COLUMN `uasValidFromHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `uasValidToDate`,
    ADD COLUMN `uasValidToHour` TINYINT UNSIGNED NULL DEFAULT NULL AFTER `uasValidFromHour`,
    ADD COLUMN `uasDurationMinutes` MEDIUMINT UNSIGNED NULL DEFAULT NULL AFTER `uasValidToHour`,
    ADD COLUMN `uasBreakedAt` DATETIME NULL DEFAULT NULL AFTER `uasDurationMinutes`,
    CHANGE COLUMN `uasOrderDateTime` `uasCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP AFTER `_InvalidatedAt`,
    DROP INDEX `uasPurchaseDate`,
    ADD INDEX `uasCreationDateTime` (`uasCreationDateTime`) USING BTREE;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasPrefered` `uasPrefered` BIT(1) NOT NULL DEFAULT 0 AFTER `uasDiscountAmount`;
