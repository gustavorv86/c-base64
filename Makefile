SHELL:=/bin/bash

########## DIRECTORIES ##########

SRCDIR:=src

BUILDDIR:=build

DISTDIR:=dist

TARGET_LIB:=$(DISTDIR)/libbase64.so

TARGET_TEST:=$(DISTDIR)/base64_test_main

BUILDOBJS:=$(BUILDDIR)/base64.o

########## COMPILER AND LINKER ############

CC:=/usr/bin/gcc

CFLAGS:=-ggdb -Wall -Wextra -I$(SRCDIR)/base64

LDFLAGS:=

########## RULES ##########

all: start make_dirs $(TARGET_LIB) $(TARGET_TEST) end

lib: start make_dirs $(TARGET_LIB) end

make_dirs:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(DISTDIR)

clean: 
	@echo "Cleaning..."
	@rm -rf $(BUILDDIR)
	@rm -rf $(DISTDIR)
	@echo "Done"

start:
	@echo "Compiling..."

$(BUILDDIR)/base64.o: $(SRCDIR)/base64/base64.h $(SRCDIR)/base64/base64.c
	$(CC) $(CFLAGS) -fPIC -c $(SRCDIR)/base64/base64.c -o $(BUILDDIR)/base64.o $(LDFLAGS) 

$(BUILDDIR)/base64_test_main.o: $(SRCDIR)/test/base64_test_main.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/test/base64_test_main.c -o $(BUILDDIR)/base64_test_main.o $(LDFLAGS)

$(TARGET_LIB): $(BUILDDIR)/base64.o
	$(CC) -shared -o $(TARGET_LIB) $(BUILDDIR)/base64.o

$(TARGET_TEST): $(TARGET_LIB) $(BUILDDIR)/base64_test_main.o
	$(CC) $(CFLAGS) -L$(DISTDIR) $(BUILDDIR)/base64_test_main.o -o $(TARGET_TEST) $(LDFLAGS) -lbase64
	
end:
	@echo "Done..."
