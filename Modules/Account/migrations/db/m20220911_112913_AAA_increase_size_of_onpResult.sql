/* Migration File: m20220911_112913_AAA_increase_size_of_onpResult.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblOnlinePayments`
    CHANGE COLUMN `onpResult` `onpResult` TEXT NULL COLLATE 'utf8mb4_general_ci' AFTER `onpTarget_walID`;
