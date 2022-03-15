/* Migration File: m20220315_161304_AAA_define_advert_service.sql */

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping data for table AAA.tblService: ~0 rows (approximately)
/*!40000 ALTER TABLE `tblService` DISABLE KEYS */;
INSERT INTO `tblService` (`svcID`, `svcName`, `svc_rolID`, `svcProcessVoucherItemEndPoint`, `svcCancelVoucherItemEndPoint`, `svcStatus`, `svcCreatedBy_usrID`) VALUES
    (1, 'Advert', 1, 'Advert/processVoucherItem', 'Advert/cancelVoucherItem', 'A', 1);
/*!40000 ALTER TABLE `tblService` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
