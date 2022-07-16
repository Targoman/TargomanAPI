/* Migration File: m20220716_103214_Advert_add_product_unit.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountProducts`
    CHANGE COLUMN `prdName` `prdName` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `prdCode`;

CREATE TABLE `tblAccountUnits` (
    `untID` SMALLINT(5) UNSIGNED NOT NULL AUTO_INCREMENT,
    `untName` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`untID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

INSERT IGNORE INTO tblAccountUnits(untID, untName)
    VALUES(1, 'Piece')
;

ALTER TABLE `tblAccountProducts`
    ADD COLUMN `prd_untID` SMALLINT UNSIGNED NULL DEFAULT NULL AFTER `prdVAT`
;

UPDATE tblAccountProducts
    SET prd_untID = 1
    WHERE prd_untID IS NULL
;

ALTER TABLE `tblAccountProducts`
    CHANGE COLUMN `prd_untID` `prd_untID` SMALLINT(5) UNSIGNED NOT NULL AFTER `prdVAT`;

ALTER TABLE `tblAccountProducts`
    ADD CONSTRAINT `FK_tblAccountProducts_tblAccountUnits` FOREIGN KEY (`prd_untID`) REFERENCES `tblAccountUnits` (`untID`) ON UPDATE NO ACTION ON DELETE NO ACTION
;

CREATE TABLE `tblAccountUnits_translate` (
    `id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `pid` SMALLINT(5) UNSIGNED NOT NULL,
    `language` VARCHAR(6) NOT NULL COLLATE 'utf8mb4_general_ci',
    `untName` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`id`) USING BTREE,
    UNIQUE INDEX `pid_language` (`pid`, `language`) USING BTREE,
    INDEX `pid` (`pid`) USING BTREE,
    INDEX `language` (`language`) USING BTREE,
    CONSTRAINT `FK_tblAccountUnits_translate_tblAccountUnits` FOREIGN KEY (`pid`) REFERENCES `tblAccountUnits` (`untID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
