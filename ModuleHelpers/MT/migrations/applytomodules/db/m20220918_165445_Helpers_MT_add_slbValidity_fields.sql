/* Migration File: m20220918_165445_Helpers_MT_add_slbValidity_fields.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountSaleables`
    ADD COLUMN `slbValidityDurationInDays` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `slbUpdatedBy_usrID`,
    ADD COLUMN `slbValidityWords` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `slbValidityDurationInDays`;
