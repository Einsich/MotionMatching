#!/bin/bash
PROJECT=${1:-RayTracing}
BUILD_TYPE=${2:-rel}
REBUILD=${3:-no}
ECS_CODEGEN=${4:-no}
CWD=$(pwd)
cd Builds

if [ $REBUILD = "yes" ]
then
    rm -r -f $PROJECT/$BUILD_TYPE
fi

if [ $ECS_CODEGEN = "yes" ]
then
    ./ECSCodeGen/ECSCodeGen-rel.exe "-ecsPath -$CWD/Projects/$PROJECT/Sources/Systems"
fi
cmake -DPROJECT=$PROJECT -DBUILD_TYPE=$BUILD_TYPE -B $PROJECT/$BUILD_TYPE  
cd $PROJECT/$BUILD_TYPE 
time -p make -j $(nproc)
mv $PROJECT-$BUILD_TYPE.exe ..
