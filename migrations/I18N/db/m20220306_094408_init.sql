/* Migration File: m20220306_094408_init.sql */

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

/*!50003 DROP FUNCTION IF EXISTS `farsiDigit` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `farsiDigit`(
    `iChar` CHAR(1)
) RETURNS varchar(1000) CHARSET utf8
    NO SQL
    DETERMINISTIC
BEGIN
  CASE iChar
    WHEN '1' THEN RETURN '۱';
    WHEN '2' THEN RETURN '۲';
    WHEN '3' THEN RETURN '۳';
    WHEN '4' THEN RETURN '۴';
    WHEN '5' THEN RETURN '۵';
    WHEN '6' THEN RETURN '۶';
    WHEN '7' THEN RETURN '۷';
    WHEN '8' THEN RETURN '۸';
    WHEN '9' THEN RETURN '۹';
    WHEN '0' THEN RETURN '۰';
    ELSE   RETURN iChar;
  END CASE;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `farsiMonthName` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `farsiMonthName`(
    `iMonth` TINYINT
) RETURNS varchar(100) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.1

    CASE iMonth
        WHEN 1  THEN RETURN 'فروردین';
        WHEN 2  THEN RETURN 'اردیبهشت';
        WHEN 3  THEN RETURN 'خرداد';
        WHEN 4  THEN RETURN 'تیر';
        WHEN 5  THEN RETURN 'مرداد';
        WHEN 6  THEN RETURN 'شهریور';
        WHEN 7  THEN RETURN 'مهر';
        WHEN 8  THEN RETURN 'آبان';
        WHEN 9  THEN RETURN 'آذر';
        WHEN 10 THEN RETURN 'دی';
        WHEN 11 THEN RETURN 'بهمن';
        WHEN 12 THEN RETURN 'اسفند';
        ELSE RETURN CONCAT_WS(':','ERROR',iMonth);
    END CASE;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `farsiNumerals` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `farsiNumerals`(
    `iText` VARCHAR(5000)
) RETURNS varchar(5000) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
  DECLARE PersianText VARCHAR(1000) DEFAULT '';

  AllChars: LOOP
    IF LENGTH(iText) = 0 THEN  LEAVE AllChars; END IF;

    SET PersianText = CONCAT(PersianText, I18N.farsiDigit(SUBSTR(iText,1,1)));
    SET iText = SUBSTR(iText,2);

  END LOOP;
  RETURN PersianText;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `fromJalali` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `fromJalali`(
    `iJalaliYear` smallint,
    `iJalaliMonth` TINYINT,
    `iJalaliDay` TINYINT
) RETURNS date
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012 Mehran . M . Spitman
# WebLog :spitman.azdaa.com
# Version V1.0.1

    DECLARE
        i, j, e, k, mo,
        gy, gm, gd,
        g_day_no, j_day_no, bkab, jmm, mday, g_day_mo, bkab1, j1
    INT DEFAULT 0; /* Can be unsigned int? */
    DECLARE resout char(100);
    DECLARE fdate datetime;


  SET bkab = MOD(iJalaliYear,33);

  IF (bkab = 1 or bkab= 5 or bkab = 9 or bkab = 13 or bkab = 17 or bkab = 22 or bkab = 26 or bkab = 30) THEN
    SET j=1;
  end IF;

  SET bkab1 = MOD(iJalaliYear+1,33);

  IF (bkab1 = 1 or bkab1= 5 or bkab1 = 9 or bkab1 = 13 or bkab1 = 17 or bkab1 = 22 or bkab1 = 26 or bkab1 = 30) THEN
    SET j1=1;
  end IF;

    CASE iJalaliMonth
        WHEN 1 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 2 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 3 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 4 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 5 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 6 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 7 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 8 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 9 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 10 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 11 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth) or iJalaliDay <= 0 THEN SET e=1; end IF;
        WHEN 12 THEN IF iJalaliDay > _jdmarray2(iJalaliMonth)+j or iJalaliDay <= 0 THEN SET e=1; end IF;
    END CASE;
  IF iJalaliMonth > 12 or iJalaliMonth <= 0 THEN SET e=1; end IF;
  IF iJalaliYear <= 0 THEN SET e=1; end IF;

  IF e>0 THEN
    RETURN 0;
  end IF;

  IF (iJalaliMonth>=11) or (iJalaliMonth=10 and iJalaliDay>=11 and j=0) or (iJalaliMonth=10 and iJalaliDay>11 and j=1) THEN
    SET i=1;
  end IF;
  SET gy = iJalaliYear + 621 + i;

  IF (MOD(gy,4)=0) THEN
    SET k=1;
  end IF;

    IF (MOD(gy,100)=0) and (MOD(gy,400)<>0) THEN
        SET k=0;
    END IF;

  SET jmm=iJalaliMonth-1;

  WHILE (jmm > 0) do
    SET mday=mday+I18N._jdmarray2(jmm);
    SET jmm=jmm-1;
  end WHILE;

  SET j_day_no=(iJalaliYear-1)*365+(I18N._intDiv(iJalaliYear,4))+mday+iJalaliDay;
  SET g_day_no=j_day_no+226899;


  SET g_day_no=g_day_no-(I18N._intDiv(gy-1,4));
  SET g_day_mo=MOD(g_day_no,365);

    IF (k=1 and j=1) THEN
        IF (g_day_mo=0) THEN
            RETURN CONCAT_WS('-',gy,'12','30');
        END IF;
        IF (g_day_mo=1) THEN
            RETURN CONCAT_WS('-',gy,'12','31');
        END IF;
    END IF;

    IF (g_day_mo=0) THEN
        RETURN CONCAT_WS('-',gy,'12','31');
    END IF;


  SET mo=0;
  SET gm=gm+1;
  while g_day_mo>I18N._gdmarray2(mo,k) do
        SET g_day_mo=g_day_mo-I18N._gdmarray2(mo,k);
    SET mo=mo+1;
    SET gm=gm+1;
  end WHILE;
  SET gd=g_day_mo;

  RETURN CONCAT_WS('-',gy,gm,gd);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `fromJalaliStr` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `fromJalaliStr`(
    `iJalaliDate` VARCHAR(50)
) RETURNS datetime
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012 Mehran . M . Spitman
# WebLog spitman.azdaa.com
# Version V1.0.1

    DECLARE jdd, jyd, jmd, jt, jd, jm, jy, jTime varchar(100);

  IF LOCATE(' ', iJalaliDate) THEN
    SET jTime = SUBSTRING_INDEX(iJalaliDate, ' ', -1);
    SET iJalaliDate = SUBSTRING_INDEX(iJalaliDate, ' ', 1);
  END IF;
  SET iJalaliDate = REPLACE (iJalaliDate, '/', '-');

    SET jdd = SUBSTRING_INDEX(iJalaliDate, '-', -1);
    SET jt  = SUBSTRING_INDEX(iJalaliDate, '-', 2);
    SET jyd = SUBSTRING_INDEX(jt, '-', 1);
    SET jmd = SUBSTRING_INDEX(jt, '-', -1);
    SET jd  = CAST(jdd AS UNSIGNED);
    SET jy  = CAST(jyd AS UNSIGNED);
    SET jm  = CAST(jmd AS UNSIGNED);

  RETURN CONCAT_WS(' ',I18N.fromJalali(jy,jm,jd), jTime);

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `jalaliDateTime` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `jalaliDateTime`(
    `iDateTime` DATETIME

) RETURNS varchar(50) CHARSET utf8mb4
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

    DECLARE
        i,
        gy, gm, gd,
        g_day_no, j_day_no, j_np,
        jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */
    DECLARE resout char(100);
    DECLARE ttime CHAR(20);

    SET gy = YEAR(iDateTime) - 1600;
    SET gm = MONTH(iDateTime) - 1;
    SET gd = DAY(iDateTime) - 1;
    SET ttime = TIME(iDateTime);
    SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - I18N._intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
    SET i = 0;

    WHILE (i < gm) do
        SET g_day_no = g_day_no + I18N._gdmarray(i);
        SET i = i + 1;
    END WHILE;

    IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN
        SET g_day_no =	g_day_no + 1;
    END IF;

    SET g_day_no = g_day_no + gd;
    SET j_day_no = g_day_no - 79;
    SET j_np = j_day_no DIV 12053;
    SET j_day_no = j_day_no % 12053;
    SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
    SET j_day_no = j_day_no % 1461;

    IF j_day_no >= 366 then
        SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
        SET j_day_no = (j_day_no - 1) % 365;
    END IF;

    SET i = 0;

    WHILE (i < 11 and j_day_no >= I18N._jdmarray(i)) do
        SET j_day_no = j_day_no - I18N._jdmarray(i);
        SET i = i + 1;
    END WHILE;

    SET jm = i + 1;
    SET jd = j_day_no + 1;
    SET resout = CONCAT_WS ('-', jy, CONCAT(IF(jm<10,'0',''), jm), CONCAT(IF(jd<10,'0',''), jd));

    IF (ttime <> '00:00:00') then
        SET resout = CONCAT_WS(' ', resout, ttime);
    END IF;

    RETURN resout;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `jalaliDay` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `jalaliDay`(
    `iDate` DATE


) RETURNS tinyint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

    DECLARE
        i,
        gy, gm, gd,
        g_day_no, j_day_no, j_np,
        jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */

    SET gy = YEAR(iDate) - 1600;
    SET gm = MONTH(iDate) - 1;
    SET gd = DAY(iDate) - 1;
    SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - _intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
    SET i = 0;

    WHILE (i < gm) do
        SET g_day_no = g_day_no + I18N._gdmarray(i);
        SET i = i + 1;
    END WHILE;

    IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN
        SET g_day_no =	g_day_no + 1;
    END IF;

    SET g_day_no = g_day_no + gd;
    SET j_day_no = g_day_no - 79;
    SET j_np = j_day_no DIV 12053;
    SET j_day_no = j_day_no % 12053;
    SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
    SET j_day_no = j_day_no % 1461;

    IF j_day_no >= 366 then
        SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
        SET j_day_no = (j_day_no - 1) % 365;
    END IF;

    SET i = 0;

    WHILE (i < 11 and j_day_no >= I18N._jdmarray(i)) do
        SET j_day_no = j_day_no - I18N._jdmarray(i);
        SET i = i + 1;
    END WHILE;

    SET jm = i + 1;
    SET jd = j_day_no + 1;

    RETURN jd;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `jalaliMonth` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `jalaliMonth`(
    `iDate` DATE
) RETURNS tinyint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

    DECLARE
        i,
        gy, gm, gd,
        g_day_no, j_day_no, j_np,
        jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */

    SET gy = YEAR(iDate) - 1600;
    SET gm = MONTH(iDate) - 1;
    SET gd = DAY(iDate) - 1;
    SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - I18N._intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
    SET i = 0;

    WHILE (i < gm) do
        SET g_day_no = g_day_no + I18N._gdmarray(i);
        SET i = i + 1;
    END WHILE;

    IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN
        SET g_day_no =	g_day_no + 1;
    END IF;

    SET g_day_no = g_day_no + gd;
    SET j_day_no = g_day_no - 79;
    SET j_np = j_day_no DIV 12053;
    SET j_day_no = j_day_no % 12053;
    SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
    SET j_day_no = j_day_no % 1461;

    IF j_day_no >= 366 then
        SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
        SET j_day_no = (j_day_no - 1) % 365;
    END IF;

    SET i = 0;

    WHILE (i < 11 and j_day_no >= I18N._jdmarray(i)) do
        SET j_day_no = j_day_no - I18N._jdmarray(i);
        SET i = i + 1;
    END WHILE;

    SET jm = i + 1;
