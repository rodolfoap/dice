#include "../chip_desc.h"

/*
74279
Quad /S-/R latches.

     +---+--+---+
 /1R |1  +--+ 16| VCC
/1S1 |2       15| /4S
/1S2 |3       14| /4R
  1Q |4   74  13| 4Q
 /2R |5  279  12| /3S2
 /2S |6       11| /3S1
  2Q |7       10| /3R
 GND |8        9| 3Q
     +----------+

*/

static CHIP_LOGIC( 74279A )
{
    if(!pin[2] || !pin[3])
        pin[4] = 1;
    else if(!pin[1])
        pin[4] = 0;
    else
        pin[4] = prev_pin[4];
}

static CHIP_LOGIC( 74279B )
{
    if(!pin[6])
        pin[7] = 1;
    else if(!pin[5])
        pin[7] = 0;
    else
        pin[7] = prev_pin[7];
}

static CHIP_LOGIC( 74279C )
{
    if(!pin[11] || !pin[12])
        pin[9] = 1;
    else if(!pin[10])
        pin[9] = 0;
    else
        pin[9] = prev_pin[9];
}

static CHIP_LOGIC( 74279D )
{
    if(!pin[15])
        pin[13] = 1;
    else if(!pin[14])
        pin[13] = 0;
    else
        pin[13] = prev_pin[13];
}

// TODO: Accurate timing?
// tplh S->Q = 12,
// tphl S->Q = 9,
// tphl R->Q = 15

CHIP_DESC( 74279 ) =
{
	CHIP_START( 74279A )
        INPUT_PINS( 1, 2, 3 )
        OUTPUT_PIN( 4 )
        PREV_OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 74279B )
        INPUT_PINS( 5, 6 )
        OUTPUT_PIN( 7 )
        PREV_OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 74279C )
        INPUT_PINS( 10, 11, 12 )
        OUTPUT_PIN( 9 )
        PREV_OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 74279D )
        INPUT_PINS( 14, 15 )
        OUTPUT_PIN( 13 )
        PREV_OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_DESC_END
};
