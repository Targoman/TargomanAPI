/* Migration File: m20220905_153042_Interfaces_ObjectStorage_make_gateway_name_unique.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblUploadGateways`
    ADD UNIQUE INDEX `ugwName` (`ugwName`);
