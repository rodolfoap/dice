#ifndef _2533_H
#define _2533_H

#include "../chip_desc.h"

#define _2533_BIT_COUNT 1024

class Ram2533Desc
{
public:
    uint8_t data[_2533_BIT_COUNT];
    uint32_t addr;

    Ram2533Desc() : data{0}, addr(0) { }
};

extern CHIP_DESC( 2533 );

#endif
