LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
    hci_wr.c 

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= hci_wr

include $(BUILD_EXECUTABLE)
