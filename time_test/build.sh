#!/bin/bash

APP_NAME="time_test"

SRC_FILES=""
SRC_FILES+=" main.cpp"
SRC_FILES+=" TimeUtils.cpp"

if [ -f "$APP_NAME" ]; then
    rm ./${APP_NAME}
fi

g++ -std=c++11 -pthread -o ${APP_NAME} ${SRC_FILES}
if [ $? -ne 0 ]; then
    echo "build ${APP_NAME} failed."
    exit 1
fi

./${APP_NAME}
rm ./${APP_NAME}
