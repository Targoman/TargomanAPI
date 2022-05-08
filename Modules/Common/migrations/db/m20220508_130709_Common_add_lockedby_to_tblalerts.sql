/* Migration File: m20220508_130709_Common_add_lockedby_to_tblalerts.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

ALTER TABLE `tblAlerts`
    ADD COLUMN `alrLockedBy` VARCHAR(50) NULL DEFAULT NULL AFTER `alrLockedAt`;
