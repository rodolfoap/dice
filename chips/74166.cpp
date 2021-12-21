#include "../chip_desc.h"

/*
74166
8-bit parallel-in serial-out shift register with asynchronous reset and two OR gated clock inputs.

     +---+--+---+
   D |1  +--+ 16| VCC
  P0 |2       15| SH//LD
  P1 |3       14| P7
  P2 |4   74  13| Q7
  P3 |5  166  12| P6
CLK1 |6       11| P5
CLK2 |7       10| P4
 GND |8        9| /RST
     +----------+
*/

static const uint8_t P_PINS[]  = { 2, 3, 4, 5, 10, 11, 12, 14 };
static const uint8_t S_PINS[]  = { 1, i1, i2, i3, i4, i5, i6, i7 };
static const uint8_t Q_PINS[]  = { i1, i2, i3, i4, i5, i6, i7, 13 };

static CHIP_LOGIC( 74166_CLK )
{
    pin[i8] = pin[6] || pin[7];
}

// Need to buffer these inputs to reduce connections?
static CHIP_LOGIC( 74166_SH_BUF ) { pin[i9] = pin[15]; }
static CHIP_LOGIC( 74166_RST_BUF ) { pin[i10] = pin[9]; }

template <int N>
CHIP_LOGIC( 74166 )
{
    int P = P_PINS[N]; // Parallel input
    int S = S_PINS[N]; // Serial input
    int Q = Q_PINS[N]; // Data output
    
    if(!pin[i10])
        pin[Q] = 0;
    else if(POS_EDGE_PIN(i8) && !pin[i9])
        pin[Q] = pin[P];
    else if(POS_EDGE_PIN(i8))
        pin[Q] = pin[S];
    else
        pin[Q] = prev_pin[Q];
}

// TODO: Fix delay from load?

CHIP_DESC( 74166 ) =
{
	CHIP_START( 74166_CLK )
        INPUT_PINS( 6, 7 )
        OUTPUT_PIN( i8 )
        OUTPUT_DELAY_NS( 10.0, 10.0 ),

	CHIP_START( 74166_SH_BUF )
        INPUT_PINS( 15 )
        OUTPUT_PIN( i9 )
        OUTPUT_DELAY_NS( 1.0, 1.0 ),

    CHIP_START( 74166_RST_BUF )
        INPUT_PINS( 9 )
        OUTPUT_PIN( i10 )
        OUTPUT_DELAY_NS( 10.0, 13.0 ),

    CHIP_START( 74166<0> )
		INPUT_PINS( i8, i9, i10, P_PINS[0], S_PINS[0] )
        OUTPUT_PIN( Q_PINS[0] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[0] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

    CHIP_START( 74166<1> )
		INPUT_PINS( i8, i9, i10, P_PINS[1], S_PINS[1] )
        OUTPUT_PIN( Q_PINS[1] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[1] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

    CHIP_START( 74166<2> )
		INPUT_PINS( i8, i9, i10, P_PINS[2], S_PINS[2] )
        OUTPUT_PIN( Q_PINS[2] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[2] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

    CHIP_START( 74166<3> )
		INPUT_PINS( i8, i9, i10, P_PINS[3], S_PINS[3] )
        OUTPUT_PIN( Q_PINS[3] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[3] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),


    CHIP_START( 74166<4> )
		INPUT_PINS( i8, i9, i10, P_PINS[4], S_PINS[4] )
        OUTPUT_PIN( Q_PINS[4] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[4] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

    CHIP_START( 74166<5> )
		INPUT_PINS( i8, i9, i10, P_PINS[5], S_PINS[5] )
        OUTPUT_PIN( Q_PINS[5] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[5] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

    CHIP_START( 74166<6> )
		INPUT_PINS( i8, i9, i10, P_PINS[6], S_PINS[6] )
        OUTPUT_PIN( Q_PINS[6] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[6] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

    CHIP_START( 74166<7> )
		INPUT_PINS( i8, i9, i10, P_PINS[7], S_PINS[7] )
        OUTPUT_PIN( Q_PINS[7] )
		EVENT_PINS( i8 )
		PREV_OUTPUT_PIN( Q_PINS[7] )
		OUTPUT_DELAY_NS( 7.0, 10.0 ),

	CHIP_DESC_END
};

