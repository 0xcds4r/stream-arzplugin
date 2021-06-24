LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := fisting

# LDLIBS
LOCAL_LDLIBS := -llog -lOpenSLES -ldl -landroid

# PATH
FILE_LIST += $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

# FLAGS
LOCAL_CFLAGS := -w
LOCAL_CPPFLAGS := -w -s -fvisibility=hidden -pthread -Wall -fpack-struct=1 -O2 -std=c++14 -fstrict-aliasing -fno-omit-frame-pointer -mfloat-abi=soft -fstack-protector -fno-short-enums -frtti -g2 -gdwarf-2 -mthumb -ferror-limit=3 -fexceptions

include $(BUILD_SHARED_LIBRARY)