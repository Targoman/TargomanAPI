/* Migration File: m20220501_201315_Interfaces_ObjectStorage_updating.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

ALTER TABLE `tblUploadGateways`
    ADD COLUMN `ugwName` VARCHAR(50) NULL DEFAULT 'A' AFTER `ugwID`;

ALTER TABLE `tblUploadFiles`
    CHANGE COLUMN `uflFileName` `uflOriginalFileName` VARCHAR(256) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `uflID`,
    ADD COLUMN `uflStoredFileName` VARCHAR(256) NULL AFTER `uflOriginalFileName`,
    ADD COLUMN `uflCounter` MEDIUMINT UNSIGNED NULL AFTER `uflStoredFileName`;

ALTER TABLE `tblUploadFiles`
    CHANGE COLUMN `uflFileUUID` `uflPath` VARCHAR(256) NULL COLLATE 'utf8mb4_general_ci' AFTER `uflCounter`;

ALTER TABLE `tblUploadFiles`
    CHANGE COLUMN `uflLocalFullFileName` `uflLocalFullFileName` TEXT NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `uflMimeType`;

UPDATE tblUploadFiles
    SET uflStoredFileName = uflOriginalFileName
    WHERE uflStoredFileName IS NULL;

ALTER TABLE `tblUploadFiles`
    CHANGE COLUMN `uflPath` `uflPath` VARCHAR(64) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci' AFTER `uflID`,
    CHANGE COLUMN `uflCounter` `uflCounter` MEDIUMINT(7) UNSIGNED NULL DEFAULT NULL AFTER `uflOriginalFileName`;

UPDATE tblUploadFiles
    SET uflCreatedBy_usrID = 0
    WHERE uflCreatedBy_usrID IS NULL;

ALTER TABLE `tblUploadFiles`
    CHANGE COLUMN `uflCreatedBy_usrID` `uflCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL AFTER `uflCreationDateTime`;

UPDATE tblUploadFiles
    SET uflPath = CONCAT('user_', uflCreatedBy_usrID, '/', uflPath)
    WHERE uflPath IS NOT NULL;

UPDATE tblUploadFiles
    SET uflPath = CONCAT('user_', uflCreatedBy_usrID)
    WHERE uflPath IS NULL;

ALTER TABLE `tblUploadFiles`
    CHANGE COLUMN `uflPath` `uflPath` VARCHAR(256) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `uflID`;

ALTER TABLE `tblUploadFiles`
    ADD uflUniqueMD5 char(32) AS
        (MD5(CONCAT_WS('X',
            IFNULL(uflPath, ''),
            uflOriginalFileName,
            IFNULL(uflCounter, 0),
            uflCreatedBy_usrID
        )))
    UNIQUE;

ALTER TABLE `tblUploadQueue`
    ADD COLUMN `uquLockedAt` TIMESTAMP NULL AFTER `uqu_ugwID`,
    ADD COLUMN `uquLastTryAt` TIMESTAMP NULL AFTER `uquLockedAt`,
    ADD COLUMN `uquStoredAt` TIMESTAMP NULL AFTER `uquLastTryAt`;

DROP FUNCTION IF EXISTS `fnApplyCounterToFileName`;
DELIMITER ;;
CREATE FUNCTION `fnApplyCounterToFileName`(
    `iFileName` TEXT,
    `iCounter` INT UNSIGNED
)
RETURNS TEXT CHARSET utf8mb4
LANGUAGE SQL
NOT DETERMINISTIC
READS SQL DATA
SQL SECURITY DEFINER
BEGIN
    DECLARE vName TEXT;
    DECLARE vExt TEXT;
    DECLARE vResult TEXT;

/*
        ORG             NAME        EXT     RESULT
    -----------------------------------------------------------
    1/  aaa              aaa                 aaa (1)
    2/ .aaa                         aaa     .aaa (2)
    3/  aaa.txt          aaa        txt      aaa (3).txt
    4/ .aaa.txt         .aaa        txt     .aaa (4).txt
    5/  aaa.txt.log      aaa.txt    log      aaa.txt (5).log
    6/ .aaa.txt.log     .aaa.txt    log     .aaa.txt (6).log

    SELECT fnApplyCounterToFileName('aaa', 1)
    UNION
    SELECT fnApplyCounterToFileName('.aaa', 2)
    UNION
    SELECT fnApplyCounterToFileName('aaa.txt', 3)
    UNION
    SELECT fnApplyCounterToFileName('.aaa.txt', 4)
    UNION
    SELECT fnApplyCounterToFileName('aaa.txt.log', 5)
    UNION
    SELECT fnApplyCounterToFileName('.aaa.txt.log', 6)
    ;
*/

    SELECT SUBSTRING_INDEX(iFileName, '.', -1) INTO vExt;

    IF (LOCATE('.', iFileName) = 0) OR (LENGTH(vExt)+1 = LENGTH(iFileName)) THEN -- 1, 2
        SET vResult = CONCAT(iFileName, ' (', iCounter, ')');
    ELSE
        SET vName = LEFT(iFileName, LENGTH(iFileName) - LENGTH(vExt) - 1);
        SET vResult = CONCAT(vName, ' (', iCounter, ')', '.', vExt);
    END IF;

    RETURN vResult;

