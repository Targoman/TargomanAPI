/* Migration File: m20220806_125130_Interfaces_Accounting_change_translate_to_i18n.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

RENAME TABLE `tblAccountProducts_translate` TO `tblAccountProductsI18N`;
ALTER TABLE `tblAccountProductsI18N`
    DROP FOREIGN KEY `FK_tblAccountProducts_translate_tblAccountProducts`;
ALTER TABLE `tblAccountProductsI18N`
    ADD CONSTRAINT `FK_tblAccountProductsI18N_tblAccountProducts` FOREIGN KEY (`pid`) REFERENCES `tblAccountProducts` (`prdID`) ON UPDATE NO ACTION ON DELETE CASCADE;

RENAME TABLE `tblAccountSaleables_translate` TO `tblAccountSaleablesI18N`;
ALTER TABLE `tblAccountSaleablesI18N`
    DROP FOREIGN KEY `FK_tblAccountSaleables_translate_tblAccountSaleables`;
ALTER TABLE `tblAccountSaleablesI18N`
    ADD CONSTRAINT `FK_tblAccountSaleablesI18N_tblAccountSaleables` FOREIGN KEY (`pid`) REFERENCES `tblAccountSaleables` (`slbID`) ON UPDATE NO ACTION ON DELETE CASCADE;

RENAME TABLE `tblAccountUnits_translate` TO `tblAccountUnitsI18N`;
ALTER TABLE `tblAccountUnitsI18N`
    DROP FOREIGN KEY `FK_tblAccountUnits_translate_tblAccountUnits`;
ALTER TABLE `tblAccountUnitsI18N`
    ADD CONSTRAINT `FK_tblAccountUnitsI18N_tblAccountUnits` FOREIGN KEY (`pid`) REFERENCES `tblAccountUnits` (`untID`) ON UPDATE NO ACTION ON DELETE CASCADE;
