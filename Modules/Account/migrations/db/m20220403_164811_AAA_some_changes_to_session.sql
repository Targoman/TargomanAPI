/* Migration File: m20220403_164811_AAA_some_changes_to_session.sql */

DROP PROCEDURE IF EXISTS `spSession_UpdateActivity`;

ALTER TABLE `tblActiveSessions`
    DROP INDEX `ssnLastActivity`;

ALTER TABLE `tblActiveSessions`
    ADD COLUMN `ssnLastRenew` DATETIME NULL DEFAULT NULL AFTER `ssnLastActivity`;

ALTER TABLE `tblActiveSessions`
    DROP COLUMN `ssnJWT`;

DROP PROCEDURE IF EXISTS `spSessionRetrieveInfo`;
DELIMITER ;;
CREATE PROCEDURE `spSessionRetrieveInfo`(
    IN `iSSID` CHAR(32),
    IN `iIP` VARCHAR(50),
    IN `iIssuance` BIGINT UNSIGNED
)
LANGUAGE SQL
NOT DETERMINISTIC
CONTAINS SQL
SQL SECURITY DEFINER
COMMENT ''
BEGIN
  DECLARE vSessionStatus CHAR(1);
  DECLARE vUserStatus CHAR(1);
  DECLARE vUserID BIGINT UNSIGNED;
  DECLARE vErr VARCHAR(500);
  DECLARE vLastRenew BIGINT UNSIGNED;

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;
    INSERT INTO tblActionLogs
     SET tblActionLogs.atlBy_usrID = vUserID,
         tblActionLogs.atlType = 'Session.act',
         tblActionLogs.atlDescription = JSON_OBJECT(
              "err", vErr,
              "iSSID", iSSID,
              "iIP", iIP,
              "iIssuance", iIssuance
          );
      ROLLBACK;
      RESIGNAL;
  END;

  SELECT tblActiveSessions.ssnStatus,
         tblActiveSessions.ssn_usrID,
         tblActiveSessions.ssnLastRenew,
         tblUser.usrStatus
    INTO vSessionStatus,
         vUserID,
         vLastRenew,
         vUserStatus
    FROM tblActiveSessions
    JOIN tblUser
      ON tblUser.usrID = tblActiveSessions.ssn_usrID
   WHERE tblActiveSessions.ssnKey = iSSID;

  IF (NOT ISNULL(vLastRenew) AND (vLastRenew != iIssuance)) THEN
        SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '501:JWT not replaced by client';
  END IF;

  IF ISNULL(vSessionStatus) THEN
        SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Invalid Session';
  ELSEIF vSessionStatus = 'E' THEN
        SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:Session expired';
  ELSEIF vSessionStatus = 'F' THEN
        SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:You were fired out. contact admin';
  ELSEIF vSessionStatus = 'G' THEN
        SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '401:You were logged out';
  ELSEIF vUserStatus = 'B' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '405:User Blocked. Ask administrator';
  ELSEIF vUserStatus = 'R' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '405:User Removed. Ask administrator';
  ELSEIF vUserStatus != 'A' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '501:Invalid Session State';
  END IF;

  -- CHECK for same IP was discarded
  SET vLastRenew = UNIX_TIMESTAMP();

  UPDATE tblActiveSessions
     SET tblActiveSessions.ssnLastActivity = NOW(),
         tblActiveSessions.ssnLastRenew = vLastRenew
   WHERE tblActiveSessions.ssnKey = iSSID;

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
         iSSID AS ssnKey,
         vLastRenew AS Issuance
    FROM tblUser
    JOIN tblRoles
      ON tblRoles.rolID = tblUser.usr_rolID
   WHERE tblUser.usrID = vUserID;
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spApproval_Accept`;
DELIMITER ;;
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
LANGUAGE SQL
NOT DETERMINISTIC
CONTAINS SQL
SQL SECURITY DEFINER
COMMENT ''
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
        UPDATE tblUser
           SET usrEmail = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','E'), 'E', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUserID
         WHERE usrID = vUserID
        ;
    ELSE
        UPDATE tblUser
           SET usrMobile = vNewKey,
               usrApprovalState = IF(usrApprovalState IN ('N','M'), 'M', 'A'),
               usrStatus = IF(usrStatus IN('A','V'), 'A', usrStatus),
               usrUpdatedBy_usrID = vUserID
         WHERE usrID = vUserID
        ;
    END IF;

    IF iLogin = 1 THEN
        SET vSessionGUID = SUBSTRING({{dbprefix}}CommonFuncs.guid(NULL), 1, 32);

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
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spLogin`;
DELIMITER ;;
CREATE PROCEDURE `spLogin`(
    IN `iLogin` VARCHAR(100),
    IN `iIP` VARCHAR(50),
    IN `iPass` CHAR(32),
    IN `iSalt` VARCHAR(50),
    IN `iInfo` JSON,
    IN `iRemember` TINYINT,
    IN `iOAuthInfo` VARCHAR(10000)
)
LANGUAGE SQL
NOT DETERMINISTIC
CONTAINS SQL
SQL SECURITY DEFINER
COMMENT ''
BEGIN
    DECLARE vLoginStatus CHAR(1);
    DECLARE vUserID BIGINT UNSIGNED;
