LOCAL_PATH:= $(call my-dir)

# ========================================================
# bash
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	shell.c eval.c y.tab.c general.c make_cmd.c print_cmd.c \
	dispose_cmd.c execute_cmd.c variables.c copy_cmd.c error.c \
	expr.c flags.c jobs.c subst.c hashcmd.c hashlib.c mailcheck.c \
	trap.c input.c unwind_prot.c pathexp.c sig.c test.c version.c \
	alias.c array.c arrayfunc.c assoc.c braces.c bracecomp.c \
	bashhist.c bashline.c list.c stringlib.c locale.c findcmd.c \
	redir.c pcomplete.c pcomplib.c syntax.c xmalloc.c
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/lib
LOCAL_CFLAGS += \
	-DHAVE_CONFIG_H \
	-DPROGRAM=\"bash\" \
	-DPACKAGE=\"bash\" \
	-DCONF_HOSTTYPE=\"arm\" \
	-DCONF_OSTYPE=\"android-eabi\" \
	-DCONF_MACHTYPE=\"arm-android-eabi\" \
	-DCONF_VENDOR=\"unknown\" \
	-DLOCALEDIR=\"/data/locale\" \
	-DSHELL
LOCAL_SHARED_LIBRARIES += \
	libncurses
LOCAL_STATIC_LIBRARIES += \
	libbuiltins \
	libglob \
	libsh \
	libtilde \
	libhistory \
	libreadline \
	libclearsilverregex
LOCAL_MODULE := bash
LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

# ========================================================
# bash configs
# ========================================================
etc_files := $(shell ls -1 $(LOCAL_PATH)/etc/)

copy_to := $(addprefix $(TARGET_OUT)/etc/$(LOCAL_MODULE)/,$(etc_files))
copy_from := $(addprefix $(LOCAL_PATH)/etc/,$(etc_files))

$(copy_to) : PRIVATE_MODULE := system_etcdir
$(copy_to) : $(TARGET_OUT)/etc/$(LOCAL_MODULE)/% : $(LOCAL_PATH)/etc/% | $(ACP)
	$(transform-prebuilt-to-target)

ALL_PREBUILT += $(copy_to)

# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
