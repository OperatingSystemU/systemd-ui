#  This file is part of systemd.
#
#  Copyright 2011 Lennart Poettering
#  Copyright 2011 Kay Sievers
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  systemd is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with systemd; If not, see <http://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = -I m4
SUBDIRS =

CLEANFILES =
EXTRA_DIST =
INSTALL_EXEC_HOOKS =
UNINSTALL_EXEC_HOOKS =
INSTALL_DATA_HOOKS =

AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-I $(top_srcdir)/src

bin_PROGRAMS = \
	systemadm \
	systemd-gnome-ask-password-agent

systemadm_SOURCES = \
	src/systemadm.vala \
	src/systemd-interfaces.vala \
	src/wraplabel.vala

systemadm_CFLAGS = \
	$(AM_CFLAGS) \
	$(GTK_CFLAGS) \
	-Wno-unused-variable \
	-Wno-unused-function \
	-Wno-shadow \
	-Wno-format-nonliteral

systemadm_VALAFLAGS = \
	--pkg=posix \
	--pkg=gtk+-2.0 \
	--pkg=gee-1.0 \
	-g

systemadm_LDADD = \
	$(GTK_LIBS)

desktopdir = $(datadir)/applications
desktop_DATA = \
	src/systemadm.desktop

EXTRA_DIST += \
	src/systemadm.desktop

systemd_gnome_ask_password_agent_SOURCES = \
	src/gnome-ask-password-agent.vala

systemd_gnome_ask_password_agent_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBNOTIFY_CFLAGS) \
	$(GTK_CFLAGS) \
	-Wno-unused-variable \
	-Wno-unused-function \
	-Wno-shadow \
	-Wno-format-nonliteral

systemd_gnome_ask_password_agent_VALAFLAGS = \
	--pkg=posix \
	--pkg=gtk+-2.0 \
	--pkg=linux \
	--pkg=gio-unix-2.0 \
	--pkg=libnotify \
	-g

systemd_gnome_ask_password_agent_LDADD = \
	$(LIBNOTIFY_LIBS) \
	$(GTK_LIBS)

if HAVE_VALAC
CLEANFILES += \
	${systemadm_SOURCES:.vala=.c}
endif

dist_man_MANS = \
	man/systemadm.1

EXTRA_DIST += \
	man/systemadm.xml

if HAVE_XSLTPROC
XSLTPROC_FLAGS = \
	--nonet \
	--stringparam funcsynopsis.style ansi

XSLTPROC_PROCESS_MAN = \
	$(AM_V_GEN)$(MKDIR_P) $(dir $@) && \
	$(XSLTPROC) -o $@ $(XSLTPROC_FLAGS) http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<

man/%.1: man/%.xml
	$(XSLTPROC_PROCESS_MAN)

CLEANFILES += \
	$(dist_man_MANS)
endif

install-exec-hook: $(INSTALL_EXEC_HOOKS)
uninstall-hook: $(UNINSTALL_EXEC_HOOKS)
install-data-hook: $(INSTALL_DATA_HOOKS)

upload: all distcheck
	cp -v systemd-ui-$(VERSION).tar.xz /home/lennart/git.fedora/systemd-ui/
	scp systemd-ui-$(VERSION).tar.xz fdo:/srv/www.freedesktop.org/www/software/systemd/

git-tag:
	git tag "v$(VERSION)" -m "systemd-ui $(VERSION)"
