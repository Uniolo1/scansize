// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#if !defined(RUNNING_ON_POSIX) && !defined(__unix__) && !defined(__APPLE__)
#error                                                                         \
    "This software is intended for POSIX Systems, compile with 'RUNNING_ON_POSIX' to override this check!"
#endif

#include "h/parse.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	return parse_from(argv[1], 0);
}