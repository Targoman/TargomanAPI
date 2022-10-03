/* Migration File: m20221003_122240_AAA_fix_non_created_def_wallet.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblUserWallets`
    ADD INDEX `wal_usrID` (`wal_usrID`);

ALTER TABLE `tblUserWallets`
    DROP INDEX `wal_usrID_walName__InvalidatedAt`;

ALTER TABLE `tblUserWallets`
    ADD walUniqueMD5 char(32) AS
        (MD5(CONCAT_WS('X',
            wal_usrID,
            IF(walDefault, '1:DEFAULT', walName),
            _InvalidatedAt
        )))
    UNIQUE;

INSERT INTO tblUserWallets(
        wal_usrID,
        walDefault,
        walCreatedBy_usrID
    )
    SELECT tblUser.usrID
         , 1
         , tblUser.usrID
      FROM tblUser
 LEFT JOIN (
    SELECT *
      FROM tblUserWallets
     WHERE tblUserWallets.walDefault = 1
           ) tblUserWallets
        ON tblUserWallets.wal_usrID = tblUser.usrID
     WHERE tblUserWallets.walID IS NULL
    ;

DROP TRIGGER IF EXISTS `trg_tblUser_after_insert`;
DELIMITER //
CREATE TRIGGER `trg_tblUser_after_insert` AFTER INSERT ON `tblUser` FOR EACH ROW BEGIN
    INSERT IGNORE INTO tblUserWallets(
        wal_usrID,
        walDefault,
        walCreatedBy_usrID
    )
    VALUES (
        NEW.usrID,
        1,
        NEW.usrID
    );

END//
DELIMITER ;

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
    IN `iResendApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iExpireApprovalTTLSecs` BIGINT UNSIGNED,
    IN `iUserLanguage` CHAR(2),
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

    /****************************************/
    START TRANSACTION;
    /****************************************/

    -- invalidate last removed user
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
           tblUser.usrEnableSMSAlerts = iEnableSMSAlerts,
           tblUser.usrLanguage = iUserLanguage
    ;

    SET oUserID = LAST_INSERT_ID();

    CALL spApproval_Request(
        /* iBy                    */ iBy,
        /* iKey                   */ iLogin,
        /* iUserID                */ oUserID,
        /* iPass                  */ NULL,
        /* iSalt                  */ NULL,
        /* iThrowIfPassNotSet     */ 0,
        /* iResendApprovalTTLSecs */ iResendApprovalTTLSecs,
        /* iExpireApprovalTTLSecs */ iExpireApprovalTTLSecs,
        /* iUserLanguage          */ iUserLanguage
    );
/*
moved to trigger:
    INSERT INTO tblUserWallets
       SET tblUserWallets.wal_usrID = oUserID,
           tblUserWallets.walName = 'Default',
           tblUserWallets.walDefault = 1,
           tblUserWallets.walCreatedBy_usrID = IFNULL(iCreatorUserID, 1)
    ;
*/
    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = oUserID,
           tblActionLogs.atlType = 'UserCreated'
    ;

    COMMIT;
END//
DELIMITER ;
