/* vi: set sw=4 ts=4: */
/*
 * Busybox main internal header file
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */
#ifndef BUSYBOX_H
#define BUSYBOX_H 1

#include "libbb.h"

PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN

/* order matters: used as index into "install_dir[]" in appletlib.c */
typedef enum bb_install_loc_t {
	BB_DIR_ROOT = 0,
	BB_DIR_BIN,
	BB_DIR_SBIN,
#if ENABLE_INSTALL_NO_USR
	BB_DIR_USR_BIN  = BB_DIR_BIN,
	BB_DIR_USR_SBIN = BB_DIR_SBIN,
#else
	BB_DIR_USR_BIN,
	BB_DIR_USR_SBIN,
#endif
} bb_install_loc_t;

typedef enum bb_suid_t {
	BB_SUID_DROP = 0,
	BB_SUID_MAYBE,
	BB_SUID_REQUIRE
} bb_suid_t;


/* Defined in appletlib.c (by including generated applet_tables.h) */
/* Keep in sync with applets/applet_tables.c! */
extern const char applet_names[];
extern int (*const applet_main[])(int argc, char **argv);
extern const uint16_t applet_nameofs[];
extern const uint8_t applet_install_loc[];

#if ENABLE_FEATURE_SUID || ENABLE_FEATURE_PREFER_APPLETS
# define APPLET_NAME(i) (applet_names + (applet_nameofs[i] & 0x0fff))
#else
# define APPLET_NAME(i) (applet_names + applet_nameofs[i])
#endif

#if ENABLE_FEATURE_PREFER_APPLETS
# define APPLET_IS_NOFORK(i) (applet_nameofs[i] & (1 << 12))
# define APPLET_IS_NOEXEC(i) (applet_nameofs[i] & (1 << 13))
#else
# define APPLET_IS_NOFORK(i) 0
# define APPLET_IS_NOEXEC(i) 0
#endif

#if ENABLE_FEATURE_SUID
# define APPLET_SUID(i) ((applet_nameofs[i] >> 14) & 0x3)
#endif

#if ENABLE_FEATURE_INSTALLER
#define APPLET_INSTALL_LOC(i) ({ \
	unsigned v = (i); \
	if (v & 1) v = applet_install_loc[v/2] >> 4; \
	else v = applet_install_loc[v/2] & 0xf; \
	v; })
#endif


/* Length of these names has effect on size of libbusybox
 * and "individual" binaries. Keep them short.
 */
#if ENABLE_BUILD_LIBBUSYBOX
#if ENABLE_FEATURE_SHARED_BUSYBOX
int lbb_main(char **argv) EXTERNALLY_VISIBLE;
#else
int lbb_main(char **argv);
#endif
#endif

POP_SAVED_FUNCTION_VISIBILITY

#endif
