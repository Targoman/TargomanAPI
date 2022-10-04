/* Migration File: m20221004_094508_AAA_improve_services.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblService`
    ADD COLUMN `svcHasSaleable` BIT DEFAULT 0 AFTER `svcCancelVoucherItemEndPoint`,
    ADD COLUMN `svcDesc` TEXT NULL AFTER `svcHasSaleable`;
