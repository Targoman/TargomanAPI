/* Migration File: m20221029_185258_Helpers_MT_create_assetusagehistory_table.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountAssetUsageHistory`
    ADD COLUMN `ushSumUsedTotalWords` INT UNSIGNED NOT NULL AFTER `ushUniqueMD5`;
