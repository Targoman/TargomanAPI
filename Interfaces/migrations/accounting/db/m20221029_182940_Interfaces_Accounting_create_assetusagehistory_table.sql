/* Migration File: m20221029_182940_Interfaces_Accounting_create_assetusagehistory_table.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblAccountAssetUsageHistory` (
    `ush_uasID` BIGINT(20) UNSIGNED NOT NULL,
    `ushLastDateTime` DATETIME NOT NULL,
    PRIMARY KEY (`ush_uasID`) USING BTREE,
    CONSTRAINT `FK_tblAccountAssetUsageHistory_tblAccountUserAssets` FOREIGN KEY (`ush_uasID`) REFERENCES `tblAccountUserAssets` (`uasID`) ON UPDATE NO ACTION ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
