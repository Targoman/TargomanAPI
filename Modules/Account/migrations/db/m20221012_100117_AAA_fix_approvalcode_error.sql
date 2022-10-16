/* Migration File: m20221012_100117_AAA_fix_approvalcode_error.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP PROCEDURE IF EXISTS `spApproval_Request`;
DELIMITER //
CREATE PROCEDURE `spApproval_Request`(
    IN `iBy` CHAR(1),
    IN `iKey` VARCHAR(128),
    IN `iUserID` BIGINT UNSIGNED,
    IN `iPass` VARCHAR(100),
    IN `iSalt` VARCHAR(100),
    IN `iThrowIfPassNotSet` TINYINT UNSIGNED,
    IN `iResendApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iExpireApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iUserLanguage` CHAR(2)
)
proc: BEGIN
    DECLARE vErr VARCHAR(500);

    DECLARE vApprovalCode VARCHAR(50);
    DECLARE vAprStatus CHAR(1);
    DECLARE vAprElapsedSecs BIGINT;

    DECLARE vUsrID BIGINT UNSIGNED;
    DECLARE vUsrName VARCHAR(100);
    DECLARE vUsrFamily VARCHAR(100);
    DECLARE vUsrLanguage CHAR(2);
    DECLARE vUsrEnableEmailAlerts TINYINT;
    DECLARE vUsrEnableSMSAlerts TINYINT;

    DECLARE vExpiredAt DateTime;

    -- default values
    SET vUsrLanguage = iUserLanguage;
    SET vUsrEnableEmailAlerts = 1;
    SET vUsrEnableSMSAlerts = 1;

    -- bypass checkpassword ?
    IF (NOT ISNULL(iPass) OR iThrowIfPassNotSet) THEN
        CALL spUser_CheckPassword(iUserID, iPass, iSalt, iThrowIfPassNotSet);
    END IF;

    -- expire old requests
    UPDATE tblApprovalRequest
       SET tblApprovalRequest.aprStatus = 'E'
     WHERE tblApprovalRequest.aprStatus != 'E'
       AND tblApprovalRequest.aprExpireDate <= NOW()
    ;

    -- search in tblApprovalRequest and get last code -> vApprovalCode
    SELECT tblApprovalRequest.aprApprovalCode
         , tblApprovalRequest.aprStatus
         , TIME_TO_SEC(TIMEDIFF(NOW(), IFNULL(tblApprovalRequest.aprSentDate, tblApprovalRequest.aprRequestDate)))

         , tblApprovalRequest.apr_usrID
         , tblUser.usrName
         , tblUser.usrFamily
         , tblUser.usrLanguage
         , tblUser.usrEnableEmailAlerts
         , tblUser.usrEnableSMSAlerts
      INTO vApprovalCode
         , vAprStatus
         , vAprElapsedSecs

         , vUsrID
         , vUsrName
         , vUsrFamily
         , vUsrLanguage
         , vUsrEnableEmailAlerts
         , vUsrEnableSMSAlerts
      FROM tblApprovalRequest
 LEFT JOIN tblUser
        ON tblUser.usrID = tblApprovalRequest.apr_usrID
     WHERE tblApprovalRequest.aprApprovalKey = iKey
       AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2') -- , 'E')
--                            N:New, S:Sent, A:Applied, R:Removed, 1:FirstTry, 2:SecondTry, E:Expired
     ORDER BY aprRequestDate DESC
     LIMIT 1
    ;

if 1=0 and NOT ISNULL(vAprElapsedSecs) then
            SET vErr = CONCAT(' ', SEC_TO_TIME(vAprElapsedSecs), ' elapsed. (', SEC_TO_TIME(CAST(iResendApprovalTTLSecs AS SIGNED) - vAprElapsedSecs), ' remained)');

            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = vErr;
END if;


    IF ISNULL(vUsrID) THEN
        SET vUsrName = NULL;
        SET vUsrFamily = NULL;
        SET vUsrLanguage = iUserLanguage;
        SET vUsrEnableEmailAlerts = 1;
        SET vUsrEnableSMSAlerts = 1;
    END IF;

    -- check for code conflict
    IF NOT ISNULL(vApprovalCode) AND IFNULL(iUserID, 0) != IFNULL(vUsrID, 0) THEN
        UPDATE tblApprovalRequest
           SET tblApprovalRequest.aprStatus = 'E'
         WHERE tblApprovalRequest.aprApprovalKey = iKey
           AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2')
    --         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
        ;

        SET vApprovalCode = NULL;
        SET vUsrLanguage = iUserLanguage;
        SET vUsrEnableEmailAlerts = 1;
        SET vUsrEnableSMSAlerts = 1;
    END IF;

    -- found in tblApprovalRequest
    IF NOT ISNULL(vApprovalCode) THEN
        IF vAprElapsedSecs < CAST(iResendApprovalTTLSecs AS SIGNED) THEN
            SET vErr = CONCAT('401:The waiting time has not elapsed. (', SEC_TO_TIME(CAST(iResendApprovalTTLSecs AS SIGNED) - vAprElapsedSecs), ' remained)');

            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = vErr;
        END IF;

        UPDATE tblApprovalRequest
           SET tblApprovalRequest.aprRequestDate = NOW()
             , tblApprovalRequest.aprSentDate = NULL
         WHERE tblApprovalRequest.aprApprovalKey = iKey
           AND tblApprovalRequest.aprApprovalCode = vApprovalCode
           AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2')
    --         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
        ;

        -- resend last code
        IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
            INSERT INTO {{dbprefix}}Common.tblAlerts
               SET alr_usrID = vUsrID,
                   alr_altCode = IF(iBy = 'E', 'approveEmail', 'approveMobile'),
                   alrLanguage = vUsrLanguage,
                   alrReplacedContactInfo = iKey,
                   alrReplacements = JSON_OBJECT(
                      'usrName', vUsrName,
                      'usrFamily', vUsrFamily,
                      'usrFullName', TRIM(CONCAT_WS(' ', vUsrName, vUsrFamily)),
                      IF (iBy = 'E', 'UUID', 'ApprovalCode'), vApprovalCode,
                      IF (iBy = 'E', 'Email', 'Mobile'), iKey
                   )
            ;
        END IF;

        LEAVE proc;
    END IF;

    -- not found in tblApprovalRequest IN ('N', 'S', '1', '2')

    -- iUserID is NULL in case of loginByMobileOnly or resendApprovalCode
    IF ISNULL(iUserID) THEN
        SELECT tblUser.usrID
             , tblUser.usrName
             , tblUser.usrFamily
             , tblUser.usrLanguage
             , tblUser.usrEnableEmailAlerts
             , tblUser.usrEnableSMSAlerts
          INTO iUserID
             , vUsrName
             , vUsrFamily
             , vUsrLanguage
             , vUsrEnableEmailAlerts
             , vUsrEnableSMSAlerts
          FROM tblUser
         WHERE (
               tblUser.usrEmail = iKey
            OR tblUser.usrMobile = iKey
               )
           AND usrStatus != 'R'
           AND _InvalidatedAt = 0
         LIMIT 1
        ;

        IF ISNULL(iUserID) AND iBy != 'M' THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User ID not found by provided email';
        END IF;

        SET vUsrLanguage = iUserLanguage;
        SET vUsrEnableEmailAlerts = 1;
        SET vUsrEnableSMSAlerts = 1;
    ELSE -- check iUserID existance and get info
        SELECT tblUser.usrID
             , tblUser.usrName
             , tblUser.usrFamily
             , tblUser.usrLanguage
             , tblUser.usrEnableEmailAlerts
             , tblUser.usrEnableSMSAlerts
          INTO vUsrID
             , vUsrName
             , vUsrFamily
             , vUsrLanguage
             , vUsrEnableEmailAlerts
             , vUsrEnableSMSAlerts
          FROM tblUser
         WHERE tblUser.usrID = iUserID
           AND usrStatus != 'R'
           AND _InvalidatedAt = 0
        ;

        IF ISNULL(vUsrID) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User Not Found';
        END IF;
    END IF;

/*
    SELECT 1
         , tblUser.usrName
         , tblUser.usrFamily
         , tblUser.usrLanguage
         , tblUser.usrEnableEmailAlerts
         , tblUser.usrEnableSMSAlerts
      INTO vApprovalCode
         , vUsrName
         , vUsrFamily
         , vUsrLanguage
         , vUsrEnableEmailAlerts
         , vUsrEnableSMSAlerts
      FROM tblUser
     WHERE tblUser.usrID = iUserID
    ;

    IF (NOT ISNULL(iPass)) THEN
        IF ISNULL(vApprovalCode) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User Not Found';
        END IF;

        SELECT IF(fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass), 'Ok', 'Err')
             , tblUser.usrName
             , tblUser.usrFamily
             , tblUser.usrLanguage
             , tblUser.usrEnableEmailAlerts
             , tblUser.usrEnableSMSAlerts
          INTO vApprovalCode
             , vUsrName
             , vUsrFamily
             , vUsrLanguage
             , vUsrEnableEmailAlerts
             , vUsrEnableSMSAlerts
          FROM tblUser
         WHERE tblUser.usrID = iUserID
        ;

        IF ISNULL(vApprovalCode) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid userID';
        END IF;

        IF vApprovalCode = 'Err' THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid password';
        END IF;
    END IF;

    -- find user id if not provided
    IF ISNULL(vApprovalCode) THEN
        IF (NOT ISNULL(iUserID)) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid userID';
        END IF;

        SELECT tblApprovalRequest.apr_usrID
             , tblUser.usrName
             , tblUser.usrFamily
             , tblUser.usrLanguage
             , tblUser.usrEnableEmailAlerts
             , tblUser.usrEnableSMSAlerts
          INTO vApprovalCode
             , vUsrName
             , vUsrFamily
             , vUsrLanguage
             , vUsrEnableEmailAlerts
             , vUsrEnableSMSAlerts
          FROM tblApprovalRequest
     LEFT JOIN tblUser
            ON tblUser.usrID = tblApprovalRequest.apr_usrID
         WHERE tblApprovalRequest.aprApprovalKey = iKey
           AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2', 'E')
--             N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
      ORDER BY aprRequestDate DESC
         LIMIT 1
        ;

        IF ISNULL(vApprovalCode) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Could not find user by key';
        END IF;

        SET iUserID = CAST(vApprovalCode AS UNSIGNED);
    END IF;
*/
    -- check if new email or mobile registered before
    IF iBy = 'E' THEN
        IF ISNULL(iUserID) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User ID not provided';
        END IF;

        IF (EXISTS(
            SELECT *
              FROM tblUser
             WHERE usrEmail = iKey
               AND usrStatus != 'R'
               AND _InvalidatedAt = 0
               AND usrID != iUserID
        )) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '400:This email address is already taken';
        END IF;
    ELSE -- 'M'
        IF ISNULL(iUserID) THEN
            IF (EXISTS(
                SELECT *
                  FROM tblUser
                 WHERE usrMobile = iKey
                   AND usrStatus != 'R'
                   AND _InvalidatedAt = 0
--                   AND usrID != iUserID
            )) THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '400:This mobile number is already taken';
            END IF;
        ELSE
            IF (EXISTS(
                SELECT *
                  FROM tblUser
                 WHERE usrMobile = iKey
                   AND usrStatus != 'R'
                   AND _InvalidatedAt = 0
                   AND usrID != iUserID
            )) THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '400:This mobile number is already taken';
            END IF;
        END IF;
    END IF;

    -- check if active approval request exists, expire it
    UPDATE tblApprovalRequest
       SET tblApprovalRequest.aprStatus = 'E'
     WHERE tblApprovalRequest.aprApprovalKey = iKey
       AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2')
