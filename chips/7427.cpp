#include "../chip_desc.h"

/*
7427
Triple 3-input NOR gates.

    +---+--+---+             +---+---+---*---+       _____
 1A |1  +--+ 14| VCC         | A | B | C |/Y |  /Y = A+B+C
 1B |2       13| 1C          +===+===+===*===+
 2A |3       12| /1Y         | 0 | 0 | 0 | 1 |
 2B |4  7427 11| 3C          | 0 | 0 | 1 | 0 |
 2C |5       10| 3B          | 0 | 1 | X | 0 |
/2Y |6        9| 3A          | 1 | X | X | 0 |
GND |7        8| /3Y         +---+---+---*---+
    +----------+
*/

static CHIP_LOGIC( 7427A )
{
	pin[12] = (pin[1] | pin[2] | pin[13]) ^ 1;
}

static CHIP_LOGIC( 7427B )
{
    pin[6] = (pin[3] | pin[4] | pin[5]) ^ 1;
}

static CHIP_LOGIC( 7427C )
{
    pin[8] = (pin[9] | pin[10] | pin[11]) ^ 1;
}

CHIP_DESC( 7427 ) =
{
	CHIP_START( 7427A )
        INPUT_PINS( 1, 2, 13 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 10.0, 7.0 ),

	CHIP_START( 7427B )
        INPUT_PINS( 3, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 10.0, 7.0 ),

	CHIP_START( 7427C )
        INPUT_PINS( 9, 10, 11 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 10.0, 7.0 ),

    CHIP_DESC_END
};

