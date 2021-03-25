#!/bin/bash
PROJECT=${1:-RayTracing}
BUILD_TYPE=${2:-rel}
REBUILD=${3:-no}

cd Builds

if [ $REBUILD = "yes" ]
then
    rm -r -f $PROJECT/$BUILD_TYPE
fi
cmake -DPROJECT=$PROJECT -DBUILD_TYPE=$BUILD_TYPE -B $PROJECT/$BUILD_TYPE  
cd $PROJECT/$BUILD_TYPE 
time -p make -j 8
mv $PROJECT-$BUILD_TYPE.exe ..
