#!/bin/bash
token=$1
project=$2
tag=$3
deploy_url=$4

declare -i counter success
counter=0
success=0

curl -s "$deploy_url/pull?token=$token&project=$project&tag=$tag" > /dev/null 2>&1
sleep 6m
while [ $counter -lt 120 ];do
    curl -s "$deploy_url/check-pull?token=$token&project=$project&tag=$tag" | grep pulled > /dev/null 2>&1
    if [ $? -eq 0 ]
    then
        success=1
        echo -e "\n------------# $project:$tag has been successfully pulled. #-----------"
        break
    else
        sleep 1
        counter=$counter+1
    fi
done
if [ $success -eq 1 ]
then
    curl -s "$deploy_url/deploy?token=$token&project=$project&tag=$tag" | grep successfully > /dev/null 2>&1
    if [ $? -eq 0 ]
    then
        echo -e "\n------------# $project:$tag has been successfully deployed. #-----------"
        curl -s "$deploy_url/upgrade?token=$token&project=$project&tag=$tag" | grep successfully > /dev/null 2>&1
        if [ $? -eq 0 ]
        then
            echo -e "\n------------# $project:$tag has been successfully upgrade. #-----------"
            exit 0
        else
            echo -e "\n------------# $project:$tag didn't upgrade. #-----------"
            exit 3
        fi
    else
        echo -e "\n------------# $project:$tag didn't deploy. #-----------"
        exit 2
    fi
else
    echo -e "\n------------# $project:$tag didn't pull on server #-----------"
    exit 1
fi
~               

