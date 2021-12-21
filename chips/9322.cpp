#include "../chip_desc.h"

/*
74157 (9322)
8-to-4 line noninverting data selector/multiplexer.

    +---+--+---+
  S |1  +--+ 16| VCC
1A0 |2       15| /EN
1A1 |3       14| 4A0
 1Y |4   74  13| 4A1
2A0 |5  157  12| 4Y
2A1 |6       11| 3A0
 2Y |7       10| 3A1
GND |8        9| 3Y
    +----------+
*/

static CHIP_LOGIC( 9322A )
{
    if(pin[15])
        pin[4] = 0;
    else if(pin[1])
        pin[4] = pin[3];
    else
        pin[4] = pin[2];
}

static CHIP_LOGIC( 9322B )
{
    if(pin[15])
        pin[7] = 0;
    else if(pin[1])
        pin[7] = pin[6];
    else
        pin[7] = pin[5];
}

static CHIP_LOGIC( 9322C )
{
    if(pin[15])
        pin[9] = 0;
    else if(pin[1])
        pin[9] = pin[10];
    else
        pin[9] = pin[11];
}

static CHIP_LOGIC( 9322D )
{
    if(pin[15])
        pin[12] = 0;
    else if(pin[1])
        pin[12] = pin[13];
    else
        pin[12] = pin[14];
}

// tPLH from Data: 10   Select: 17 Enable: 12
// tPHL from Data: 11.5 Select: 20 Enable: 19
// TODO: Make delays more accurate

CHIP_DESC( 9322 ) =
{
	CHIP_START( 9322A )
        INPUT_PINS( 1, 2, 3, 15 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 14.0, 16.0 ),

	CHIP_START( 9322B )
        INPUT_PINS( 1, 5, 6, 15 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 14.0, 16.0 ),

	CHIP_START( 9322C )
        INPUT_PINS( 1, 10, 11, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 14.0, 16.0 ),

	CHIP_START( 9322D )
        INPUT_PINS( 1, 13, 14, 15 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 14.0, 16.0 ),

   	CHIP_DESC_END
};

