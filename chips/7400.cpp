#include "../chip_desc.h"

/*
7400
Quad 2-input NAND gates.

    +---+--+---+             +---+---*---+           __
 1A |1  +--+ 14| VCC         | A | B |/Y |      /Y = AB
 1B |2       13| 4B          +===+===*===+
/1Y |3       12| 4A          | 0 | 0 | 1 |
 2A |4  7400 11| /4Y         | 0 | 1 | 1 |
 2B |5       10| 3B          | 1 | 0 | 1 |
/2Y |6        9| 3A          | 1 | 1 | 0 |
GND |7        8| /3Y         +---+---*---+
    +----------+
*/

static CHIP_LOGIC( 7400A )
{
	pin[3] = (pin[1] & pin[2]) ^ 1;
}

static CHIP_LOGIC( 7400B )
{
    pin[6] = (pin[4] & pin[5]) ^ 1;
}

static CHIP_LOGIC( 7400C )
{
    pin[8] = (pin[9] & pin[10]) ^ 1;
}

static CHIP_LOGIC( 7400D )
{
    pin[11] = (pin[12] & pin[13]) ^ 1;
}

CHIP_DESC( 7400 ) =
{
	CHIP_START( 7400A )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

	CHIP_START( 7400B )
        INPUT_PINS( 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

	CHIP_START( 7400C )
        INPUT_PINS( 9, 10 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

	CHIP_START( 7400D )
        INPUT_PINS( 12, 13 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 11.0, 7.0 ),

	CHIP_DESC_END
};
