/* Migration File: m20220428_213216_AAA_disable_alert_for_fixture_users.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

UPDATE tblUser
   SET usrEnableEmailAlerts = 0,
       usrEnableSMSAlerts = 0
 WHERE usrEmail LIKE '%fixture%'
;
