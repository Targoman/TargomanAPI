#!/bin/sh

export LD_LIBRARY_PATH=/targoman/lib64:/targoman/lib
/targoman/bin/targomanAPI -c /targoman/conf/api.conf --plugins /targoman/modules
