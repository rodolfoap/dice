#include "../chip_desc.h"

/*
7410
Triple 3-input NAND gates.

    +---+--+---+             +---+---+---*---+       ___
 1A |1  +--+ 14| VCC         | A | B | C |/Y |  /Y = ABC
 1B |2       13| 1C          +===+===+===*===+
 2A |3       12| /1Y         | 0 | X | X | 1 |
 2B |4  7410 11| 3C          | 1 | 0 | X | 1 |
 2C |5       10| 3B          | 1 | 1 | 0 | 1 |
/2Y |6        9| 3A          | 1 | 1 | 1 | 0 |
GND |7        8| /3Y         +---+---+---*---+
    +----------+
*/

static CHIP_LOGIC( 7410A )
{
	pin[12] = (pin[1] & pin[2] & pin[13]) ^ 1;
}

static CHIP_LOGIC( 7410B )
{
    pin[6] = (pin[3] & pin[4] & pin[5]) ^ 1;
}

static CHIP_LOGIC( 7410C )
{
    pin[8] = (pin[9] & pin[10] & pin[11]) ^ 1;
}

CHIP_DESC( 7410 ) =
{
	CHIP_START( 7410A )
        INPUT_PINS( 1, 2, 13 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

	CHIP_START( 7410B )
        INPUT_PINS( 3, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

	CHIP_START( 7410C )
        INPUT_PINS( 9, 10, 11 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

    CHIP_DESC_END
};

