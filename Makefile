INCL := include
SRC := src
OBJ := obj
OBJS_LIST := $(OBJ)/structs-array.o $(OBJ)/structs-arena.o $(OBJ)/structs-slice.o \
		 $(OBJ)/structs-string.o \
		 $(OBJ)/util-log.o $(OBJ)/util-error.o $(OBJ)/util-misc.o

EXTERNAL_PROJECTS_ROOT := $(HOME)/Coding
EXTERN_LIB_DIR := $(EXTERNAL_PROJECTS_ROOT)/lib
EXTERN_INCL_DIR := $(EXTERNAL_PROJECTS_ROOT)/include

COMPILE_WITHOUT_LOGGING_OPTION := -DAPLCORE__DISABLE_LOGGING

CFLAGS := -Wall -Wextra -pedantic -Wshadow -Werror \
	    $(COMPILE_WITHOUT_LOGGING_OPTION) \
	    -Iinclude -g -static

LIB_NAME := aplcore
LIB_FILE_NAME := lib$(LIB_NAME).a
EXT_APL_LIB_DIR := $(EXTERN_INCL_DIR)/$(LIB_NAME)
EXT_APL_INCL_DIR := $(EXTERN_INCL_DIR)/$(LIB_NAME)

all: Makefile $(INCL) $(OBJ) \
	build_objects_list \
	check $(EXTERN_LIB_DIR)/$(LIB_FILE_NAME) \
	$(LIB_FILE_NAME)

.PHONY: clean check build_objects_list

# object build commands
$(OBJ)/structs-array.o: $(SRC)/structs/array.c $(INCL)/structs/array.h $(INCL)/types.h Makefile
	gcc -o $(OBJ)/structs-array.o -c $(SRC)/structs/array.c $(CFLAGS)

$(OBJ)/structs-arena.o: $(SRC)/structs/arena.c $(INCL)/structs/arena.h $(INCL)/types.h $(INCL)/structs/array.h Makefile
	gcc -o $(OBJ)/structs-arena.o -c $(SRC)/structs/arena.c $(CFLAGS)

$(OBJ)/structs-slice.o: $(SRC)/structs/slice.c $(INCL)/structs/slice.h $(INCL)/types.h $(INCL)/structs/slice.h Makefile
	gcc -o $(OBJ)/structs-slice.o -c $(SRC)/structs/slice.c $(CFLAGS)

$(OBJ)/structs-string.o: $(SRC)/structs/string.c $(INCL)/structs/string.h $(INCL)/types.h $(INCL)/util/error.h Makefile
	gcc -o $(OBJ)/structs-string.o -c $(SRC)/structs/string.c $(CFLAGS)

$(OBJ)/util-log.o: $(SRC)/util/log.c $(INCL)/util/log.h $(INCL)/types.h Makefile
	gcc -o $(OBJ)/util-log.o -c $(SRC)/util/log.c $(CFLAGS)

$(OBJ)/util-error.o: $(SRC)/util/error.c $(INCL)/util/error.h $(INCL)/types.h Makefile
	gcc -o $(OBJ)/util-error.o -c $(SRC)/util/error.c $(CFLAGS)

$(OBJ)/util-misc.o: $(SRC)/util/misc.c $(INCL)/util/misc.h $(INCL)/types.h Makefile
	gcc -o $(OBJ)/util-misc.o -c $(SRC)/util/misc.c $(CFLAGS)

build_objects_list:
	$(info [INFO] Building object files...)
	$(MAKE) $(OBJS_LIST)

$(LIB_FILE_NAME): $(OBJS_LIST)
	$(info [INFO] Creating static library archive...)
	ar rcs $(LIB_FILE_NAME) $(OBJS_LIST)

$(EXTERN_LIB_DIR)/$(LIB_FILE_NAME): $(LIB_FILE_NAME)
	$(info [INFO] Copying library archive into $(EXTERN_LIB_DIR)...)
	cp $(LIB_FILE_NAME) $(EXTERN_LIB_DIR)/$(LIB_FILE_NAME)


$(EXT_APL_INCL_DIR)/types.h: $(EXT_APL_INCL_DIR) $(INCL)
	$(info [INFO] Copying contents of the $(INCL)/ directory into $(EXT_APL_INCL_DIR)...)
	cp -r $(INCL)/* $(EXT_APL_INCL_DIR)


$(INCL):
	mkdir $(INCL)
$(OBJ):
	mkdir $(OBJ)
$(EXT_APL_INCL_DIR): $(INCL)
	rm -rf $(EXT_APL_INCL_DIR)
	mkdir -p $(EXT_APL_INCL_DIR)

clean:
	$(info [INFO] Cleaning...)
	rm -rf $(LIB_FILE_NAME) $(EXTERN_LIB_DIR)/$(LIB_FILE_NAME) $(OBJ)/* $(EXT_APL_INCL_DIR)

check:
	@if [ "$(find include -type f -newer last_run.txt)" != "" ] || [ ! -d "~/Coding/include/aplcore" ]; then \
		$(MAKE) $(EXT_APL_INCL_DIR)/types.h; \
		touch last_run.txt; \
	fi
