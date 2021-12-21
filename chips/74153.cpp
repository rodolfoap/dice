#include "../chip_desc.h"

/*
74153
8-to-2 line noninverting data selector/multiplexer with separate enables.

     +---+--+---+
/1EN |1  +--+ 16| VCC
  S1 |2       15| /2EN
 1A3 |3       14| S0
 1A2 |4   74  13| 2A3
 1A1 |5  153  12| 2A2
 1A0 |6       11| 2A1
  1Y |7       10| 2A0
 GND |8        9| 2Y
     +----------+
*/

static CHIP_LOGIC( 74153A )
{
	if(pin[1])                    pin[7] = 0;
    else if(!pin[14] && !pin[2])  pin[7] = pin[6];
    else if( pin[14] && !pin[2])  pin[7] = pin[5];
    else if(!pin[14] &&  pin[2])  pin[7] = pin[4];
    else                          pin[7] = pin[3];
}

static CHIP_LOGIC( 74153B )
{
	if(pin[15])                   pin[9] = 0;
    else if(!pin[14] && !pin[2])  pin[9] = pin[10];
    else if( pin[14] && !pin[2])  pin[9] = pin[11];
    else if(!pin[14] &&  pin[2])  pin[9] = pin[12];
    else                          pin[9] = pin[13];
}

// tPLH from Data: 12 Select: 22 Strobe: 19
// tPHL from Data: 15 Select: 22 Strobe: 15

CHIP_DESC( 74153 ) =
{
	CHIP_START( 74153A )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 14 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 16.0, 18.0 ),

	CHIP_START( 74153B )
        INPUT_PINS( 2, 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 16.0, 18.0 ),

   	CHIP_DESC_END
};

