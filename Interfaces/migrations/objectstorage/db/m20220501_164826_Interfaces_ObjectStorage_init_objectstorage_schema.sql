/* Migration File: m20220501_164826_Interfaces_ObjectStorage_init_objectstorage_schema.sql */
/* CAUTION: don't forget to use {{dbprefix}} for schemas */

CREATE TABLE `tblUploadGateways` (
    `ugwID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `ugwType` CHAR(1) NOT NULL COMMENT '3:AWS_S3, N:NFS' COLLATE 'utf8mb4_general_ci',
    `ugwMetaInfo` JSON NOT NULL,
    `ugwAllowedFileTypes` VARCHAR(512) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `ugwAllowedMimeTypes` VARCHAR(1024) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `ugwAllowedMinFileSize` BIGINT(19) NULL DEFAULT NULL,
    `ugwAllowedMaxFileSize` BIGINT(19) NULL DEFAULT NULL,
    `ugwMaxFilesCount` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `ugwMaxFilesSize` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `ugwCreatedFilesCount` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0',
    `ugwCreatedFilesSize` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0',
    `ugwDeletedFilesCount` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0',
    `ugwDeletedFilesSize` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0',
    `ugwLastActionTime` DATETIME NULL DEFAULT NULL,
    `ugwStatus` CHAR(1) NOT NULL DEFAULT 'A' COMMENT 'A:Active, D:Disabled, R:Removed' COLLATE 'utf8mb4_general_ci',
    `ugwCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `ugwCreatedBy_usrID` BIGINT(20) UNSIGNED NOT NULL,
    `ugwUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`ugwID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `tblUploadFiles` (
    `uflID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `uflFileName` VARCHAR(256) NOT NULL COLLATE 'utf8mb4_general_ci',
    `uflFileUUID` VARCHAR(64) NOT NULL COLLATE 'utf8mb4_general_ci',
    `uflSize` BIGINT(20) UNSIGNED NOT NULL,
    `uflFileType` VARCHAR(64) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `uflMimeType` VARCHAR(128) NULL DEFAULT NULL COLLATE 'utf8mb4_general_ci',
    `uflLocalFullFileName` VARCHAR(1024) NOT NULL COLLATE 'utf8mb4_general_ci',
    `uflStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N:New, R:Removed' COLLATE 'utf8mb4_general_ci',
    `uflCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `uflCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `uflUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`uflID`) USING BTREE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `tblUploadQueue` (
    `uquID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `uqu_uflID` BIGINT(20) UNSIGNED NOT NULL,
    `uqu_ugwID` INT(10) UNSIGNED NOT NULL,
    `uquStatus` CHAR(1) NOT NULL DEFAULT 'N' COMMENT 'N:New, U:Uploading, S:Stored, R:Removed' COLLATE 'utf8mb4_general_ci',
    `uquCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `uquCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `uquUpdatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`uquID`) USING BTREE,
    INDEX `FK_tblUploadQueue_tblUploadFiles` (`uqu_uflID`) USING BTREE,
    INDEX `FK_tblUploadQueue_tblUploadGateways` (`uqu_ugwID`) USING BTREE,
    CONSTRAINT `FK_tblUploadQueue_tblUploadFiles` FOREIGN KEY (`uqu_uflID`) REFERENCES `tblUploadFiles` (`uflID`) ON UPDATE NO ACTION ON DELETE CASCADE,
    CONSTRAINT `FK_tblUploadQueue_tblUploadGateways` FOREIGN KEY (`uqu_ugwID`) REFERENCES `tblUploadGateways` (`ugwID`) ON UPDATE NO ACTION ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

DELIMITER ;;
CREATE PROCEDURE `spUploadedFile_Create`(
    IN `iFileName` VARCHAR(256),
    IN `iFileUUID` VARCHAR(64),
    IN `iFileSize` BIGINT UNSIGNED,
    IN `iFileType` VARCHAR(64),
    IN `iMimeType` VARCHAR(128),
    IN `iFullFileName` VARCHAR(1024),
    IN `iCreatorUserID` BIGINT UNSIGNED,
    IN `iQueueStatus` CHAR(1),
    OUT `oUploadedFileID` BIGINT UNSIGNED,
    OUT `oQueueRowsCount` INT UNSIGNED
)
BEGIN
    DECLARE vErr VARCHAR(500);

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
                   "err",            vErr,
                   "iFileName",      iFileName,
                   "iFileUUID",      iFileUUID,
                   "iFileSize",      iFileSize,
                   "iFileType",      iFileType,
                   "iMimeType",      iMimeType,
                   "iFullFileName",  iFullFileName,
                   "iCreatorUserID", iCreatorUserID
               )
        ;

        RESIGNAL;
    END;

    /****************/
    START TRANSACTION;
    /****************/

    INSERT INTO tblUploadFiles
       SET tblUploadFiles.uflFileName = iFileName,
           tblUploadFiles.uflFileUUID = iFileUUID,
           tblUploadFiles.uflSize = iFileSize,
           tblUploadFiles.uflFileType = iFileType,
           tblUploadFiles.uflMimeType = iMimeType,
           tblUploadFiles.uflLocalFullFileName = iFullFileName,
           tblUploadFiles.uflStatus = 'N'
    ;
    SET oUploadedFileID = LAST_INSERT_ID();

    INSERT INTO tblUploadQueue(
           uqu_uflID
         , uqu_ugwID
         , uquStatus
           )
    SELECT oUploadedFileID
         , tblUploadGateways.ugwID
         , iQueueStatus
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
               "iFileName",      iFileName,
               "iFileUUID",      iFileUUID,
               "iFileSize",      iFileSize,
               "iFileType",      iFileType,
               "iMimeType",      iMimeType,
               "iFullFileName",  iFullFileName,
               "iCreatorUserID", iCreatorUserID,
               "FileID",         oUploadedFileID,
               "QueuedCount",    oQueueRowsCount
           )
    ;

    /****************/
    COMMIT;
    /****************/
END;;
DELIMITER ;
