#include "../chip_desc.h"

/*
7425
Dual 4-input NOR gates.

    +---+--+---+                 _________
 1A |1  +--+ 14| VCC         Y = (A+B+C+D)
 1B |2       13| 2D
    |3       12| 2C
 1C |4  7425 11|
 1D |5       10| 2B
/1Y |6        9| 2A
GND |7        8| /2Y
    +----------+
*/

static CHIP_LOGIC( 7425A )
{
	pin[6] = (pin[1] | pin[2] | pin[4] | pin[5]) ^ 1;
}

static CHIP_LOGIC( 7425B )
{
	pin[8] = (pin[9] | pin[10] | pin[12] | pin[13]) ^ 1;
}

CHIP_DESC( 7425 ) =
{
	CHIP_START( 7425A )
        INPUT_PINS( 1, 2, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 13.0, 8.0 ),

	CHIP_START( 7425B )
        INPUT_PINS( 9, 10, 12, 13 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 13.0, 8.0 ),

	CHIP_DESC_END
};

