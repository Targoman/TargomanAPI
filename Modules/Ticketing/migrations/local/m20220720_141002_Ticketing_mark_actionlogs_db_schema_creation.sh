#!/bin/bash
# Migration File: m20220720_141002_Ticketing_mark_actionlogs_db_schema_creation.sh

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
   --migration-name m20220720_154650_Interfaces_ActionLogs_init_schema.sql \
   --project Ticketing

