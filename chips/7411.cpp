#include "../chip_desc.h"

/*
7411
Triple 3-input AND gates.

    +---+--+---+             +---+---+---*---+
 1A |1  +--+ 14| VCC         | A | B | C | Y |   Y = ABC
 1B |2       13| 1C          +===+===+===*===+
 2A |3       12| 1Y          | 0 | X | X | 0 |
 2B |4  7411 11| 3C          | 1 | 0 | X | 0 |
 2C |5       10| 3B          | 1 | 1 | 0 | 0 |
 2Y |6        9| 3A          | 1 | 1 | 1 | 1 |
GND |7        8| 3Y          +---+---+---*---+
    +----------+

*/

static CHIP_LOGIC( 7411A )
{
	pin[12] = pin[1] & pin[2] & pin[13];
}

static CHIP_LOGIC( 7411B )
{
    pin[6] = pin[3] & pin[4] & pin[5];
}

static CHIP_LOGIC( 7411C )
{
    pin[8] = pin[9] & pin[10] & pin[11];
}

// TI Datasheet only lists 74H11, so using H11 timing values
CHIP_DESC( 7411 ) =
{
	CHIP_START( 7411A )
        INPUT_PINS( 1, 2, 13 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_START( 7411B )
        INPUT_PINS( 3, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_START( 7411C )
        INPUT_PINS( 9, 10, 11 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

    CHIP_DESC_END
};

