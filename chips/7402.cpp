#include "../chip_desc.h"

/*
7402
Quad 2-input NOR gates.

    +---+--+---+             +---+---*---+           ___
/1Y |1  +--+ 14| VCC         | A | B |/Y |      /Y = A+B
 1A |2       13| /4Y         +===+===*===+
 1B |3       12| 4B          | 0 | 0 | 1 |
/2Y |4  7402 11| 4A          | 0 | 1 | 0 |
 2A |5       10| /3Y         | 1 | 0 | 0 |
 2B |6        9| 3B          | 1 | 1 | 0 |
GND |7        8| 3A          +---+---*---+
    +----------+
*/

static CHIP_LOGIC( 7402A )
{
	pin[1] = (pin[2] | pin[3]) ^ 1;
}

static CHIP_LOGIC( 7402B )
{
    pin[4] = (pin[5] | pin[6]) ^ 1;
}

static CHIP_LOGIC( 7402C )
{
    pin[10] = (pin[8] | pin[9]) ^ 1;
}

static CHIP_LOGIC( 7402D )
{
    pin[13] = (pin[11] | pin[12]) ^ 1;
}

CHIP_DESC( 7402 ) =
{
	CHIP_START( 7402A )
        INPUT_PINS( 2, 3 )
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_START( 7402B )
        INPUT_PINS( 5, 6 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_START( 7402C )
        INPUT_PINS( 8, 9 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_START( 7402D )
        INPUT_PINS( 11, 12 )
        OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_DESC_END
};

