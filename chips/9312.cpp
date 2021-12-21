#include "../chip_desc.h"

/*
9312
8-to-1 line data selector/multiplexer with complementary outputs.
(Same as 74151, but different pinout)

    +---+--+---+
 D0 |1  +--+ 16| VCC
 D1 |2       15| Y
 D2 |3       14| /Y
 D3 |4  9312 13| S2
 D4 |5       12| S1
 D5 |6       11| S0
 D6 |7       10| /EN
GND |8        9| D7
    +----------+
*/



static CHIP_LOGIC( 9312_n )
{
	if(pin[10])
    {
        pin[14] = 1;
    }
    else
    {
        switch(4*pin[13] + 2*pin[12] + pin[11])
        {
            case 0: pin[14] = pin[1] ^ 1; break;
            case 1: pin[14] = pin[2] ^ 1; break;
            case 2: pin[14] = pin[3] ^ 1; break;
            case 3: pin[14] = pin[4] ^ 1; break;
            case 4: pin[14] = pin[5] ^ 1; break;
            case 5: pin[14] = pin[6] ^ 1; break;
            case 6: pin[14] = pin[7] ^ 1; break;
            case 7: pin[14] = pin[9] ^ 1; break;
        }
    }
}

static CHIP_LOGIC( 9312 )
{
    pin[15] = pin[14] ^ 1;
}

/*
 * Taken from graph, may not be accurate
  From      To      tp_lh       tp_hl
  A,B,C     Y       23-24       25
  A,B,C     /Y      17          17
  /EN       /Y      11          15
  D         /Y      8           11
*/

CHIP_DESC( 9312 ) =
{
	CHIP_START( 9312 )
        INPUT_PINS( 14 )
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 6.0, 8.0 ),

	CHIP_START( 9312_n )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13 )
        OUTPUT_PIN( 14 )
        OUTPUT_DELAY_NS( 10.0, 10.0 ),

   	CHIP_DESC_END
};

