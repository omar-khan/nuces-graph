if HAVE_LLVM
noinst_PROGRAMS = callgraph
endif

callgraph_SOURCES = callgraph.c
EXTRA_DIST = dummy.c
callgraph_CPPFLAGS = -I$(top_srcdir)/include $(LLVM_CFLAGS)
callgraph_LDADD = $(top_builddir)/src/libNucesGraph.la $(LLVM_LIBS)
callgraph_LDFLAGS = -Wl,-rpath,$(abs_top_builddir)/lib

clean-local:
	rm -f *.dot *.pdf *.log *~
