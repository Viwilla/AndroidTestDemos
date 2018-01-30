LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := EncDemo
LOCAL_SRC_FILES := EncDemo.cpp
LOCAL_C_INCLUDES :=  $(LOCAL_PATH) $(LOCAL_PATH)/..  
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -fPIC -Wl,-init=hellojni
LOCAL_STATIC_LIBRARIES := libzlib  

include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))

