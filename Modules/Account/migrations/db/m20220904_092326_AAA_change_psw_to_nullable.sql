/* Migration File: m20220904_092326_AAA_change_psw_to_nullable.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblUser`
    CHANGE COLUMN `usrPass` `usrPass` CHAR(32) NULL COLLATE 'utf8mb4_general_ci' AFTER `usrApprovalState`;

UPDATE tblUser
    SET usrPass = NULL
    WHERE usrPass = 'NOT_SET';

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

    CALL {{dbprefix}}Common.spLogDebug('AAA', 'signup');

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
           tblUser.usrEnableSMSAlerts = iEnableSMSAlerts
    ;

    SET oUserID = LAST_INSERT_ID();

    CALL spApproval_Request(iBy, oUserID, iLogin, NULL, NULL);

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

DROP FUNCTION IF EXISTS `fnPasswordsAreEqual`;
DELIMITER //
CREATE FUNCTION `fnPasswordsAreEqual`(
    `iSaltedPass` VARCHAR(50),
    `iSalt` VARCHAR(50),
    `iHashedPass` VARCHAR(50)
)
RETURNS tinyint(1)
LANGUAGE SQL
NOT DETERMINISTIC
READS SQL DATA
SQL SECURITY DEFINER
COMMENT ''
BEGIN
    IF (iSaltedPass IS NULL OR iHashedPass IS NULL) THEN
        RETURN FALSE;
    END IF;

    RETURN MD5(CONCAT(iSalt, LOWER(iHashedPass))) = LOWER(iSaltedPass);
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `spPassword_Change`;
DELIMITER //
CREATE PROCEDURE `spPassword_Change`(
    IN `iUserID` BIGINT UNSIGNED,
    IN `iSessionGUID` CHAR(32),
    IN `iOldPass` CHAR(32),
    IN `iOldPassSalt` VARCHAR(50),
    IN `iNewPass` CHAR(32)
)
BEGIN
    DECLARE vUsrID BIGINT UNSIGNED;
    DECLARE vCurrentPassword CHAR(32);
    DECLARE vPasswordsAreEqual tinyint;

    SELECT usrID
         , usrPass
         , fnPasswordsAreEqual(iOldPass, iOldPassSalt, tblUser.usrPass)
      INTO vUsrID
         , vCurrentPassword
         , vPasswordsAreEqual
      FROM tblUser
     WHERE tblUser.usrID = iUserID
    ;

    IF ISNULL(vUsrID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '401:Invalid User and/or password';
    END IF;

    IF NOT ISNULL(vCurrentPassword) THEN
        IF ISNULL(iOldPass) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Current password not provided';
        END IF;

        IF NOT vPasswordsAreEqual THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '401:Invalid user and/or Password';
        END IF;
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

END//
DELIMITER ;
