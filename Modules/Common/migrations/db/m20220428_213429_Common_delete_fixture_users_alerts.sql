/* Migration File: m20220428_213429_Common_delete_fixture_users_alerts.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

DELETE FROM tblAlerts
 WHERE alrReplacedContactInfo LIKE '%fixture%'
;
