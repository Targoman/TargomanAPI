/* Migration File: m20220813_204611_AAA_rename_priviledge.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAPITokens`
    CHANGE COLUMN `aptExtraPriviledges` `aptExtraPrivileges` JSON NULL DEFAULT NULL AFTER `aptValidateIP`;

DROP TRIGGER IF EXISTS `trg_tblAPIToken_after_update`;
DELIMITER ;;
CREATE TRIGGER `trg_tblAPIToken_after_update` AFTER UPDATE ON `tblAPITokens` FOR EACH ROW BEGIN
    DECLARE Changes JSON DEFAULT JSON_OBJECT("aptID", OLD.aptID);

    IF NEW.aptToken           != OLD.aptToken           THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptToken", OLD.aptToken)); END IF;
    IF NEW.apt_usrID          != OLD.apt_usrID          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("apt_usrID", OLD.apt_usrID)); END IF;
    IF NEW.apt_svcID          != OLD.apt_svcID          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("apt_svcID", OLD.apt_svcID)); END IF;
    IF NEW.aptLang            != OLD.aptLang            THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptLang", OLD.aptLang)); END IF;
    IF NEW.aptValidateIP      != OLD.aptValidateIP      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptValidateIP", OLD.aptValidateIP)); END IF;
    IF NEW.aptExtraPrivileges != OLD.aptExtraPrivileges THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptExtraPrivileges", OLD.aptExtraPrivileges)); END IF;
    IF NEW.aptExpiryDate      != OLD.aptExpiryDate      THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptExpiryDate", OLD.aptExpiryDate)); END IF;
    IF NEW.aptStatus          != OLD.aptStatus          THEN SET Changes = JSON_MERGE_PATCH(Changes, JSON_OBJECT("aptStatus", OLD.aptStatus)); END IF;

    IF JSON_LENGTH(Changes) > 0 THEN
        INSERT INTO tblActionLogs
                SET tblActionLogs.atlBy_usrID = NEW.aptUpdatedBy_usrID,
                    tblActionLogs.atlType = "tblAPIToken",
                    tblActionLogs.atlDescription = Changes
        ;
    END IF;
END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spToken_RetrieveInfo`;
DELIMITER ;;
CREATE PROCEDURE `spToken_RetrieveInfo`(
    IN `iToken` VARCHAR(50),
    IN `iIP` VARCHAR(50)
)
BEGIN
    DECLARE UserID BIGINT DEFAULT NULL;
    DECLARE TokenID BIGINT DEFAULT NULL;
    DECLARE ValidateIP BOOL DEFAULT FALSE;
    DECLARE IP BIGINT UNSIGNED;
    DECLARE AccessCount BIGINT UNSIGNED;
    DECLARE ExpiryDate DATE;
    DECLARE Privs VARCHAR(5000);
    DECLARE RoleName VARCHAR(50);
    DECLARE PreferedLang CHAR(2);
    DECLARE LastStatus CHAR(1);
    DECLARE Err VARCHAR(500);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 Err = MESSAGE_TEXT;
        INSERT INTO tblActionLogs
                SET tblActionLogs.atlBy_usrID = UserID,
                    tblActionLogs.atlType = 'Token.Error',
                    tblActionLogs.atlDescription = JSON_OBJECT(
                        "err", Err,
                        "iToken", iToken,
                        "iIP", iIP
                    );

        ROLLBACK;
        RESIGNAL;
    END;


    SET IP = INET_ATON(iIP);
    CALL spIP_ValidateAccess(IP);

    SELECT tblAPITokens.aptID,
           tblAPITokens.apt_usrID,
           tblAPITokens.aptValidateIP,
           tblAPITokens.aptAccessCount,
           fnGetAllPrivs(tblAPITokens.apt_rolID, tblAPITokens.aptExtraPrivileges),
           tblAPITokens.aptExpiryDate,
           LOWER(tblRoles.rolName),
           tblAPITokens.aptLang,
           tblAPITokens.aptStatus
      INTO TokenID,
           UserID,
           ValidateIP,
           AccessCount,
           Privs,
           ExpiryDate,
           RoleName,
           PreferedLang,
           LastStatus
      FROM tblAPITokens
      JOIN tblUser
        ON tblUser.usrID = tblAPITokens.apt_usrID
      JOIN tblRoles
        ON tblRoles.rolID = tblAPITokens.apt_rolID
     WHERE fnSanitizeToken(tblAPITokens.aptToken)  = fnSanitizeToken(iToken)
       AND (
           ISNULL(tblAPITokens.aptExpiryDate)
        OR tblAPITokens.aptExpiryDate = "2000-01-01" OR tblAPITokens.aptExpiryDate > now()
           )
       AND tblAPITokens.aptStatus IN ('A', 'C')
       AND tblUser.usrStatus = 'A';

    IF ISNULL(UserID) THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '403:User blocked or Token Expired';
    END IF;

    IF LastStatus = 'C' THEN
        SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = '402:Your credit has been finished please recharge it.';
    END IF;

    IF (ValidateIP = 1) THEN
        SET ValidateIP = NULL;

        SELECT 1
          INTO ValidateIP
          FROM tblAPITokenValidIPs
         WHERE tblAPITokenValidIPs.tviIP = IP
           AND tblAPITokenValidIPs.tvi_aptID = TokenID
           AND tblAPITokenValidIPs.tviStatus = 'A';

        IF (ISNULL(ValidateIP)) THEN
            SIGNAL SQLSTATE '45000'
               SET MESSAGE_TEXT = '403:Token usage on %IP% is restricted.';
        END IF;
    END IF;

    START TRANSACTION;

    UPDATE tblAPITokens
       SET tblAPITokens.aptAccessCount = tblAPITokens.aptAccessCount + 1,
           tblAPITokens.aptLastActivity = now(),
           tblAPITokens.aptExpiryDate = IF ((NOT ISNULL(ExpiryDate) AND ExpiryDate <= "2010-01-01"),
           DATE_ADD(CURDATE(),INTERVAL MONTH(ExpiryDate) * 30 + DAY(ExpiryDate) DAY),
           tblAPITokens.aptExpiryDate)
     WHERE tblAPITokens.aptID = TokenID;

    INSERT IGNORE
      INTO tblIPBin
       SET tblIPBin.ipbIP = IP;

    INSERT
      INTO tblIPStats
           (
           tblIPStats.ips_ipbIP,
           tblIPStats.ipsTimeStamp
           )
    VALUES (
           IP,
           UNIX_TIMESTAMP(CURTIME(4))
           );

    SELECT iToken       AS `token`,
           TokenID      AS `tokID`,
           UserID       AS `usrID`,
           iIP          AS `ip`,
           RoleName     AS `usrRole`,
           AccessCount  AS `tokAccessCount`,
           Privs        AS `privs`,
           PreferedLang AS `intfLang`;

    COMMIT;
END;;
DELIMITER ;
