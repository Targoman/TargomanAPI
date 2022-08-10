/* Migration File: m20220810_232558_Advert_remove_ex.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountAssetUsage`
    CHANGE COLUMN `usgExRemainingDays` `usgRemainingDays` SMALLINT(5) NOT NULL DEFAULT '0' AFTER `usg_uasID`,
    CHANGE COLUMN `usgExDayShow` `usgDayShow` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgRemainingDays`,
    CHANGE COLUMN `usgExTotalShow` `usgTotalShow` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgDayShow`,
    CHANGE COLUMN `usgExDayClicks` `usgDayClicks` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgTotalShow`,
    CHANGE COLUMN `usgExMonthClicks` `usgMonthClicks` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgDayClicks`,
    CHANGE COLUMN `usgExTotalClicks` `usgTotalClicks` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `usgMonthClicks`;

ALTER TABLE `tblAccountProducts`
    CHANGE COLUMN `prdExType` `prdType` CHAR(1) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `prdUpdatedBy_usrID`,
    CHANGE COLUMN `prdEx_locID` `prd_locID` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `prdType`,
    CHANGE COLUMN `prdExDuration` `prdDuration` INT(10) NULL DEFAULT '-1' AFTER `prd_locID`,
    CHANGE COLUMN `prdExShowPerDay` `prdShowPerDay` INT(10) NULL DEFAULT '-1' AFTER `prdDuration`,
    CHANGE COLUMN `prdExShowTotal` `prdShowTotal` INT(10) NULL DEFAULT '-1' AFTER `prdShowPerDay`,
    CHANGE COLUMN `prdExClicksPerDay` `prdClicksPerDay` INT(10) NULL DEFAULT '-1' AFTER `prdShowTotal`,
    CHANGE COLUMN `prdExClicksPerMonth` `prdClicksPerMonth` INT(10) NULL DEFAULT '-1' AFTER `prdClicksPerDay`,
    CHANGE COLUMN `prdExClicksTotal` `prdClicksTotal` BIGINT(19) NULL DEFAULT '-1' AFTER `prdClicksPerMonth`,
    DROP INDEX `FK_tblAccountProducts_tblLocations`,
    ADD INDEX `FK_tblAccountProducts_tblLocations` (`prd_locID`) USING BTREE;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbExShowPerDay` `slbShowPerDay` INT(10) NOT NULL DEFAULT '0' AFTER `slbUpdatedBy_usrID`,
    CHANGE COLUMN `slbExShowTotal` `slbShowTotal` INT(10) NOT NULL DEFAULT '0' AFTER `slbShowPerDay`,
    CHANGE COLUMN `slbExClicksPerDay` `slbClicksPerDay` INT(10) NOT NULL DEFAULT '0' AFTER `slbShowTotal`,
    CHANGE COLUMN `slbExClicksPerMonth` `slbClicksPerMonth` INT(10) NOT NULL DEFAULT '0' AFTER `slbClicksPerDay`,
    CHANGE COLUMN `slbExClicksTotal` `slbClicksTotal` INT(10) NOT NULL DEFAULT '0' AFTER `slbClicksPerMonth`;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasExDays` `uasDays` MEDIUMINT(7) NULL DEFAULT '0' AFTER `uasUpdatedBy_usrID`,
    CHANGE COLUMN `uasExDayShow` `uasDayShow` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `uasDays`,
    CHANGE COLUMN `uasExTotalShow` `uasTotalShow` BIGINT(20) UNSIGNED NULL DEFAULT '0' AFTER `uasDayShow`,
    CHANGE COLUMN `uasExDayClicks` `uasDayClicks` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `uasTotalShow`,
    CHANGE COLUMN `uasExMonthClicks` `uasMonthClicks` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `uasDayClicks`,
    CHANGE COLUMN `uasExTotalClicks` `uasTotalClicks` BIGINT(20) UNSIGNED NULL DEFAULT '0' AFTER `uasMonthClicks`;
