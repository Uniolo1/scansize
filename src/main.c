// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense
#include <stdint.h>
#include <stdio.h>

#if defined(RUNNING_ON_POSIX) || defined(__unix__) || defined(__APPLE__)
#define RUNNING_ON_POSIX
#endif

#ifdef RUNNING_ON_POSIX
#include <sys/stat.h>

// implementation for POSIX systems
intmax_t get_size(const char *filename)
{
	struct stat st;
	if (stat(filename, &st) == -1)
		return -1; // failure

	return st.st_size;
}

#else

// implementation for non-POSIX systems
intmax_t get_size(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if (f == NULL)
		return -1;

	if (fseek(f, 0, SEEK_END) == -1)
		return -2;

	intmax_t size = ftell(f);
	fclose(f);

	return size;
}
#endif

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	printf("%jd bytes\n", get_size(argv[1]));
}