#include "../chip_desc.h"

/*
74109
Dual J-/K flip-flops with set and reset.

      +---+--+---+           +---+---+---+----+----*---+---+
/1RST |1  +--+ 16| VCC       | J |/K |CLK|/SET|/RST| Q |/Q |
   1J |2       15| /2RST     +===+===+===+====+====*===+===+
  /1K |3       14| 2J        | X | X | X |  0 |  0 | 1 | 1 |
 1CLK |4   74  13| /2K       | X | X | X |  0 |  1 | 1 | 0 |
/1SET |5  109  12| 2CLK      | X | X | X |  1 |  0 | 0 | 1 |
   1Q |6       11| /2SET     | 0 | 0 | / |  1 |  1 | 0 | 1 |
  /1Q |7       10| 2Q        | 0 | 1 | / |  1 |  1 | - | - |
  GND |8        9| /2Q       | 1 | 0 | / |  1 |  1 |/Q | Q |
      +----------+           | 1 | 1 | / |  1 |  1 | 1 | 0 |
                             | X | X |!/ |  1 |  1 | - | - |
                             +---+---+---+----+----*---+---+
*/

static CHIP_LOGIC( 74109A )
{
	if(!pin[1] && !pin[5])
		pin[i1] = 1;
    else if(pin[1] && !pin[5])
        pin[i1] = 1;
    else if(!pin[1] && pin[5])
        pin[i1] = 0;
	else if(POS_EDGE_PIN(4))
	{
		if(!pin[2] && pin[3])
			pin[i1] = prev_pin[i1];
		else if(!pin[2] && !pin[3])
			pin[i1] = 0;
		else if(pin[2] && pin[3])
			pin[i1] = 1;
		else
			pin[i1] = prev_pin[i1] ^ 1;
	}
	else
		pin[i1] = prev_pin[i1];
}

static CHIP_LOGIC( 74109A_Q )
{
    pin[6] = pin[i1];
}

static CHIP_LOGIC( 74109A_Qn )
{
    if(!pin[1] && !pin[5])
		pin[7] = 1;
    else
        pin[7] = pin[i1] ^ 1;
}



static CHIP_LOGIC( 74109B )
{
	if(!pin[15] && !pin[11])
		pin[i2] = 1;
    else if(pin[15] && !pin[11])
        pin[i2] = 1;
    else if(!pin[15] && pin[11])
        pin[i2] = 0;
	else if(POS_EDGE_PIN(12))
	{
		if(!pin[14] && pin[13])
			pin[i2] = prev_pin[i2];
		else if(!pin[14] && !pin[13])
			pin[i2] = 0;
		else if(pin[14] && pin[13])
			pin[i2] = 1;
		else
			pin[i2] = prev_pin[i2] ^ 1;
	}
	else
		pin[i2] = prev_pin[i2];
}

static CHIP_LOGIC( 74109B_Q )
{
    pin[10] = pin[i2];
}

static CHIP_LOGIC( 74109B_Qn )
{
    if(!pin[15] && !pin[11])
		pin[9] = 1;
    else
        pin[9] = pin[i2] ^ 1;
}



CHIP_DESC( 74109 ) =
{
	CHIP_START( 74109A )
		INPUT_PINS( 1, 2, 3, 4, 5 )
		OUTPUT_PIN( i1 )
		EVENT_PINS( 4 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 74109A_Q )
		INPUT_PINS( i1 )
		OUTPUT_PIN( 6 )
		OUTPUT_DELAY_NS( 5.0, 13.0 ),

	CHIP_START( 74109A_Qn )
		INPUT_PINS( i1, 1, 5 )
		OUTPUT_PIN( 7 )
		OUTPUT_DELAY_NS( 5.0, 13.0 ),

	CHIP_START( 74109B )
		INPUT_PINS( 11, 12, 13, 14, 15 )
		OUTPUT_PIN( i2 )
		EVENT_PINS( 12 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 74109B_Q )
		INPUT_PINS( i2 )
		OUTPUT_PIN( 10 )
		OUTPUT_DELAY_NS( 5.0, 13.0 ),

	CHIP_START( 74109B_Qn )
		INPUT_PINS( i2, 15, 11 )
		OUTPUT_PIN( 9 )
		OUTPUT_DELAY_NS( 5.0, 13.0 ),

	CHIP_DESC_END
};

