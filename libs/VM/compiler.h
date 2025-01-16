#ifndef COMPILER_H
#define COMPILER_H

#include "../common.h"
#include "../representation/chunk.h"
#include "./scanner.h"

bool compile(char *source, Chunk *chunk);

#endif
