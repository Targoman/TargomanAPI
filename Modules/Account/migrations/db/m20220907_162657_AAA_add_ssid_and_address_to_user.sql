/* Migration File: m20220907_162657_AAA_add_ssid_and_address_to_user.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblUser`
    ADD COLUMN `usrSSID` VARCHAR(10) NULL DEFAULT NULL AFTER `usrSpecialPrivs`,
    ADD COLUMN `usrAddress` VARCHAR(1024) NULL DEFAULT NULL AFTER `usrSSID`;
