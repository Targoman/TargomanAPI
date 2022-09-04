#!/bin/sh

./migrate-dev.sh --command commit --all

export LD_LIBRARY_PATH=/targoman/lib64:/targoman/lib
cd /targoman
AWS_EC2_METADATA_DISABLED=true ./bin/targomanAPI -c ./conf/api.conf --plugins ./modules --dbprefix dev_
