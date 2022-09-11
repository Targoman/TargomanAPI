/* Migration File: m20220911_121311_AAA_create_gtw_type_i18n.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblPaymentGatewayTypes`
    ADD COLUMN `pgtID` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT FIRST,
    DROP PRIMARY KEY,
    ADD UNIQUE INDEX `pgtType` (`pgtType`),
    ADD PRIMARY KEY (`pgtID`);

CREATE TABLE `tblPaymentGatewayTypesI18N` (
    `pid` SMALLINT(5) UNSIGNED NOT NULL,
    `language` VARCHAR(6) NOT NULL COLLATE 'utf8mb4_general_ci',
    `pgtNameI18N` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`pid`, `language`) USING BTREE,
    INDEX `pid` (`pid`) USING BTREE,
    INDEX `language` (`language`) USING BTREE,
    CONSTRAINT `FK_tblPaymentGatewayTypesI18N_tblPaymentGatewayTypes` FOREIGN KEY (`pid`) REFERENCES `tblPaymentGatewayTypes` (`pgtID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'تست دولوپر'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = '-';

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'رمز ارز'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = 'B';

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'کردیت کارت بین المللی'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = 'C';

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'دبیت کارت بین المللی'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = 'D';

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'بانک ایران'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = 'I';

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'درگاه واسطه ایران'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = 'M';

INSERT IGNORE INTO tblPaymentGatewayTypesI18N(pid, language, pgtNameI18N)
    SELECT pgtID, 'fa', 'پرداخت در محل'
    FROM tblPaymentGatewayTypes
    WHERE pgtType = 'O';
