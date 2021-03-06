LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := simple_player
LOCAL_MODULE_TAGS := samples
LOCAL_SRC_FILES := simple_player.c
LOCAL_ARM_MODE := arm
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../amplayer/player/include \
    $(LOCAL_PATH)/../amplayer/control/include \
    $(LOCAL_PATH)/../amcodec/include \
    $(LOCAL_PATH)/../amadec/include \
    $(LOCAL_PATH)/../amffmpeg \
    $(JNI_H_INCLUDE) 

LOCAL_STATIC_LIBRARIES := libamplayer libamplayer libamcodec libavformat libavcodec libavutil libamadec 
LOCAL_SHARED_LIBRARIES += libutils libmedia libbinder libz

include $(BUILD_EXECUTABLE)


