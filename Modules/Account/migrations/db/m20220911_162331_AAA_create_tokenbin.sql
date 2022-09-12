/* Migration File: m20220911_162331_AAA_create_tokenbin.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAPITokens`
    CHANGE COLUMN `aptStatus` `aptStatus` CHAR(1) NOT NULL DEFAULT 'A' COMMENT 'P:Pending, A:Active, C:CreditFinished, R:Removed' COLLATE 'utf8mb4_general_ci' AFTER `aptAccessCount`;

ALTER TABLE `tblAPITokens`
    ADD COLUMN `aptTokenMD5` CHAR(32) AS (MD5(aptToken)) VIRTUAL AFTER `aptToken`;

ALTER TABLE `tblAPITokens`
    DROP INDEX `aptToken__InvalidatedAt`;

ALTER TABLE `tblAPITokens`
    ADD UNIQUE INDEX `aptTokenMD5__InvalidatedAt` (`aptTokenMD5`, `_InvalidatedAt`);

ALTER TABLE `tblAPITokens`
    CHANGE COLUMN `aptToken` `aptToken` TEXT NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `aptID`;

CREATE TABLE `tblTokenBin` (
    `tkbID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `tkbTokenMD5` CHAR(32) NOT NULL COLLATE 'utf8mb4_general_ci',
    `tkbDueDateTime` DATETIME NULL DEFAULT NULL,
    `tkbType` CHAR(1) NOT NULL DEFAULT 'B' COMMENT 'B:Block, P:Pause' COLLATE 'utf8mb4_general_ci',
    `tkbCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `tkbCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL,
    PRIMARY KEY (`tkbID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

DROP PROCEDURE `spToken_Revoke`;
DELIMITER //
CREATE PROCEDURE `spToken_Revoke`(
    IN `iToken` TEXT,
    IN `iDueDateTime` DATETIME,
    IN `iNewToken` TEXT,
    IN `iUserID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vTokenID BIGINT UNSIGNED;
    DECLARE vTokenUserID BIGINT UNSIGNED;
    DECLARE vTokenStatus CHAR(1);

    DECLARE vTokenBinID BIGINT UNSIGNED;
    DECLARE vTokenMD5 CHAR(32);
    DECLARE vType CHAR(1);

    SET vTokenMD5 = MD5(iToken);

    -- find token
    SELECT aptID
         , apt_usrID
         , aptStatus
      INTO vTokenID
         , vTokenUserID
         , vTokenStatus
      FROM tblAPITokens
     WHERE aptTokenMD5 = vTokenMD5;

    IF ISNULL(vTokenID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token not found';
    END IF;

    IF vTokenUserID != iUserID THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token is not yours';
    END IF;

    IF vTokenStatus != 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token is not active';
    END IF;

    -- remove expired items from bin
    DELETE FROM tblTokenBin
     WHERE tkbDueDateTime IS NOT NULL
       AND tkbDueDateTime <= NOW();

    /*****************************/
    START TRANSACTION;
    /*****************************/

    UPDATE tblAPITokens
       SET aptToken = iNewToken
     WHERE aptID = vTokenID;

    -- delete block or paused from bin
    DELETE FROM tblTokenBin
     WHERE tkbTokenMD5 = vTokenMD5;

    INSERT INTO tblTokenBin
       SET tkbTokenMD5 = vTokenMD5
         , tkbDueDateTime = iDueDateTime
         , tkbType = 'B'
         , tkbCreatedBy_usrID = iUserID
    ;

    /*****************************/
    COMMIT;
    /*****************************/

END//
DELIMITER ;

DROP PROCEDURE `spToken_Pause`;
DELIMITER //
CREATE PROCEDURE `spToken_Pause`(
    IN `iToken` TEXT,
    IN `iUserID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vTokenID BIGINT UNSIGNED;
    DECLARE vTokenUserID BIGINT UNSIGNED;
    DECLARE vTokenStatus CHAR(1);

    DECLARE vTokenBinID BIGINT UNSIGNED;
    DECLARE vTokenMD5 CHAR(32);
    DECLARE vType CHAR(1);

    SET vTokenMD5 = MD5(iToken);

    -- find token
    SELECT aptID
         , apt_usrID
         , aptStatus
      INTO vTokenID
         , vTokenUserID
         , vTokenStatus
      FROM tblAPITokens
     WHERE aptTokenMD5 = vTokenMD5;

    IF ISNULL(vTokenID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token not found';
    END IF;

    IF vTokenUserID != iUserID THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token is not yours';
    END IF;

    IF vTokenStatus != 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token is not active';
    END IF;

    -- remove expired items from bin
    DELETE FROM tblTokenBin
     WHERE tkbDueDateTime IS NOT NULL
       AND tkbDueDateTime <= NOW();

    SELECT tkbID
         , tkbType
      INTO vTokenBinID
         , vType
      FROM tblTokenBin
     WHERE tkbTokenMD5 = vTokenMD5;

    IF (vTokenBinID > 0 AND vType = 'B') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token revoked before and could not be paused';
    END IF;

    IF ISNULL(vTokenBinID) THEN
        INSERT INTO tblTokenBin
           SET tkbTokenMD5 = vTokenMD5
             , tkbType = 'P'
             , tkbCreatedBy_usrID = iUserID
        ;
    END IF;

END//
DELIMITER ;

DROP PROCEDURE `spToken_Resume`;
DELIMITER //
CREATE PROCEDURE `spToken_Resume`(
    IN `iToken` TEXT,
    IN `iUserID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vTokenID BIGINT UNSIGNED;
    DECLARE vTokenUserID BIGINT UNSIGNED;
    DECLARE vTokenStatus CHAR(1);

    DECLARE vTokenBinID BIGINT UNSIGNED;
    DECLARE vTokenMD5 CHAR(32);
    DECLARE vType CHAR(1);

    SET vTokenMD5 = MD5(iToken);

    -- find token
    SELECT aptID
         , apt_usrID
         , aptStatus
      INTO vTokenID
         , vTokenUserID
         , vTokenStatus
      FROM tblAPITokens
     WHERE aptTokenMD5 = vTokenMD5;

    IF ISNULL(vTokenID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token not found';
    END IF;

    IF vTokenUserID != iUserID THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token is not yours';
    END IF;

    IF vTokenStatus != 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token is not active';
    END IF;

    SELECT tkbID
         , tkbType
      INTO vTokenBinID
         , vType
      FROM tblTokenBin
     WHERE tkbTokenMD5 = vTokenMD5;

    IF ISNULL(vTokenBinID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token not paused before';
    END IF;

    IF (vType != 'P') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Only paused token could be resumed';
    END IF;

    DELETE FROM tblTokenBin
     WHERE tkbID = vTokenBinID
        OR (tkbDueDateTime IS NOT NULL
       AND tkbDueDateTime <= NOW()
           )
    ;

END//
DELIMITER ;
