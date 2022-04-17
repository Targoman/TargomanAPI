/* Migration File: m20220417_124700_AAA_make_forgotpass_diff_via_email_or_mobile.sql */
/* CAUTION: don't forgot to use {{dbprefix}} for schemas */

ALTER TABLE `tblForgotPassRequest`
    ADD COLUMN `fprID` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT FIRST,
    DROP PRIMARY KEY,
    ADD PRIMARY KEY (`fprID`);

ALTER TABLE `tblForgotPassRequest`
    CHANGE COLUMN `fprUUID` `fprCode` CHAR(36) NOT NULL COLLATE 'utf8_general_ci' AFTER `fprRequestedVia`,
    ADD INDEX `fprCode` (`fprCode`);

DROP PROCEDURE IF EXISTS `spMobileVerifyCode_Request`;
DELIMITER ;;
CREATE PROCEDURE `spMobileVerifyCode_Request`(
    IN `iMobile` VARCHAR(50),
    IN `iSignupIfNotExists` TINYINT,
    IN `iSignupRole` VARCHAR(50)
)
BEGIN
    DECLARE UserID BIGINT UNSIGNED;
    DECLARE ApprovalCode VARCHAR(50);

    IF ISNULL(iMobile) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid mobile';
    END IF;

    SELECT usrID
      INTO UserID
      FROM tblUser
     WHERE usrMobile = iMobile
    ;

    IF ISNULL(UserID) THEN
        IF iSignupIfNotExists = 1 THEN
            INSERT
              INTO tblUser
               SET tblUser.usrMobile = iMobile,
                   tblUser.usr_rolID = (
            SELECT tblRoles.rolID
              FROM tblRoles
             WHERE LOWER(tblRoles.rolName) = LOWER(iSignupRole)
--               AND (tblRoles.rolSignupAllowedIPs IS NULL
--                OR tblRoles.rolSignupAllowedIPs LIKE CONCAT("%,',iIP,',%"))
                   )
            ;

            SET UserID = LAST_INSERT_ID();
        ELSE
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:User Not Found';
        END IF;
    END IF;

    SET ApprovalCode = FLOOR(RAND() * 90000) + 10000;

    INSERT
      INTO tblApprovalRequest
       SET apr_usrID = UserID,
           aprRequestedFor = 'M',
           aprApprovalKey = iMobile,
           aprApprovalCode = ApprovalCode
    ;

    INSERT
      INTO {{dbprefix}}Common.tblAlerts
       SET alr_usrID = UserID,
           alr_altCode = 'approveMobileOnly',
           alrReplacedContactInfo = iMobile,
           alrReplacements = JSON_OBJECT(
--               'usrName', vUserName,
--               'usrFamily', vUserFamily,
               'ApprovalCode', ApprovalCode
           )
    ;
END;;
DELIMITER ;

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

    SELECT 1
         , tblUser.usrName
         , tblUser.usrFamily
      INTO vApprovalCode
         , vUserName
         , vUserFamily
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
          INTO vApprovalCode
             , vUserName
             , vUserFamily
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
            INTO vApprovalCode
               , vUserName
               , vUserFamily
            FROM tblApprovalRequest
       LEFT JOIN tblUser
              ON tblUser.usrID = tblApprovalRequest.apr_usrID
           WHERE tblApprovalRequest.aprApprovalKey = iKey
             AND tblApprovalRequest.aprStatus IN ('N', 'S', '1', '2', 'E')
--               N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2: SecondTry, E: Expired
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

    INSERT
      INTO tblApprovalRequest
       SET apr_usrID = iUserID,
           aprRequestedFor = iBy,
           aprApprovalKey = iKey,
           aprApprovalCode = vApprovalCode
    ;

    INSERT
      INTO {{dbprefix}}Common.tblAlerts
       SET alr_usrID = iUserID,
           alr_altCode = IF(iBy = 'E', 'approveEmail', 'approveMobile'),
           alrReplacedContactInfo = iKey,
           alrReplacements = JSON_OBJECT(
              'usrName', vUserName,
              'usrFamily', vUserFamily,
              IF(iBy = 'E', 'UUID', 'ApprovalCode'), vApprovalCode
           );
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spForgotPass_Request`;
DELIMITER ;;
CREATE PROCEDURE `spForgotPass_Request`(
    IN `iLogin` VARCHAR(50),
    IN `iVia` CHAR(1)
)
BEGIN
    DECLARE vUserID INT UNSIGNED;
    DECLARE vUserName VARCHAR(50);
    DECLARE vUserFamily VARCHAR(50);
    DECLARE vApprovalCode CHAR(32);

    SELECT tblUser.usrID
         , tblUser.usrName
         , tblUser.usrFamily
      INTO vUserID
         , vUserName
         , vUserFamily
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

    IF iVia = 'E' THEN
        SET vApprovalCode = {{dbprefix}}Common.fnCreateRandomMD5();
    ELSE
        SET vApprovalCode = FLOOR(RAND() * 90000) + 10000;
    END IF;

    INSERT
      INTO tblForgotPassRequest
       SET tblForgotPassRequest.fpr_usrID = vUserID
         , tblForgotPassRequest.fprRequestedVia = iVia
         , tblForgotPassRequest.fprCode = vApprovalCode
    ;

    INSERT
      INTO {{dbprefix}}Common.tblAlerts
       SET alr_usrID = vUserID,
           alr_altCode = IF(iVia = 'E', 'passResetByEmail', 'passResetByMobile'),
           alrReplacedContactInfo = iLogin,
           alrReplacements = JSON_OBJECT(
             'usrName',   vUserName,
             'usrFamily', vUserFamily,
             'via',       iVia,
             IF(iVia = 'E', 'UUID', 'ApprovalCode'), vApprovalCode
           )
    ;
END ;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spPassword_ChangeByUUID`;
DROP PROCEDURE IF EXISTS `spPassword_ChangeByCode`;
DELIMITER ;;
CREATE PROCEDURE `spPassword_ChangeByCode`(
    IN `iVia` VARCHAR(1),
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

    IF ISNULL (vIsExpired) OR vIsExpired THEN
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
END;;
DELIMITER ;
