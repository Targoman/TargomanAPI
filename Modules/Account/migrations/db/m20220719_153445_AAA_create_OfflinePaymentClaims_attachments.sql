/* Migration File: m20220719_153445_AAA_create_OfflinePaymentClaims_attachments.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblOfflinePaymentClaims`
    ADD COLUMN `ofpcReceiptImage_uflID` BIGINT UNSIGNED NULL DEFAULT NULL AFTER `ofpcNotes`,
    ADD CONSTRAINT `FK_tblOfflinePaymentClaims_tblUploadFiles` FOREIGN KEY (`ofpcReceiptImage_uflID`) REFERENCES `tblUploadFiles` (`uflID`) ON UPDATE NO ACTION ON DELETE NO ACTION
;

ALTER TABLE `tblOfflinePayments`
    ADD COLUMN `ofpReceiptImage_uflID` BIGINT UNSIGNED NULL DEFAULT NULL AFTER `ofpNotes`,
    ADD CONSTRAINT `FK_tblOfflinePayments_tblUploadFiles` FOREIGN KEY (`ofpReceiptImage_uflID`) REFERENCES `tblUploadFiles` (`uflID`) ON UPDATE NO ACTION ON DELETE NO ACTION
;
