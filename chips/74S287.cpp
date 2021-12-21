#include "../chip_desc.h"
#include "rom.h"

/*
74S287
1024-Bit Bipolar PROM (256x4)

    +---+--+---+
 A6 |1  +--+ 16| VCC
 A5 |2       15| A7
 A4 |3       14| /EN2
 A3 |4   74  13| /EN1
 A0 |5  287  12| Q0
 A1 |6       11| Q1
 A2 |7       10| Q2
GND |8        9| Q3
    +----------+
*/

// TODO: Chip enables assume outputs are pulled up... add tri-state logic?

template <int BIT>
CHIP_LOGIC( 74S287 )
{
    const RomDesc* rom = (const RomDesc*)custom_data;

    int addr = (pin[15] << 7) | (pin[1] << 6) | (pin[2] << 5) | (pin[3] << 4) |
               (pin[4] << 3) | (pin[7] << 2) | (pin[6] << 1) | pin[5];

    int P = 12 - BIT;
    
    if(pin[13] || pin[14])
        pin[P] = 1;
    else
        pin[P] = (RomDesc::get_data(rom, addr) >> BIT) & 1;
}

// TODO: Delay from chip enable should be lower?

CHIP_DESC( 74S287 ) = 
{
    CHIP_START( 74S287<0> )
        INPUT_PINS( 5, 6, 7, 4, 3, 2, 1, 13, 14, 15 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 74S287<1> )
        INPUT_PINS( 5, 6, 7, 4, 3, 2, 1, 13, 14, 15 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 74S287<2> )
        INPUT_PINS( 5, 6, 7, 4, 3, 2, 1, 13, 14, 15 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 74S287<3> )
        INPUT_PINS( 5, 6, 7, 4, 3, 2, 1, 13, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

   	CHIP_DESC_END
};

