/* Migration File: m20221003_122240_AAA_fix_non_created_def_wallet.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblUserWallets`
    ADD walUniqueMD5 char(32) AS
        (MD5(CONCAT_WS('X',
            wal_usrID,
            IF(walDefault, '1:DEFAULT', walName),
            _InvalidatedAt
        )))
    UNIQUE;

INSERT INTO tblUserWallets(
        wal_usrID,
        walDefault,
        walCreatedBy_usrID
    )
    SELECT tblUser.usrID
         , 1
         , tblUser.usrID
      FROM tblUser
 LEFT JOIN (
    SELECT *
      FROM tblUserWallets
     WHERE tblUserWallets.walDefault = 1
           ) tblUserWallets
        ON tblUserWallets.wal_usrID = tblUser.usrID
     WHERE tblUserWallets.walID IS NULL
    ;

DROP TRIGGER IF EXISTS `trg_tblUser_after_insert`;
DELIMITER //
CREATE TRIGGER `trg_tblUser_after_insert` AFTER INSERT ON `tblUser` FOR EACH ROW BEGIN
    INSERT IGNORE INTO tblUserWallets(
        wal_usrID,
        walDefault,
        walCreatedBy_usrID
    )
    VALUES (
        NEW.usrID,
        1,
        NEW.usrID
    );

END//
DELIMITER ;
