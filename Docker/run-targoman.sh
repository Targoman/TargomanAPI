#!/bin/sh

export LD_LIBRARY_PATH=/targoman/lib64:/targoman/lib
cd /targoman
./bin/targomanAPI -c ./conf/api.conf --plugins ./modules
