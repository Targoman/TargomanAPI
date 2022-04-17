/* Migration File: m20220417_130419_Common_insert_data_into_tblAlertTemplates.sql */

ALTER TABLE `tblAlerts`
    DROP COLUMN `alrEmail`,
    DROP COLUMN `alrMobile`;

UPDATE tblAlerts
    SET alrReplacedContactInfo = '__UNKNOWN__'
    WHERE alrReplacedContactInfo IS NULL;

ALTER TABLE `tblAlerts`
    CHANGE COLUMN `alrReplacedContactInfo` `alrReplacedContactInfo` VARCHAR(50) NOT NULL COLLATE 'utf8_general_ci' AFTER `alr_usrID`;

UPDATE `tblAlerts` SET alr_altCode = 'passResetByEmail' WHERE alr_altCode = 'passReset';

UPDATE `tblAlertTemplates` SET altCode = 'passResetByEmail' WHERE altCode = 'passReset';

INSERT INTO `tblAlertTemplates` (`altCode`, `altMedia`, `altLanguage`, `altTitleTemplate`, `altBodyTemplate`, `altParamsPrefix`, `altParamsSuffix`) VALUES
    ('passResetByMobile', 'M', 'en', NULL, 'Dear %@%usrName %@%usrFamily, this is approval code for you. Code: %@%ApprovalCode', '%@%', NULL);

INSERT INTO `tblAlertTemplates` (`altCode`, `altMedia`, `altLanguage`, `altTitleTemplate`, `altBodyTemplate`, `altParamsPrefix`, `altParamsSuffix`) VALUES
    ('approveMobileOnly', 'M', 'en', NULL, 'Dear user, this is approval code for you. Code: %@%ApprovalCode', '%@%', NULL);

UPDATE `tblAlertTemplates`
    SET altBodyTemplate = 'Dear %@%usrName %@%usrFamily, use this code: %@%UUID for confirming your email address.'
    WHERE altCode = 'approveEmail'
    AND altMedia = 'E'
    AND altLanguage = 'en'
    AND altBodyTemplate = 'Dear %@%usrName %@%usrFamily, use this code: %@%ApprovalCode for confirming your email address.'
;
