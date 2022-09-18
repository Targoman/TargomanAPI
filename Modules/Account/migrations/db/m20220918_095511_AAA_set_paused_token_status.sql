/* Migration File: m20220918_095511_AAA_set_paused_token_status.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAPITokens`
    ADD COLUMN `aptPaused` BIT NOT NULL DEFAULT 0 AFTER `aptAccessCount`,
    CHANGE COLUMN `aptRevokeCount` `aptRevokeCount` SMALLINT(5) UNSIGNED NULL DEFAULT NULL AFTER `aptPaused`;

DROP PROCEDURE IF EXISTS `spUser_CheckPassword`;
DELIMITER //
CREATE PROCEDURE `spUser_CheckPassword`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iPass` VARCHAR(100),
    IN `iSalt` VARCHAR(100),
    IN `iThrowIfPassNotSet` TINYINT UNSIGNED
)
BEGIN
    DECLARE vPasswordsAreEqual TINYINT;
    DECLARE vPasswordsIsSet TINYINT;
    DECLARE vStatus CHAR(1);

    SELECT fnPasswordsAreEqual(iPass, iSalt, usrPass)
         , NOT ISNULL(usrPass)
         , usrStatus
      INTO vPasswordsAreEqual
         , vPasswordsIsSet
         , vStatus
      FROM tblUser
     WHERE usrID = iUserID
       AND usrStatus != 'R'
       AND _InvalidatedAt = 0
    ;

    IF ISNULL(vStatus) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:User not found';
    END IF;

    IF NOT vPasswordsIsSet AND iThrowIfPassNotSet THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Password not set';
    END IF;

    IF vPasswordsIsSet AND NOT vPasswordsAreEqual THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Incorrect password';
    END IF;

    IF vStatus != 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:User is not active';
    END IF;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spToken_Pause`;
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

    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET -- tblActionLogs.atlBy_usrID = iFromUserID,
               tblActionLogs.atlType = 'spToken_Pause',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iUserID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    -- ----------------------------
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

    IF (vTokenBinID > 0 AND vType = 'P') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Token paused before';
    END IF;

    IF ISNULL(vTokenBinID) THEN
        /*****************************/
        START TRANSACTION;
        /*****************************/

        UPDATE tblAPITokens
           SET aptPaused = 1
         WHERE aptID = vTokenID;

        INSERT INTO tblTokenBin
           SET tkbTokenMD5 = vTokenMD5
             , tkbType = 'P'
             , tkbCreatedBy_usrID = iUserID
        ;

        /*****************************/
        COMMIT;
        /*****************************/
    END IF;

END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spToken_Resume`;
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

    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET -- tblActionLogs.atlBy_usrID = iFromUserID,
               tblActionLogs.atlType = 'spToken_Resume',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iUserID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    -- ----------------------------
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

    /*****************************/
    START TRANSACTION;
    /*****************************/

    UPDATE tblAPITokens
       SET aptPaused = 0
     WHERE aptID = vTokenID;

    DELETE FROM tblTokenBin
     WHERE tkbID = vTokenBinID
        OR (tkbDueDateTime IS NOT NULL
       AND tkbDueDateTime <= NOW()
           )
    ;

    /*****************************/
    COMMIT;
    /*****************************/
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spToken_Revoke`;
DELIMITER //
CREATE PROCEDURE `spToken_Revoke`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iPass` VARCHAR(100),
    IN `iSalt` VARCHAR(100),
    IN `iToken` TEXT,
    IN `iDueDateTime` DATETIME,
    IN `iNewToken` TEXT
)
BEGIN
    DECLARE vTokenID BIGINT UNSIGNED;
    DECLARE vTokenUserID BIGINT UNSIGNED;
    DECLARE vTokenStatus CHAR(1);

    DECLARE vTokenBinID BIGINT UNSIGNED;
    DECLARE vTokenMD5 CHAR(32);
    DECLARE vType CHAR(1);

    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET -- tblActionLogs.atlBy_usrID = iFromUserID,
               tblActionLogs.atlType = 'spToken_Pause',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iUserID", iUserID
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    -- ----------------------------
    CALL spUser_CheckPassword(iUserID, iPass, iSalt, 0);

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
         , aptPaused = 0 -- remove paused flag if paused before
         , aptRevokeCount = IFNULL(aptRevokeCount, 0) + 1
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
