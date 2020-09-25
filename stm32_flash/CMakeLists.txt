cmake_minimum_required(VERSION 3.1)

#SET(CROSS_COMPILE /usr/local/arago-2019.07/sysroots/x86_64-arago-linux/usr/bin)
#SET(CMAKE_CXX_COMPILER "${CROSS_COMPILE}/arm-linux-gnueabihf-g++")
#SET(CMAKE_C_COMPILER "${CROSS_COMPILE}/arm-linux-gnueabihf-gcc")
#SET(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../targetNFS/usr/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(stm32_flash LANGUAGES C CXX)

set(BUILD_TYPE BUILD_EXECUTABLE)

#include_directories(
#  ${CMAKE_CURRENT_SOURCE_DIR}/include
#  ${CMAKE_INSTALL_PREFIX}/include
#)

set(LOCAL_SRC_FILES
  main.c
#  binary.c
  serial_common.c
  serial_port.c
#  can.c
  port.c
  ymodem.c
)

# set(LOCAL_SHARED_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libglog.so)
#set(LOCAL_STATIC_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libglog.a)

add_executable(${PROJECT_NAME} ${LOCAL_SRC_FILES})
target_link_libraries(${PROJECT_NAME} pthread)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LOCAL_STATIC_LIBRARIES})
#install(TARGETS ${PROJECT_NAME} DESTINATION bin)
#install(FILES update_engine.service DESTINATION etc/systemd/system)
#install(TARGETS ${PROJECT_NAME}
#       RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
