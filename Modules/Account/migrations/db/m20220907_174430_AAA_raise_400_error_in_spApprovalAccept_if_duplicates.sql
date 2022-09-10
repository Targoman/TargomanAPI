/* Migration File: m20220907_174430_AAA_raise_400_error_in_spApprovalAccept_if_duplicates.sql */
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
    IN `iLoginRemember` TINYINT,
    IN `iTTL` INT
)
BEGIN
    DECLARE vAprID BIGINT UNSIGNED;
    DECLARE vUserID BIGINT UNSIGNED;
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
    IF iTTL = 0 THEN
        SET iTTL = NULL;
    END IF;

      SELECT tblApprovalRequest.aprID
           , tblApprovalRequest.apr_usrID
           , tblApprovalRequest.aprApprovalKey
           , tblApprovalRequest.aprRequestedFor
           , tblApprovalRequest.aprStatus
           , (tblApprovalRequest.aprStatus = 'E'
          OR (iTTL IS NOT NULL
         AND tblApprovalRequest.aprSentDate IS NOT NULL
         AND TIME_TO_SEC(TIMEDIFF(NOW(), tblApprovalRequest.aprSentDate)) > iTTL)
             )
        INTO vAprID
           , vUserID
           , vNewKey
           , vByType
           , vAprStatus
           , vIsExpired
        FROM tblApprovalRequest
   LEFT JOIN tblUser
          ON tblUser.usrID = tblApprovalRequest.apr_usrID
       WHERE tblApprovalRequest.aprApprovalKey = iKey
--         AND tblApprovalRequest.aprIsForLogin = 0
         AND tblApprovalRequest.aprApprovalCode = iCode
         AND tblApprovalRequest.aprStatus IN ('N', 'S', 'A', '1', '2', 'E')
--         N: New, S: Sent, A: Applied, R: Removed, 1: FirstTry, 2:SecondTry, E: Expired
    ORDER BY aprRequestDate DESC
       LIMIT 1
             ;

    IF vAprStatus = 'N' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code not sent to the client';
    END IF;

    IF vAprStatus = 'A' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Already applied before';
    END IF;

    IF ISNULL(vUserID) THEN
        IF (iBy = 'M') THEN
            UPDATE tblApprovalRequest
               SET aprStatus = IF(aprStatus = 'S', '1',
                                   IF(aprStatus = '1', '2', 'E'))
             WHERE aprApprovalKey = iKey
--               AND aprIsForLogin = 0
               AND aprStatus IN ('S', '1', '2')
            ;
        END IF;

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid user or code';
    END IF;

    IF vIsExpired THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Code expired';
    END IF;

    START TRANSACTION;

    UPDATE tblApprovalRequest
       SET aprApplyDate = NOW(),
           aprStatus = 'A'
     WHERE aprID = vAprID;

    IF vByType = 'E' THEN
        IF (EXISTS(SELECT * FROM tblUser
                WHERE usrEmail = vNewKey
                AND _InvalidatedAt = 0
                AND usrID != vUserID
        )) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '400:The email address is already assigned';
        END IF;

        UPDATE tblUser
           SET usrEmail = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','E'), 'E', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUserID
         WHERE usrID = vUserID
        ;
    ELSE
        IF (EXISTS(SELECT * FROM tblUser
                WHERE usrMobile = vNewKey
                AND _InvalidatedAt = 0
                AND usrID != vUserID
        )) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '400:The mobile is already assigned';
        END IF;

        UPDATE tblUser
           SET usrMobile = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','M'), 'M', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUserID
         WHERE usrID = vUserID
        ;
    END IF;

    IF iLogin = 1 THEN
        SET vSessionGUID = SUBSTRING(dev_CommonFuncs.guid(NULL), 1, 32);

        INSERT
          INTO tblActiveSessions
           SET tblActiveSessions.ssnKey          = vSessionGUID,
               tblActiveSessions.ssn_usrID       = vUserID,
               tblActiveSessions.ssnIP           = INET_ATON(iLoginIP),
               tblActiveSessions.ssnRemember     = iLoginRemember,
               tblActiveSessions.ssnLastActivity = NOW(),
               tblActiveSessions.ssnInfo         = iLoginInfo;

        UPDATE tblUser
           SET tblUser.usrLastLogin = NOW(),
               tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
         WHERE tblUser.usrID = vUserID
        ;

        INSERT
          INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'UserLoggedIn'
        ;
    END IF;

    COMMIT;

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
     WHERE tblUser.usrID = vUserID
    ;
END//
DELIMITER ;
