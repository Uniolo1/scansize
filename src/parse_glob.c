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
#include "h/parse_glob.h"
#include "h/parse.h"
#include <glob.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// wrapper to 'parse_from" that glob's input
int parse_from_glob(const char *path, bool recursive)
{
	// glob the input
	glob_t matches;
	int ret = glob(path, 0, NULL, &matches);

	if (ret == 0)
	{
		int result = 0;

		for (size_t i = 0; i < matches.gl_pathc; ++i)
		{
			int r = parse_from(matches.gl_pathv[i], 0, recursive);
			if (r != 0)
				// we set the result to the last failed call
				result = r;
		}

		globfree(&matches); // free memory allocated by glob
		return result;
	}

	// handle errors
	else if (ret == GLOB_NOMATCH)
	{
		return 1;
	}
	else
	{
		fprintf(stderr, "Failed to glob \"%s\"!\n", path);
		return 2;
	}
}
