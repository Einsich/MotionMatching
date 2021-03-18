#!/bin/bash
PROJECT=$1
BUILD_TYPE=$2
REBUILD=$3

cd Builds

if [ $REBUILD = "yes" ]
then
    rm -r -f $PROJECT/$BUILD_TYPE
fi
cmake -DPROJECT=$PROJECT -DBUILD_TYPE=$BUILD_TYPE -B $PROJECT/$BUILD_TYPE  
cd $PROJECT/$BUILD_TYPE 
time -p make -j 8
