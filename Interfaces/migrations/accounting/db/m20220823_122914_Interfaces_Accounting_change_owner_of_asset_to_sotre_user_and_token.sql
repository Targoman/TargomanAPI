/* Migration File: m20220823_122914_Interfaces_Accounting_change_owner_of_asset_to_sotre_user_and_token.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountUserAssets`
    CHANGE COLUMN `uas_usrID` `uas_actorID` BIGINT(20) UNSIGNED NOT NULL AFTER `uasID`,
    DROP INDEX `uas_usrID_uasVoucherItemUUID__InvalidatedAt`,
    ADD UNIQUE INDEX `uas_actorID_uasVoucherItemUUID__InvalidatedAt` (`uas_actorID`, `uasVoucherItemUUID`, `_InvalidatedAt`) USING BTREE,
    DROP INDEX `uas_usrID`,
    ADD INDEX `uas_actorID` (`uas_actorID`) USING BTREE;

DROP PROCEDURE IF EXISTS `spUserAsset_SetAsPrefered`;
DELIMITER //
CREATE PROCEDURE `spUserAsset_SetAsPrefered`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iUASID` BIGINT UNSIGNED
)
BEGIN
  DECLARE PackageUser BIGINT UNSIGNED;

  SELECT tblAccountUserAssets.uas_actorID
    INTO PackageUser
    FROM tblAccountUserAssets
   WHERE tblAccountUserAssets.uasID = iAUPID;

  IF PackageUser IS NULL THEN
        SIGNAL SQLSTATE '45403'
          SET MESSAGE_TEXT = "403:UserPackage Not Found";
  END IF;

  START TRANSACTION;
    UPDATE tblAccountUserAssets
       SET tblAccountUserAssets.uasPrefered = NULL,
           tblAccountUserAssets.uasUpdatedBy_usrID = iUserID
     WHERE tblAccountUserAssets.uas_actorID = PackageUser
       AND tblAccountUserAssets.uasID != iUASID;

    UPDATE tblAccountUserAssets
       SET tblAccountUserAssets.uasPrefered = TRUE,
           tblAccountUserAssets.uasUpdatedBy_usrID = iUserID
     WHERE tblAccountUserAssets.uas_actorID = PackageUser
       AND tblAccountUserAssets.uasID = iUASID;
  COMMIT;

END//
DELIMITER ;
