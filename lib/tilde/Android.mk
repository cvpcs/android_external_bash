LOCAL_PATH:= $(call my-dir)

# ========================================================
# libtilde.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	tilde.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../.. \
	$(LOCAL_PATH)/../../include \
	$(LOCAL_PATH)/../../lib

LOCAL_CFLAGS += \
	-DHAVE_CONFIG_H

LOCAL_MODULE := libtilde

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
