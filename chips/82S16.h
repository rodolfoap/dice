#ifndef _82S16_H
#define _82S16_H

#include "../chip_desc.h"

#define _82S16_WORD_SIZE 1
#define _82S16_WORD_COUNT 256

typedef uint8_t Ram82S16Desc[_82S16_WORD_COUNT];

extern CHIP_DESC( 82S16 );

#endif
