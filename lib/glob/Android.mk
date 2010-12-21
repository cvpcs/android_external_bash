LOCAL_PATH:= $(call my-dir)

# ========================================================
# libglob.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	glob.c \
	smatch.c \
	strmatch.c \
	xmbsrtowcs.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../.. \
	$(LOCAL_PATH)/../../include \
	$(LOCAL_PATH)/../../lib

LOCAL_CFLAGS += \
	-DHAVE_CONFIG_H

LOCAL_MODULE_TAGS := eng

LOCAL_MODULE := libglob

include $(BUILD_STATIC_LIBRARY)

# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
