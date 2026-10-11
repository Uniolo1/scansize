// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#if !defined(RUNNING_ON_POSIX) && !defined(__unix__) && !defined(__APPLE__)
#error                                                                         \
    "This software is intended for POSIX Systems, compile with 'RUNNING_ON_POSIX' to override this check!"
#endif

#define _POSIX_C_SOURCE 200112L
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

intmax_t get_size(const char *filename)
{
	struct stat st;
	if (stat(filename, &st) == -1)
		return -1; // failure

	return st.st_size;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	printf("%jd bytes\n", get_size(argv[1]));
}