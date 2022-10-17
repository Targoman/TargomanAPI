/* Migration File: m20221017_101642_Common_fix_update_apr_in_trigger.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP TRIGGER IF EXISTS `tblAlerts_after_update`;
DELIMITER //
CREATE TRIGGER `tblAlerts_after_update` AFTER UPDATE ON `tblAlerts` FOR EACH ROW BEGIN
    IF (NEW.alrStatus = 'S' AND NEW.alrSentDate IS NOT NULL AND OLD.alrSentDate IS NULL) THEN
        -- 1: update tblApprovalRequest
        UPDATE {{dbprefix}}AAA.tblApprovalRequest
           SET aprStatus = 'S'
             , aprSentDate = NEW.alrSentDate
         WHERE aprStatus != 'S'
           AND aprSentDate IS NULL
           AND IFNULL(apr_usrID, 0) = IFNULL(NEW.alr_usrID, 0)
           AND aprRequestedFor = IF(NEW.alr_altCode = 'approveEmail', 'E', 'M')
           AND aprApprovalKey = NEW.alrReplacedContactInfo
           AND aprApprovalCode = JSON_EXTRACT(NEW.alrReplacements, IF(NEW.alr_altCode = 'approveEmail', '$.UUID', '$.ApprovalCode'))
        ;

        -- 2: update tblForgotPassRequest
        UPDATE {{dbprefix}}AAA.tblForgotPassRequest
           SET fprStatus = 'S'
--             , fprSentDate = NEW.alrSentDate
         WHERE fprStatus != 'S'
--           AND fprSentDate IS NULL
           AND fpr_usrID = NEW.alr_usrID
           AND fprRequestedVia = IF(NEW.alr_altCode = 'passResetByEmail', 'E', 'M')
           AND fprCode = JSON_EXTRACT(NEW.alrReplacements, IF(NEW.alr_altCode = 'passResetByEmail', '$.UUID', '$.ApprovalCode'))
        ;

    END IF;
END//
DELIMITER ;
