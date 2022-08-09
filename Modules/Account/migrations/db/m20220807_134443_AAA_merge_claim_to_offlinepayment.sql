/* Migration File: m20220807_134443_AAA_merge_claim_to_offlinepayment.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblOfflinePayments`
    CHANGE COLUMN `ofp_vchID` `ofp_vchID` BIGINT(20) UNSIGNED NULL AFTER `ofpID`;

INSERT
  INTO tblOfflinePayments(
       ofp_vchID,
       ofpBank,
       ofpReceiptCode,
       ofpReceiptDate,
       ofpAmount,
       ofpTarget_walID,
       ofpNotes,
       ofpReceiptImage_uflID,
       ofpStatus,
       ofpCreationDateTime,
       ofpCreatedBy_usrID,
       ofpUpdatedBy_usrID
       )
SELECT ofpc_vchID,
       ofpcBank,
       ofpcReceiptCode,
       ofpcReceiptDate,
       ofpcAmount,
       ofpcTarget_walID,
       ofpcNotes,
       ofpcReceiptImage_uflID,
       'P', -- ofpcStatus,
       ofpcCreationDateTime,
       ofpcCreatedBy_usrID,
       ofpcUpdatedBy_usrID
  FROM tblOfflinePaymentClaims
 WHERE ofpcStatus = 'N'
;

ALTER TABLE `tblOfflinePaymentClaims`
    DROP FOREIGN KEY `FK_tblOfflinePaymentClaims_tblInvoice`,
    DROP FOREIGN KEY `FK_tblOfflinePaymentClaims_tblUploadFiles`,
    DROP FOREIGN KEY `FK_tblOfflinePaymentClaims_tblUser_creator`,
    DROP FOREIGN KEY `FK_tblOfflinePaymentClaims_tblUser_modifier`;

RENAME TABLE `tblOfflinePaymentClaims` TO `DELETED_tblOfflinePaymentClaims`;
-- will be dropped in next minor version
-- DROP TABLE tblOfflinePaymentClaims;

DROP TRIGGER IF EXISTS `trg_tblOfflinePayments_before_update`;
DELIMITER ;;
CREATE TRIGGER `trg_tblOfflinePayments_before_update` BEFORE UPDATE ON `tblOfflinePayments` FOR EACH ROW BEGIN
--    SIGNAL SQLSTATE '45000'
--    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblOfflinePayments';
END ;;
DELIMITER ;
