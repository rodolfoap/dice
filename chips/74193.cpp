#include "../chip_desc.h"

/*
74193
4-bit synchronous binary up/down counter with asynchronous load and reset,
and separate up and down clocks. Carry and borrow outputs.

     +---+--+---+
  P1 |1  +--+ 16| VCC
  Q1 |2       15| P0
  Q0 |3       14| RST
DOWN |4   74  13| /BORROW
  UP |5  193  12| /CARRY
  Q2 |6       11| /LOAD
  Q3 |7       10| P2
 GND |8        9| P3
     +----------+
*/

enum { COUNT = i1, UP = i2, DOWN = i3 };

static CHIP_LOGIC( 74193_COUNT )
{
    pin[COUNT] = pin[4] & pin[5];
}

static CHIP_LOGIC( 74193_UP )
{
    pin[UP] = pin[5];
}

static CHIP_LOGIC( 74193_DOWN )
{
    pin[DOWN] = pin[4];
}

static CHIP_LOGIC( 74193_A )
{
    if(pin[14])
        pin[3] = 0;
    else if(!pin[11])
        pin[3] = pin[15];
    else if(POS_EDGE_PIN(COUNT))
        pin[3] = prev_pin[3] ^ 1;
    else
        pin[3] = prev_pin[3];
}

static CHIP_LOGIC( 74193_B )
{
    if(pin[14])
        pin[2] = 0;
    else if(!pin[11])
        pin[2] = pin[1];
    else if(POS_EDGE_PIN(COUNT) && !pin[UP])
        pin[2] = pin[3] ^ prev_pin[2];
    else if(POS_EDGE_PIN(COUNT) && !pin[DOWN])
        pin[2] = (pin[3] ^ prev_pin[2]) ^ 1;
    else
        pin[2] = prev_pin[2];
}

static CHIP_LOGIC( 74193_C )
{
    if(pin[14])
        pin[6] = 0;
    else if(!pin[11])
        pin[6] = pin[10];
    else if(POS_EDGE_PIN(COUNT) && !pin[UP])
        pin[6] = (pin[3] & pin[2]) ^ prev_pin[6];
    else if(POS_EDGE_PIN(COUNT) && !pin[DOWN])
        pin[6] = ((pin[3] | pin[2]) ^ prev_pin[6]) ^ 1;
    else
        pin[6] = prev_pin[6];
}

static CHIP_LOGIC( 74193_D )
{
    if(pin[14])
        pin[7] = 0;
    else if(!pin[11])
        pin[7] = pin[9];
    else if(POS_EDGE_PIN(COUNT) && !pin[UP])
        pin[7] = (pin[3] & pin[2] & pin[6]) ^ prev_pin[7];
    else if(POS_EDGE_PIN(COUNT) && !pin[DOWN])
        pin[7] = ((pin[3] | pin[2] | pin[6]) ^ prev_pin[7]) ^ 1;
    else
        pin[7] = prev_pin[7];
}

static CHIP_LOGIC( 74193_CO )
{
    pin[12] = (pin[3] & pin[2] & pin[6] & pin[7] & (pin[5] ^ 1)) ^ 1;
}

static CHIP_LOGIC( 74193_BO )
{
    pin[13] = pin[3] | pin[2] | pin[6] | pin[7] | pin[4];
}

CHIP_DESC( 74193 ) =
{
	CHIP_START( 74193_COUNT )
        INPUT_PINS( 4, 5 )
        OUTPUT_PIN( COUNT )
        OUTPUT_DELAY_NS( 1.0, 1.0 ),

	CHIP_START( 74193_UP )
        INPUT_PINS( 5 )
        OUTPUT_PIN( UP )
        OUTPUT_DELAY_NS( 2.0, 2.0 ),
    
	CHIP_START( 74193_DOWN )
        INPUT_PINS( 4 )
        OUTPUT_PIN( DOWN )
        OUTPUT_DELAY_NS( 2.0, 2.0 ),

    CHIP_START( 74193_A )
        INPUT_PINS( 11, 14, 15, COUNT )
		EVENT_PINS( COUNT )
        OUTPUT_PIN( 3 )
		PREV_OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 25.0, 29.0 ), // Somewhat arbitrary

    CHIP_START( 74193_B )
        INPUT_PINS( UP, DOWN, 1, 3, 11, 14, COUNT )
		EVENT_PINS( COUNT )
        OUTPUT_PIN( 2 )
		PREV_OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 25.0, 29.0 ),

    CHIP_START( 74193_C )
        INPUT_PINS( UP, DOWN, 2, 3, 10, 11, 14, COUNT )
		EVENT_PINS( COUNT )
        OUTPUT_PIN( 6 )
		PREV_OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 25.0, 29.0 ),

    CHIP_START( 74193_D )
        INPUT_PINS( UP, DOWN, 2, 3, 6, 9, 11, 14, COUNT )
		EVENT_PINS( COUNT )
        OUTPUT_PIN( 7 )
		PREV_OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 25.0, 29.0 ),

    CHIP_START( 74193_CO )
        INPUT_PINS( 2, 3, 5, 6, 7 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 17.0, 16.0 ),

    CHIP_START( 74193_BO )
        INPUT_PINS( 2, 3, 4, 6, 7 )
        OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_DESC_END
};
