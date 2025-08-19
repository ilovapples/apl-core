INCL := include
SRC := src
OBJ := obj
CFLAGS := -Wall -Wextra -pedantic -Wshadow -Werror -I.. -O3 -static
OBJS_LIST := $(OBJ)/array.o $(OBJ)/arena.o $(OBJ)/slice.o $(OBJ)/util-log.o $(OBJ)/util-error.o $(OBJ)/util-misc.o

LIB_NAME := aplcore
EXTERN_LIB_DIR := ~/Coding/lib
EXTERN_INCL_DIR := ~/Coding/include

LIB_FILE_NAME := lib$(LIB_NAME).a
EXT_APL_LIB_DIR := $(EXTERN_INCL_DIR)/$(LIB_NAME)
EXT_APL_INCL_DIR := $(EXTERN_INCL_DIR)/$(LIB_NAME)

all: Makefile $(INCL) $(SRC) $(OBJ) \
	build_objects_list \
	$(LIB_FILE_NAME) $(EXTERN_LIB_DIR)/$(LIB_FILE_NAME) \
	check

.PHONY: clean check build_objects_list

# object build commands
$(OBJ)/array.o: $(SRC)/array.c $(INCL)/array.h $(INCL)/types.h
	gcc -o $(OBJ)/array.o -c $(SRC)/array.c $(CFLAGS)

$(OBJ)/arena.o: $(SRC)/arena.c $(INCL)/arena.h $(INCL)/types.h $(INCL)/array.h
	gcc -o $(OBJ)/arena.o -c $(SRC)/arena.c $(CFLAGS)

$(OBJ)/slice.o: $(SRC)/slice.c $(INCL)/slice.h $(INCL)/types.h $(INCL)/slice.h
	gcc -o $(OBJ)/slice.o -c $(SRC)/slice.c $(CFLAGS)

$(OBJ)/util-log.o: $(SRC)/util/log.c $(INCL)/util/log.h $(INCL)/types.h
	gcc -o $(OBJ)/util-log.o -c $(SRC)/util/log.c $(CFLAGS)

$(OBJ)/util-error.o: $(SRC)/util/error.c $(INCL)/util/error.h $(INCL)/types.h
	gcc -o $(OBJ)/util-error.o -c $(SRC)/util/error.c $(CFLAGS)

$(OBJ)/util-misc.o: $(SRC)/util/misc.c $(INCL)/util/misc.h $(INCL)/types.h
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
$(SRC):
	mkdir $(SRC)
$(OBJ):
	mkdir $(OBJ)
$(EXT_APL_INCL_DIR): $(INCL)
	rm -rf $(EXT_APL_INCL_DIR)
	mkdir -p $(EXT_APL_INCL_DIR)

clean:
	$(info [INFO] Cleaning...)
	rm -f $(LIB_FILE_NAME) $(EXTERN_LIB_DIR)/$(LIB_FILE_NAME) $(OBJ)/* -r $(EXT_APL_INCL_DIR)

check:
	@if [ "$(find include -type f -newer last_run.txt)" != "" ] || [ ! -d "~/Coding/include/aplcore" ]; then \
		$(MAKE) $(EXT_APL_INCL_DIR)/types.h; \
		touch last_run.txt; \
	fi
