#include "../chip_desc.h"

/*
7421
Dual 4-input AND gates.

    +---+--+---+             +---+---+---+---*---+
 1A |1  +--+ 14| VCC         | A | B | C | D | Y |    Y = ABCD
 1B |2       13| 2D          +===+===+===+===*===+
    |3       12| 2C          | 0 | X | X | X | 0 |
 1C |4  7421 11|             | 1 | 0 | X | X | 0 |
 1D |5       10| 2B          | 1 | 1 | 0 | X | 0 |
 1Y |6        9| 2A          | 1 | 1 | 1 | 0 | 0 |
GND |7        8| 2Y          | 1 | 1 | 1 | 1 | 1 |
    +----------+             +---+---+---+---*---+
*/

static CHIP_LOGIC( 7421A )
{
	pin[6] = pin[1] & pin[2] & pin[4] & pin[5];
}

static CHIP_LOGIC( 7421B )
{
	pin[8] = pin[9] & pin[10] & pin[12] & pin[13];
}

// Only 74LS21 and 74H21 exist?
// Using 74H timing
CHIP_DESC( 7421 ) =
{
	CHIP_START( 7421A )
        INPUT_PINS( 1, 2, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_START( 7421B )
        INPUT_PINS( 9, 10, 12, 13 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_DESC_END
};

