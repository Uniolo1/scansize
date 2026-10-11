// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute
 * this software, either in source code form or as a compiled binary, for any
 * purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors of this
 * software dedicate any and all copyright interest in the software to the
 * public domain. We make this dedication for the benefit of the public at large
 * and to the detriment of our heirs and successors. We intend this dedication
 * to be an overt act of relinquishment in perpetuity of all present and future
 * rights to this software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#if !defined(RUNNING_ON_POSIX) && !defined(__unix__) && !defined(__APPLE__)
#error                                                                         \
    "This software is intended for POSIX Systems, compile with 'RUNNING_ON_POSIX' to override this check!"
#endif

#define _POSIX_C_SOURCE 200112L
#include "h/parse.h"
#include "h/parse_glob.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// This function currently only checks if the 'stdout' is a terminal or not
static inline bool terminal_supports_color(void)
{
	if (!isatty(STDOUT_FILENO))
		return false;
	else
		return true; // assume true (for now)
}

int main(int argc, char *argv[])
{
	// set no_colors flag appropriately
	if (!terminal_supports_color())
		no_colors = true;

	if (argc < 2)
	{
		fprintf(stderr, "  Usage: %s <file>      [-r]\n", argv[0]);
		fprintf(stderr, "  Usage: %s <directory> [-r]\n", argv[0]);
		return 1;
	}

	if (argc > 2)
	{
		// check if argv[2] is '-r' or '-R'
		if (strcmp(argv[2], "-r") == 0 || strcmp(argv[2], "-R") == 0)
			return parse_from_glob(argv[1], true);

		// check to see if the user did "scansize -r <>" instead of
		// "scansize <> -r"
		else if (strcmp(argv[1], "-r") == 0 ||
		         strcmp(argv[1], "-R") == 0)
		{
			return parse_from_glob(argv[2], true);
		}
	}

	return parse_from_glob(argv[1], false);
}