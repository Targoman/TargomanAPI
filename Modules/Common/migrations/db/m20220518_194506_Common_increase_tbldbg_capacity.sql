/* Migration File: m20220518_194506_Common_increase_tbldbg_capacity.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

ALTER TABLE `tblDBG`
    CHANGE COLUMN `dbgFrom` `dbgFrom` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `dbgID`,
    CHANGE COLUMN `dbgInfo` `dbgInfo` TEXT NULL COLLATE 'utf8mb4_general_ci' AFTER `dbgTimestamp`;

DROP PROCEDURE IF EXISTS `spLogDebug`;
DELIMITER ;;
CREATE PROCEDURE `spLogDebug`(
    IN `iFrom` VARCHAR(128),
    IN `iInfo` TEXT
)
BEGIN
    INSERT
      INTO tblDBG
       SET tblDBG.dbgFrom = iFrom
         , tblDBG.dbgInfo = iInfo
    ;

END;;
DELIMITER ;
