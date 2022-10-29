/* Migration File: m20221018_163341_Helpers_MT_move_duration_to_account.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountProducts`
    ADD COLUMN `prdCreditTotalWords` INT UNSIGNED NULL DEFAULT NULL AFTER `prdUpdatedBy_usrID`;

UPDATE tblAccountSaleables
    SET slbDurationMinutes = slbValidityDurationInDays * 24 * 60
    WHERE slbDurationMinutes IS NULL
    AND IFNULL(slbValidityDurationInDays, 0) > 0
    ;

ALTER TABLE `tblAccountSaleables`
    DROP COLUMN `slbValidityDurationInDays`;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbValidityWords` `slbTotalWords` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `slbUpdatedBy_usrID`;

ALTER TABLE `tblAccountSaleables`
    CHANGE COLUMN `slbTotalWords` `slbCreditTotalWords` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `slbUpdatedBy_usrID`;

ALTER TABLE `tblAccountUserAssets`
    ADD COLUMN `uasTotalWords` INT UNSIGNED NULL DEFAULT NULL AFTER `uasUpdatedBy_usrID`;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uasTotalWords` `uasCreditTotalWords` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `uasUpdatedBy_usrID`;

ALTER TABLE `tblAccountAssetUsage`
    ADD COLUMN `usgRemainingTotalWords` INT UNSIGNED NULL DEFAULT NULL AFTER `usg_uasID`;

ALTER TABLE `tblAccountAssetUsage`
    CHANGE COLUMN `usgRemainingTotalWords` `usgUsedTotalWords` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `usg_uasID`;
