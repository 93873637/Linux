#!/bin/bash

APP_NAME="logtest"

if [ ! -f "$APP_NAME" ]; then
    rm ./${APP_NAME}
fi

g++ -std=c++11 -pthread -o ${APP_NAME} main.cpp LogUtil.cpp FileUtil.cpp
if [ $? -ne 0 ]; then
    echo "build ${APP_NAME} failed."
    exit 1
fi

./${APP_NAME}
rm ./${APP_NAME}
