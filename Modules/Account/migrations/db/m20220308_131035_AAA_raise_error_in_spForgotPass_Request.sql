/* Migration File: m20220308_131035_AAA_raise_error_in_spForgotPass_Request.sql */

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
    DECLARE vLinkUUID CHAR(32);

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

    SET vLinkUUID = Common.fnCreateRandomMD5();

    INSERT
      INTO tblForgotPassRequest
       SET tblForgotPassRequest.fpr_usrID = vUserID
         , tblForgotPassRequest.fprRequestedVia = iVia
         , tblForgotPassRequest.fprUUID = vLinkUUID
    ;

    INSERT
      INTO Common.tblAlerts
       SET Common.tblAlerts.alr_usrID = vUserID
         , Common.tblAlerts.alr_altCode = 'passReset'
         , Common.tblAlerts.alrReplacements = JSON_OBJECT(
             'usrName',   vUserName,
             'usrFamily', vUserFamily,
             'via',       iVia,
             'UUID',      vLinkUUID
           )
    ;
END ;;
DELIMITER ;
