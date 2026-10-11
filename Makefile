CC := cc
CFLAGS := -Wall -Wextra

SRC_DIR := src
OUT := out

.PHONY: all
all:
	$(CC) -O2 $(CFLAGS) -o $(OUT)/scansize $(SRC_DIR)/*.c

.PHONY: debug
debug:
	$(CC) -O0 -g $(CFLAGS) -o $(OUT)/scansize $(SRC_DIR)/*.c