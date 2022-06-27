/* Migration File: m20220627_150459_AAA_dbdiffof_dev_AAA.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

/************************************************************\
| binlog.000078 --start-position=4091449                     |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000079 --start-position=3401604                     |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000080 --start-position=33378                       |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000081 --start-position=722121                      |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
COMMIT /* added by mysqlbinlog *//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000082 --start-position=179                         |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000083 --start-position=1305109                     |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000084 --start-position=55980                       |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000085 --start-position=156                         |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000086 --start-position=1949111                     |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000087 --start-position=307580                      |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;

/************************************************************\
| binlog.000088 --start-position=241224                      |
\************************************************************/

# The proper term is pseudo_replica_mode, but we use this compatibility alias
# to make the statement usable on server versions 8.0.24 and older.
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
/*!50616 SET @@SESSION.GTID_NEXT='AUTOMATIC'*//*!*/;
SET @@session.pseudo_thread_id=184/*!*/;
SET @@session.foreign_key_checks=1, @@session.sql_auto_is_null=0, @@session.unique_checks=1, @@session.autocommit=1/*!*/;
SET @@session.sql_mode=1168113696/*!*/;
SET @@session.auto_increment_increment=1, @@session.auto_increment_offset=1/*!*/;
/*!\C utf8mb4 *//*!*/;
SET @@session.character_set_client=255,@@session.collation_connection=255,@@session.collation_server=255/*!*/;
SET @@session.time_zone='SYSTEM'/*!*/;
SET @@session.lc_time_names=0/*!*/;
SET @@session.collation_database=DEFAULT/*!*/;
/*!80011 SET @@session.default_collation_for_utf8mb4=255*//*!*/;
BEGIN
/*!*/;
COMMIT/*!*/;
use `{{dbprefix}}AAA`/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
CREATE TABLE `tblPaymentGatewayTypes` (
	`pgtID` INT NOT NULL AUTO_INCREMENT,
	`pgtName` VARCHAR(50) NOT NULL,
	PRIMARY KEY (`pgtID`)
)
COLLATE='utf8mb4_general_ci'
/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
ALTER TABLE `tblPaymentGatewayTypes`
	ADD COLUMN `pgtMinRequestAmount` INT NOT NULL AFTER `pgtName`,
	ADD COLUMN `pgtMaxRequestAmount` INT NULL AFTER `pgtMinRequestAmount`,
	ADD COLUMN `pgtMaxPerDayAmount` INT NULL AFTER `pgtMaxRequestAmount`
/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
ALTER TABLE `tblPaymentGatewayTypes`
	ADD COLUMN `pgtType` CHAR(1) NOT NULL AFTER `pgtID`
/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
ALTER TABLE `tblPaymentGatewayTypes`
	CHANGE COLUMN `pgtID` `pgtID` INT(10) NOT NULL FIRST,
	DROP PRIMARY KEY
/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
ALTER TABLE `tblPaymentGatewayTypes`
	DROP COLUMN `pgtID`,
	ADD PRIMARY KEY (`pgtType`)
/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
ALTER TABLE `tblPaymentGatewayTypes`
	DROP COLUMN `pgtMaxPerDayAmount`
/*!*/;
/*!80013 SET @@session.sql_require_primary_key=0*//*!*/;
ALTER TABLE `tblPaymentGateways`
	DROP COLUMN `pgwMinRequestAmount`,
	DROP COLUMN `pgwMaxRequestAmount`
/*!*/;
DELIMITER ;
# End of log file
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;









ALTER TABLE `tblPaymentGatewayTypes`
    ADD COLUMN `pgtStatus` CHAR(1) NOT NULL DEFAULT 'A' AFTER `pgtMaxRequestAmount`;

ALTER TABLE `tblPaymentGatewayTypes`
    CHANGE COLUMN `pgtMinRequestAmount` `pgtMinRequestAmount` FLOAT NOT NULL DEFAULT 0 AFTER `pgtName`,
    CHANGE COLUMN `pgtMaxRequestAmount` `pgtMaxRequestAmount` FLOAT NULL DEFAULT NULL AFTER `pgtMinRequestAmount`;

INSERT IGNORE INTO `tblPaymentGatewayTypes` (`pgtType`, `pgtName`, `pgtMinRequestAmount`, `pgtMaxRequestAmount`) VALUES
    ('-', '_DeveloperTest', 1, NULL),
    ('O', 'COD', 1, NULL),
    ('I', 'IranBank', 1, 50000000),
    ('M', 'IranIntermediateGateway', 1, 50000000),
    ('D', 'InternationalDebitCart', 1, NULL),
    ('C', 'InternationalCreditCart', 1, NULL),
    ('B', 'CryptoCurrency', 1, NULL)
;

ALTER TABLE `tblPaymentGateways`
    ADD CONSTRAINT `FK_tblPaymentGateways_tblPaymentGatewayTypes` FOREIGN KEY (`pgwType`) REFERENCES `tblPaymentGatewayTypes` (`pgtType`) ON UPDATE NO ACTION ON DELETE NO ACTION;











