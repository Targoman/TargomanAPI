/* Migration File: m20220823_122914_Interfaces_Accounting_add_tokenbase_to_product.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

/*
ALTER TABLE `tblAccountProducts`
    ADD COLUMN `prdJustForAPIToken` BIT(1) NOT NULL DEFAULT 0 AFTER `prdQtyIsDecimal`;

ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasRelatedAPITokenID` BIGINT UNSIGNED NULL AFTER `uasOrderDateTime`;
*/
