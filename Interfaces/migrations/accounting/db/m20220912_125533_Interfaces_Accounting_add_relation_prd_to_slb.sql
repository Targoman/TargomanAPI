/* Migration File: m20220912_125533_Interfaces_Accounting_add_relation_prd_to_slb.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountSaleables`
    ADD CONSTRAINT `FK_tblAccountSaleables_tblAccountProducts` FOREIGN KEY (`slb_prdID`) REFERENCES `tblAccountProducts` (`prdID`) ON UPDATE NO ACTION ON DELETE CASCADE;
