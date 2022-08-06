/* Migration File: m20220806_203532_Interfaces_Accounting_suffix_i18n_fields.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountUnitsI18N`
    CHANGE COLUMN `untName` `untNameI18N` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `language`;

ALTER TABLE `tblAccountProductsI18N`
    CHANGE COLUMN `prdName` `prdNameI18N` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `language`,
    CHANGE COLUMN `prdDesc` `prdDescI18N` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `prdNameI18N`;

ALTER TABLE `tblAccountSaleablesI18N`
    CHANGE COLUMN `slbName` `slbNameI18N` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `language`,
    CHANGE COLUMN `slbDesc` `slbDescI18N` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `slbNameI18N`;
