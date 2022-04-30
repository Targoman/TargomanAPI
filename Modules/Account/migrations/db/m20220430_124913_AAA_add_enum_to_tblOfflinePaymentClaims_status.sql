/* Migration File: m20220430_124913_AAA_add_enum_to_tblOfflinePaymentClaims_status.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

ALTER TABLE `tblOfflinePaymentClaims`
    CHANGE COLUMN `ofpcStatus` `ofpcStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N:New, P:Pending, Y:Payed, A:Succeded, E:Error, J:Rejected, R:Removed' COLLATE 'utf8mb4_general_ci' AFTER `ofpcNotes`;

ALTER TABLE `tblOfflinePayments`
    CHANGE COLUMN `ofpStatus` `ofpStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N:New, P:Pending, Y:Payed, A:Succeded, E:Error, J:Rejected, R:Removed' COLLATE 'utf8mb4_general_ci' AFTER `ofpNotes`;

ALTER TABLE `tblOnlinePayments`
    CHANGE COLUMN `onpStatus` `onpStatus` CHAR(1) NOT NULL DEFAULT 'P' COMMENT 'N:New, P:Pending, Y:Payed, A:Succeded, E:Error, J:Rejected, R:Removed' COLLATE 'utf8mb4_general_ci' AFTER `onpResult`;