--    DECLARE InnerRolID BIGINT;
    DECLARE vSessionGUID CHAR(32);
    DECLARE vLastOAuthInfo JSON;
    DECLARE vUserApprovalState CHAR(1);
    DECLARE vUserEmail CHAR(50);
    DECLARE vMessage CHAR(128);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    START TRANSACTION;

    SELECT IF (tblUser.usrMaxSessions > 0 AND tblUser.usrMaxSessions - tblUser.usrActiveSessions <= 0,
              'O',
              IF (fnPasswordsAreEqual(iPass, iSalt, tblUser.usrPass),
                  tblUser.usrStatus,
                  'I'
              )
           ),
           tblUser.usrID,
           tblUserExtraInfo.ueiOAuthAccounts,
           usrApprovalState,
           usrEmail
      INTO vLoginStatus,
           vUserID,
           vLastOAuthInfo,
           vUserApprovalState,
           vUserEmail
      FROM tblUser
 LEFT JOIN tblUserExtraInfo
        ON tblUserExtraInfo.uei_usrID = tblUser.usrID
     WHERE (
           tblUser.usrEmail  = iLogin
        OR tblUser.usrMobile = iLogin
        OR (
           NOT ISNULL(iOAuthInfo)
       AND JSON_EXTRACT(iOAuthInfo, "$.type") = 'Linkedin'
       AND tblUserExtraInfo.ueiOAuthAccounts->"$.Linkedin" = JSON_EXTRACT(iOAuthInfo, "$.id")
           )
           )
       AND tblUser.usrStatus IN ('A','V');

    IF NOT ISNULL(vUserApprovalState) AND vUserApprovalState NOT IN ('A', IF(vUserEmail = iLogin, 'E', 'M')) THEN
        SET vMessage = CONCAT('428:', IF(vUserEmail = iLogin, 'Email', 'Mobile'), ' not approved');

        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = vMessage;
    END IF;

    IF ISNULL(vLoginStatus) THEN
      IF ISNULL(iOAuthInfo) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid User or Password';
      ELSE
