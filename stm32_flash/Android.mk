TOP_LOCAL_PATH := $(call my-dir)

include $(call all-named-subdir-makefiles)

LOCAL_PATH := $(TOP_LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE := my_stm32_flash
LOCAL_SRC_FILES :=	\
	binary.c	\
	main.c		\
	port.c		\
	serial_common.c		\
	serial_port.c		\
	ymodem.c

#LOCAL_STATIC_LIBRARIES := libparsers
include $(BUILD_EXECUTABLE)
