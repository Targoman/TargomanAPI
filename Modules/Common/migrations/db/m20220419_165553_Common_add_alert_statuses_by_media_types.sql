/* Migration File: m20220419_165553_Common_add_alert_statuses_by_media_types.sql */

ALTER TABLE `tblAlerts`
    ADD COLUMN `alrResult` JSON NULL AFTER `alrStatus`;

UPDATE tblAlertTemplates
    SET altMedia='S'
    WHERE altMedia='M';
