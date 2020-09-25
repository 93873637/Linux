#!/bin/bash

APP_NAME="cmds_input_test"

if [ -d "${APP_NAME}" ]; then
    rm ${APP_NAME}
fi

g++ main.cpp CmdInput.cpp -o ${APP_NAME}

if [ $? != 0 ]; then
    echo "***build failed."
    exit
fi
echo "***build successfully."

echo "run ${APP_NAME}..."
./${APP_NAME}
