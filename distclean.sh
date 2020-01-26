#!/bin/bash
  
for file in $(ls 3rdParty) ;
do
        pushd 3rdParty/$file
        make distclean
        popd
done
make distclean
