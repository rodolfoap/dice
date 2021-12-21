#include "../chip_desc.h"

/*
7407
Hex open-collector high-voltage buffers. 

    +---+--+---+             +---*---+
 1A |1  +--+ 14| VCC         | A | Y |           Y = A
 1Y |2       13| 6A          +===*===+
 2A |3       12| 6Y          | 0 | 0 |
 2Y |4  7407 11| 5A          | 1 | Z |
 3A |5       10| 5Y          +---*---+
 3Y |6        9| 4A
GND |7        8| 4Y
    +----------+
*/

static CHIP_LOGIC( 7407A )
{
	pin[2] = pin[1];
}

static CHIP_LOGIC( 7407B )
{
	pin[4] = pin[3];
}

static CHIP_LOGIC( 7407C )
{
	pin[6] = pin[5];
}

static CHIP_LOGIC( 7407D )
{
	pin[8] = pin[9];
}

static CHIP_LOGIC( 7407E )
{
	pin[10] = pin[11];
}

static CHIP_LOGIC( 7407F )
{
	pin[12] = pin[13];
}

// TODO: Adjust timings based on pull-up resistor
CHIP_DESC( 7407 ) =
{
	CHIP_START( 7407A )
        INPUT_PINS( 1 ) OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 6.0, 20.0 ),

	CHIP_START( 7407B )
        INPUT_PINS( 3 ) OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 6.0, 20.0 ),
	
    CHIP_START( 7407C )
        INPUT_PINS( 5 ) OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 6.0, 20.0 ),

	CHIP_START( 7407D )
        INPUT_PINS( 9 ) OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 6.0, 20.0 ),

 	CHIP_START( 7407E )
        INPUT_PINS( 11 ) OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 6.0, 20.0 ),

 	CHIP_START( 7407F )
        INPUT_PINS( 13 ) OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 6.0, 20.0 ),

	CHIP_DESC_END
};

