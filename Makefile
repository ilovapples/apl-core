LINC := include
LSRC := src
LOBJ := obj
OBJS_LIST := $(LOBJ)/structs-array.o $(LOBJ)/structs-arena.o $(LOBJ)/structs-slice.o \
		 $(LOBJ)/structs-string.o \
		 $(LOBJ)/util-log.o $(LOBJ)/util-error.o $(LOBJ)/util-misc.o

PROJROOT := $(HOME)/Coding
BLIB := $(PROJROOT)/lib
BINC := $(PROJROOT)/include

NO_LOGGING :=

CFLAGS := -Wall -Wextra -pedantic -Wshadow \
	    $(NO_LOGGING) \
	    -I$(LINC) -g -static

LIB_NAME := aplcore
LIBFILENAME := lib$(LIB_NAME).a
APL_INCL := $(BINC)/$(LIB_NAME)

all: $(OBJ) \
	$(LIBFILENAME)

.PHONY: clean export

# object build commands
$(LOBJ)/structs-array.o: $(LSRC)/structs/array.c $(LINC)/structs/array.h \
	$(LINC)/types.h Makefile
	gcc -o $(LOBJ)/structs-array.o -c $(LSRC)/structs/array.c $(CFLAGS)

$(LOBJ)/structs-arena.o: $(LSRC)/structs/arena.c $(LINC)/structs/arena.h \
	$(LINC)/types.h $(LINC)/structs/array.h $(LINC)/structs/string.h Makefile
	gcc -o $(LOBJ)/structs-arena.o -c $(LSRC)/structs/arena.c $(CFLAGS)

$(LOBJ)/structs-slice.o: $(LSRC)/structs/slice.c $(LINC)/structs/slice.h \
	$(LINC)/types.h $(LINC)/structs/slice.h Makefile
	gcc -o $(LOBJ)/structs-slice.o -c $(LSRC)/structs/slice.c $(CFLAGS)

$(LOBJ)/structs-string.o: $(LSRC)/structs/string.c $(LINC)/structs/string.h \
	$(LINC)/types.h $(LINC)/util/error.h Makefile
	gcc -o $(LOBJ)/structs-string.o -c $(LSRC)/structs/string.c $(CFLAGS)

$(LOBJ)/util-log.o: $(LSRC)/util/log.c $(LINC)/util/log.h \
	$(LINC)/types.h Makefile
	gcc -o $(LOBJ)/util-log.o -c $(LSRC)/util/log.c $(CFLAGS)

$(LOBJ)/util-error.o: $(LSRC)/util/error.c $(LINC)/util/error.h \
	$(LINC)/types.h Makefile
	gcc -o $(LOBJ)/util-error.o -c $(LSRC)/util/error.c $(CFLAGS)

$(LOBJ)/util-misc.o: $(LSRC)/util/misc.c $(LINC)/util/misc.h \
	$(LINC)/types.h Makefile
	gcc -o $(LOBJ)/util-misc.o -c $(LSRC)/util/misc.c $(CFLAGS)

$(LIBFILENAME): $(OBJS_LIST)
	ar rcs $(LIBFILENAME) $(OBJS_LIST)

clean:
	rm -rf $(LIBFILENAME) $(LOBJ)/* $(BLIB)/$(LIBFILENAME) $(APL_INCL)

export:
	rm -rf $(BLIB) $(APL_INCL)
	mkdir $(BLIB) $(APL_INCL)
	cp -r $(LINC)/* $(APL_INCL)
	cp $(LIBFILENAME) $(BLIB)
