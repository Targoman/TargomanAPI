/* Migration File: m20221102_155456_Interfaces_Accounting_remove_slb_valid_fields.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountProducts`
    ADD COLUMN `prdStartAtFirstUse` BIT NOT NULL DEFAULT 0 AFTER `prdDurationMinutes`;

ALTER TABLE `tblAccountSaleables`
    DROP COLUMN `slbValidFromDate`,
    DROP COLUMN `slbValidToDate`,
    DROP COLUMN `slbValidFromHour`,
    DROP COLUMN `slbValidToHour`,
    DROP COLUMN `slbDurationMinutes`,
    DROP COLUMN `slbStartAtFirstUse`;
