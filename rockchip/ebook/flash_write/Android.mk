LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	flash_write.c

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := 

LOCAL_MODULE:= flash_write

include $(BUILD_EXECUTABLE)
