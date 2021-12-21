#include "../chip_desc.h"

/*
7420
Dual 4-input NAND gates.

    +---+--+---+             +---+---+---+---*---+        ____
 1A |1  +--+ 14| VCC         | A | B | C | D |/Y |   /Y = ABCD
 1B |2       13| 2D          +===+===+===+===*===+
    |3       12| 2C          | 0 | X | X | X | 1 |
 1C |4  7420 11|             | 1 | 0 | X | X | 1 |
 1D |5       10| 2B          | 1 | 1 | 0 | X | 1 |
/1Y |6        9| 2A          | 1 | 1 | 1 | 0 | 1 |
GND |7        8| /2Y         | 1 | 1 | 1 | 1 | 0 |
    +----------+             +---+---+---+---*---+

*/

static CHIP_LOGIC( 7420A )
{
	pin[6] = (pin[1] & pin[2] & pin[4] & pin[5]) ^ 1;
}

static CHIP_LOGIC( 7420B )
{
	pin[8] = (pin[9] & pin[10] & pin[12] & pin[13]) ^ 1;
}

CHIP_DESC( 7420 ) =
{
	CHIP_START( 7420A )
        INPUT_PINS( 1, 2, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_START( 7420B )
        INPUT_PINS( 9, 10, 12, 13 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_DESC_END
};


// TODO: Propagation delay is (3, 3) or (4.5, 5) depending on load capacitance?
CHIP_DESC( 74S20 ) =
{
	CHIP_START( 7420A )
        INPUT_PINS( 1, 2, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 3.0, 3.0 ),

	CHIP_START( 7420B )
        INPUT_PINS( 9, 10, 12, 13 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 3.0, 3.0 ),

	CHIP_DESC_END
};

