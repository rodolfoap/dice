#include "../chip_desc.h"

/*
74164
8-bit serial-in parallel-out shift register with asynchronous reset and two AND gated serial inputs.

    +---+--+---+
  D |1  +--+ 14| VCC
  E |2       13| Q7
 Q0 |3   74  12| Q6
 Q1 |4  164  11| Q5
 Q2 |5       10| Q4
 Q3 |6        9| /RST
GND |7        8| CLK
    +----------+

*/

// TODO: Counts as 8 inputs on clock and reset chips, change this?

static const uint8_t D_PINS[]  = { i1,  3,  4,  5,  6, 10, 11, 12 };
static const uint8_t Q_PINS[]  = {  3,  4,  5,  6, 10, 11, 12, 13 };

template <int N>
CHIP_LOGIC( 74164 )
{
    int D = D_PINS[N]; // Serial input
    int Q = Q_PINS[N]; // Parallel output
    
    if(!pin[9])
        pin[Q] = 0;
    else if(POS_EDGE_PIN(8))
        pin[Q] = pin[D];
    else
        pin[Q] = prev_pin[Q];
}

static CHIP_LOGIC( 74164_S1 )
{
    pin[i1] = pin[1] && pin[2];
}

// TODO: Fix delay from reset?

CHIP_DESC( 74164 ) =
{
    CHIP_START( 74164_S1 )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_NS( 10.0, 10.0 ),

	CHIP_START( 74164<0> )
		INPUT_PINS( 8, 9, D_PINS[0] )
        OUTPUT_PIN( Q_PINS[0] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[0] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<1> )
		INPUT_PINS( 8, 9, D_PINS[1] )
        OUTPUT_PIN( Q_PINS[1] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[1] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<2> )
		INPUT_PINS( 8, 9, D_PINS[2] )
        OUTPUT_PIN( Q_PINS[2] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[2] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<3> )
		INPUT_PINS( 8, 9, D_PINS[3] )
        OUTPUT_PIN( Q_PINS[3] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[3] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<4> )
		INPUT_PINS( 8, 9, D_PINS[4] )
        OUTPUT_PIN( Q_PINS[4] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[4] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<5> )
		INPUT_PINS( 8, 9, D_PINS[5] )
        OUTPUT_PIN( Q_PINS[5] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[5] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<6> )
		INPUT_PINS( 8, 9, D_PINS[6] )
        OUTPUT_PIN( Q_PINS[6] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[6] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_START( 74164<7> )
		INPUT_PINS( 8, 9, D_PINS[7] )
        OUTPUT_PIN( Q_PINS[7] )
		EVENT_PINS( 8 )
		PREV_OUTPUT_PIN( Q_PINS[7] )
		OUTPUT_DELAY_NS( 17.0, 21.0 ),

	CHIP_DESC_END
};

