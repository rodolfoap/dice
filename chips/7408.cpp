#include "../chip_desc.h"

/*
7408
Quad 2-input AND gates.

    +---+--+---+             +---+---*---+
 1A |1  +--+ 14| VCC         | A | B | Y |       Y = AB
 1B |2       13| 4B          +===+===*===+
 1Y |3       12| 4A          | 0 | 0 | 0 |
 2A |4  7408 11| 4Y          | 0 | 1 | 0 |
 2B |5       10| 3B          | 1 | 0 | 0 |
 2Y |6        9| 3A          | 1 | 1 | 1 |
GND |7        8| 3Y          +---+---*---+
    +----------+
*/

static CHIP_LOGIC( 7408A )
{
	pin[3] = pin[1] & pin[2];
}

static CHIP_LOGIC( 7408B )
{
    pin[6] = pin[4] & pin[5];
}

static CHIP_LOGIC( 7408C )
{
    pin[8] = pin[9] & pin[10];
}

static CHIP_LOGIC( 7408D )
{
    pin[11] = pin[12] & pin[13];
}

CHIP_DESC( 7408 ) =
{
	CHIP_START( 7408A )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 17.5, 12.0 ),

	CHIP_START( 7408B )
        INPUT_PINS( 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 17.5, 12.0 ),

	CHIP_START( 7408C )
        INPUT_PINS( 9, 10 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 17.5, 12.0 ),

	CHIP_START( 7408D )
        INPUT_PINS( 12, 13 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 17.5, 12.0 ),

	CHIP_DESC_END
};


// Timing from Fairchild 9H08.  TI never made a 74H08 ?
CHIP_DESC( 74H08 ) =
{
	CHIP_START( 7408A )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_START( 7408B )
        INPUT_PINS( 4, 5 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_START( 7408C )
        INPUT_PINS( 9, 10 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_START( 7408D )
        INPUT_PINS( 12, 13 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 7.6, 8.8 ),

	CHIP_DESC_END
};

