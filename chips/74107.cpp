#include "../chip_desc.h"

/*
74107
Dual master-slave J-K flip-flops with reset.

    +---+--+---+             +---+---+----+----*---+---+
 1J |1  +--+ 14| VCC         | J | K |/CLK|/RST| Q |/Q |
/1Q |2       13| /1RST       +===+===+====+====*===+===+
 1Q |3   74  12| /1CLK       | X | X |  X |  0 | 0 | 1 |
 1K |4  107  11| 2K          | 0 | 0 | /\ |  1 | - | - |
 2Q |5       10| /2RST       | 0 | 1 | /\ |  1 | 0 | 1 |
/2Q |6        9| /2CLK       | 1 | 0 | /\ |  1 | 1 | 0 |
GND |7        8| 2J          | 1 | 1 | /\ |  1 |/Q | Q |
    +----------+             | X | X |!/\ |  1 | - | - |
                             +---+---+----+----*---+---+
*/

static CHIP_LOGIC( 74107_M1 )
{
    if(!pin[13])
        pin[i1] = 0;
    else if(pin[12] && pin[1] && !pin[3])
        pin[i1] = 1;
    else if(pin[12] && pin[4] && pin[3])
        pin[i1] = 0;
    else
        pin[i1] = prev_pin[i1];
}

static CHIP_LOGIC( 74107_S1 )
{
    if(!pin[13])
        pin[3] = 0;
    else if(!pin[12])
        pin[3] = pin[i1];
    else
        pin[3] = prev_pin[3];
}

static CHIP_LOGIC( 74107_S1_n )
{
    if(!pin[13])
        pin[2] = 1;
    else if(!pin[12])
        pin[2] = pin[i1] ^ 1;
    else
        pin[2] = prev_pin[2];
}



static CHIP_LOGIC( 74107_M2 )
{
    if(!pin[10])
        pin[i2] = 0;
    else if(pin[9] && pin[8] && !pin[5])
        pin[i2] = 1;
    else if(pin[9] && pin[11] && pin[5])
        pin[i2] = 0;
    else
        pin[i2] = prev_pin[i2];
}

static CHIP_LOGIC( 74107_S2 )
{
    if(!pin[10])
        pin[5] = 0;
    else if(!pin[9])
        pin[5] = pin[i2];
    else
        pin[5] = prev_pin[5];
}

static CHIP_LOGIC( 74107_S2_n )
{
    if(!pin[10])
        pin[6] = 1;
    else if(!pin[9])
        pin[6] = pin[i2] ^ 1;
    else
        pin[6] = prev_pin[6];
}

// TODO: What should master latch timings be?

CHIP_DESC( 74107 ) =
{
	CHIP_START( 74107_M1 )
		INPUT_PINS( 1, 3, 4, 12, 13 )
		OUTPUT_PIN( i1 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 15.0, 15.0 ),

	CHIP_START( 74107_S1 )
		INPUT_PINS( i1, 12, 13 )
		OUTPUT_PIN( 3 )
        PREV_OUTPUT_PIN( 3 )
		OUTPUT_DELAY_NS( 16.0, 25.0 ),

	CHIP_START( 74107_S1_n )
		INPUT_PINS( i1, 12, 13 )
		OUTPUT_PIN( 2 )
        PREV_OUTPUT_PIN( 2 )
		OUTPUT_DELAY_NS( 16.0, 25.0 ),

	CHIP_START( 74107_M2 )
		INPUT_PINS( 8, 5, 11, 9, 10 )
		OUTPUT_PIN( i2 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 15.0, 15.0 ),

	CHIP_START( 74107_S2 )
		INPUT_PINS( i2, 9, 10 )
		OUTPUT_PIN( 5 )
        PREV_OUTPUT_PIN( 5 )
		OUTPUT_DELAY_NS( 16.0, 25.0 ),

	CHIP_START( 74107_S2_n )
		INPUT_PINS( i2, 9, 10 )
		OUTPUT_PIN( 6 )
        PREV_OUTPUT_PIN( 6 )
		OUTPUT_DELAY_NS( 16.0, 25.0 ),

	CHIP_DESC_END
};

