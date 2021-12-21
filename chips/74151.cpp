#include "../chip_desc.h"

/*
74151
8-to-1 line data selector/multiplexer with complementary outputs.

    +---+--+---+
 D3 |1  +--+ 16| VCC
 D2 |2       15| D4
 D1 |3       14| D5
 D0 |4   74  13| D6
  Y |5  151  12| D7
 /Y |6       11| S0
/EN |7       10| S1
GND |8        9| S2
    +----------+
*/


static CHIP_LOGIC( 74151_n )
{
	if(pin[7])
    {
        pin[6] = 1;
    }
    else
    {
        switch(4*pin[9] + 2*pin[10] + pin[11])
        {
            case 0: pin[6] = pin[4] ^ 1; break;
            case 1: pin[6] = pin[3] ^ 1; break;
            case 2: pin[6] = pin[2] ^ 1; break;
            case 3: pin[6] = pin[1] ^ 1; break;
            case 4: pin[6] = pin[15] ^ 1; break;
            case 5: pin[6] = pin[14] ^ 1; break;
            case 6: pin[6] = pin[13] ^ 1; break;
            case 7: pin[6] = pin[12] ^ 1; break;
        }
    }
}

static CHIP_LOGIC( 74151 )
{
    pin[5] = pin[6] ^ 1;
}

/*
  From      To      tp_lh       tp_hl
  A,B,C     Y       25          25
  A,B,C     /Y      17          19       
  /EN       Y       21          22
  /EN       /Y      14          15
  D         Y       13          18
  D         /Y      8           8
*/

CHIP_DESC( 74151 ) =
{
	CHIP_START( 74151 )
        INPUT_PINS( 6 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 7.0, 7.0 ),

	CHIP_START( 74151_n )
        INPUT_PINS( 1, 2, 3, 4, 7, 9, 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 14.0, 15.0 ),

   	CHIP_DESC_END
};


CHIP_DESC( 74S151 ) =
{
	CHIP_START( 74151 )
        INPUT_PINS( 6 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 2.0, 3.0 ),

	CHIP_START( 74151_n )
        INPUT_PINS( 1, 2, 3, 4, 7, 9, 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 9.0, 8.5 ),

   	CHIP_DESC_END
};

