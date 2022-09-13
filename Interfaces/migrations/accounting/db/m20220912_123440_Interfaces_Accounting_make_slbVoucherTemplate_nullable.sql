/* Migration File: m20220912_123440_Interfaces_Accounting_make_slbVoucherTemplate_nullable.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

ALTER TABLE `tblAccountSaleables`
	CHANGE COLUMN `slbVoucherTemplate` `slbVoucherTemplate` TEXT NULL COLLATE 'utf8mb4_general_ci' AFTER `slbReturnedQty`;