--	SET jd = j_day_no + 1;

    RETURN jm;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `jalaliYear` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `jalaliYear`(
    `iDate` DATE

) RETURNS smallint unsigned
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

    DECLARE
        i,
        gy, gm, gd,
        g_day_no, j_day_no, j_np,
        jy, jm, jd INT DEFAULT 0; /* Can be unsigned int? */

    SET gy = YEAR(iDate) - 1600;
    SET gm = MONTH(iDate) - 1;
    SET gd = DAY(iDate) - 1;
    SET g_day_no = ((365 * gy) + I18N._intDiv(gy + 3, 4) - I18N._intDiv(gy + 99, 100) + I18N._intDiv (gy + 399, 400));
    SET i = 0;

    WHILE (i < gm) do
        SET g_day_no = g_day_no + I18N._gdmarray(i);
        SET i = i + 1;
    END WHILE;

    IF gm > 1 and ((gy % 4 = 0 and gy % 100 <> 0)) or gy % 400 = 0 THEN
        SET g_day_no =	g_day_no + 1;
    END IF;

    SET g_day_no = g_day_no + gd;
    SET j_day_no = g_day_no - 79;
    SET j_np = j_day_no DIV 12053;
    SET j_day_no = j_day_no % 12053;
    SET jy = 979 + 33 * j_np + 4 * I18N._intDiv(j_day_no, 1461);
    SET j_day_no = j_day_no % 1461;

    IF j_day_no >= 366 then
        SET jy = jy + I18N._intDiv(j_day_no - 1, 365);
    END IF;

    RETURN jy;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `_gdmarray` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `_gdmarray`(
    `m` smallint
) RETURNS smallint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.1

    CASE m
        WHEN 0  THEN RETURN 31;
        WHEN 1  THEN RETURN 28;
        WHEN 2  THEN RETURN 31;
        WHEN 3  THEN RETURN 30;
        WHEN 4  THEN RETURN 31;
        WHEN 5  THEN RETURN 30;
        WHEN 6  THEN RETURN 31;
        WHEN 7  THEN RETURN 31;
        WHEN 8  THEN RETURN 30;
        WHEN 9  THEN RETURN 31;
        WHEN 10 THEN RETURN 30;
        WHEN 11 THEN RETURN 31;
    END CASE;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `_gdmarray2` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `_gdmarray2`(
    `m` smallint,
    `k` SMALLINT
) RETURNS tinyint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012  Mehran . M . Spitman
# WebLog :spitman.azdaa.com
# Version V1.0

    CASE m
        WHEN 0  THEN RETURN 31;
        WHEN 1  THEN RETURN 28+k;
        WHEN 2  THEN RETURN 31;
        WHEN 3  THEN RETURN 30;
        WHEN 4  THEN RETURN 31;
        WHEN 5  THEN RETURN 30;
        WHEN 6  THEN RETURN 31;
        WHEN 7  THEN RETURN 31;
        WHEN 8  THEN RETURN 30;
        WHEN 9  THEN RETURN 31;
        WHEN 10 THEN RETURN 30;
        WHEN 11 THEN RETURN 31;
    END CASE;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `_intDiv` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `_intDiv`(
    `a` int,
    `b` int
) RETURNS bigint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.2

    return FLOOR(a / b);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `_jdmarray` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `_jdmarray`(
    `m` smallint
) RETURNS smallint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2009-2012 Mohammad Saleh Souzanchi
# WebLog : www.saleh.soozanchi.ir
# Version V1.0.1

    CASE m
        WHEN 0  THEN RETURN 31;
        WHEN 1  THEN RETURN 31;
        WHEN 2  THEN RETURN 31;
        WHEN 3  THEN RETURN 31;
        WHEN 4  THEN RETURN 31;
        WHEN 5  THEN RETURN 31;
        WHEN 6  THEN RETURN 30;
        WHEN 7  THEN RETURN 30;
        WHEN 8  THEN RETURN 30;
        WHEN 9  THEN RETURN 30;
        WHEN 10 THEN RETURN 30;
        WHEN 11 THEN RETURN 29;
    END CASE;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `_jdmarray2` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE FUNCTION `_jdmarray2`(`m` smallint) RETURNS smallint
    NO SQL
    DETERMINISTIC
BEGIN
# Copyright (C) 2011-2012 Mehran . M . Spitman
# WebLog :spitman.azdaa.com
# Version V1.0.1

    CASE m
        WHEN 1 THEN RETURN 31;
        WHEN 2 THEN RETURN 31;
        WHEN 3 THEN RETURN 31;
        WHEN 4 THEN RETURN 31;
        WHEN 5 THEN RETURN 31;
        WHEN 6 THEN RETURN 31;
        WHEN 7 THEN RETURN 30;
        WHEN 8 THEN RETURN 30;
        WHEN 9 THEN RETURN 30;
        WHEN 10 THEN RETURN 30;
        WHEN 11 THEN RETURN 30;
        WHEN 12 THEN RETURN 29;
    END CASE;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
