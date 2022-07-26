/* Migration File: m20220726_101745_Interfaces_Accounting_add_files_to_saleable_and_asset.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblAccountSaleableFiles` (
    `slfID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `slf_slbID` INT(10) UNSIGNED NOT NULL,
    `slfName` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci',
    `slfDesc` VARCHAR(512) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `slfMimeTypes` VARCHAR(256) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `slfExtensions` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `slfMinSize` BIGINT(20) UNSIGNED NULL DEFAULT 0,
    `slfMaxSize` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `slfMinCount` SMALLINT(5) UNSIGNED NULL DEFAULT 0,
    `slfMaxCount` SMALLINT(5) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`slfID`) USING BTREE,
    INDEX `FK_tblAccountSaleableFiles_tblAccountSaleables` (`slf_slbID`) USING BTREE,
    CONSTRAINT `FK_tblAccountSaleableFiles_tblAccountSaleables` FOREIGN KEY (`slf_slbID`) REFERENCES `tblAccountSaleables` (`slbID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `tblAccountUserAssets_files` (
    `uasuflID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `uasufl_uasID` BIGINT(20) UNSIGNED NOT NULL,
    `uasufl_slfID` BIGINT(20) UNSIGNED NOT NULL,
    `uasufl_uflID` BIGINT(20) UNSIGNED NOT NULL,
    `uasuflCreationDateTime` DATETIME NULL DEFAULT CURRENT_TIMESTAMP,
    `uasuflCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`uasuflID`) USING BTREE,
    INDEX `FK_tblAccountUserAssets_files_tblAccountUserAssets` (`uasufl_uasID`) USING BTREE,
    INDEX `FK_tblAccountUserAssets_files_tblAccountSaleableFiles` (`uasufl_slfID`) USING BTREE,
    INDEX `FK_tblAccountUserAssets_files_tblUploadFiles` (`uasufl_uflID`) USING BTREE,
    CONSTRAINT `FK_tblAccountUserAssets_files_tblAccountSaleableFiles` FOREIGN KEY (`uasufl_slfID`) REFERENCES `tblAccountSaleableFiles` (`slfID`) ON UPDATE NO ACTION ON DELETE NO ACTION,
    CONSTRAINT `FK_tblAccountUserAssets_files_tblAccountUserAssets` FOREIGN KEY (`uasufl_uasID`) REFERENCES `tblAccountUserAssets` (`uasID`) ON UPDATE NO ACTION ON DELETE NO ACTION,
    CONSTRAINT `FK_tblAccountUserAssets_files_tblUploadFiles` FOREIGN KEY (`uasufl_uflID`) REFERENCES `tblUploadFiles` (`uflID`) ON UPDATE NO ACTION ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
