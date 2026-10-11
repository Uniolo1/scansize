# SPDX-FileCopyrightText: NONE
# SPDX-License-Identifier: Unlicense

CC := cc
CFLAGS := -Wall -Wextra

SRC_DIR := src
OUT := out

.PHONY: all
all:
	mkdir -p out
	$(CC) -O2 $(CFLAGS) -o $(OUT)/scansize $(SRC_DIR)/*.c

.PHONY: debug
debug:
	mkdir -p out
	$(CC) -O0 -g $(CFLAGS) -o $(OUT)/scansize $(SRC_DIR)/*.c -fsanitize=address,undefined -fno-omit-frame-pointer