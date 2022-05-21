/* Migration File: m20220521_164100_MigrationTool_add_apply_type_to_tblmigrations.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `{{GlobalHistoryTableName}}`
    ADD COLUMN `migRunType` CHAR(1) NOT NULL DEFAULT 'C' COMMENT 'C:Commit, M:Mark' AFTER `migAppliedAt`;
