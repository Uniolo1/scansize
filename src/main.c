// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#if !defined(RUNNING_ON_POSIX) && !defined(__unix__) && !defined(__APPLE__)
#error                                                                         \
    "This software is intended for POSIX Systems, compile with 'RUNNING_ON_POSIX' to override this check!"
#endif

#define _POSIX_C_SOURCE 200112L
#include "h/parse.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static inline bool terminal_supports_color(void)
{
	if (!isatty(STDOUT_FILENO))
		return false;
	else
		return true; // assume true (for now)
}

int main(int argc, char *argv[])
{
	if (!terminal_supports_color())
		no_colors = true;

	if (argc < 2)
	{
		fprintf(stderr, "  Usage: %s <file>      [-r]\n", argv[0]);
		fprintf(stderr, "  Usage: %s <directory> [-r]\n", argv[0]);
		return 1;
	}

	if (argc > 2)
		if (strcmp(argv[2], "-r") == 0 || strcmp(argv[2], "-R") == 0)
			return parse_from(argv[1], 0, true);

	return parse_from(argv[1], 0, false);
}