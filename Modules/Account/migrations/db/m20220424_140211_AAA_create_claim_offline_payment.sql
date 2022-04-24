/* Migration File: m20220424_140211_AAA_create_claim_offline_payment.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

CREATE TABLE `tblOfflinePaymentClaims` (
    `ofpcID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `ofpc_vchID` BIGINT(20) UNSIGNED NOT NULL,
    `ofpcBank` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    `ofpcReceiptCode` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    `ofpcReceiptDate` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    `ofpcAmount` INT(10) UNSIGNED NOT NULL,
    `ofpcNotes` VARCHAR(500) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `ofpcStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, A: Succeded, E: Error, R: Removed' COLLATE 'utf8mb4_general_ci',
    `ofpcCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `ofpcCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL,
    `ofpcUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`ofpcID`) USING BTREE,
    INDEX `ofpcBank` (`ofpcBank`) USING BTREE,
    INDEX `ofpcReceiptCode` (`ofpcReceiptCode`) USING BTREE,
    INDEX `ofpcCreationDateTime` (`ofpcCreationDateTime`) USING BTREE,
    INDEX `ofpcStatus` (`ofpcStatus`) USING BTREE,
    INDEX `ofpcReceiptDate` (`ofpcReceiptDate`) USING BTREE,
    INDEX `FK_tblOfflinePaymentClaims_tblUser_creator` (`ofpcCreatedBy_usrID`) USING BTREE,
    INDEX `FK_tblOfflinePaymentClaims_tblUser_modifier` (`ofpcUpdatedBy_usrID`) USING BTREE,
    INDEX `FK_tblOfflinePaymentClaims_tblInvoice` (`ofpc_vchID`) USING BTREE,
    FULLTEXT INDEX `ofpcNotes` (`ofpcNotes`),
    CONSTRAINT `FK_tblOfflinePaymentClaims_tblInvoice` FOREIGN KEY (`ofpc_vchID`) REFERENCES `tblVoucher` (`vchID`) ON UPDATE CASCADE ON DELETE NO ACTION,
    CONSTRAINT `FK_tblOfflinePaymentClaims_tblUser_creator` FOREIGN KEY (`ofpcCreatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE ON DELETE NO ACTION,
    CONSTRAINT `FK_tblOfflinePaymentClaims_tblUser_modifier` FOREIGN KEY (`ofpcUpdatedBy_usrID`) REFERENCES `tblUser` (`usrID`) ON UPDATE CASCADE ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
