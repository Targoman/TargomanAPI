#!/bin/sh

echo copying TargomanAPI
docker cp out/bin/TargomanAPI activeAPI:/targoman/bin/
echo copying modules
docker cp out/modules activeAPI:/targoman/
echo copying common library 
docker cp out/lib64 activeAPI:/targoman/
echo copying common library 
docker cp conf activeAPI:/targoman/

echo Restarting 
docker restart activeAPI
echo "Ready!!!"
