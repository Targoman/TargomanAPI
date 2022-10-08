/* Migration File: m20221006_164012_AAA_fix_bug_in_unpause_revoked_token.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

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

    INSERT INTO tblTokenBin
       SET tkbTokenMD5 = vTokenMD5
         , tkbDueDateTime = iDueDateTime
         , tkbType = 'B'
         , tkbCreatedBy_usrID = iUserID
    ;

    -- md5 changed in trigger
    SET vTokenMD5 = MD5(iNewToken);

    -- delete block or paused from bin
    DELETE FROM tblTokenBin
     WHERE tkbTokenMD5 = vTokenMD5;

    /*****************************/
    COMMIT;
    /*****************************/

END//
DELIMITER ;
