/* Migration File: m20220413_163630_Common_add_column_alrLockedAt_to_table_alert.sql */

ALTER TABLE `tblAlerts`
    ADD COLUMN `alrLockedAt` TIMESTAMP NULL AFTER `alrCreateDate`;

ALTER TABLE `tblAlerts`
    ADD COLUMN `alrLastTryAt` TIMESTAMP NULL DEFAULT NULL AFTER `alrLockedAt`;

ALTER TABLE `tblAlerts`
    CHANGE COLUMN `alrStatus` `alrStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N: New, S: Sent, E: Error' COLLATE 'utf8_general_ci' AFTER `alrSentDate`;

ALTER TABLE `tblAlertTemplates`
    CHANGE COLUMN `altMedia` `altMedia` CHAR(1) NOT NULL COMMENT 'E:Email, S:SMS, P:Push, A:All' COLLATE 'utf8_general_ci' AFTER `altCode`,
    CHANGE COLUMN `altTitleTemplate` `altTitleTemplate` VARCHAR(512) NULL COLLATE 'utf8_general_ci' AFTER `altLanguage`;

ALTER TABLE `tblAlertTemplates`
    ADD COLUMN `altParamsPrefix` VARCHAR(10) NOT NULL DEFAULT '%@%' AFTER `altBodyTemplate`,
    ADD COLUMN `altParamsSuffix` VARCHAR(10) NULL AFTER `altParamsPrefix`;

DELIMITER ;;
INSERT INTO `tblAlertTemplates` (`altlID`, `altCode`, `altMedia`, `altLanguage`, `altTitleTemplate`, `altBodyTemplate`, `altParamsPrefix`, `altParamsSuffix`) VALUES
    (1, 'approveMobile', 'M', 'en', NULL, 'Dear %@%usrName %@%usrFamily, this is approval code for you. Code: %@%ApprovalCode', '%@%', NULL),
    (2, 'approveEmail', 'E', 'en', 'Email approval', 'Dear %@%usrName %@%usrFamily, use this code: %@%ApprovalCode for confirming your email address.', '%@%', NULL),
    (3, 'passReset', 'E', 'en', 'Reset password request', 'Dear %@%usrName %@%usrFamily, use this code: %@%UUID for reseting your password.', '%@%', NULL);;
DELIMITER ;
