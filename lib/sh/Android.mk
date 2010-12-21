LOCAL_PATH:= $(call my-dir)

# ========================================================
# libsh.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	clktck.c clock.c getenv.c oslib.c setlinebuf.c strnlen.c \
	itos.c zread.c zwrite.c shtty.c shmatch.c eaccess.c \
	netconn.c netopen.c timeval.c makepath.c pathcanon.c \
	pathphys.c tmpfile.c stringlist.c stringvec.c spell.c shquote.c \
	strtrans.c snprintf.c mailstat.c fmtulong.c \
	fmtullong.c fmtumax.c zcatfd.c zmapfd.c winsize.c wcsdup.c \
	fpurge.c zgetline.c mbscmp.c uconvert.c ufuncs.c casemod.c \
	fdprintf.c input_avail.c mbscasecmp.c fnxform.c mbschr.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../.. \
	$(LOCAL_PATH)/../../include \
	$(LOCAL_PATH)/../../lib \
	external/clearsilver \
	external/clearsilver/util/regex

LOCAL_CFLAGS += \
	-DHAVE_CONFIG_H

LOCAL_MODULE := libsh

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
