// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#if !defined(RUNNING_ON_POSIX) && !defined(__unix__) && !defined(__APPLE__)
#error                                                                         \
    "This software is intended for POSIX Systems, compile with 'RUNNING_ON_POSIX' to override this check!"
#endif

#define _POSIX_C_SOURCE 200112L
#include "h/parse.h"
#include "h/colors.h"
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static inline intmax_t get_size(struct stat st)
{
	return st.st_size;
}

static inline char *pathjoin(const char *parent, const char *child)
{
	size_t plen = strlen(parent);
	size_t clen = strlen(child);

	int need_slash = plen > 0 && parent[plen - 1] != '/';

	char *out = malloc(plen + (size_t)need_slash + clen + 1);
	if (out == NULL)
		return NULL;

	memcpy(out, parent, plen);

	if (need_slash)
		out[plen++] = '/';

	memcpy(out + plen, child, clen + 1);

	return out;
}

int parse_from(const char *path, uintmax_t depth)
{
	struct stat st;
	if (lstat(path, &st) != 0)
	{
		perror(path);
		return -1;
	}

	if (S_ISDIR(st.st_mode))
	{
		DIR *dir = opendir(path);
		if (dir == NULL)
		{
			perror("opendir");
			return 1;
		}

		struct dirent *entry;

		for (uintmax_t i = 0; i < depth; i++)
			putchar(' ');
		printf("" GREEN "%s" GRAY "/: " MAGENTA "%zu" GRAY
		       " bytes" RESET "\n",
		       path, get_size(st));

		while ((entry = readdir(dir)) != NULL)
		{
			if (strcmp(entry->d_name, ".") == 0 ||
			    strcmp(entry->d_name, "..") == 0)
				continue;

			char *entry_full = pathjoin(path, entry->d_name);
			if (entry_full == NULL)
			{
				perror("pathjoin");
				return 3;
			}

			parse_from(entry_full, depth + 1);
			free(entry_full);
		}

		closedir(dir);
		return 0;
	}
	else if (S_ISREG(st.st_mode))
	{
		for (uintmax_t i = 0; i < depth; i++)
			putchar(' ');

		printf(BLUE "%s" RESET ": " MAGENTA "%jd" RESET " " GRAY
		            "bytes" RESET "\n",
		       path, get_size(st));
		return 0;
	}

	return 2;
}