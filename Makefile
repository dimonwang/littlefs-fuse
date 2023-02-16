TARGET_FUSE = lfs_fuse
TARGET_TEST = lfs_test

OS := $(shell uname -s)

CC = cc

SRC += $(wildcard *.c littlefs/*.c)
SRC_FUSE += $(filter-out test.c, $(SRC))
SRC_TEST += $(filter-out lfs_fuse.c, $(SRC))
OBJ_FUSE := $(SRC_FUSE:.c=.o)
OBJ_TEST := $(SRC_TEST:.c=.o)
DEP_FUSE := $(SRC_FUSE:.c=.d)
DEP_TEST := $(SRC_TEST:.c=.d)
ASM_FUSE := $(SRC_FUSE:.c=.s)
ASM_TEST := $(SRC_TEST:.c=.s)

ifdef DEBUG
override CFLAGS += -O0 -g3
else
override CFLAGS += -Os
endif
ifdef WORD
override CFLAGS += -m$(WORD)
endif
override CFLAGS += -I. -Ilittlefs
override CFLAGS += -std=c99 -Wall -pedantic
override CFLAGS += -D_FILE_OFFSET_BITS=64
override CFLAGS += -D_XOPEN_SOURCE=700
override CFLAGS += -DLFS_MIGRATE

override LFLAGS += -lfuse

ifeq ($(OS), FreeBSD)
override CFLAGS += -I /usr/local/include
override CFLAGS += -D __BSD_VISIBLE
override LFLAGS += -L /usr/local/lib
endif

all: $(TARGET_FUSE) $(TARGET_TEST)

-include $(DEP_FUSE) $(DEP_TEST)

$(TARGET_FUSE): $(OBJ_FUSE)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

$(TARGET_TEST): $(OBJ_TEST)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

%.o: %.c
	$(CC) -c -MMD $(CFLAGS) $< -o $@

%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET_FUSE) $(TARGET_TEST)
	rm -f $(OBJ_FUSE) $(OBJ_TEST)
	rm -f $(DEP_FUSE) $(DEP_TEST)
	rm -f $(ASM_FUSE) $(ASM_TEST)
