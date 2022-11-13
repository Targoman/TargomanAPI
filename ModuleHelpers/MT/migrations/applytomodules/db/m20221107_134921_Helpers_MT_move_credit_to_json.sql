/* Migration File: m20221107_134921_Helpers_MT_move_credit_to_json.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountSaleables`
    DROP COLUMN `slbCreditTotalWords`;





ALTER TABLE `tblAccountProducts`
    ADD COLUMN `prdCreditSpecs` JSON NULL AFTER `prdUpdatedBy_usrID`;

UPDATE tblAccountProducts
    SET prdCreditSpecs = JSON_OBJECT(
        'ALL', prdCreditTotalWords
    )
    WHERE prdCreditTotalWords IS NOT NULL
    ;

ALTER TABLE `tblAccountProducts`
    DROP COLUMN `prdCreditTotalWords`;








ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasCreditSpecs` JSON NULL AFTER `uasUpdatedBy_usrID`;

UPDATE tblAccountUserAssets
    SET uasCreditSpecs = JSON_OBJECT(
        'ALL', uasCreditTotalWords
    )
    WHERE uasCreditTotalWords IS NOT NULL
    ;

ALTER TABLE `tblAccountUserAssets`
    DROP COLUMN `uasCreditTotalWords`;
