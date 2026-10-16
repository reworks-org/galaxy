LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

# use amalgam source for easier maintenance
LOCAL_SRC_FILES = \
    src/harfbuzz.cc


LOCAL_ARM_MODE := arm

LOCAL_CPP_EXTENSION := .cc

LOCAL_C_INCLUDES = \
    $(LOCAL_PATH)/ \
    $(LOCAL_PATH)/src/ \
    $(LOCAL_PATH)/../freetype/include/ \

LOCAL_STATIC_LIBRARIES += freetype

#LOCAL_CFLAGS += -DHB_NO_MT -DHAVE_OT -DHAVE_UCDN -fPIC
LOCAL_CFLAGS += -DHAVE_CONFIG_H -fPIC

LOCAL_EXPORT_C_INCLUDES = $(LOCAL_PATH)/src/

# -DHAVE_ICU -DHAVE_ICU_BUILTIN
LOCAL_MODULE:= harfbuzz

include $(BUILD_STATIC_LIBRARY)
