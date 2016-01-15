CC = gcc

# Production
# CFLAGS = -Wall -O2
# Development
CFLAGS = -Wall -g -O0

RM = rm -f
TOP = ..
BIN_DIR = $(TOP)/bin
TARGET_THREAD = $(BIN_DIR)/thread
TARGET_MULTI_PROC = $(BIN_DIR)/multi_proc

.SUFFIXES: .c .o

.PHONY: all
all: $(TARGET_THREAD) $(TARGET_MULTI_PROC)

$(TARGET_THREAD): thread_main.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_MULTI_PROC): multi_proc_main.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

thread_main.o: multi.h
multi_proc_main.o: multi.h

.PHONY: clean
clean:
	$(RM) *.o $(TARGET_THREAD) $(TARGET_MULTI_PROC)

.PHONY: format
format:
	find . -name "*.c" -o -name "*.h" | xargs gindent -kr -nut
