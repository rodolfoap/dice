#ifndef N8225_H
#define N8225_H

#include "../chip_desc.h"

#define N8225_WORD_SIZE 4
#define N8225_WORD_COUNT 16

typedef uint8_t N8225Desc[N8225_WORD_COUNT][N8225_WORD_SIZE];

extern CHIP_DESC( 8225 );

#endif
