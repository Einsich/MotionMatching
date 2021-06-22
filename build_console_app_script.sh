#!/bin/bash
PROJECT=${1:-ECSCodeGen}
BUILD_TYPE=${2:-rel}

export CC=/usr/bin/clang-6.0
export CXX=/usr/bin/clang++-6.0
cmake -G Ninja -DPROJECT=$PROJECT -DBUILD_TYPE=$BUILD_TYPE -B Builds/unix/$PROJECT/$BUILD_TYPE -S Projects/$PROJECT

cd Builds/unix/$PROJECT/$BUILD_TYPE 
time -p ninja -j $(nproc)
mv $PROJECT-$BUILD_TYPE.exe ..
