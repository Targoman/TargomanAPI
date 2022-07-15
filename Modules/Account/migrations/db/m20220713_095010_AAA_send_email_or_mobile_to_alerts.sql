/* Migration File: m20220713_095010_AAA_send_email_or_mobile_to_alerts.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP PROCEDURE IF EXISTS `spApproval_Request`;
DELIMITER ;;
CREATE PROCEDURE `spApproval_Request`(
    IN `iBy` CHAR(1),
    IN `iUserID` BIGINT UNSIGNED,
    IN `iKey` VARCHAR(128),
    IN `iPass` VARCHAR(100),
    IN `iSalt` VARCHAR(100)
)
BEGIN
    DECLARE vApprovalCode VARCHAR(50);
    DECLARE vUserName VARCHAR(100);
    DECLARE vUserFamily VARCHAR(100);
    DECLARE vUsrLanguage CHAR(2);
    DECLARE vUsrEnableEmailAlerts TINYINT;
    DECLARE vUsrEnableSMSAlerts TINYINT;

    SELECT 1
         , tblUser.usrName
         , tblUser.usrFamily
         , tblUser.usrLanguage
         , tblUser.usrEnableEmailAlerts
         , tblUser.usrEnableSMSAlerts
      INTO vApprovalCode
         , vUserName
         , vUserFamily
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
             , vUserName
             , vUserFamily
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
             , vUserName
             , vUserFamily
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

    INSERT INTO tblApprovalRequest
       SET apr_usrID = iUserID,
           aprRequestedFor = iBy,
           aprApprovalKey = iKey,
           aprApprovalCode = vApprovalCode
    ;

    IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
        INSERT INTO {{dbprefix}}Common.tblAlerts
           SET alr_usrID = iUserID,
               alr_altCode = IF(iBy = 'E', 'approveEmail', 'approveMobile'),
               alrLanguage = vUsrLanguage,
               alrReplacedContactInfo = iKey,
               alrReplacements = JSON_OBJECT(
                  'usrName', vUserName,
                  'usrFamily', vUserFamily,
                  IF (iBy = 'E', 'UUID', 'ApprovalCode'), vApprovalCode,
                  IF (iBy = 'E', 'Email', 'Mobile'), iKey
               )
        ;
    END IF;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spForgotPass_Request`;
DELIMITER ;;
CREATE PROCEDURE `spForgotPass_Request`(
    IN `iLogin` VARCHAR(50),
    IN `iBy` CHAR(1)
)
BEGIN
    DECLARE vUserID INT UNSIGNED;
    DECLARE vUserName VARCHAR(50);
    DECLARE vUserFamily VARCHAR(50);
    DECLARE vApprovalCode CHAR(32);
    DECLARE vUsrLanguage CHAR(2);
    DECLARE vUsrEnableEmailAlerts TINYINT;
    DECLARE vUsrEnableSMSAlerts TINYINT;

    SELECT tblUser.usrID
         , tblUser.usrName
         , tblUser.usrFamily
         , tblUser.usrLanguage
         , tblUser.usrEnableEmailAlerts
         , tblUser.usrEnableSMSAlerts
      INTO vUserID
         , vUserName
         , vUserFamily
         , vUsrLanguage
         , vUsrEnableEmailAlerts
         , vUsrEnableSMSAlerts
      FROM tblUser
 LEFT JOIN tblForgotPassRequest
        ON tblForgotPassRequest.fpr_usrID = tblUser.usrID
     WHERE (
           tblUser.usrEmail = iLogin
        OR tblUser.usrMobile = iLogin
           )
       AND (
           ISNULL(tblForgotPassRequest.fprStatus)
        OR tblForgotPassRequest.fprStatus != 'N'
        OR TIME_TO_SEC(TIMEDIFF(NOW(), tblForgotPassRequest.fprRequestDate)) > 60
           )
     LIMIT 1
    ;

    IF ISNULL(vUserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:User not found';
    END IF;

    IF iBy = 'E' THEN
        SET vApprovalCode = {{dbprefix}}Common.fnCreateRandomMD5();
    ELSE
        SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;
    END IF;

    INSERT INTO tblForgotPassRequest
       SET tblForgotPassRequest.fpr_usrID = vUserID
         , tblForgotPassRequest.fprRequestedVia = iBy
         , tblForgotPassRequest.fprCode = vApprovalCode
    ;

    IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
        INSERT INTO {{dbprefix}}Common.tblAlerts
           SET alr_usrID = vUserID,
               alr_altCode = IF (iBy = 'E', 'passResetByEmail', 'passResetByMobile'),
               alrLanguage = vUsrLanguage,
               alrReplacedContactInfo = iLogin,
               alrReplacements = JSON_OBJECT(
                  'usrName',   vUserName,
                  'usrFamily', vUserFamily,
                  'via',       iBy,
                  IF (iBy = 'E', 'UUID', 'ApprovalCode'), vApprovalCode,
                  IF (iBy = 'E', 'Email', 'Mobile'), iLogin
               )
        ;
    END IF;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spMobileVerifyCode_Request`;
DELIMITER ;;
CREATE PROCEDURE `spMobileVerifyCode_Request`(
    IN `iMobile` VARCHAR(50),
    IN `iSignupIfNotExists` TINYINT,
    IN `iSignupRole` VARCHAR(50),
    IN `iSignupEnableEmailAlerts` TINYINT,
    IN `iSignupEnableSMSAlerts` TINYINT
)
BEGIN
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vApprovalCode VARCHAR(50);
    DECLARE vUsrLanguage CHAR(2);
    DECLARE vUsrEnableSMSAlerts TINYINT;

    IF ISNULL(iMobile) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid mobile';
    END IF;

    SELECT usrID
         , usrLanguage
         , tblUser.usrEnableSMSAlerts
      INTO vUserID
         , vUsrLanguage
         , vUsrEnableSMSAlerts
      FROM tblUser
     WHERE usrMobile = iMobile
    ;

    IF ISNULL(vUserID) THEN
        IF iSignupIfNotExists = 1 THEN
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
                   tblUser.usrEnableSMSAlerts = iSignupEnableSMSAlerts
            ;

            SET vUserID = LAST_INSERT_ID();
            SET vUsrLanguage = 'fa';
            SET vUsrEnableSMSAlerts = iSignupEnableSMSAlerts;
        ELSE
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User Not Found';
        END IF;
    END IF;

    SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;

    INSERT INTO tblApprovalRequest
       SET apr_usrID = vUserID,
           aprRequestedFor = 'M',
           aprApprovalKey = iMobile,
           aprApprovalCode = vApprovalCode
    ;

    IF (vUsrEnableSMSAlerts = 1) THEN
        INSERT INTO {{dbprefix}}Common.tblAlerts
           SET alr_usrID = vUserID,
               alr_altCode = 'approveMobileOnly',
               alrLanguage = vUsrLanguage,
               alrReplacedContactInfo = iMobile,
               alrReplacements = JSON_OBJECT(
--                   'usrName', vUserName,
--                   'usrFamily', vUserFamily,
                   'ApprovalCode', vApprovalCode,
                   'Mobile', iMobile
               )
        ;
    END IF;

END;;
DELIMITER ;
