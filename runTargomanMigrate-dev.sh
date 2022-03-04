#!/bin/bash

export LD_LIBRARY_PATH=out/lib:out/lib64
out/bin/targomanMigrate -c conf/migration.conf -r dev $@
