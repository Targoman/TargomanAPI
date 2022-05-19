#!/bin/bash
# Migration File: m20220501_164741_Ticketing_mark_objectstorage_db_schema_creation.sh
# use $@ for MIGRATIONTOOL and necessary arguments passed from migration runner
#     e.g.: $@ --command mark --migration-name ...

if [ $# -ge 1 ]; then
  case $1 in
    "checkbc")
      echo "checking backward compatibility"
      # exit 0 if this script is not backward compatible
      exit 1
    ;;
  esac
fi

$@ --command mark \
   --migration-name m20220501_164826_Interfaces_ObjectStorage_init_objectstorage_schema.sql \
   --project Ticketing
