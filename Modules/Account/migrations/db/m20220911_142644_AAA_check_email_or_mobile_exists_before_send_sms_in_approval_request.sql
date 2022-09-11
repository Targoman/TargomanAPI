/* Migration File: m20220911_142644_AAA_check_email_or_mobile_exists_before_send_sms_in_approval_request.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP PROCEDURE `spApproval_Request`;
DELIMITER //
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

    -- check if new email or mobile registered before
    IF iBy = 'E' THEN
        IF (EXISTS(SELECT * FROM tblUser
                WHERE usrEmail = iKey
                AND _InvalidatedAt = 0
                AND usrID != iUserID
        )) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '400:The email address is already assigned';
        END IF;
    ELSE
        IF (EXISTS(SELECT * FROM tblUser
                WHERE usrMobile = iKey
                AND _InvalidatedAt = 0
                AND usrID != iUserID
        )) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '400:The mobile is already assigned';
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
END//
DELIMITER ;
