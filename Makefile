ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CC=gcc
CFLAGS=-Wall -Werror -fPIC -O3
LDFLAGS=-g -pg -ggdb3
INCLUDE=-I$(ROOT_DIR)/include
LIBRARY=-L$(ROOT_DIR)/lib
LDFLAGSLIB = -lm -lNucesGraph

SRCSSRC = $(wildcard src/*.c)
SRCSTEST = $(wildcard tests/*.c)

OBJSSRC = $(SRCSSRC:.c=.o)
OBJSTEST = $(SRCSTEST:.c=.o)

.PHONY: all tests install clean

all: $(SRCSSRC) $(OBJSSRC) 
	mkdir -p lib
	$(CC) -shared -Wl,-soname,libNucesGraph.so.1 -o lib/libNucesGraph.so.1.0 $(OBJSSRC)
	ln -sf libNucesGraph.so.1.0 lib/libNucesGraph.so.1
	ln -sf libNucesGraph.so.1.0 lib/libNucesGraph.so

$(OBJSSRC): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

tests: $(SRCSTEST) $(OBJSTEST)

$(OBJSTEST): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBRARY) $< -o $@ $(LDFLAGSLIB) $(LDFLAGS)

doc: doxygen.conf
	doxygen doxygen.conf
	make -C doc/latex
	mv doc/latex/refman.pdf doc/doc.pdf

install:
	mkdir -p /opt/nucesGraph
	cp lib/libNucesGraph.so.1.0 /opt/nucesGraph/libNucesGraph.so.1.0
	ln -sf /opt/nucesGraph/libNucesGraph.so.1.0 /opt/nucesGraph/libNucesGraph.so.1
	ln -sf /opt/nucesGraph/libNucesGraph.so.1.0 /opt/nucesGraph/libNucesGraph.so

clean:
	rm -f $(OBJSTEST) $(OBJSSRC) 
	rm -rf lib
	rm -f *.log a.out *.dat *.dimac
	rm -f tests/*.eps
	rm -f tests/*.o
	rm -f src/.s*
	rm -f tests/.*.swp
	rm -f gmon.out	
	rm doc -rf
