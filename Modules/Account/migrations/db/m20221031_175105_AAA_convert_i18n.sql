/* Migration File: m20221031_175105_AAA_convert_i18n.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

/* PaymentGatewayTypes */
RENAME TABLE `tblPaymentGatewayTypesI18N` TO `tblPaymentGatewayTypesI18N_DELETED`;

ALTER TABLE `tblPaymentGatewayTypesI18N_DELETED`
    DROP FOREIGN KEY `FK_tblPaymentGatewayTypesI18N_tblPaymentGatewayTypes`;

CREATE TABLE `tblPaymentGatewayTypesI18N` (
    `i18nPID` SMALLINT(5) UNSIGNED NOT NULL,
    `i18nData` JSON NOT NULL,
    PRIMARY KEY (`i18nPID`) USING BTREE,
    CONSTRAINT `FK_tblPaymentGatewayTypesI18N_tblPaymentGatewayTypes` FOREIGN KEY (`i18nPID`) REFERENCES `tblPaymentGatewayTypes` (`pgtID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

INSERT INTO `tblPaymentGatewayTypesI18N` (i18nPID, i18nData)
    SELECT pid
         , JSON_OBJECT(
               'pgtName', JSON_OBJECTAGG(`language`, pgtNameI18N)
           ) AS pgtName
      FROM tblPaymentGatewayTypesI18N_DELETED
  GROUP BY pid
  ;

DROP TABLE tblPaymentGatewayTypesI18N_DELETED;
