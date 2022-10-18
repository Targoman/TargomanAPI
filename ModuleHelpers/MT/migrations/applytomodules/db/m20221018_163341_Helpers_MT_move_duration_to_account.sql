/* Migration File: m20221018_163341_Helpers_MT_move_duration_to_account.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

UPDATE tblAccountSaleables
    SET slbDurationDays = slbValidityDurationInDays
    WHERE slbDurationDays IS NULL
    AND IFNULL(slbValidityDurationInDays, 0) > 0
    ;

ALTER TABLE `tblAccountSaleables`
    DROP COLUMN `slbValidityDurationInDays`;
