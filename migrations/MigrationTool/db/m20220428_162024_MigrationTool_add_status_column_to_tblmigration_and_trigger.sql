/* Migration File: m20220428_162024_MigrationTool_add_status_column_to_tblmigration_and_trigger.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DELIMITER ;;
CREATE TRIGGER `{{GlobalHistoryTableName}}_before_delete` BEFORE DELETE ON `{{GlobalHistoryTableName}}` FOR EACH ROW BEGIN
    SIGNAL SQLSTATE '45000'
       SET MESSAGE_TEXT = '500:DELETE is not allowed on {{GlobalHistoryTableName}}. Set migStatus to R instead.';
END;;
DELIMITER ;

ALTER TABLE `{{GlobalHistoryTableName}}`
    ADD COLUMN `migStatus` CHAR(1) NULL DEFAULT 'A' COMMENT 'A:Active, R:Removed' AFTER `migAppliedAt`;

ALTER TABLE `{{GlobalHistoryTableName}}`
    DROP PRIMARY KEY,
    ADD UNIQUE INDEX `migName_migAppliedAt` (`migName`, `migAppliedAt`);
