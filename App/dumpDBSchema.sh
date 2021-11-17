#!/bin/bash

mysqldump \
    --tables --routines --triggers --events \
    --no-data --skip-comments --add-drop-trigger \
    --protocol=tcp --user=root --password=targoman123 \
    Common \
    | sed 's/ AUTO_INCREMENT=[0-9]*//g' > Schemas/Common.my.sql

mysqldump \
    --tables --routines --triggers --events \
    --no-data --skip-comments --add-drop-trigger \
    --protocol=tcp --user=root --password=targoman123 \
    CommonFuncs \
    | sed 's/ AUTO_INCREMENT=[0-9]*//g' > Schemas/CommonFuncs.my.sql

mysqldump \
    --tables --routines --triggers --events \
    --no-data --skip-comments --add-drop-trigger \
    --protocol=tcp --user=root --password=targoman123 \
    I18N \
    | sed 's/ AUTO_INCREMENT=[0-9]*//g' > Schemas/I18N.my.sql
