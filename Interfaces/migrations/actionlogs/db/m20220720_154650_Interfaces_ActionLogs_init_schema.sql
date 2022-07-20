/* Migration File: m20220720_154650_Interfaces_ActionLogs_init_schema.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblActionLogs` (
    `atlID` BIGINT(19) NOT NULL AUTO_INCREMENT,
    `atlBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `atlInsertionDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `atlType` VARCHAR(50) NOT NULL COLLATE 'utf8mb4_general_ci',
    `atlDescription` JSON NULL DEFAULT NULL,
    PRIMARY KEY (`atlID`) USING BTREE,
    INDEX `atlType` (`atlType`) USING BTREE,
    INDEX `atlInsertionDateTime` (`atlInsertionDateTime`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

DROP TRIGGER IF EXISTS trg_tblActionLogs_before_update;
DELIMITER ;;
CREATE TRIGGER `trg_tblActionLogs_before_update` BEFORE UPDATE ON `tblActionLogs` FOR EACH ROW BEGIN
    SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:UPDATE is not allowed on tblActionLogs';
END;;
DELIMITER ;

DROP TRIGGER IF EXISTS trg_tblActionLogs_before_delete;
DELIMITER ;;
CREATE TRIGGER `trg_tblActionLogs_before_delete` BEFORE DELETE ON `tblActionLogs` FOR EACH ROW BEGIN
    SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '500:DELETE is not allowed on tblActionLogs';
END;;
DELIMITER ;
