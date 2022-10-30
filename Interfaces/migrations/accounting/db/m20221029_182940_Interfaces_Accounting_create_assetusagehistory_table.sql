/* Migration File: m20221029_182940_Interfaces_Accounting_create_assetusagehistory_table.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblAccountAssetUsageHistory` (
    `ush_uasID` BIGINT(20) UNSIGNED NOT NULL,
    `ushLastDateTime` DATETIME NOT NULL,
    `ushUniqueMD5` CHAR(32) AS (md5(concat_ws('X',`ush_uasID`,date_format(`ushLastDateTime`,'%Y-%m-%d %H:%i:00')))) virtual,
    UNIQUE INDEX `ushUniqueMD5` (`ushUniqueMD5`) USING BTREE,
    INDEX `ush_uasID` (`ush_uasID`) USING BTREE,
    CONSTRAINT `FK_tblAccountAssetUsageHistory_tblAccountUserAssets` FOREIGN KEY (`ush_uasID`) REFERENCES `tblAccountUserAssets` (`uasID`) ON UPDATE NO ACTION ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

ALTER TABLE `tblAccountAssetUsageHistory`
    ADD INDEX `ushLastDateTime` (`ushLastDateTime`);

