LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := model
LOCAL_SRC_FILES := duktape.c DuktapeJNI.c model_callbacks.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/..
LOCAL_LDLIBS    := -llog -landroid
include $(BUILD_SHARED_LIBRARY)
