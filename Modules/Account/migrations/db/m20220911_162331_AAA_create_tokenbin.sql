/* Migration File: m20220911_162331_AAA_create_tokenbin.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

CREATE TABLE `tblTokenBin` (
    `tkbID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `tkbToken` TEXT NOT NULL COLLATE 'utf8mb4_general_ci',
    `tkbTokenExpiredAt` DATETIME NOT NULL,
    `tkbBlockedAt` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `tkbBlockedBy_usrID` BIGINT(20) UNSIGNED NOT NULL,
    PRIMARY KEY (`tkbID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;
