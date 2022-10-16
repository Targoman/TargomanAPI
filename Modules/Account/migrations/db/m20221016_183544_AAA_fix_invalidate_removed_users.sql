/* Migration File: m20221016_183544_AAA_fix_invalidate_removed_users.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP PROCEDURE IF EXISTS `spApproval_Accept`;
DELIMITER //
CREATE PROCEDURE `spApproval_Accept`(
    IN `iBy` CHAR(1),
    IN `iKey` VARCHAR(128),
    IN `iCode` VARCHAR(50),
    IN `iLogin` TINYINT,
    IN `iLoginIP` VARCHAR(50),
    IN `iLoginInfo` JSON,
    IN `iLoginRemember` TINYINT
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

      SELECT tblApprovalRequest.aprID
           , tblApprovalRequest.apr_usrID
           , tblApprovalRequest.aprApprovalKey
           , tblApprovalRequest.aprRequestedFor
           , tblApprovalRequest.aprStatus
           , (
             tblApprovalRequest.aprStatus = 'E'
          OR tblApprovalRequest.aprExpireDate <= NOW()
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
--           N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2:SecondTry, E: Expired
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
       SET aprApplyDate = NOW()
         , aprStatus = 'A'
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

        -- invalidate last removed user
        UPDATE tblUser
           SET _InvalidatedAt = UNIX_TIMESTAMP()
         WHERE _InvalidatedAt = 0
           AND usrStatus = 'R'
           AND usrMobile = vNewKey
        ;

        UPDATE tblUser
           SET tblUser.usrEmail = vNewKey,
               tblUser.usrApprovalState = IF(tblUser.usrApprovalState IN ('N', 'E'), 'E', 'A'),
               tblUser.usrStatus = IF(tblUser.usrStatus IN('A', 'V'), 'A', tblUser.usrStatus),
               tblUser.usrUpdatedBy_usrID = vUsrID
         WHERE tblUser.usrID = vUsrID
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

            -- invalidate last removed user
            UPDATE tblUser
               SET _InvalidatedAt = UNIX_TIMESTAMP()
             WHERE _InvalidatedAt = 0
               AND usrStatus = 'R'
               AND usrMobile = vNewKey
            ;

            -- create user in case of loginByMobileOnly
            INSERT INTO tblUser
               SET tblUser.usrMobile = vNewKey,
                   tblUser.usr_rolID = (
            SELECT tblRoles.rolID
              FROM tblRoles
             WHERE LOWER(tblRoles.rolName) = 'user' -- LOWER(iSignupRole)
                   ),
                   tblUser.usrEnableEmailAlerts = 1, -- iSignupEnableEmailAlerts,
                   tblUser.usrEnableSMSAlerts = 1, -- iSignupEnableSMSAlerts
                   tblUser.usrApprovalState = 'M',
                   tblUser.usrStatus = 'A'
            ;

            SET vUsrID = LAST_INSERT_ID();

            UPDATE tblApprovalRequest
               SET apr_usrID = vUsrID
             WHERE aprID = vAprID;

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

            -- invalidate last removed user
            UPDATE tblUser
               SET _InvalidatedAt = UNIX_TIMESTAMP()
             WHERE _InvalidatedAt = 0
               AND usrStatus = 'R'
               AND usrMobile = vNewKey
            ;

            UPDATE tblUser
               SET tblUser.usrMobile = vNewKey,
                   tblUser.usrApprovalState = IF(tblUser.usrApprovalState IN ('N', 'M'), 'M', 'A'),
                   tblUser.usrStatus = IF(tblUser.usrStatus IN('A', 'V'), 'A', tblUser.usrStatus),
                   tblUser.usrUpdatedBy_usrID = vUsrID
             WHERE tblUser.usrID = vUsrID
            ;
        END IF;
    END IF;

    IF iLogin = 1 THEN
        SET vSessionGUID = SUBSTRING({{dbprefix}}CommonFuncs.guid(NULL), 1, 32);

        INSERT INTO tblActiveSessions
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

        INSERT INTO tblActionLogs
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
