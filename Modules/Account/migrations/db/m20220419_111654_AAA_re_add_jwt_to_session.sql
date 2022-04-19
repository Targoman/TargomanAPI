/* Migration File: m20220419_111654_AAA_re_add_jwt_to_session.sql */
/* CAUTION: don't forgot to use {{dbprefix}} for schemas */

ALTER TABLE `tblActiveSessions`
    ADD COLUMN `ssnJWT` TEXT NULL AFTER `ssnRemember`;

DELIMITER ;;
CREATE PROCEDURE `spSession_UpdateJWT`(
    IN `iSSID` CHAR(32),
    IN `iJWT` TEXT,
    IN `iIssuance` BIGINT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = vUserID,
               tblActionLogs.atlType = 'Session.savejwt',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err", vErr,
                   "iSSID", iSSID,
                   "iJWT", iJWT,
                   "iIssuance", iIssuance
               )
        ;

        ROLLBACK;
        RESIGNAL;
    END;

    UPDATE tblActiveSessions
       SET tblActiveSessions.ssnLastActivity = NOW(),
           tblActiveSessions.ssnJWT = iJWT,
           tblActiveSessions.ssnLastRenew = iIssuance
     WHERE tblActiveSessions.ssnKey = iSSID
    ;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spSessionRetrieveInfo`;
DELIMITER ;;
CREATE PROCEDURE `spSessionRetrieveInfo`(
    IN `iSSID` CHAR(32),
    IN `iIP` VARCHAR(50),
    IN `iIssuance` BIGINT UNSIGNED
)
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

/*
  IF (NOT ISNULL(vLastRenew) AND (vLastRenew != iIssuance)) THEN
        SIGNAL SQLSTATE '45000'
      SET MESSAGE_TEXT = '501:JWT not replaced by client';
  END IF;
*/

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
