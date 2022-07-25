/* Migration File: m20220725_093105_Interfaces_FAQ_init_schema.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblFaqQuestions` (
    `qsnID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `qsnBody` TEXT NOT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`qsnID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `tblFaqAnswers` (
    `ansID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `ans_qsnID` INT(10) UNSIGNED NOT NULL,
    `ansBody` TEXT NOT NULL COLLATE 'utf8mb4_general_ci',
    PRIMARY KEY (`ansID`) USING BTREE,
    INDEX `FK_tblFaqAnswers_tblFaqQuestions` (`ans_qsnID`) USING BTREE,
    CONSTRAINT `FK_tblFaqAnswers_tblFaqQuestions` FOREIGN KEY (`ans_qsnID`) REFERENCES `tblFaqQuestions` (`qsnID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
