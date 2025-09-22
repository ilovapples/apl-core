LINC := include
LSRC := src
LOBJ := obj
OBJS_LIST := $(LOBJ)/structs-array.o $(LOBJ)/structs-arena.o $(LOBJ)/structs-slice.o \
		 $(LOBJ)/structs-string.o \
		 $(LOBJ)/util-log.o $(LOBJ)/util-error.o $(LOBJ)/util-misc.o

EXTERNAL_DIR := $(HOME)/.local
BLIB := $(EXTERNAL_DIR)/lib
BINC := $(EXTERNAL_DIR)/include

# uncomment to remove (debug) functionality (so debug messages don't appear,
# but doesn't disallow error logging)
#NO_LOGGING := -DAPLCORE__DISABLE_LOGGING

# comment out the next line to enable the library's internal logging
HAS_NO_DEBUG := -DNDEBUG

CFLAGS := -Wall -Wextra -pedantic -Wshadow \
	    $(NO_LOGGING) \
	    $(HAS_NO_DEBUG) \
	    -I$(LINC) -g -static

LIB_NAME := aplcore
LIBFILENAME := lib$(LIB_NAME).a
APL_INCL := $(BINC)/$(LIB_NAME)

all: $(OBJ) \
	$(LIBFILENAME)

.PHONY: clean export clean_export clean_build

# object build commands
$(LOBJ)/structs-array.o: $(LSRC)/structs/array.c $(LINC)/aplcore/structs/array.h \
	$(LINC)/aplcore/types.h Makefile
	gcc -o $(LOBJ)/structs-array.o -c $(LSRC)/structs/array.c $(CFLAGS)

$(LOBJ)/structs-arena.o: $(LSRC)/structs/arena.c $(LINC)/aplcore/structs/arena.h \
	$(LINC)/aplcore/types.h $(LINC)/aplcore/structs/array.h $(LINC)/aplcore/structs/string.h Makefile
	gcc -o $(LOBJ)/structs-arena.o -c $(LSRC)/structs/arena.c $(CFLAGS)

$(LOBJ)/structs-slice.o: $(LSRC)/structs/slice.c $(LINC)/aplcore/structs/slice.h \
	$(LINC)/aplcore/types.h $(LINC)/aplcore/structs/slice.h Makefile
	gcc -o $(LOBJ)/structs-slice.o -c $(LSRC)/structs/slice.c $(CFLAGS)

$(LOBJ)/structs-string.o: $(LSRC)/structs/string.c $(LINC)/aplcore/structs/string.h \
	$(LINC)/aplcore/types.h $(LINC)/aplcore/util/error.h Makefile
	gcc -o $(LOBJ)/structs-string.o -c $(LSRC)/structs/string.c $(CFLAGS)

$(LOBJ)/util-log.o: $(LSRC)/util/log.c $(LINC)/aplcore/util/log.h \
	$(LINC)/aplcore/types.h Makefile
	gcc -o $(LOBJ)/util-log.o -c $(LSRC)/util/log.c $(CFLAGS)

$(LOBJ)/util-error.o: $(LSRC)/util/error.c $(LINC)/aplcore/util/error.h \
	$(LINC)/aplcore/types.h Makefile
	gcc -o $(LOBJ)/util-error.o -c $(LSRC)/util/error.c $(CFLAGS)

$(LOBJ)/util-misc.o: $(LSRC)/util/misc.c $(LINC)/aplcore/util/misc.h \
	$(LINC)/aplcore/types.h Makefile
	gcc -o $(LOBJ)/util-misc.o -c $(LSRC)/util/misc.c $(CFLAGS)

$(LIBFILENAME): $(OBJS_LIST)
	ar rcs $(LIBFILENAME) $(OBJS_LIST)

clean:
	rm -rf $(LIBFILENAME) $(LOBJ)/* $(BLIB)/$(LIBFILENAME) $(APL_INCL)

export: $(LIBFILENAME)
	rm -rf $(APL_INCL)
	mkdir -p $(BLIB)
	cp -r $(LINC)/$(LIB_NAME) $(BINC)
	cp $(LIBFILENAME) $(BLIB)

clean_export: clean export
clean_build: clean $(LIBFILENAME)
