#include "../chip_desc.h"

/*
7404
Hex inverters.

    +---+--+---+             +---*---+               _
 1A |1  +--+ 14| VCC         | A |/Y |          /Y = A
/1Y |2       13| 6A          +===*===+
 2A |3       12| /6Y         | 0 | 1 |
/2Y |4  7404 11| 5A          | 1 | 0 |
 3A |5       10| /5Y         +---*---+
/3Y |6        9| 4A
GND |7        8| /4Y
    +----------+
*/

static CHIP_LOGIC( 7404A )
{
	pin[2] = pin[1] ^ 1;
}

static CHIP_LOGIC( 7404B )
{
	pin[4] = pin[3] ^ 1;
}

static CHIP_LOGIC( 7404C )
{
	pin[6] = pin[5] ^ 1;
}

static CHIP_LOGIC( 7404D )
{
	pin[8] = pin[9] ^ 1;
}

static CHIP_LOGIC( 7404E )
{
	pin[10] = pin[11] ^ 1;
}

static CHIP_LOGIC( 7404F )
{
	pin[12] = pin[13] ^ 1;
}

CHIP_DESC( 7404 ) =
{
	CHIP_START( 7404A )
        INPUT_PINS( 1 ) OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_START( 7404B )
        INPUT_PINS( 3 ) OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),
	
    CHIP_START( 7404C )
        INPUT_PINS( 5 ) OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_START( 7404D )
        INPUT_PINS( 9 ) OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

 	CHIP_START( 7404E )
        INPUT_PINS( 11 ) OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

 	CHIP_START( 7404F )
        INPUT_PINS( 13 ) OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 12.0, 8.0 ),

	CHIP_DESC_END
};

CHIP_DESC( 74H04 ) =
{
	CHIP_START( 7404A )
        INPUT_PINS( 1 ) OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 6.0, 6.5 ),

	CHIP_START( 7404B )
        INPUT_PINS( 3 ) OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 6.0, 6.5 ),
	
    CHIP_START( 7404C )
        INPUT_PINS( 5 ) OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 6.0, 6.5 ),

	CHIP_START( 7404D )
        INPUT_PINS( 9 ) OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 6.0, 6.5 ),

 	CHIP_START( 7404E )
        INPUT_PINS( 11 ) OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 6.0, 6.5 ),

 	CHIP_START( 7404F )
        INPUT_PINS( 13 ) OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 6.0, 6.5 ),

	CHIP_DESC_END
};

// TODO: Adjust tp_lh based up pull up resistor value?
CHIP_DESC( 7405 ) =
{
	CHIP_START( 7404A )
        INPUT_PINS( 1 ) OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 30.0, 8.0 ),

	CHIP_START( 7404B )
        INPUT_PINS( 3 ) OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 30.0, 8.0 ),
	
    CHIP_START( 7404C )
        INPUT_PINS( 5 ) OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 30.0, 8.0 ),

	CHIP_START( 7404D )
        INPUT_PINS( 9 ) OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 30.0, 8.0 ),

 	CHIP_START( 7404E )
        INPUT_PINS( 11 ) OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 30.0, 8.0 ),

 	CHIP_START( 7404F )
        INPUT_PINS( 13 ) OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 30.0, 8.0 ),

	CHIP_DESC_END
};

