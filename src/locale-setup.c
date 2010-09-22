/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

/***
  This file is part of systemd.

  Copyright 2010 Lennart Poettering

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "locale-setup.h"
#include "util.h"
#include "macro.h"

enum {
        /* We don't list LC_ALL here on purpose. People should be
         * using LANG instead. */

        VARIABLE_LANG,
        VARIABLE_LC_CTYPE,
        VARIABLE_LC_NUMERIC,
        VARIABLE_LC_TIME,
        VARIABLE_LC_COLLATE,
        VARIABLE_LC_MONETARY,
        VARIABLE_LC_MESSAGES,
        VARIABLE_LC_PAPER,
        VARIABLE_LC_NAME,
        VARIABLE_LC_ADDRESS,
        VARIABLE_LC_TELEPHONE,
        VARIABLE_LC_MEASUREMENT,
        VARIABLE_LC_IDENTIFICATION,
        _VARIABLE_MAX
};

static const char * const variable_names[_VARIABLE_MAX] = {
        [VARIABLE_LANG] = "LANG",
        [VARIABLE_LC_CTYPE] = "LC_CTYPE",
        [VARIABLE_LC_NUMERIC] = "LC_NUMERIC",
        [VARIABLE_LC_TIME] = "TIME",
        [VARIABLE_LC_COLLATE] = "COLLATE",
        [VARIABLE_LC_MONETARY] = "MONETARY",
        [VARIABLE_LC_MESSAGES] = "MESSAGE",
        [VARIABLE_LC_PAPER] = "PAPER",
        [VARIABLE_LC_NAME] = "NAME",
        [VARIABLE_LC_ADDRESS] = "ADDRESS",
        [VARIABLE_LC_TELEPHONE] = "TELEPHONE",
        [VARIABLE_LC_MEASUREMENT] = "MEASUREMENT",
        [VARIABLE_LC_IDENTIFICATION] = "IDENTIFICATION"
};

