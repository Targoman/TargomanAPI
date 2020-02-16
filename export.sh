#!/bin/sh

echo copying TargomanAPI
docker cp out/bin/TargomanAPI activeAPI:/targoman/bin/
echo copying modules
docker cp out/modules activeAPI:/targoman/
echo copying library 
docker cp out/lib64 activeAPI:/targoman/
echo copying common library 
home=/home/user/

docker cp $home/local/lib64/libQFieldValidator.so.1.0.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libQFieldValidator.so.1.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libQFieldValidator.so.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libQFieldValidator.so activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libTargomanDBM.so.1.0.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libTargomanDBM.so.1.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libTargomanDBM.so.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libTargomanDBM.so activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libURLCanonicalizer.so.1.0.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libURLCanonicalizer.so.1.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libURLCanonicalizer.so.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libURLCanonicalizer.so activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libqhttp.so.4.0.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libqhttp.so.4.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libqhttp.so.4 activeAPI:/targoman/lib64/
docker cp $home/local/lib64/libqhttp.so activeAPI:/targoman/lib64/
docker cp $home/local/lib/libfasttext.so activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanCommon.so.1.0.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanCommon.so.1.0 activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanCommon.so.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanCommon.so activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanTextProcessor.so.1.1.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanTextProcessor.so.1.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanTextProcessor.so.1 activeAPI:/targoman/lib64/
docker cp $home/local/lib/libTargomanTextProcessor.so activeAPI:/targoman/lib64/

echo Restarting 
#docker restart activeAPI
echo "Ready!!!"
