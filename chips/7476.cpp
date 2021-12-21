#include "../chip_desc.h"

/*
7476
Dual J-K flip-flops with set and reset.

      +---+--+---+           +---+---+---+----+----*---+---+
/1CLK |1  +--+ 16| 1K        | J | K |CLK|/SET|/RST| Q |/Q |
/1SET |2       15| 1Q        +===+===+===+====+====*===+===+
/1RST |3       14| /1Q       | X | X | X |  0 |  0 | 0 | 0 |
   1J |4       13| GND       | X | X | X |  0 |  1 | 1 | 0 |
  VCC |5  7476 12| 2K        | X | X | X |  1 |  0 | 0 | 1 |
/2CLK |6       11| 2Q        | 0 | 0 | /\|  1 |  1 | - | - |
/2SET |7       10| /2Q       | 0 | 1 | /\|  1 |  1 | 0 | 1 |
/2RST |8        9| 2J        | 1 | 0 | /\|  1 |  1 | 1 | 0 |
      +----------+           | 1 | 1 | /\|  1 |  1 |/Q | Q |
                             | X | X |!/\|  1 |  1 | - | - |
                             +---+---+---+----+----*---+---+
*/

// TODO: Make positive pulse triggered instead of negative edge

static CHIP_LOGIC( 7476A )
{
	if(!pin[2] && !pin[3])
		pin[i1] = 0;
    else if(!pin[2] && pin[3])
        pin[i1] = 1;
    else if(pin[2] && !pin[3])
        pin[i1] = 0;
	else if(NEG_EDGE_PIN(1))
	{
		if(!pin[4] && !pin[16])
			pin[i1] = prev_pin[i1];
		else if(!pin[4] && pin[16])
			pin[i1] = 0;
		else if(pin[4] && !pin[16])
			pin[i1] = 1;
		else
			pin[i1] = prev_pin[i1] ^ 1;
	}
	else
		pin[i1] = prev_pin[i1];
}

static CHIP_LOGIC( 7476A_Q )
{
    pin[15] = pin[i1];
}

static CHIP_LOGIC( 7476A_Qn )
{
    if(!pin[2] && !pin[3])
		pin[14] = 0;
    else
        pin[14] = pin[i1] ^ 1;
}



static CHIP_LOGIC( 7476B )
{
	if(!pin[7] && !pin[8])
		pin[i2] = 0;
    else if(!pin[7] && pin[8])
        pin[i2] = 1;
    else if(pin[7] && !pin[8])
        pin[i2] = 0;
	else if(NEG_EDGE_PIN(6))
	{
		if(!pin[9] && !pin[12])
			pin[i2] = prev_pin[i2];
		else if(!pin[9] && pin[12])
			pin[i2] = 0;
		else if(pin[9] && !pin[12])
			pin[i2] = 1;
		else
			pin[i2] = prev_pin[i2] ^ 1;
	}
	else
		pin[i2] = prev_pin[i2];
}

static CHIP_LOGIC( 7476B_Q )
{
    pin[11] = pin[i2];
}

static CHIP_LOGIC( 7476B_Qn )
{
    if(!pin[7] && !pin[8])
		pin[10] = 0;
    else
        pin[10] = pin[i2] ^ 1;
}



CHIP_DESC( 7476 ) =
{
	CHIP_START( 7476A )
		INPUT_PINS( 1, 2, 3, 4, 16 )
		OUTPUT_PIN( i1 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 10.0, 10.0 ),

	CHIP_START( 7476A_Q )
		INPUT_PINS( i1 )
		OUTPUT_PIN( 15 )
		OUTPUT_DELAY_NS( 6.0, 15.0 ),

	CHIP_START( 7476A_Qn )
		INPUT_PINS( i1, 2, 3 )
		OUTPUT_PIN( 14 )
		OUTPUT_DELAY_NS( 6.0, 15.0 ),

	CHIP_START( 7476B )
		INPUT_PINS( 6, 7, 8, 9, 12 )
		OUTPUT_PIN( i2 )
		EVENT_PINS( 6 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 10.0, 10.0 ),

	CHIP_START( 7476B_Q )
		INPUT_PINS( i2 )
		OUTPUT_PIN( 11 )
		OUTPUT_DELAY_NS( 6.0, 15.0 ),

	CHIP_START( 7476B_Qn )
		INPUT_PINS( i2, 7, 8 )
		OUTPUT_PIN( 10 )
		OUTPUT_DELAY_NS( 6.0, 15.0 ),

	CHIP_DESC_END
};

