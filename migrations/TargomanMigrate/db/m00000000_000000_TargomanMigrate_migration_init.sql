/* Migration File: m00000000_000000_TargomanMigrate_migration_init.sql */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `{{GlobalHistoryTableName}}` (
    `migName` VARCHAR(128) NOT NULL COLLATE 'utf8mb4_general_ci',
    `migAppliedAt` DATETIME NOT NULL,
    PRIMARY KEY (`migName`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
