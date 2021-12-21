#include "../chip_desc.h"

/*
7474
Dual D flip-flop with set and reset.

      +---+--+---+           +---+---+----+----*---+---+
/1RST |1  +--+ 14| VCC       | D |CLK|/SET|/RST| Q |/Q |
   1D |2       13| /2RST     +===+===+====+====*===+===+
 1CLK |3       12| 2D        | X | X |  0 |  0 | 1 | 1 |
/1SET |4  7474 11| 2CLK      | X | X |  0 |  1 | 1 | 0 |
   1Q |5       10| /2SET     | X | X |  1 |  0 | 0 | 1 |
  /1Q |6        9| 2Q        | 0 | / |  1 |  1 | 0 | 1 |
  GND |7        8| /2Q       | 1 | / |  1 |  1 | 1 | 0 |
      +----------+           | X |!/ |  1 |  1 | - | - |
                             +---+---+----+----*---+---+
*/

static CHIP_LOGIC( 7474A1 )
{
	if(!pin[4] && !pin[1])
		pin[i1] = 1;
	else if(!pin[4] && pin[1])
		pin[i1] = 1;	
	else if(pin[4] && !pin[1])
		pin[i1] = 0;
	else if(POS_EDGE_PIN(3))
		pin[i1] = pin[2];
	else
		pin[i1] = prev_pin[i1];
}

static CHIP_LOGIC( 7474A2 )
{
    if(!pin[1] && !pin[4])
        pin[6] = 1;
    else
        pin[6] = pin[i1] ^ 1;
}

static CHIP_LOGIC( 7474B1 )
{
	if(!pin[10] && !pin[13])
		pin[i2] = 1;
	else if(!pin[10] && pin[13])
		pin[i2] = 1;	
	else if(pin[10] && !pin[13])
		pin[i2] = 0;
	else if(POS_EDGE_PIN(11))
		pin[i2] = pin[12];
	else
		pin[i2] = prev_pin[i2];
}

static CHIP_LOGIC( 7474B2 )
{
    if(!pin[10] && !pin[13])
        pin[8] = 1;
    else
        pin[8] = pin[i2] ^ 1; 
}

static CHIP_LOGIC( 7474A_Buf )
{
    pin[5] = pin[i1];
}

static CHIP_LOGIC( 7474B_Buf )
{
    pin[9] = pin[i2];
}

// TODO: Fix delays from set/clear ?
CHIP_DESC( 7474 ) =
{
	CHIP_START( 7474A1 )
		INPUT_PINS( 1, 2, 3, 4 )
		EVENT_PINS( 3 )
        OUTPUT_PIN( i1 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 10.0, 10.0 ),

    CHIP_START( 7474A_Buf )
        INPUT_PINS( i1 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 4.0, 10.0 ),

	CHIP_START( 7474A2 )
		INPUT_PINS( 1, 4, i1 )
		OUTPUT_PIN( 6 )
		OUTPUT_DELAY_NS( 4.0, 10.0 ),

	CHIP_START( 7474B1 )
		INPUT_PINS( 10, 11, 12, 13 )
		EVENT_PINS( 11 )
        OUTPUT_PIN( i2 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 10.0, 10.0 ),

    CHIP_START( 7474B_Buf )
        INPUT_PINS( i2 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 4.0, 10.0 ),

	CHIP_START( 7474B2 )
		INPUT_PINS( i2, 10, 13 )
		OUTPUT_PIN( 8 )
		OUTPUT_DELAY_NS( 4.0, 10.0 ),

	CHIP_DESC_END
};

