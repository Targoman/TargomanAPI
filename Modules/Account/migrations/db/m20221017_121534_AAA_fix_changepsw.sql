/* Migration File: m20221017_121534_AAA_fix_changepsw.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP PROCEDURE IF EXISTS `spForgotPass_Request`;
DELIMITER //
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
       AND usrStatus != 'R'
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
        SET vApprovalCode = dev_Common.fnCreateRandomMD5();
    ELSE
        SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;
    END IF;

    INSERT INTO tblForgotPassRequest
       SET tblForgotPassRequest.fpr_usrID = vUserID
         , tblForgotPassRequest.fprRequestedVia = iBy
         , tblForgotPassRequest.fprCode = vApprovalCode
    ;

    IF ((iBy = 'E' AND vUsrEnableEmailAlerts = 1) OR (iBy = 'M' AND vUsrEnableSMSAlerts = 1)) THEN
        INSERT INTO dev_Common.tblAlerts
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
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spPassword_ChangeByCode`;
DELIMITER //
CREATE PROCEDURE `spPassword_ChangeByCode`(
    IN `iBy` VARCHAR(1),
    IN `iLogin` VARCHAR(50),
    IN `iCode` VARCHAR(50),
    IN `iNewPass` VARCHAR(50)
)
BEGIN
    DECLARE vUserID BIGINT UNSIGNED;
    DECLARE vIsExpired BOOL;

    SELECT tblForgotPassRequest.fpr_usrID
         , TIMEDIFF(NOW(), tblForgotPassRequest.fprRequestDate) > "00:30:00"
      INTO vUserID
         , vIsExpired
      FROM tblForgotPassRequest
INNER JOIN tblUser
        ON tblUser.usrID = tblForgotPassRequest.fpr_usrID
     WHERE (
           tblUser.usrEmail = iLogin
        OR tblUser.usrMobile = iLogin
           )
       AND usrStatus != 'R'
       AND tblForgotPassRequest.fprCode = iCode
       AND tblForgotPassRequest.fprStatus = 'S'
    ;

    IF ISNULL(vIsExpired) OR vIsExpired THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = '401:Code is invalid or expired';
    END IF;

    UPDATE tblForgotPassRequest
       SET tblForgotPassRequest.fprStatus = IF(tblForgotPassRequest.fprCode = iCode, 'A', 'E')
     WHERE tblForgotPassRequest.fpr_usrID = vUserID
       AND tblForgotPassRequest.fprStatus IN ('S', 'N')
    ;

    UPDATE tblUser
       SET tblUser.usrPass = iNewPass
     WHERE tblUser.usrID = vUserID
    ;

    -- forcibly logout all user's sessions
    UPDATE tblActiveSessions
       SET tblActiveSessions.ssnStatus = 'F'
     WHERE tblActiveSessions.ssn_usrID = vUserID
       AND tblActiveSessions.ssnStatus = 'A'
    ;

END//
DELIMITER ;