--         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
    ;

    IF iBy = 'E' THEN
        SET vApprovalCode = {{dbprefix}}Common.fnCreateRandomMD5();
    ELSE
        SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;
    END IF;

    SET vExpiredAt = DATE_ADD(NOW(), INTERVAL iExpireApprovalTTLSecs SECOND);

    INSERT INTO tblApprovalRequest
       SET apr_usrID = iUserID,
           aprRequestedFor = iBy,
           aprApprovalKey = iKey,
           aprApprovalCode = vApprovalCode,
           aprExpireDate = DATE_ADD(NOW(), INTERVAL iExpireApprovalTTLSecs SECOND)
    ;

    IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
        INSERT INTO {{dbprefix}}Common.tblAlerts
           SET alr_usrID = iUserID,
               alr_altCode = IF(iBy = 'E', 'approveEmail', 'approveMobile'),
               alrLanguage = vUsrLanguage,
               alrReplacedContactInfo = iKey,
               alrReplacements = JSON_OBJECT(
                  'usrName', vUsrName,
                  'usrFamily', vUsrFamily,
                  'usrFullName', TRIM(CONCAT_WS(' ', vUsrName, vUsrFamily)),
                  IF (iBy = 'E', 'UUID', 'ApprovalCode'), vApprovalCode,
                  IF (iBy = 'E', 'Email', 'Mobile'), iKey,
                  'expiredAt', vExpiredAt
               )
        ;
    END IF;
END//
DELIMITER ;
