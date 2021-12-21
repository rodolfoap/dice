#include "../chip_desc.h"

/*
74165
8-bit parallel-in serial-out shift register with asynchronous parallel load and two OR gated clock inputs.

       +---+--+---+
SH//LD |1  +--+ 16| VCC
  CLK1 |2       15| CLK2
    P4 |3       14| P3
    P5 |4   74  13| P2
    P6 |5  165  12| P1
    P7 |6       11| P0
   /Q7 |7       10| D
   GND |8        9| Q7
       +----------+
*/

static const uint8_t P_PINS[]  = { 11, 12, 13, 14, 3, 4, 5, 6 };
static const uint8_t S_PINS[]  = { 10, i1, i2, i3, i4, i5, i6, i7 };

static CHIP_LOGIC( 74165_CLK )
{
    pin[i9] = pin[2] || pin[15];
}

template <int N>
CHIP_LOGIC( 74165 )
{
    int P = P_PINS[N]; // Parallel input
    int S = S_PINS[N]; // Serial input
    int Q = i1 + N;
    
    if(!pin[1])
        pin[Q] = pin[P];
    else if(POS_EDGE_PIN(i9))
        pin[Q] = pin[S];
    else
        pin[Q] = prev_pin[Q];
}

static CHIP_LOGIC( Q7_Buf )
{
    pin[9] = pin[i8];
}

static CHIP_LOGIC( Q7_Inv )
{
    pin[7] = pin[i8] ^ 1;
}

// TODO: Fix delay from load?

CHIP_DESC( 74165 ) =
{
    CHIP_START( 74165_CLK )
        INPUT_PINS( 2, 15 )
        OUTPUT_PIN( i9 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),
    
    CHIP_START( 74165<0> )
		INPUT_PINS( 1, i9, P_PINS[0], S_PINS[0] )
		OUTPUT_PIN( i1 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

    CHIP_START( 74165<1> )
		INPUT_PINS( 1, i9, P_PINS[1], S_PINS[1] )
		OUTPUT_PIN( i2 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 74165<2> )
		INPUT_PINS( 1, i9, P_PINS[2], S_PINS[2] )
		OUTPUT_PIN( i3 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i3 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 74165<3> )
		INPUT_PINS( 1, i9, P_PINS[3], S_PINS[3] )
		OUTPUT_PIN( i4 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i4 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),


	CHIP_START( 74165<4> )
		INPUT_PINS( 1, i9, P_PINS[4], S_PINS[4] )
		OUTPUT_PIN( i5 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i5 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

    CHIP_START( 74165<5> )
		INPUT_PINS( 1, i9, P_PINS[5], S_PINS[5] )
		OUTPUT_PIN( i6 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i6 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 74165<6> )
		INPUT_PINS( 1, i9, P_PINS[6], S_PINS[6] )
		OUTPUT_PIN( i7 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i7 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_START( 74165<7> )
		INPUT_PINS( 1, i9, P_PINS[7], S_PINS[7] )
		OUTPUT_PIN( i8 )
		EVENT_PINS( i9 )
		PREV_OUTPUT_PIN( i8 )
		OUTPUT_DELAY_NS( 5.0, 5.0 ),


    CHIP_START( Q7_Buf )
        INPUT_PINS( i8 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 6.0, 11.0 ),

    CHIP_START( Q7_Inv )
        INPUT_PINS( i8 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 6.0, 11.0 ),

	CHIP_DESC_END
};