int locale_setup(void) {
        char *variables[_VARIABLE_MAX];
        int r, i;

        zero(variables);

        if ((r = parse_env_file("/proc/cmdline", WHITESPACE,
#ifdef TARGET_FEDORA
                                "LANG",                     &variables[VARIABLE_LANG],
#endif
                                "locale.LANG",              &variables[VARIABLE_LANG],
                                "locale.LC_CTYPE",          &variables[VARIABLE_LC_CTYPE],
                                "locale.LC_NUMERIC",        &variables[VARIABLE_LC_NUMERIC],
                                "locale.LC_TIME",           &variables[VARIABLE_LC_TIME],
                                "locale.LC_COLLATE",        &variables[VARIABLE_LC_COLLATE],
                                "locale.LC_MONETARY",       &variables[VARIABLE_LC_MONETARY],
                                "locale.LC_MESSAGES",       &variables[VARIABLE_LC_MESSAGES],
                                "locale.LC_PAPER",          &variables[VARIABLE_LC_PAPER],
                                "locale.LC_NAME",           &variables[VARIABLE_LC_NAME],
                                "locale.LC_ADDRESS",        &variables[VARIABLE_LC_ADDRESS],
                                "locale.LC_TELEPHONE",      &variables[VARIABLE_LC_TELEPHONE],
                                "locale.LC_MEASUREMENT",    &variables[VARIABLE_LC_MEASUREMENT],
                                "locale.LC_IDENTIFICATION", &variables[VARIABLE_LC_IDENTIFICATION],
                                NULL)) < 0) {

                if (r != -ENOENT)
                        log_warning("Failed to read /proc/cmdline: %s", strerror(-r));
        }

        /* Hmm, nothing set on the kernel cmd line? Then let's
         * try /etc/locale */
        if (r <= 0 &&
            (r = parse_env_file("/etc/locale", NEWLINE,
                               "LANG",              &variables[VARIABLE_LANG],
                               "LC_CTYPE",          &variables[VARIABLE_LC_CTYPE],
                               "LC_NUMERIC",        &variables[VARIABLE_LC_NUMERIC],
                               "LC_TIME",           &variables[VARIABLE_LC_TIME],
                               "LC_COLLATE",        &variables[VARIABLE_LC_COLLATE],
                               "LC_MONETARY",       &variables[VARIABLE_LC_MONETARY],
                               "LC_MESSAGES",       &variables[VARIABLE_LC_MESSAGES],
                               "LC_PAPER",          &variables[VARIABLE_LC_PAPER],
                               "LC_NAME",           &variables[VARIABLE_LC_NAME],
                               "LC_ADDRESS",        &variables[VARIABLE_LC_ADDRESS],
                               "LC_TELEPHONE",      &variables[VARIABLE_LC_TELEPHONE],
                               "LC_MEASUREMENT",    &variables[VARIABLE_LC_MEASUREMENT],
                               "LC_IDENTIFICATION", &variables[VARIABLE_LC_IDENTIFICATION],
                                NULL)) < 0) {

                if (r != -ENOENT)
                        log_warning("Failed to read /etc/locale: %s", strerror(-r));
        }

#ifdef TARGET_FEDORA
        if (r <= 0 &&
            (r = parse_env_file("/etc/sysconfig/i18n", NEWLINE,
                                "LANG", &variables[VARIABLE_LANG],
                                NULL)) < 0) {

                if (r != -ENOENT)
                        log_warning("Failed to read /etc/sysconfig/i18n: %s", strerror(-r));
        }

#elif defined(TARGET_SUSE)
        if (r <= 0 &&
            (r = parse_env_file("/etc/sysconfig/language", NEWLINE,
                                "RC_LANG", &variables[VARIABLE_LANG],
                                NULL)) < 0) {

                if (r != -ENOENT)
                        log_warning("Failed to read /etc/sysconfig/language: %s", strerror(-r));
        }

#elif defined(TARGET_ARCH)
        if (r <= 0 &&
            (r = parse_env_file("/etc/rc.conf", NEWLINE,
                                "LOCALE", &variables[VARIABLE_LANG],
                                NULL)) < 0) {

                if (r != -ENOENT)
                        log_warning("Failed to read /etc/rc.conf: %s", strerror(-r));
        }

#elif defined(TARGET_GENTOO)
        /* Gentoo's openrc expects locale variables in /etc/env.d/
         * These files are later compiled by env-update into shell
         * export commands at /etc/profile.env, with variables being
         * exported by openrc's runscript (so /etc/init.d/)
         */
        if (r <= 0 &&
            (r = parse_env_file("/etc/profile.env", NEWLINE,
                                "export LANG",              &variables[VARIABLE_LANG],
                                "export LC_CTYPE",          &variables[VARIABLE_LC_CTYPE],
                                "export LC_NUMERIC",        &variables[VARIABLE_LC_NUMERIC],
                                "export LC_TIME",           &variables[VARIABLE_LC_TIME],
                                "export LC_COLLATE",        &variables[VARIABLE_LC_COLLATE],
                                "export LC_MONETARY",       &variables[VARIABLE_LC_MONETARY],
                                "export LC_MESSAGES",       &variables[VARIABLE_LC_MESSAGES],
                                "export LC_PAPER",          &variables[VARIABLE_LC_PAPER],
                                "export LC_NAME",           &variables[VARIABLE_LC_NAME],
                                "export LC_ADDRESS",        &variables[VARIABLE_LC_ADDRESS],
                                "export LC_TELEPHONE",      &variables[VARIABLE_LC_TELEPHONE],
                                "export LC_MEASUREMENT",    &variables[VARIABLE_LC_MEASUREMENT],
                                "export LC_IDENTIFICATION", &variables[VARIABLE_LC_IDENTIFICATION],
                                NULL)) < 0) {

                if (r != -ENOENT)
                        log_warning("Failed to read /etc/profile.env: %s", strerror(-r));
        }
#endif

        if (!variables[VARIABLE_LANG]) {
                if (!(variables[VARIABLE_LANG] = strdup("C"))) {
                        r = -ENOMEM;
                        goto finish;
                }
        }

        for (i = 0; i < _VARIABLE_MAX; i++) {

                if (variables[i]) {
                        if (setenv(variable_names[i], variables[i], 1) < 0) {
                                r = -errno;
                                goto finish;
                        }
                } else
                        unsetenv(variable_names[i]);
        }

        r = 0;

finish:
        for (i = 0; i < _VARIABLE_MAX; i++)
                free(variables[i]);

        return r;
}