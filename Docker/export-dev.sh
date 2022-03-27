#!/bin/sh

echo copying TargomanAPI
docker cp ../out/bin activeAPI:/targoman/.
docker cp run-targoman-dev.sh activeAPI:/.
echo copying modules
docker cp ../out/modules activeAPI:/targoman/.
echo copying library 
docker cp ../out/lib64 activeAPI:/targoman/.
echo copying config files
docker cp ../conf/TextProcessor/. activeAPI:/targoman/conf/TextProcessor/.
echo copying swaggerui files
docker cp ../SwaggerUI activeAPI:/targoman/.

echo Restarting 
#sudo docker restart activeAPI
echo "Ready!!!"
