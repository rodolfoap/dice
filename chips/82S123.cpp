#include "../chip_desc.h"
#include "rom.h"

/*
82S123
256-Bit Bipolar PROM (32x8)

   +------+
O1 |1   16| Vcc
O2 |2   15| CE/
O3 |3   14| A4
O4 |4   13| A3
O5 |5   12| A2
O6 |6   11| A1
O7 |7   10| A0
GND|8    9| O8
   +------+
*/

// Assumes chip outputs are pulled high when chip is disabled

template <int BIT>
CHIP_LOGIC( 82S123 )
{
    const RomDesc* rom = (const RomDesc*)custom_data;

    int addr = (pin[14] << 4) | (pin[13] << 3) | (pin[12] << 2) | (pin[11] << 1) | pin[10]; 

    int P = (BIT < 7) ? (BIT + 1) : (BIT + 2);
    
    if(pin[15]) pin[P] = 1;
    else        pin[P] = (RomDesc::get_data(rom, addr) >> BIT) & 1;
}

// TODO: Adjust delay from chip enable?
CHIP_DESC( 82S123 ) = 
{
    CHIP_START( 82S123<0> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<1> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<2> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<3> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<4> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<5> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<6> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

    CHIP_START( 82S123<7> )
        INPUT_PINS( 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 35.0, 35.0 ),

   	CHIP_DESC_END
};

