/* Migration File: m20220401_144613_AAA_add_jwt_to_session.sql */

ALTER TABLE `tblActiveSessions`
    ADD COLUMN `ssnJWT` TEXT NULL AFTER `ssnRemember`;
