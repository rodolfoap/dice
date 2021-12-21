#include "../chip_desc.h"

/*
7496
5-bit shift register with asynchronous reset and asynchronous preset inputs.

    +---+--+---+
CLK |1  +--+ 16| /RST
 P0 |2       15| Q0
 P1 |3       14| Q1
 P2 |4       13| Q2
VCC |5  7496 12| GND
 P3 |6       11| Q3
 P4 |7       10| Q4
 PE |8        9| D
    +----------+

*/

static const uint8_t S_PINS[] = { 9, 15, 14, 13, 11 };
static const uint8_t Q_PINS[] = { 15, 14, 13, 11, 10 };
static const uint8_t P_PINS[] = { 2, 3, 4, 6, 7 };

template <int N>
CHIP_LOGIC( 7496 )
{
    int P = P_PINS[N]; // Preset input
    int S = S_PINS[N]; // Shift right serial input
    int Q = Q_PINS[N];
    
    if(!pin[16]) // Clear
        pin[Q] = 0;
    else if(pin[8] && pin[P]) // Preset
        pin[Q] = 1;
    else if(POS_EDGE_PIN(1)) // Shift right
        pin[Q] = pin[S];
    else
        pin[Q] = prev_pin[Q];
}

// TODO: Fix delay from clear/preset?

CHIP_DESC( 7496 ) =
{
	CHIP_START( 7496<0> )
		INPUT_PINS( 1, 8, 16, S_PINS[0], P_PINS[0] )
		OUTPUT_PIN( 15 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( 15 )
		OUTPUT_DELAY_NS( 25.0, 25.0 ),

    CHIP_START( 7496<1> )
		INPUT_PINS( 1, 8, 16, S_PINS[1], P_PINS[1] )
		OUTPUT_PIN( 14 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( 14 )
		OUTPUT_DELAY_NS( 25.0, 25.0 ),

	CHIP_START( 7496<2> )
		INPUT_PINS( 1, 8, 16, S_PINS[2], P_PINS[2] )
		OUTPUT_PIN( 13 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( 13 )
		OUTPUT_DELAY_NS( 25.0, 25.0 ),

	CHIP_START( 7496<3> )
		INPUT_PINS( 1, 8, 16, S_PINS[3], P_PINS[3] )
		OUTPUT_PIN( 11 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( 11 )
		OUTPUT_DELAY_NS( 25.0, 25.0 ),

	CHIP_START( 7496<4> )
		INPUT_PINS( 1, 8, 16, S_PINS[4], P_PINS[4] )
		OUTPUT_PIN( 10 )
		EVENT_PINS( 1 )
		PREV_OUTPUT_PIN( 10 )
		OUTPUT_DELAY_NS( 25.0, 25.0 ),

	CHIP_DESC_END
};

