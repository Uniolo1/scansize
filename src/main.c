// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#include <string.h>
#if !defined(RUNNING_ON_POSIX) && !defined(__unix__) && !defined(__APPLE__)
#error                                                                         \
    "This software is intended for POSIX Systems, compile with 'RUNNING_ON_POSIX' to override this check!"
#endif

#define _POSIX_C_SOURCE 200112L
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

intmax_t get_size(struct stat st)
{
	return st.st_size;
}

int parse_from(const char *path, uintmax_t depth)
{
	if (depth != 0)
		if (strcmp(".", path) == 0 || strcmp("..", path) == 0)
			return 0;

	struct stat st;
	if (stat(path, &st) != 0)
		return -1; // failure

	if (S_ISDIR(st.st_mode))
	{
		DIR *dir = opendir(path);
		if (dir == NULL)
		{
			perror("opendir");
			return 1;
		}

		struct dirent *entry;

		printf("%s/\n", path);
		while ((entry = readdir(dir)) != NULL)
		{
			parse_from(entry->d_name, depth + 1);
		}

		closedir(dir);
		return 0;
	}
	else if (S_ISREG(st.st_mode))
	{
		for (uintmax_t i = 0; i < depth; i++)
			putchar(' ');

		printf("%s: %jd bytes\n", path, get_size(st));
		return 0;
	}
	else
	{
	}

	return 2;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	parse_from(argv[1], 0);
}