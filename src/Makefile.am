AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libNucesGraph.la
libNucesGraph_la_SOURCES = \
    clique.c \
    display.c \
    generate.c \
    metrics.c \
    operations.c \
    paths.c \
    util.c \
    utilmatrix.c

libNucesGraph_la_LDFLAGS = -version-info 1:0:0 $(LDFLAGS)
libNucesGraph_la_LIBADD = -lm -lglpk
