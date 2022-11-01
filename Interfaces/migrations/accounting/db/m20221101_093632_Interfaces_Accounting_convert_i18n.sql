/* Migration File: m20221101_093632_Interfaces_Accounting_convert_i18n.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

/* The next line is to prevent this file from being committed. When done, delete this and next line: */
ERROR("THIS MIGRATION FILE IS NOT READY FOR EXECUTE.")

USE `{{dbprefix}}{{Schema}}`;

/* AccountProducts */
RENAME TABLE `tblAccountProductsI18N` TO `tblAccountProductsI18N_DELETED`;

ALTER TABLE `tblAccountProductsI18N_DELETED`
    DROP FOREIGN KEY `FK_tblAccountProductsI18N_tblAccountProducts`;

CREATE TABLE `tblAccountProductsI18N` (
    `i18nPID` INT UNSIGNED NOT NULL,
    `i18nData` JSON NOT NULL,
    PRIMARY KEY (`i18nPID`) USING BTREE,
    CONSTRAINT `FK_tblAccountProductsI18N_tblAccountProducts` FOREIGN KEY (`i18nPID`) REFERENCES `tblAccountProducts` (`prdID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

INSERT INTO `tblAccountProductsI18N` (i18nPID, i18nData)
     SELECT pid
          , JSON_OBJECT(
                'prdName', JSON_OBJECTAGG(`language`, prdNameI18N)
            ) AS prdName
       FROM tblAccountProductsI18N_DELETED
   GROUP BY pid
;

INSERT INTO `tblAccountProductsI18N` (i18nPID, i18nData)
     SELECT *
       FROM (
     SELECT pid
          , JSON_OBJECT(
                'prdDesc', JSON_OBJECTAGG(`language`, prdDescI18N)
            ) AS prdDesc
       FROM tblAccountProductsI18N_DELETED
      WHERE prdDescI18N IS NOT NULL
   GROUP BY pid
            ) src
         ON DUPLICATE KEY
     UPDATE i18nData = JSON_MERGE_PATCH(i18nData, src.prdDesc)
;

DROP TABLE tblAccountProductsI18N_DELETED;

/* AccountSaleables */
RENAME TABLE `tblAccountSaleablesI18N` TO `tblAccountSaleablesI18N_DELETED`;

ALTER TABLE `tblAccountSaleablesI18N_DELETED`
    DROP FOREIGN KEY `FK_tblAccountSaleablesI18N_tblAccountSaleables`;

CREATE TABLE `tblAccountSaleablesI18N` (
    `i18nPID` INT UNSIGNED NOT NULL,
    `i18nData` JSON NOT NULL,
    PRIMARY KEY (`i18nPID`) USING BTREE,
    CONSTRAINT `FK_tblAccountSaleablesI18N_tblAccountSaleables` FOREIGN KEY (`i18nPID`) REFERENCES `tblAccountSaleables` (`slbID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

INSERT INTO `tblAccountSaleablesI18N` (i18nPID, i18nData)
     SELECT pid
          , JSON_OBJECT(
                'slbName', JSON_OBJECTAGG(`language`, slbNameI18N)
            ) AS slbName
       FROM tblAccountSaleablesI18N_DELETED
   GROUP BY pid
;

INSERT INTO `tblAccountSaleablesI18N` (i18nPID, i18nData)
     SELECT *
       FROM (
     SELECT pid
          , JSON_OBJECT(
                'slbDesc', JSON_OBJECTAGG(`language`, slbDescI18N)
            ) AS slbDesc
       FROM tblAccountSaleablesI18N_DELETED
      WHERE slbDescI18N IS NOT NULL
   GROUP BY pid
            ) src
         ON DUPLICATE KEY
     UPDATE i18nData = JSON_MERGE_PATCH(i18nData, src.slbDesc)
;

DROP TABLE tblAccountSaleablesI18N_DELETED;

/* AccountUnits */
RENAME TABLE `tblAccountUnitsI18N` TO `tblAccountUnitsI18N_DELETED`;

ALTER TABLE `tblAccountUnitsI18N_DELETED`
    DROP FOREIGN KEY `FK_tblAccountUnitsI18N_tblAccountUnits`;

CREATE TABLE `tblAccountUnitsI18N` (
    `i18nPID` SMALLINT UNSIGNED NOT NULL,
    `i18nData` JSON NOT NULL,
    PRIMARY KEY (`i18nPID`) USING BTREE,
    CONSTRAINT `FK_tblAccountUnitsI18N_tblAccountUnits` FOREIGN KEY (`i18nPID`) REFERENCES `tblAccountUnits` (`untID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

INSERT INTO `tblAccountUnitsI18N` (i18nPID, i18nData)
     SELECT pid
          , JSON_OBJECT(
                'untName', JSON_OBJECTAGG(`language`, untNameI18N)
            ) AS untName
       FROM tblAccountUnitsI18N_DELETED
   GROUP BY pid
;

DROP TABLE tblAccountUnitsI18N_DELETED;
