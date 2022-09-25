/* Migration File: m20220920_115420_AAA_set_pass_mandatory_for_change_email_and_mobile.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")



***********
dev_ to {{dbprefix}}
***********



USE `{{dbprefix}}{{Schema}}`;


ALTER TABLE `tblApprovalRequest`
    CHANGE COLUMN `apr_usrID` `apr_usrID` BIGINT(20) UNSIGNED NULL AFTER `aprID`;




DROP PROCEDURE IF EXISTS `spUser_CheckPassword`;
DELIMITER //
CREATE PROCEDURE `spUser_CheckPassword`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iPass` VARCHAR(100),
    IN `iSalt` VARCHAR(100),
    IN `iThrowIfPassNotSet` TINYINT UNSIGNED
)
proc: BEGIN
    DECLARE vPasswordsAreEqual TINYINT;
    DECLARE vPasswordsIsSet TINYINT;
    DECLARE vStatus CHAR(1);

    IF ISNULL(iUserID) THEN
        LEAVE proc;
    END IF;

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

    IF NOT vPasswordsIsSet AND TRIM(COALESCE(iPass, '')) != '' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Password not need';
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

DROP PROCEDURE IF EXISTS `spApproval_Request`;
DELIMITER //
CREATE PROCEDURE `spApproval_Request`(
    IN `iBy` CHAR(1),
    IN `iKey` VARCHAR(128),
    IN `iUserID` BIGINT UNSIGNED,
    IN `iPass` VARCHAR(100),
    IN `iSalt` VARCHAR(100),
    IN `iThrowIfPassNotSet` TINYINT UNSIGNED,
    IN `iApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iUserLanguage` CHAR(2)
)
proc: BEGIN
    DECLARE vApprovalCode VARCHAR(50);
    DECLARE vAprStatus CHAR(1);
    DECLARE vAprElapsedSecs BIGINT UNSIGNED;

    DECLARE vUsrID BIGINT UNSIGNED;
    DECLARE vUsrName VARCHAR(100);
    DECLARE vUsrFamily VARCHAR(100);
    DECLARE vUsrLanguage CHAR(2);
    DECLARE vUsrEnableEmailAlerts TINYINT;
    DECLARE vUsrEnableSMSAlerts TINYINT;

    -- default values
    SET vUsrLanguage = iUserLanguage;
    SET vUsrEnableEmailAlerts = 1;
    SET vUsrEnableSMSAlerts = 1;

    -- bypass checkpassword ?
    IF (NOT ISNULL(iPass) OR iThrowIfPassNotSet) THEN
        CALL spUser_CheckPassword(iUserID, iPass, iSalt, iThrowIfPassNotSet);
    END IF;

    -- search in tblApprovalRequest and get last code -> vApprovalCode
    SELECT tblApprovalRequest.apr_usrID
         , tblApprovalRequest.aprApprovalCode
         , tblApprovalRequest.aprStatus
         , TIME_TO_SEC(TIMEDIFF(NOW(), IFNULL(tblApprovalRequest.aprSentDate, tblApprovalRequest.aprRequestDate)))

         , tblUser.usrName
         , tblUser.usrFamily
         , tblUser.usrLanguage
         , tblUser.usrEnableEmailAlerts
         , tblUser.usrEnableSMSAlerts
      INTO vUsrID
         , vApprovalCode
         , vAprStatus
         , vAprElapsedSecs

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

    -- found in tblApprovalRequest
    IF NOT ISNULL(vApprovalCode) THEN
        IF vAprElapsedSecs < iApprovalTTLSecs THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:The waiting time has not elapsed';
        END IF;

        -- resend last code
        IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
            INSERT INTO dev_Common.tblAlerts
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
        SET vApprovalCode = dev_Common.fnCreateRandomMD5();
    ELSE
        SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;
    END IF;

    INSERT INTO tblApprovalRequest
       SET apr_usrID = iUserID,
           aprRequestedFor = iBy,
           aprApprovalKey = iKey,
           aprApprovalCode = vApprovalCode
    ;

    IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
        INSERT INTO dev_Common.tblAlerts
           SET alr_usrID = iUserID,
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
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spApproval_Accept`;
DELIMITER //
CREATE PROCEDURE `spApproval_Accept`(
    IN `iBy` CHAR(1),
    IN `iKey` VARCHAR(128),
    IN `iCode` VARCHAR(50),
    IN `iLogin` TINYINT,
    IN `iLoginIP` VARCHAR(50),
    IN `iLoginInfo` JSON,
    IN `iLoginRemember` TINYINT,
    IN `iApprovalTTLSecs` INT
)
BEGIN
    DECLARE vAprID BIGINT UNSIGNED;
    DECLARE vUsrID BIGINT UNSIGNED;
    DECLARE vNewKey VARCHAR(128);
    DECLARE vByType CHAR(1);
    DECLARE vAprStatus CHAR(1);
    DECLARE vIsExpired BOOL;
    DECLARE vSessionGUID VARCHAR(32);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    IF ISNULL(iKey) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid key';
    END IF;

    -- 24*60*60
    -- never expire?
    IF iApprovalTTLSecs = 0 THEN
        SET iApprovalTTLSecs = NULL;
    END IF;

      SELECT tblApprovalRequest.aprID
           , tblApprovalRequest.apr_usrID
           , tblApprovalRequest.aprApprovalKey
           , tblApprovalRequest.aprRequestedFor
           , tblApprovalRequest.aprStatus
           , (tblApprovalRequest.aprStatus = 'E'
          OR (iApprovalTTLSecs IS NOT NULL
         AND tblApprovalRequest.aprSentDate IS NOT NULL
         AND TIME_TO_SEC(TIMEDIFF(NOW(), tblApprovalRequest.aprSentDate)) > iApprovalTTLSecs)
             )
        INTO vAprID
           , vUsrID
           , vNewKey
           , vByType
           , vAprStatus
           , vIsExpired
        FROM tblApprovalRequest
   LEFT JOIN tblUser
          ON tblUser.usrID = tblApprovalRequest.apr_usrID
       WHERE tblApprovalRequest.aprApprovalKey = iKey
         AND tblApprovalRequest.aprApprovalCode = iCode
         AND tblApprovalRequest.aprStatus IN ('N', 'S', 'A', '1', '2', 'E')
--         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2:SecondTry, E: Expired
    ORDER BY aprRequestDate DESC
       LIMIT 1
             ;

    IF ISNULL(vAprID) THEN
        UPDATE tblApprovalRequest
           SET aprStatus = IF(aprStatus = 'S', '1',
                               IF(aprStatus = '1', '2', 'E'))
         WHERE aprApprovalKey = iKey
           AND aprStatus IN ('S', '1', '2')
        ;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid key or code';
    END IF;

    IF vAprStatus = 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code not sent to the client';
    END IF;

    IF vAprStatus = 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Already applied before';
    END IF;
/*
    IF ISNULL(vUsrID) THEN
        IF (iBy = 'M') THEN
            UPDATE tblApprovalRequest
               SET aprStatus = IF(aprStatus = 'S', '1',
                                   IF(aprStatus = '1', '2', 'E'))
             WHERE aprApprovalKey = iKey
               AND aprStatus IN ('S', '1', '2')
            ;
        END IF;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid user or code';
    END IF;
*/
    IF vIsExpired THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code expired';
    END IF;

    /****************************************/
    START TRANSACTION;
    /****************************************/

    UPDATE tblApprovalRequest
       SET aprApplyDate = NOW(),
           aprStatus = 'A'
     WHERE aprID = vAprID;

    IF vByType = 'E' THEN
        IF (EXISTS(SELECT * FROM tblUser
            WHERE usrEmail = vNewKey
              AND usrStatus != 'R'
              AND _InvalidatedAt = 0
              AND usrID != vUsrID
        )) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '400:This email address is already taken';
        END IF;

        UPDATE tblUser
           SET usrEmail = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','E'), 'E', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUsrID
         WHERE usrID = vUsrID
        ;
    ELSE
        IF ISNULL(vUsrID) THEN
            IF (EXISTS(SELECT * FROM tblUser
                WHERE usrMobile = vNewKey
                  AND usrStatus != 'R'
                  AND _InvalidatedAt = 0
--                  AND usrID != vUsrID
            )) THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '400:This mobile number is already taken';
            END IF;

            -- create user in case of loginByMobileOnly
            INSERT INTO tblUser
               SET tblUser.usrMobile = vNewKey,
                   tblUser.usr_rolID = (
            SELECT tblRoles.rolID
              FROM tblRoles
             WHERE LOWER(tblRoles.rolName) = 'user' -- LOWER(iSignupRole)
                   ),
                   tblUser.usrEnableEmailAlerts = 1, -- iSignupEnableEmailAlerts,
                   tblUser.usrEnableSMSAlerts = 1 -- iSignupEnableSMSAlerts
            ;

            SET vUsrID = LAST_INSERT_ID();
        ELSE
            IF (EXISTS(SELECT * FROM tblUser
                WHERE usrMobile = vNewKey
                  AND usrStatus != 'R'
                  AND _InvalidatedAt = 0
                  AND usrID != vUsrID
            )) THEN
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '400:This mobile number is already taken';
            END IF;

            UPDATE tblUser
               SET usrMobile = vNewKey,
                   usrApprovalState = IF(usrApprovalState IN ('N','M'), 'M', 'A'),
                   usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
                   usrUpdatedBy_usrID = vUsrID
             WHERE usrID = vUsrID
            ;
        END IF;
    END IF;

    IF iLogin = 1 THEN
        SET vSessionGUID = SUBSTRING(dev_CommonFuncs.guid(NULL), 1, 32);

        INSERT
          INTO tblActiveSessions
           SET tblActiveSessions.ssnKey          = vSessionGUID,
               tblActiveSessions.ssn_usrID       = vUsrID,
               tblActiveSessions.ssnIP           = INET_ATON(iLoginIP),
               tblActiveSessions.ssnRemember     = iLoginRemember,
               tblActiveSessions.ssnLastActivity = NOW(),
               tblActiveSessions.ssnInfo         = iLoginInfo;

        UPDATE tblUser
           SET tblUser.usrLastLogin = NOW(),
               tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
         WHERE tblUser.usrID = vUsrID
        ;

        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUsrID,
               tblActionLogs.atlType = 'UserLoggedIn'
        ;
    END IF;

    /****************************************/
    COMMIT;
    /****************************************/

    SELECT tblUser.usrID,
           tblUser.usrName,
           tblUser.usrFamily,
           tblUser.usrEmail,
           tblUser.usr_rolID,
           tblUser.usrApprovalState,
           tblRoles.rolName,
           fnGetAllPrivs(tblUser.usr_rolID, tblUser.usrSpecialPrivs) AS privs,
           NOT ISNULL(tblUser.usrPass) AS hasPass,
           tblUser.usrStatus,
           vSessionGUID AS ssnKey,
           UNIX_TIMESTAMP() AS Issuance
      FROM tblUser
      JOIN tblRoles
        ON tblRoles.rolID = tblUser.usr_rolID
     WHERE tblUser.usrID = vUsrID
    ;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spSignup`;
DELIMITER //
CREATE PROCEDURE `spSignup`(
    IN `iBy` CHAR(1),
    IN `iLogin` VARCHAR(50),
    IN `iPass` CHAR(32),
    IN `iRole` VARCHAR(50),
    IN `iIP` VARCHAR(50),
    IN `iName` VARCHAR(100),
    IN `iFamily` VARCHAR(100),
    IN `iSpecialPrivs` VARCHAR(5000),
    IN `iMaxSessions` INT,
    IN `iCreatorUserID` BIGINT UNSIGNED,
    IN `iEnableEmailAlerts` TINYINT,
    IN `iEnableSMSAlerts` TINYINT,
    IN `iApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iUserLanguage` CHAR(2),
    OUT `oUserID` BIGINT UNSIGNED
)
BEGIN
    DECLARE vRoleID BIGINT UNSIGNED;
    DECLARE vInnerRolID BIGINT;
    DECLARE vSessionGUID CHAR(32);
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        ROLLBACK;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = 1,
               tblActionLogs.atlType = 'Signup.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iBy", iBy,
                   "iLogin", iLogin,
                   "iPass", iPass,
                   "iRole", iRole,
                   "iIP", iIP,
                   "iName", iName,
                   "iFamily", iFamily,
                   "iSpecialPrivs", iSpecialPrivs,
                   "iMaxSessions", iMaxSessions,
                   "iCreatorUserID", iCreatorUserID
               )
        ;

        RESIGNAL;
    END;

    DECLARE EXIT HANDLER FOR 1062
    BEGIN
        ROLLBACK;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '409:Already registered.';
    END;

    CALL dev_Common.spLogDebug('AAA', 'signup');

    SELECT tblRoles.rolID
      INTO vRoleID
      FROM tblRoles
     WHERE tblRoles.rolName = iRole
       AND (tblRoles.rolSignupAllowedIPs IS NULL
        OR tblRoles.rolSignupAllowedIPs LIKE CONCAT("%,',iIP,',%"))
    ;

    IF ISNULL(vRoleID) THEN
        SIGNAL SQLSTATE '45403'
           SET MESSAGE_TEXT = "403:Role not found or is not allowed to signup from this IP";
    END IF;

    START TRANSACTION;

    UPDATE tblUser
       SET _InvalidatedAt = UNIX_TIMESTAMP()
     WHERE _InvalidatedAt = 0
       AND usrStatus = 'R'
       AND (
           (IFNULL(IF(iBy = 'E', iLogin, NULL), '') <> '' AND IFNULL(usrEmail, '') = IF(iBy = 'E', iLogin, NULL))
        OR (IFNULL(IF(iBy = 'M', iLogin, NULL), '') <> '' AND IFNULL(usrMobile, '') = IF(iBy = 'M', iLogin, NULL))
           )
    ;

    IF ((iPass IS NULL) OR (TRIM(iPass) = '')) THEN
        SET iPass = NULL; -- 'NOT_SET'
    ELSE
        SET iPass = LOWER(TRIM(iPass));
    END IF;

    INSERT INTO tblUser
       SET tblUser.usrName = iName,
           tblUser.usrFamily = iFamily,
           tblUser.usrEmail = IF(iBy = 'E', iLogin, NULL),
           tblUser.usrMobile = IF(iBy = 'M', iLogin, NULL),
           tblUser.usrPass = iPass,
           tblUser.usr_rolID = vRoleID,
           tblUser.usrSpecialPrivs = iSpecialPrivs,
           tblUser.usrMaxSessions = iMaxSessions,
           tblUser.usrCreatedBy_usrID = IFNULL(iCreatorUserID, 1),
           tblUser.usrEnableEmailAlerts = iEnableEmailAlerts,
           tblUser.usrEnableSMSAlerts = iEnableSMSAlerts,
           tblUser.usrLanguage = iUserLanguage
    ;

    SET oUserID = LAST_INSERT_ID();

    CALL spApproval_Request(
        /* iBy                */ iBy,
        /* iKey               */ iLogin,
        /* iUserID            */ oUserID,
        /* iPass              */ NULL,
        /* iSalt              */ NULL,
        /* iThrowIfPassNotSet */ 0,
        /* iApprovalTTLSecs   */ iApprovalTTLSecs,
        /* iUserLanguage      */ iUserLanguage
    );

    INSERT INTO tblUserWallets
       SET tblUserWallets.wal_usrID = oUserID,
           tblUserWallets.walName = 'Default',
           tblUserWallets.walDefault = 1,
           tblUserWallets.walCreatedBy_usrID = IFNULL(iCreatorUserID, 1)
    ;

    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = oUserID,
           tblActionLogs.atlType = 'UserCreated'
    ;

    COMMIT;
END//
DELIMITER ;

-- RENAME PROCEDURE `spMobileVerifyCode_Request` TO `spLogin_ByMobileOnly`;
DROP PROCEDURE IF EXISTS `spMobileVerifyCode_Request`;
DROP PROCEDURE IF EXISTS `spLogin_ByMobileOnly`;
DELIMITER //
CREATE PROCEDURE `spLogin_ByMobileOnly`(
    IN `iMobile` VARCHAR(50),
    IN `iPreSignup` TINYINT,
    IN `iSignupRole` VARCHAR(50),
    IN `iSignupEnableEmailAlerts` TINYINT,
    IN `iSignupEnableSMSAlerts` TINYINT,
    IN `iApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iUserLanguage` CHAR(2)
)
BEGIN
    DECLARE vUsrID BIGINT UNSIGNED;
--    DECLARE vApprovalCode VARCHAR(50);
--    DECLARE vUsrLanguage CHAR(2);
--    DECLARE vUsrEnableSMSAlerts TINYINT;

    IF TRIM(IFNULL(iMobile, '') = '') THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Mobile is empty';
    END IF;

    SELECT usrID
--         , usrLanguage
--         , tblUser.usrEnableSMSAlerts
      INTO vUsrID
--         , vUsrLanguage
--         , vUsrEnableSMSAlerts
      FROM tblUser
     WHERE usrMobile = iMobile
       AND usrStatus != 'R'
       AND _InvalidatedAt = 0
    ;

    IF ISNULL(vUsrID) AND iPreSignup = 1 THEN
        INSERT INTO tblUser
           SET tblUser.usrMobile = iMobile,
               tblUser.usr_rolID = (
        SELECT tblRoles.rolID
          FROM tblRoles
         WHERE LOWER(tblRoles.rolName) = LOWER(iSignupRole)
--               AND (tblRoles.rolSignupAllowedIPs IS NULL
--                OR tblRoles.rolSignupAllowedIPs LIKE CONCAT("%,',iIP,',%"))
               ),
               tblUser.usrEnableEmailAlerts = iSignupEnableEmailAlerts,
               tblUser.usrEnableSMSAlerts = iSignupEnableSMSAlerts,
               tblUser.usrLanguage = iUserLanguage
        ;

        SET vUsrID = LAST_INSERT_ID();
--        SET vUsrLanguage = 'fa';
--        SET vUsrEnableSMSAlerts = iSignupEnableSMSAlerts;

--        ELSE
--            SIGNAL SQLSTATE '45000'
--               SET MESSAGE_TEXT = '401:User Not Found';
--        END IF;
    END IF;

    CALL spApproval_Request(
        /* iBy                */ 'M',
        /* iKey               */ imobile,
        /* iUserID            */ vUsrID,
        /* iPass              */ NULL,
        /* iSalt              */ NULL,
        /* iThrowIfPassNotSet */ 0,
        /* iApprovalTTLSecs   */ iApprovalTTLSecs,
        /* iUserLanguage      */ iUserLanguage
    );
/*
    SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;

    INSERT INTO tblApprovalRequest
       SET apr_usrID = vUsrID,
           aprRequestedFor = 'M',
           aprApprovalKey = iMobile,
           aprApprovalCode = vApprovalCode
    ;

    IF (vUsrEnableSMSAlerts = 1) THEN
        INSERT INTO dev_Common.tblAlerts
           SET alr_usrID = vUsrID,
               alr_altCode = 'approveMobileOnly',
               alrLanguage = vUsrLanguage,
               alrReplacedContactInfo = iMobile,
               alrReplacements = JSON_OBJECT(
--                   'usrName', vUsrName,
--                   'usrFamily', vUsrFamily,
                   'ApprovalCode', vApprovalCode,
                   'Mobile', iMobile
               )
        ;
    END IF;
*/

END//
DELIMITER ;
