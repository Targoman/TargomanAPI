/* Migration File: m20220401_200609_Ticketing_create_table_department_and_unit.sql */

CREATE TABLE `tblDepartments` (
    `depID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `depName` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci',
    `depCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `depCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`depID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `tblUnits` (
    `untID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `unt_depID` INT(10) UNSIGNED NOT NULL,
    `untName` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci',
    `untCreationDateTime` DATETIME NULL DEFAULT CURRENT_TIMESTAMP,
    `untCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL,
    PRIMARY KEY (`untID`) USING BTREE,
    INDEX `FK_tblUnits_tblDepartments` (`unt_depID`) USING BTREE,
    CONSTRAINT `FK_tblUnits_tblDepartments` FOREIGN KEY (`unt_depID`) REFERENCES `dev_Ticketing`.`tblDepartments` (`depID`) ON UPDATE NO ACTION ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

ALTER TABLE `tblTickets`
    ADD COLUMN `tkt_untID` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `tkt_svcID`;

ALTER TABLE `tblTickets`
    ADD CONSTRAINT `FK_tblTickets_tblUnits` FOREIGN KEY (`tkt_untID`) REFERENCES `tblUnits` (`untID`) ON UPDATE NO ACTION ON DELETE NO ACTION;

