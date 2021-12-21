#include "../chip_desc.h"

/*
74S112
Dual negative-edge-triggered J-K flip-flops with set and reset.

      +---+--+---+           +---+---+----+----+----*---+---+
/1CLK |1  +--+ 16| VCC       | J | K |/CLK|/SET|/RST| Q |/Q |
   1K |2       15| /1RST     +===+===+====+====+====*===+===+
   1J |3       14| /2RST     | X | X |  X |  0 |  0 | 0 | 0 |
/1SET |4   74  13| /2CLK     | X | X |  X |  0 |  1 | 1 | 0 |
   1Q |5  112  12| 2K        | X | X |  X |  1 |  0 | 0 | 1 |
  /1Q |6       11| 2J        | 0 | 0 |  \ |  1 |  1 | - | - |
  /2Q |7       10| /2SET     | 0 | 1 |  \ |  1 |  1 | 0 | 1 |
  GND |8        9| 2Q        | 1 | 0 |  \ |  1 |  1 | 1 | 0 |
      +----------+           | 1 | 1 |  \ |  1 |  1 |/Q | Q |
                             | X | X | !\ |  1 |  1 | - | - |
                             +---+---+----+----+----*---+---+
*/

static CHIP_LOGIC( 74S112A )
{
	if(!pin[4] && !pin[15])
		pin[i1] = 1;
    else if(!pin[4] && pin[15])
        pin[i1] = 1;
    else if(pin[4] && !pin[15])
        pin[i1] = 0;
	else if(NEG_EDGE_PIN(1))
	{
		if(!pin[3] && !pin[2])
			pin[i1] = prev_pin[i1];
		else if(!pin[3] && pin[2])
			pin[i1] = 0;
		else if(pin[3] && !pin[2])
			pin[i1] = 1;
		else
			pin[i1] = prev_pin[i1] ^ 1;
	}
	else
		pin[i1] = prev_pin[i1];
}

static CHIP_LOGIC( 74S112A_Q )
{
    pin[5] = pin[i1];
}

static CHIP_LOGIC( 74S112A_Qn )
{
    if(!pin[4] && !pin[15])
        pin[6] = 1;
    else
        pin[6] = pin[i1] ^ 1;
}



static CHIP_LOGIC( 74S112B )
{
	if(!pin[10] && !pin[14])
		pin[i2] = 1;
    else if(!pin[10] && pin[14])
        pin[i2] = 1;
    else if(pin[10] && !pin[14])
        pin[i2] = 0;
	else if(NEG_EDGE_PIN(13))
	{
		if(!pin[11] && !pin[12])
			pin[i2] = prev_pin[i2];
		else if(!pin[11] && pin[12])
			pin[i2] = 0;
		else if(pin[11] && !pin[12])
			pin[i2] = 1;
		else
			pin[i2] = prev_pin[i2] ^ 1;
	}
	else
		pin[i2] = prev_pin[i2];
}

static CHIP_LOGIC( 74S112B_Q )
{
    pin[9] = pin[i2];
}

static CHIP_LOGIC( 74S112B_Qn )
{
    if(!pin[10] && !pin[14])
        pin[7] = 1;
    else
        pin[7] = pin[i2] ^ 1;
}



CHIP_DESC( 74S112 ) =
{
	CHIP_START( 74S112A )
		INPUT_PINS( 1, 2, 3, 4, 15 )
		OUTPUT_PIN( i1 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 3.0, 3.0 ),

	CHIP_START( 74S112A_Q )
		INPUT_PINS( i1 )
		OUTPUT_PIN( 5 )
		OUTPUT_DELAY_NS( 1.0, 2.0 ),

	CHIP_START( 74S112A_Qn )
		INPUT_PINS( 4, 15, i1 )
		OUTPUT_PIN( 6 )
		OUTPUT_DELAY_NS( 1.0, 2.0 ),


	CHIP_START( 74S112B )
		INPUT_PINS( 10, 11, 12, 13, 14 )
		OUTPUT_PIN( i2 )
		EVENT_PINS( 13 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 3.0, 3.0 ),

	CHIP_START( 74S112B_Q )
		INPUT_PINS( i2 )
		OUTPUT_PIN( 9 )
		OUTPUT_DELAY_NS( 1.0, 2.0 ),

	CHIP_START( 74S112B_Qn )
		INPUT_PINS( 10, 14, i2 )
		OUTPUT_PIN( 7 )
		OUTPUT_DELAY_NS( 1.0, 2.0 ),

	CHIP_DESC_END
};
