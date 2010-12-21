LOCAL_PATH:= $(call my-dir)

# ========================================================
# libhistory.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	history.c \
	histexpand.c \
	histfile.c \
	histsearch.c \
	shell.c \
	savestring.c \
	mbutil.c \
	xmalloc.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../.. \
	$(LOCAL_PATH)/../../include \
	$(LOCAL_PATH)/../../lib

LOCAL_CFLAGS += \
	-DHAVE_CONFIG_H

LOCAL_MODULE := libhistory

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

# ========================================================
# libreadline.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	readline.c \
	vi_mode.c \
	funmap.c \
	keymaps.c \
	parens.c \
	search.c \
	rltty.c \
	complete.c \
	bind.c \
	isearch.c \
	display.c \
	signals.c \
	util.c \
	kill.c \
	undo.c \
	macro.c \
	input.c \
	callback.c \
	terminal.c \
	text.c \
	nls.c \
	misc.c \
	history.c \
	histexpand.c \
	histfile.c \
	histsearch.c \
	shell.c \
	savestring.c \
	mbutil.c \
	tilde.c \
	xmalloc.c \
	compat.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../.. \
	$(LOCAL_PATH)/../../include \
	$(LOCAL_PATH)/../../lib

LOCAL_CFLAGS += \
	-DHAVE_CONFIG_H

LOCAL_MODULE := libreadline

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
