#!/bin/bash

#############################################
# DEFINITIONS

SRC_HOME=.
BUILD_OUT=./out

APP_NAME=exam_https_post

SRC_FILES=""
SRC_FILES="${SRC_FILES} ${SRC_HOME}/http-post.c"

LIBS=""
LIBS="${LIBS} -lcurl"

# DEFINITIONS
#############################################

#############################################
# BUILD

cd $SRC_HOME

if [ ! -d "${BUILD_OUT}" ]; then
    echo "***Create build output dir \"${BUILD_OUT}\"..."
    mkdir ${BUILD_OUT}
fi

g++ -std=c++11 \
    -o ${BUILD_OUT}/${APP_NAME} \
    ${SRC_FILES} \
    ${LIBS}
    
if [ $? -ne 0 ]; then
    echo -e "\033[0;31m***build ${APP_NAME} failed, exit.\033[0m"
    exit 1
else
    echo -e "\033[0;32m***build ${APP_NAME} successfully.\033[0m"
fi

echo ""

# BUILD
#############################################

#############################################
# RUN

CMD_LINE="${BUILD_OUT}/${APP_NAME}"

echo -e "\033[0;32m${CMD_LINE}\033[0m"
echo ""

TIME_FLAG=`date +"%y%m%d.%H%M%S"`
LOG_FILE_NAME=buildlog_${TIME_FLAG}.txt
BUILD_LOG_FILE=$SRC_HOME/${BUILD_OUT}/$LOG_FILE_NAME

$CMD_LINE 2>&1 | tee $BUILD_LOG_FILE

echo ""
echo ""
BUILD_FAILED=`tail $BUILD_LOG_FILE | grep "ERROR"`
if [[ -z "$BUILD_FAILED" ]]; then
    echo -e "\033[0;32m***Run \"${CMD_LINE}\" over.\033[0m"
    echo ""
else    
    echo "\033[0;31m***Run \"${CMD_LINE}\" Failed, Please Check Log: $BUILD_LOG_FILE"
    echo ""
    exit 1
fi

# RUN
#############################################
