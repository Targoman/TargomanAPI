/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping database structure for CommonFuncs
DROP DATABASE IF EXISTS `CommonFuncs`;
CREATE DATABASE IF NOT EXISTS `CommonFuncs` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */;
USE `CommonFuncs`;

-- Dumping structure for function CommonFuncs.guid
DROP FUNCTION IF EXISTS `guid`;
DELIMITER //
CREATE FUNCTION `guid`(
	`iPrefix` VARCHAR(50)
) RETURNS varchar(50) CHARSET utf8mb4
    NO SQL
BEGIN
  RETURN CONCAT_WS('-',iPrefix,MD5(CONCAT(RAND(), UUID())));
END//
DELIMITER ;

-- Dumping structure for function CommonFuncs.randomStr
DROP FUNCTION IF EXISTS `randomStr`;
DELIMITER //
CREATE FUNCTION `randomStr`(
	`iPrefix` VARCHAR(10)
) RETURNS varchar(50) CHARSET utf8mb4
    NO SQL
BEGIN
  RETURN  CONCAT_WS('', 
                    iPrefix,
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1),
              SUBSTRING('ABCDEFGHIJKLMNPQRSTUVWXYZ123456789', rand()*36+1, 1)
         );
END//
DELIMITER ;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
