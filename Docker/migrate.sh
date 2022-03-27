#!/bin/bash

export LD_LIBRARY_PATH=lib:lib64
./bin/migrationTool -c conf/migration.conf -r prod $@
