#include "../chip_desc.h"

/*
74116
Dual 4-bit latches with clear

      +---+--+---+    
/1RST |1  +--+ 24| VCC
/1EN1 |2       23| 2Q4 
/1EN2 |3       22| 2D4
  1D1 |4       21| 2Q3
  1Q1 |5       20| 2D3
  1D2 |6   74  19| 2Q2
  1Q2 |7  116  18| 2D2
  1D3 |8       17| 2Q1
  1Q3 |9       16| 2D1
  1D4 |10      15| /2EN2 
  1Q4 |11      14| /2EN1
  GND |12      13| /2RST
      +----------+
*/

static const uint8_t D1_PINS[]  = { 4, 6, 8, 10 };
static const uint8_t Q1_PINS[]  = { 5, 7, 9, 11 };
static const uint8_t D2_PINS[]  = { 16, 18, 20, 22 };
static const uint8_t Q2_PINS[]  = { 17, 19, 21, 23 };

template <int N>
CHIP_LOGIC( 74116A )
{
    int D = D1_PINS[N];
    int Q = Q1_PINS[N];

    if(!pin[1])
		pin[Q] = 0;
	else if(!pin[2] && !pin[3])
        pin[Q] = pin[D];
	else
        pin[Q] = prev_pin[Q];
}

template <int N>
CHIP_LOGIC( 74116B )
{
    int D = D2_PINS[N];
    int Q = Q2_PINS[N];

    if(!pin[13])
		pin[Q] = 0;
	else if(!pin[14] && !pin[15])
        pin[Q] = pin[D];
	else
        pin[Q] = prev_pin[Q];
}

// TODO: Fix delay from data? (tPLH = 10, tPHL = 12)
CHIP_DESC( 74116 ) =
{
	CHIP_START( 74116A<0> )
		INPUT_PINS( 1, 2, 3, D1_PINS[0] )
        OUTPUT_PIN( Q1_PINS[0] )
		PREV_OUTPUT_PIN( Q1_PINS[0] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),

	CHIP_START( 74116A<1> )
		INPUT_PINS( 1, 2, 3, D1_PINS[1] )
        OUTPUT_PIN( Q1_PINS[1] )
		PREV_OUTPUT_PIN( Q1_PINS[1] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),

	CHIP_START( 74116A<2> )
		INPUT_PINS( 1, 2, 3, D1_PINS[2] )
        OUTPUT_PIN( Q1_PINS[2] )
		PREV_OUTPUT_PIN( Q1_PINS[2] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),

	CHIP_START( 74116A<3> )
		INPUT_PINS( 1, 2, 3, D1_PINS[3] )
        OUTPUT_PIN( Q1_PINS[3] )
		PREV_OUTPUT_PIN( Q1_PINS[3] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),


	CHIP_START( 74116B<0> )
		INPUT_PINS( 13, 14, 15, D2_PINS[0] )
        OUTPUT_PIN( Q2_PINS[0] )
		PREV_OUTPUT_PIN( Q2_PINS[0] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),

	CHIP_START( 74116B<1> )
		INPUT_PINS( 13, 14, 15, D2_PINS[1] )
        OUTPUT_PIN( Q2_PINS[1] )
		PREV_OUTPUT_PIN( Q2_PINS[1] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),

	CHIP_START( 74116B<2> )
		INPUT_PINS( 13, 14, 15, D2_PINS[2] )
        OUTPUT_PIN( Q2_PINS[2] )
		PREV_OUTPUT_PIN( Q2_PINS[2] )
		OUTPUT_DELAY_NS( 19.0, 15.0 ),

	CHIP_START( 74116B<3> )
		INPUT_PINS( 13, 14, 15, D2_PINS[3] )
        OUTPUT_PIN( Q2_PINS[3] )
		PREV_OUTPUT_PIN( Q2_PINS[3] )
    	OUTPUT_DELAY_NS( 19.0, 15.0 ),


	CHIP_DESC_END
};

