#!/bin/bash
PROJECT=$1
BUILD_TYPE=$2
REBUILD=$3

cd $PROJECT 

if [ $REBUILD = "yes" ]
then
    rm -r -f $BUILD_TYPE
fi
cmake -DBUILD_TYPE=$BUILD_TYPE -B $BUILD_TYPE  
cd $BUILD_TYPE 
time -p make -j 8
