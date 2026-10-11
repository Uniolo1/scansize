#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include <stdint.h>

extern bool no_colors;

int parse_from(const char *path, uintmax_t depth);
#endif