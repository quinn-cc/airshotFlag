lib_LTLIBRARIES = airshotFlag.la

airshotFlag_la_SOURCES = airshotFlag.cpp
airshotFlag_la_CPPFLAGS= -I$(top_srcdir)/include -I$(top_srcdir)/plugins/plugin_utils
airshotFlag_la_LDFLAGS = -module -avoid-version -shared
airshotFlag_la_LIBADD = $(top_builddir)/plugins/plugin_utils/libplugin_utils.la

AM_CPPFLAGS = $(CONF_CPPFLAGS)
AM_CFLAGS = $(CONF_CFLAGS)
AM_CXXFLAGS = $(CONF_CXXFLAGS)

EXTRA_DIST = \
	README.airshotFlag.txt \
	airshotFlag.sln \
	airshotFlag.vcxproj	\
	airshotFlag.vcxproj.filters

MAINTAINERCLEANFILES =	\
	Makefile.in