END;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `spUploadedFile_Create`;
DELIMITER ;;
CREATE PROCEDURE `spUploadedFile_Create`(
    IN `iPath` VARCHAR(256),
    IN `iOriginalFileName` VARCHAR(256),
    IN `iFullTempPath` VARCHAR(512),
    IN `iFileSize` BIGINT UNSIGNED,
    IN `iFileType` VARCHAR(64),
    IN `iMimeType` VARCHAR(128),
    IN `iCreatorUserID` BIGINT UNSIGNED,
    IN `iLocked` TINYINT,
    OUT `oStoredFileName` VARCHAR(256),
    OUT `oUploadedFileID` BIGINT UNSIGNED,
    OUT `oQueueRowsCount` INT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vUploadedFileCounter BIGINT UNSIGNED;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1 vErr = MESSAGE_TEXT;

        /****************/
        ROLLBACK;
        /****************/

        INSERT INTO tblActionLogs
           SET tblActionLogs.atlBy_usrID = iCreatorUserID,
               tblActionLogs.atlType = 'Upload.Error',
               tblActionLogs.atlDescription = JSON_OBJECT(
                   "err",                   vErr,
                   "iPath",                 iPath,
                   "iOriginalFileName",     iOriginalFileName,
                   "iFileSize",             iFileSize,
                   "iFileType",             iFileType,
                   "iMimeType",             iMimeType,
                   "iFullTempPath",         iFullTempPath,
                   "iCreatorUserID",        iCreatorUserID,
                   "UploadedFileCounter",   vUploadedFileCounter,
                   "StoredFileName",        oStoredFileName,
                   "FileID",                oUploadedFileID,
                   "QueuedCount",           oQueueRowsCount
               )
        ;

        RESIGNAL;
    END;

    /****************/
    START TRANSACTION;
    /****************/

    SET vUploadedFileCounter = NULL;

    SELECT MAX(IFNULL(uflCounter, 0))
      INTO vUploadedFileCounter
      FROM tblUploadFiles
     WHERE IFNULL(tblUploadFiles.uflPath, '') = IFNULL(iPath, '')
       AND tblUploadFiles.uflOriginalFileName = iOriginalFileName
       AND tblUploadFiles.uflCreatedBy_usrID = iCreatorUserID
    ;

    IF ISNULL(vUploadedFileCounter) THEN
        SET oStoredFileName = iOriginalFileName;
    ELSE
        SET vUploadedFileCounter = vUploadedFileCounter + 1;
        SET oStoredFileName = fnApplyCounterToFileName(iOriginalFileName, vUploadedFileCounter);
    END IF;

    INSERT INTO tblUploadFiles
       SET tblUploadFiles.uflPath = iPath,
           tblUploadFiles.uflOriginalFileName = iOriginalFileName,
           tblUploadFiles.uflCounter = vUploadedFileCounter,
           tblUploadFiles.uflStoredFileName = oStoredFileName,
           tblUploadFiles.uflSize = iFileSize,
           tblUploadFiles.uflFileType = iFileType,
           tblUploadFiles.uflMimeType = iMimeType,
           tblUploadFiles.uflLocalFullFileName = CONCAT(iFullTempPath, '/', oStoredFileName),
           tblUploadFiles.uflCreatedBy_usrID = iCreatorUserID
    ;
    SET oUploadedFileID = LAST_INSERT_ID();

    INSERT INTO tblUploadQueue(
           uqu_uflID
         , uqu_ugwID
         , uquLockedAt
           )
    SELECT oUploadedFileID
         , tblUploadGateways.ugwID
         , IF(iLocked, NOW(), NULL)
      FROM tblUploadGateways
     WHERE tblUploadGateways.ugwStatus = 'A'
       AND (tblUploadGateways.ugwAllowedFileTypes IS NULL
        OR LOWER(tblUploadGateways.ugwAllowedFileTypes) LIKE CONCAT('%', iFileType, '%')
           )
       AND (tblUploadGateways.ugwAllowedMimeTypes IS NULL
        OR LOWER(tblUploadGateways.ugwAllowedMimeTypes) LIKE CONCAT('%', iMimeType, '%')
           )
       AND (tblUploadGateways.ugwAllowedMinFileSize IS NULL
        OR tblUploadGateways.ugwAllowedMinFileSize <= iFileSize
           )
       AND (tblUploadGateways.ugwAllowedMaxFileSize IS NULL
        OR tblUploadGateways.ugwAllowedMaxFileSize >= iFileSize
           )
       AND (tblUploadGateways.ugwMaxFilesCount IS NULL
        OR tblUploadGateways.ugwMaxFilesCount < tblUploadGateways.ugwCreatedFilesCount - tblUploadGateways.ugwDeletedFilesCount
           )
       AND (tblUploadGateways.ugwMaxFilesSize IS NULL
        OR tblUploadGateways.ugwMaxFilesSize <= tblUploadGateways.ugwCreatedFilesSize - tblUploadGateways.ugwDeletedFilesSize + iFileSize
           )
    ;
    SET oQueueRowsCount = ROW_COUNT();

    /* this is for next version
    IF (oQueueRowsCount > 0) THEN
        UPDATE tblUploadFiles
           SET tblUploadFiles.uflStatus = 'Q'
         WHERE tblUploadFiles.uflID = oUploadedFileID
        ;
    END IF;
    */

    INSERT INTO tblActionLogs
       SET tblActionLogs.atlBy_usrID = iCreatorUserID,
           tblActionLogs.atlType = 'UploadFile.Success',
           tblActionLogs.atlDescription = JSON_OBJECT(
               "iPath",                 iPath,
               "iOriginalFileName",     iOriginalFileName,
               "iFileSize",             iFileSize,
               "iFileType",             iFileType,
               "iMimeType",             iMimeType,
               "iFullTempPath",         iFullTempPath,
               "iCreatorUserID",        iCreatorUserID,
               "UploadedFileCounter",   vUploadedFileCounter,
               "StoredFileName",        oStoredFileName,
               "FileID",                oUploadedFileID,
               "QueuedCount",           oQueueRowsCount
           )
    ;

    /****************/
    COMMIT;
    /****************/
END;;
DELIMITER ;
