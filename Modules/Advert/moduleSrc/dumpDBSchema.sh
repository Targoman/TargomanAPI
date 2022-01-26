#!/bin/bash

mysqldump \
    --tables --routines --triggers --events \
    --no-data --skip-comments --add-drop-trigger \
    --protocol=tcp --user=root --password=targoman123 \
<<<<<<< Updated upstream
    -B Advert \
    | sed 's/ AUTO_INCREMENT=[0-9]*//g' \
    | sed 's/ DEFINER=`root`@`%`//g' \
    > ORM/Schema.my.sql
=======
    Advert \
    | sed 's/ AUTO_INCREMENT=[0-9]*//g' > ORM/Schema.my.sql
>>>>>>> Stashed changes
