#include "../chip_desc.h"

/*
7490
4-bit asynchronous decade counter with /2 and /5 sections, set(9) and reset.

      +---+--+---+
/CLK1 |1  +--+ 14| /CLK0
 RST1 |2       13|
 RST2 |3       12| Q0
      |4  7490 11| Q3
  VCC |5       10| GND
 SET1 |6        9| Q1
 SET2 |7        8| Q2
      +----------+

*/

static CHIP_LOGIC( 7490A )
{
	if(pin[6] && pin[7])
		pin[12] = 1;
    else if(pin[2] && pin[3])
        pin[12] = 0;
	else if(NEG_EDGE_PIN(14))
		pin[12] = prev_pin[12] ^ 1;
	else
		pin[12] = prev_pin[12];
}

static CHIP_LOGIC( 7490B )
{
	if(pin[6] && pin[7])
		pin[9] = 0;
    else if(pin[2] && pin[3])
        pin[9] = 0;
	else if(NEG_EDGE_PIN(1))
    {
        if(pin[11])
            pin[9] = 0;
        else
		    pin[9] = prev_pin[9] ^ 1;
    }
    else
        pin[9] = prev_pin[9];
}

static CHIP_LOGIC( 7490C )
{
	if(pin[6] && pin[7])
		pin[8] = 0;
    else if(pin[2] && pin[3])
        pin[8] = 0;
	else if(NEG_EDGE_PIN(9))
        pin[8] = prev_pin[8] ^ 1;
    else
        pin[8] = prev_pin[8];
}

static CHIP_LOGIC( 7490D )
{
	if(pin[6] && pin[7])
		pin[11] = 1;
    else if(pin[2] && pin[3])
        pin[11] = 0;
	else if(NEG_EDGE_PIN(1))
    {
        if(prev_pin[11])
            pin[11] = 0;
        else if(pin[8] && pin[9])
            pin[11] = 1;
    }
    else
        pin[11] = prev_pin[11];
}

/* TODO:
 * More accurate gate delays
 */

CHIP_DESC( 7490 ) =
{
	CHIP_START( 7490A )
        INPUT_PINS( 2, 3, 6, 7, 14 )
		EVENT_PINS( 14 )
        OUTPUT_PIN( 12 )
		PREV_OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 10.0, 12.0 ),

	CHIP_START( 7490B )
        INPUT_PINS( 2, 3, 6, 7, 11, 1 )
		EVENT_PINS( 1 )
        OUTPUT_PIN( 9 )
		PREV_OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7490C )
        INPUT_PINS( 2, 3, 6, 7, 9 )
		EVENT_PINS( 9 )
        OUTPUT_PIN( 8 )
		PREV_OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 7.0, 9.0 ),

	CHIP_START( 7490D )
        INPUT_PINS( 2, 3, 6, 7, 8, 9, 1 )
		EVENT_PINS( 1 )
        OUTPUT_PIN( 11 )
		PREV_OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 21.0, 23.0 ),

    CHIP_DESC_END
};

