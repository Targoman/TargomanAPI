/* Migration File: m20220711_135003_Advert_change_name_to_json.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblAccountProducts_translate` (
    `id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `pid` INT(10) UNSIGNED NOT NULL,
    `language` VARCHAR(6) NOT NULL COLLATE 'utf8mb4_general_ci',
    `prdName` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci',
    `prdDesc` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`id`) USING BTREE,
    UNIQUE INDEX `pid_language` (`pid`, `language`) USING BTREE,
    INDEX `pid` (`pid`) USING BTREE,
    INDEX `language` (`language`) USING BTREE,
    CONSTRAINT `FK_tblAccountProducts_translate_tblAccountProducts` FOREIGN KEY (`pid`) REFERENCES `tblAccountProducts` (`prdID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `tblAccountSaleables_translate` (
    `id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `pid` INT(10) UNSIGNED NOT NULL,
    `language` VARCHAR(6) NOT NULL COLLATE 'utf8mb4_general_ci',
    `slbName` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci',
    `slbDesc` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`id`) USING BTREE,
    UNIQUE INDEX `pid_language` (`pid`, `language`) USING BTREE,
    INDEX `pid` (`pid`) USING BTREE,
    INDEX `language` (`language`) USING BTREE,
    CONSTRAINT `FK_tblAccountSaleables_translate_tblAccountSaleables` FOREIGN KEY (`pid`) REFERENCES `tblAccountSaleables` (`slbID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
