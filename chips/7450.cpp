#include "../chip_desc.h"

/*
7450
Dual 2-Wide 2-input AND-NOR gates.

    +---+--+---+                  __________
 1A |1  +--+ 14| VCC        /Y = (A.B)+(C.D)
 2A |2       13| 1B
 2B |3       12|
 2C |4  7450 11|
 2D |5       10| 1D
/2Y |6        9| 1C
GND |7        8| /1Y
    +----------+

*/

static CHIP_LOGIC( 7450A )
{
	pin[8] = ((pin[1] & pin[13]) | (pin[9] & pin[10])) ^ 1;
}

static CHIP_LOGIC( 7450B )
{
    pin[6] = ((pin[2] & pin[3]) | (pin[4] & pin[5])) ^ 1;
}

CHIP_DESC( 7450 ) =
{
	CHIP_START( 7450A )
        INPUT_PINS( 1, 9, 10, 13 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 13.0, 8.0 ),

	CHIP_START( 7450B )
        INPUT_PINS( 2, 3, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 13.0, 8.0 ),

	CHIP_DESC_END
};


/* 7451 is basically the same as 7450, but
 * separating out the AND gates helps performance
 * in TV basketball. */
static CHIP_LOGIC( 7451_AND1 )
{
	pin[i1] = pin[1] & pin[13];
}

static CHIP_LOGIC( 7451_AND2 )
{
    pin[i2] = pin[9] & pin[10];
}

static CHIP_LOGIC( 7451_NOR1 )
{
    pin[8] = (pin[i1] | pin[i2]) ^ 1;
}

CHIP_DESC( 7451 ) =
{
	CHIP_START( 7451_AND1 )
        INPUT_PINS( 1, 13 )
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 7451_AND2 )
        INPUT_PINS( 9, 10 )
        OUTPUT_PIN( i2 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 7451_NOR1 )
        INPUT_PINS( i1, i2 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 8.0, 3.0 ),

	CHIP_START( 7450B )
        INPUT_PINS( 2, 3, 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 13.0, 8.0 ),

	CHIP_DESC_END
};

