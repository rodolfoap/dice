#include "../chip_desc.h"

/*
74191
4-bit synchronous binary up/down counter with load and both carry out and ripple clock outputs.

       +---+--+---+
    P1 |1  +--+ 16| VCC
    Q1 |2       15| P0
    Q0 |3       14| CLK
/CLKEN |4   74  13| /RCLK
  D//U |5  191  12| /RCO
    Q2 |6       11| /LOAD
    Q3 |7       10| P2
   GND |8        9| P3
       +----------+
*/

static CHIP_LOGIC( 74191_A )
{
    if(!pin[11])
        pin[3] = pin[15];
    else if(POS_EDGE_PIN(14) && !pin[4])
        pin[3] = prev_pin[3] ^ 1;
    else
        pin[3] = prev_pin[3];
}

static CHIP_LOGIC( 74191_B )
{
    if(!pin[11])
        pin[2] = pin[1];
    else if(POS_EDGE_PIN(14) && !pin[4] && !pin[5])
        pin[2] = pin[3] ^ prev_pin[2];
    else if(POS_EDGE_PIN(14) && !pin[4] && pin[5])
        pin[2] = (pin[3] ^ prev_pin[2]) ^ 1;
    else
        pin[2] = prev_pin[2];
}

static CHIP_LOGIC( 74191_C )
{
    if(!pin[11])
        pin[6] = pin[10];
    else if(POS_EDGE_PIN(14) && !pin[4] && !pin[5])
        pin[6] = (pin[3] & pin[2]) ^ prev_pin[6];
    else if(POS_EDGE_PIN(14) && !pin[4] && pin[5])
        pin[6] = ((pin[3] | pin[2]) ^ prev_pin[6]) ^ 1;
    else
        pin[6] = prev_pin[6];
}

static CHIP_LOGIC( 74191_D )
{
    if(!pin[11])
        pin[7] = pin[9];
    else if(POS_EDGE_PIN(14) && !pin[4] && !pin[5])
        pin[7] = (pin[3] & pin[2] & pin[6]) ^ prev_pin[7];
    else if(POS_EDGE_PIN(14) && !pin[4] && pin[5])
        pin[7] = ((pin[3] | pin[2] | pin[6]) ^ prev_pin[7]) ^ 1;
    else
        pin[7] = prev_pin[7];
}

static CHIP_LOGIC( 74191_RCO )
{
    pin[12] = (pin[3] & pin[2] & pin[6] & pin[7] & !pin[5]) |
              (!pin[3] & !pin[2] & !pin[6] & !pin[7] & pin[5]);
}

static CHIP_LOGIC( 74191_RCLK )
{
    pin[13] = (pin[12] & !pin[4] & !pin[14]) ^ 1;
}

// TODO: More accurate timing (Especially RCO and RCLK)

CHIP_DESC( 74191 ) =
{
    CHIP_START( 74191_A )
        INPUT_PINS( 4, 5, 11, 14, 15 )
		EVENT_PINS( 14 )
        OUTPUT_PIN( 3 )
		PREV_OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 16.0, 24.0 ), // Somewhat arbitrary

    CHIP_START( 74191_B )
        INPUT_PINS( 4, 5, 1, 3, 11, 14 )
		EVENT_PINS( 14 )
        OUTPUT_PIN( 2 )
		PREV_OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 16.0, 24.0 ),

    CHIP_START( 74191_C )
        INPUT_PINS( 4, 5, 2, 3, 10, 11, 14 )
		EVENT_PINS( 14 )
        OUTPUT_PIN( 6 )
		PREV_OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 16.0, 24.0 ),

    CHIP_START( 74191_D )
        INPUT_PINS( 4, 5, 2, 3, 6, 9, 11, 14 )
		EVENT_PINS( 14 )
        OUTPUT_PIN( 7 )
		PREV_OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 16.0, 24.0 ),

    CHIP_START( 74191_RCO )
        INPUT_PINS( 2, 3, 5, 6, 7 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 13.0, 16.0 ), // or 30, 30 ?

    CHIP_START( 74191_RCLK )
        INPUT_PINS( 4, 12, 14 )
        OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 21.0, 22.0 ), // or 28, 37 ?

    CHIP_DESC_END
};
