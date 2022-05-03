#!/bin/bash

export LD_LIBRARY_PATH=out/lib:out/lib64
AWS_EC2_METADATA_DISABLED=true out/bin/targomanAPI --plugins out/modules -c conf/api.conf --dbprefix dev_ $@
