/* Migration File: m20220404_203854_AAA_change_session_lastrenew_to_bigint */

DELETE FROM `tblActiveSessions` WHERE ssnLastRenew IS NOT NULL;

ALTER TABLE `tblActiveSessions`
	CHANGE COLUMN `ssnLastRenew` `ssnLastRenew` BIGINT UNSIGNED NULL DEFAULT NULL AFTER `ssnLastActivity`;
