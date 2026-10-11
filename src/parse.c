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
#include "h/colors.h"
#include <dirent.h>
#include <glob.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

bool no_colors = false;

struct file_size
{
	uintmax_t number;
	char postfix[4];
};

static inline struct file_size get_size(struct stat st)
{
	// Possible suffixes
	static const char *const suffixes[] = {"B",   "KiB", "MiB", "GiB",
	                                       "TiB", "PiB", "EiB"};

	// create the return struct
	struct file_size ret = {.number = (uintmax_t)st.st_size,
	                        .postfix = "B"};

	// Set the 'size' variable
	uintmax_t size = (uintmax_t)st.st_size;
	unsigned int i = 0;

	while (size >= 1024 && i < sizeof(suffixes) / sizeof(suffixes[0]) - 1)
	{
		size /= 1024;
		++i;
	}

	ret.number = (uintmax_t)size;

	for (int j = 0; suffixes[i][j] != '\0'; ++j)
	{
		ret.postfix[j] = suffixes[i][j];
	}
	ret.postfix[sizeof(ret.postfix) - 1] = '\0';

	return ret;
}

static inline char *pathjoin(const char *parent, const char *child)
{
	// get the length of the strings
	size_t plen = strlen(parent);
	size_t clen = strlen(child);

	// check if the parent already has a '/' at the end
	int need_slash = plen > 0 && parent[plen - 1] != '/';

	// allocate the string
	char *out = malloc(plen + (size_t)need_slash + clen + 1);
	if (out == NULL)
		return NULL;

	memcpy(out, parent, plen);

	if (need_slash)
		out[plen++] = '/';

	memcpy(out + plen, child, clen + 1);

	return out;
}

int parse_from(const char *path, uintmax_t depth, bool recursive)
{
	// get the stat's of the file
	struct stat st;
	if (lstat(path, &st) != 0)
	{
		perror(path);
		return -1;
	}

	// if it is a directory
	if (S_ISDIR(st.st_mode))
	{
		// if we are not in recursive mode, don't recurse
		if (!recursive && depth != 0)
			return 0;

		DIR *dir = opendir(path);
		if (dir == NULL)
		{
			perror("opendir");
			return 1;
		}

		struct dirent *entry;

		for (uintmax_t i = 0; i < depth; i++)
			putchar(' ');

		if (no_colors)
			printf("%s/\n", path);
		else
			printf("" GREEN "%s" GRAY "/" RESET "\n", path);

		while ((entry = readdir(dir)) != NULL)
		{
			// don't search '.' or '..' for obvious reasons
			if (strcmp(entry->d_name, ".") == 0 ||
			    strcmp(entry->d_name, "..") == 0)
				continue;

			// combine the parent and child paths
			char *entry_full = pathjoin(path, entry->d_name);
			if (entry_full == NULL)
			{
				perror("pathjoin");
				return 3;
			}

			parse_from(entry_full, depth + 1, recursive);
			free(entry_full);
		}

		closedir(dir);
		return 0;
	}

	// if it is a file
	else if (S_ISREG(st.st_mode))
	{
		for (uintmax_t i = 0; i < depth; i++)
			putchar(' ');

		struct file_size size = get_size(st);

		if (no_colors)
			printf("%s: %jd %s\n", path, size.number, size.postfix);
		else
		{
			// hard to read, but same as above but colored
			printf(BLUE "%s" RESET ": " MAGENTA "%jd" RESET " " GRAY
			            "%s" RESET "\n",
			       path, size.number, size.postfix);
		}

		return 0;
	}

	return 2;
}