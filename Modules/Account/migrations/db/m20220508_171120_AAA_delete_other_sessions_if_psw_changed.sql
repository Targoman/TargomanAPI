/* Migration File: m20220508_171120_AAA_delete_other_sessions_if_psw_changed.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

DROP PROCEDURE IF EXISTS `spPassword_Change`;
DELIMITER ;;
CREATE PROCEDURE `spPassword_Change`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iSessionGUID` CHAR(32),
    IN `iOldPass` CHAR(32),
    IN `iOldPassSalt` VARCHAR(50),
    IN `iNewPass` CHAR(32)
)
BEGIN
    DECLARE vWasOK BOOL DEFAULT FALSE;

    SELECT 1
      INTO vWasOK
      FROM tblUser
     WHERE tblUser.usrID = iUserID
       AND fnPasswordsAreEqual(iOldPass, iOldPassSalt, tblUser.usrPass)
    ;

    IF NOT vWasOK THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid User or Password';
    END IF;

    UPDATE tblUser
       SET tblUser.usrPass = iNewPass
     WHERE tblUser.usrID = iUserID
    ;

    -- forcibly logout other user's sessions
    UPDATE tblActiveSessions
       SET tblActiveSessions.ssnStatus = 'F'
     WHERE tblActiveSessions.ssn_usrID = iUserID
       AND tblActiveSessions.ssnKey <> iSessionGUID
       AND tblActiveSessions.ssnStatus = 'A'
    ;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spPassword_ChangeByCode`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `spPassword_ChangeByCode`(
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

END;;
DELIMITER ;
