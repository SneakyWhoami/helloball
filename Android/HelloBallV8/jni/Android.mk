LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_base
LOCAL_SRC_FILES := ../lib/libv8_base.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_libbase
LOCAL_SRC_FILES := ../lib/libv8_libbase.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libv8_libplatform
LOCAL_SRC_FILES := ../lib/libv8_libplatform.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_nosnapshot
LOCAL_SRC_FILES :=  ../lib/libv8_nosnapshot.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := model
LOCAL_SRC_FILES := v8JNI.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/..
LOCAL_LDLIBS    := -llog -landroid
LOCAL_STATIC_LIBRARIES := v8_base v8_libbase libv8_libplatform v8_nosnapshot
include $(BUILD_SHARED_LIBRARY)
