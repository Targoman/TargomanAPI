/* Migration File: m20221018_091759_Interfaces_Accounting_add_slb_relations.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountUserAssets`
    ADD CONSTRAINT `FK_tblAccountUserAssets_tblAccountSaleables` FOREIGN KEY (`uas_slbID`) REFERENCES `tblAccountSaleables` (`slbID`) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE `tblAccountUserAssets`
    ADD CONSTRAINT `FK_tblAccountUserAssets_tblAccountCoupons` FOREIGN KEY (`uas_cpnID`) REFERENCES `tblAccountCoupons` (`cpnID`) ON UPDATE NO ACTION ON DELETE NO ACTION;
