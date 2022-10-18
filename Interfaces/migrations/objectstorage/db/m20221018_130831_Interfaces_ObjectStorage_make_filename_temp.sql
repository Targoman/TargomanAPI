/* Migration File: m20221018_130831_Interfaces_ObjectStorage_make_filename_temp.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

USE `{{dbprefix}}{{Schema}}`;

DROP PROCEDURE IF EXISTS `spUploadedFile_Create`;
DELIMITER //
CREATE PROCEDURE `spUploadedFile_Create`(
    IN `iPath` VARCHAR(256),
    IN `iOriginalFileName` VARCHAR(256),
    IN `iFullTempPath` VARCHAR(512),
    IN `iSetTempFileNameToMD5` TINYINT,
    IN `iFileSize` BIGINT UNSIGNED,
    IN `iFileType` VARCHAR(64),
    IN `iMimeType` VARCHAR(128),
    IN `iCreatorUserID` BIGINT UNSIGNED,
    IN `iLockedBy` VARCHAR(50),
    OUT `oStoredFileName` VARCHAR(256),
    OUT `oTempFileName` VARCHAR(256),
    OUT `oUploadedFileID` BIGINT UNSIGNED,
    OUT `oQueueRowsCount` INT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);
    DECLARE vUploadedFileCounter BIGINT UNSIGNED;
    DECLARE vExt VARCHAR(500);

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
                   "iLockedBy",             iLockedBy,
                   "UploadedFileCounter",   vUploadedFileCounter,
                   "StoredFileName",        oStoredFileName,
                   "oTempFileName",         oTempFileName,
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

    IF iSetTempFileNameToMD5 = 1 THEN
        SET oTempFileName = {{dbprefix}}Common.fnCreateRandomMD5(); -- MD5(oStoredFileName);

        SELECT SUBSTRING_INDEX(oStoredFileName, '.', -1) INTO vExt;

        IF (LOCATE('.', oStoredFileName) != 0) AND (LENGTH(vExt)+1 != LENGTH(oStoredFileName)) THEN
            SET oTempFileName = CONCAT(oTempFileName, '.', vExt);
        END IF;
    ELSE
        SET oTempFileName = oStoredFileName;
    END IF;

    INSERT INTO tblUploadFiles
       SET tblUploadFiles.uflPath = iPath,
           tblUploadFiles.uflOriginalFileName = iOriginalFileName,
           tblUploadFiles.uflCounter = vUploadedFileCounter,
           tblUploadFiles.uflStoredFileName = oStoredFileName,
           tblUploadFiles.uflSize = iFileSize,
           tblUploadFiles.uflFileType = iFileType,
           tblUploadFiles.uflMimeType = iMimeType,
           tblUploadFiles.uflLocalFullFileName = CONCAT(iFullTempPath, '/', oTempFileName),
           tblUploadFiles.uflCreatedBy_usrID = iCreatorUserID
    ;
    SET oUploadedFileID = LAST_INSERT_ID();

    INSERT INTO tblUploadQueue(
           uqu_uflID
         , uqu_ugwID
         , uquLockedAt
         , uquLockedBy
         , uquCreatedBy_usrID
           )
    SELECT oUploadedFileID
         , tblUploadGateways.ugwID
         , IF(iLockedBy IS NULL OR iLockedBy='', NULL, NOW())
         , IF(iLockedBy IS NULL OR iLockedBy='', NULL, iLockedBy)
         , iCreatorUserID
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
               "iLockedBy",             iLockedBy,
               "UploadedFileCounter",   vUploadedFileCounter,
               "StoredFileName",        oStoredFileName,
               "oTempFileName",         oTempFileName,
               "FileID",                oUploadedFileID,
               "QueuedCount",           oQueueRowsCount
           )
    ;

    /****************/
    COMMIT;
    /****************/
END//
DELIMITER ;
