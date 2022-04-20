/* Migration File: m20220420_161800_Common_add_language_to_alert.sql */

ALTER TABLE `tblAlerts`
    ADD COLUMN `alrLanguage` CHAR(2) NULL DEFAULT 'fa' AFTER `alr_usrID`;

ALTER TABLE `tblAlertTemplates`
    CHANGE COLUMN `altLanguage` `altLanguage` CHAR(2) NOT NULL COLLATE 'utf8_general_ci' AFTER `altMedia`;

DELIMITER ;;
INSERT INTO `tblAlertTemplates` (`altCode`, `altMedia`, `altLanguage`, `altTitleTemplate`, `altBodyTemplate`, `altParamsPrefix`, `altParamsSuffix`) VALUES
    ('approveMobile', 'S', 'fa', NULL, 'کاربر محترم %@%usrName %@%usrFamily, کد تایید شما Code: %@%ApprovalCode است.', '%@%', NULL),
    ('approveEmail', 'E', 'fa', 'تایید ایمیل', 'کاربر محترم %@%usrName %@%usrFamily, این کد %@%UUID برای تایید ایمیل شما است.', '%@%', NULL),
    ('passResetByEmail', 'E', 'fa', 'درخواست تغییر نام رمز', 'کاربر محترم %@%usrName %@%usrFamily, این کد %@%UUID برای تغییر رمز شما است.', '%@%', NULL),
    ('passResetByMobile', 'S', 'fa', NULL, 'کاربر محترم %@%usrName %@%usrFamily, کد تایید شما Code: %@%ApprovalCode است.', '%@%', NULL),
    ('approveMobileOnly', 'S', 'fa', NULL, 'کاربر محترم, این کد تایید شما است Code: %@%ApprovalCode', '%@%', NULL)
;;
DELIMITER ;