--        CALL spSignup(iBy, iLogin, iPass, iRole, iIP, iName, iFamily, iSpecialPrivs, iMaxSessions, oUserID);
-- TODO create wallet
        INSERT
          INTO tblUser
           SET tblUser.usrName   = JSON_EXTRACT(iOAuthInfo, "$.name"),
               tblUser.usrFamily = JSON_EXTRACT(iOAuthInfo, "$.family"),
               tblUser.usrEmail  = JSON_EXTRACT(iOAuthInfo, "$.email"),
               tblUser.usrApprovalState  = 'E';

        SET vUserID = LAST_INSERT_ID();

        INSERT
          INTO tblUserExtraInfo
           SET tblUserExtraInfo.uei_usrID = vUserID,
               tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
               tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"));

        SET vLoginStatus = 'H';
      END IF;
    END IF;

    CASE vLoginStatus
        WHEN 'O' THEN
            INSERT
              INTO tblActionLogs
               SET tblActionLogs.atlType = 'OverSession',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '409:Max sessions used close old sessions';

        WHEN 'I' THEN
            INSERT
              INTO tblActionLogs
               SET tblActionLogs.atlType = 'InvalidPass',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid user or Password';

        WHEN 'R' THEN
            INSERT
              INTO tblActionLogs
               SET tblActionLogs.atlType = 'UserRemoved',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '403:User Removed. Ask administrator';

        WHEN 'B' THEN
            INSERT
              INTO tblActionLogs
               SET tblActionLogs.atlType = 'UserBlocked',
                   tblActionLogs.atlBy_usrID = vUserID;
            COMMIT;
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '403:User Blocked. Ask administrator';

        WHEN 'V' THEN
            IF ISNULL(iOAuthInfo) THEN
                INSERT
                  INTO tblActionLogs
                   SET tblActionLogs.atlType = 'UserNotApprovedYet',
                       tblActionLogs.atlBy_usrID = vUserID;
                COMMIT;
                SIGNAL SQLSTATE '45000'
                   SET MESSAGE_TEXT = '428:You must approve either email or mobile';
            END IF;

        WHEN 'A' THEN
            SET @a = 1;
    END CASE;

    IF NOT ISNULL(iOAuthInfo) AND vLoginStatus != 'H' THEN
        IF ISNULL(vLastOAuthInfo) THEN
            INSERT
              INTO tblUserExtraInfo
               SET tblUserExtraInfo.uei_usrID = vUserID,
                   tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                   tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"))
                ON DUPLICATE KEY UPDATE
                   tblUserExtraInfo.uei_usrID = vUserID,
                   tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                   tblUserExtraInfo.ueiOAuthAccounts = JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"));
        ELSE
            UPDATE tblUserExtraInfo
               SET tblUserExtraInfo.ueiPhoto = JSON_EXTRACT(iOAuthInfo, "$.photo"),
                   tblUserExtraInfo.ueiOAuthAccounts = JSON_MERGE(
                       astOAuthInfo,
                       JSON_OBJECT(JSON_EXTRACT(iOAuthInfo, "$.type"), JSON_EXTRACT(iOAuthInfo, "$.id"))
                   ),
                   tblUserExtraInfo.ueiUpdatedBy_usrID = vUserID
             WHERE tblUserExtraInfo.uei_usrID = vUserID;

            INSERT
              INTO tblActionLogs
               SET tblActionLogs.atlBy_usrID = vUserID,
                   tblActionLogs.atlType = 'UserOAuthUpdated';
        END IF;
    END IF;

    SET vSessionGUID = SUBSTRING({{dbprefix}}CommonFuncs.guid(NULL), 1, 32);

    INSERT
      INTO tblActiveSessions
       SET tblActiveSessions.ssnKey          = vSessionGUID,
           tblActiveSessions.ssn_usrID       = vUserID,
           tblActiveSessions.ssnIP           = INET_ATON(iIP),
           tblActiveSessions.ssnRemember     = iRemember,
           tblActiveSessions.ssnLastActivity = NOW(),
           tblActiveSessions.ssnInfo         = iInfo;

    UPDATE tblUser
       SET tblUser.usrLastLogin = NOW(),
           tblUser.usrActiveSessions = tblUser.usrActiveSessions + 1
     WHERE tblUser.usrID = vUserID;

    INSERT
      INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = vUserID,
           tblActionLogs.atlType = 'UserLoggedIn';

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
     WHERE tblUser.usrID = vUserID;
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spLogin_VerifyByMobileCode`;
