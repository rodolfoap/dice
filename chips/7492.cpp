#include "../chip_desc.h"

/*
7492
4-bit asynchronous divide-by-twelve counter with /2 and /6 sections and reset.

      +---+--+---+
/CLK1 |1  +--+ 14| /CLK0
      |2       13|
      |3       12| Q0
      |4  7492 11| Q1
  VCC |5       10| GND
 RST1 |6        9| Q2
 RST2 |7        8| Q3
      +----------+
*/


static CHIP_LOGIC( 7492A )
{
	if(pin[6] && pin[7])
		pin[12] = 0;
	else if(NEG_EDGE_PIN(14))
		pin[12] = prev_pin[12] ^ 1;
	else
		pin[12] = prev_pin[12];
}

static CHIP_LOGIC( 7492B )
{
	if(pin[6] && pin[7])
		pin[11] = 0;
    else if(NEG_EDGE_PIN(1) && pin[9])
		pin[11] = 0;
	else if(NEG_EDGE_PIN(1) && !pin[9])
		pin[11] = prev_pin[11] ^ 1;
	else
		pin[11] = prev_pin[11];
}

static CHIP_LOGIC( 7492C )
{
	if(pin[6] && pin[7])
		pin[9] = 0;
	else if(NEG_EDGE_PIN(1) && pin[11])
		pin[9] = prev_pin[9] ^ 1;
    else if(NEG_EDGE_PIN(1) && !pin[11])
        pin[9] = 0;
	else
		pin[9] = prev_pin[9];
}

static CHIP_LOGIC( 7492D )
{
	if(pin[6] && pin[7])
		pin[8] = 0;
	else if(NEG_EDGE_PIN(9))
		pin[8] = prev_pin[8] ^ 1;
	else
		pin[8] = prev_pin[8];
}

// Assuming 7492A Timing

CHIP_DESC( 7492 ) =
{
	CHIP_START( 7492A )
        INPUT_PINS( 6, 7, 14 )
		EVENT_PINS( 14 )
        OUTPUT_PIN( 12 )
		PREV_OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 10.0, 12.0 ),

	CHIP_START( 7492B )
        INPUT_PINS( 6, 7, 1, 9 )
		EVENT_PINS( 1 )
        OUTPUT_PIN( 11 )
		PREV_OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7492C )
        INPUT_PINS( 6, 7, 1, 11 )
		EVENT_PINS( 1 )
        OUTPUT_PIN( 9 )
		PREV_OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7492D )
        INPUT_PINS( 6, 7, 9 )
		EVENT_PINS( 9 )
        OUTPUT_PIN( 8 )
		PREV_OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 11.0, 9.0 ),

    CHIP_DESC_END
};

