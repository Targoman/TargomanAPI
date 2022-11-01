/* Migration File: m20221031_172717_Interfaces_Services_create_config_and_i18n.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblConfigurations` (
    `cfgKey` VARCHAR(256) NOT NULL,
    `cfgValue` JSON NOT NULL,
    PRIMARY KEY (`cfgKey`)
)
COLLATE='utf8mb4_general_ci'
;

CREATE TABLE `tblI18N` (
    `i18nKey` VARCHAR(256) NOT NULL,
    `i18nValue` JSON NOT NULL,
    PRIMARY KEY (`i18nKey`)
)
COLLATE='utf8mb4_general_ci'
;
