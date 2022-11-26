#!/bin/bash

export LD_LIBRARY_PATH=../out/lib:../out/lib64
export AWS_EC2_METADATA_DISABLED=true

#valgrind --log-file=val.log --show-leak-kinds=all --leak-check=full --undef-value-errors=yes

valgrind --leak-check=full --undef-value-errors=yes --log-file=val.log \
	../out/bin/targomanAPI --plugins ../out/modules -c ../conf/api.conf --dbprefix dev_ $@
