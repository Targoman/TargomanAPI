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

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbValidityWords` `slbTotalWords` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `slbUpdatedBy_usrID`;


ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasTotalWords` INT UNSIGNED NULL DEFAULT NULL AFTER `uasUpdatedBy_usrID`;


ALTER TABLE `tblAccountAssetUsage`
    ADD COLUMN `usgRemainingTotalWords` INT UNSIGNED NULL DEFAULT NULL AFTER `usg_uasID`;

