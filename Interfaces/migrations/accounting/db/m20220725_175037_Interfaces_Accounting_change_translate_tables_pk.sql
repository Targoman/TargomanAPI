/* Migration File: m20220725_175037_Interfaces_Accounting_change_translate_tables_pk.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountProducts_translate`
    DROP COLUMN `id`,
    DROP INDEX `pid_language`,
    DROP PRIMARY KEY,
    ADD PRIMARY KEY (`pid`, `language`)
;

ALTER TABLE `tblAccountSaleables_translate`
    DROP COLUMN `id`,
    DROP INDEX `pid_language`,
    DROP PRIMARY KEY,
    ADD PRIMARY KEY (`pid`, `language`)
;

ALTER TABLE `tblAccountUnits_translate`
    DROP COLUMN `id`,
    DROP INDEX `pid_language`,
    DROP PRIMARY KEY,
    ADD PRIMARY KEY (`pid`, `language`)
;
