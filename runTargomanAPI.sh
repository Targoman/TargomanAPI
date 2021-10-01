#!/bin/bash

cd out/bin
export LD_LIBRARY_PATH=../lib:../lib64
./targomanAPI --plugins ../modules -c ../../conf/api.conf

