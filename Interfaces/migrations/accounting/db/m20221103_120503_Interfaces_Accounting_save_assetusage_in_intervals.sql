/* Migration File: m20221103_120503_Interfaces_Accounting_save_assetusage_in_intervals.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

TRUNCATE tblAccountAssetUsage;

ALTER TABLE `tblAccountAssetUsage`
    ADD COLUMN `usgResolution` CHAR(1) NOT NULL DEFAULT 'T' COMMENT 'T:Total, Y:Year, M:Month, D:Day, H:Hour, I:Minute' AFTER `usg_uasID`,
    ADD COLUMN `usgLastDateTime` DATETIME NOT NULL AFTER `usgResolution`,
    ADD COLUMN `usgKey` VARCHAR(128) NULL AFTER `usgLastDateTime`;

ALTER TABLE `tblAccountAssetUsage`
    ADD COLUMN `usgUniqueMD5` CHAR(32) AS (md5(concat_ws('X',
        `usg_uasID`,
        `usgResolution`,
        CASE `usgResolution`
            WHEN 'Y' THEN DATE_FORMAT(`usgLastDateTime`,'%Y')
            WHEN 'M' THEN DATE_FORMAT(`usgLastDateTime`,'%Y-%m')
            WHEN 'D' THEN DATE_FORMAT(`usgLastDateTime`,'%Y-%m-%d')
            WHEN 'H' THEN DATE_FORMAT(`usgLastDateTime`,'%Y-%m-%d %H')
            WHEN 'I' THEN DATE_FORMAT(`usgLastDateTime`,'%Y-%m-%d %H:%i')
            ELSE 'TOTAL'
        END,
        IFNULL(`usgKey`,'[[NO-KEY]]')
        ))) virtual NOT NULL AFTER `usgKey`;

ALTER TABLE `tblAccountAssetUsage`
    ADD UNIQUE INDEX `usgUniqueMD5` (`usgUniqueMD5`);

ALTER TABLE `tblAccountAssetUsage`
    DROP PRIMARY KEY;

DROP TABLE tblAccountAssetUsageHistory;
