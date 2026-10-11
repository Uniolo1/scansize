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
		fprintf(stderr, "  Usage: %s <file>\n", argv[0]);
		fprintf(stderr, "  Usage: %s <directory>\n", argv[0]);
		return 1;
	}

	return parse_from(argv[1], 0);
}