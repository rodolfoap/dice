#include "../chip_desc.h"

/*
74174
6-bit D flip-flop with reset.

     +---+--+---+            +----+---+---*---+
/RST |1  +--+ 16| VCC        |/RST|CLK| D | Q |
  Q0 |2       15| Q5         +====+===+===*===+
  D0 |3       14| D5         |  0 | X | X | 0 |
  D1 |4   74  13| D4         |  1 | / | 0 | 0 |
  Q1 |5  174  12| Q4         |  1 | / | 1 | 1 |
  D2 |6       11| D3         |  1 |!/ | X | - |
  Q2 |7       10| Q3         +----+---+---*---+
 GND |8        9| CLK
     +----------+
*/

static const uint8_t D_PINS[]  = { 3, 4, 6, 11, 13, 14 };
static const uint8_t Q_PINS[]  = { 2, 5, 7, 10, 12, 15 };

template <int N>
CHIP_LOGIC( 74174 )
{
    int D = D_PINS[N];
    int Q = Q_PINS[N];

    if(!pin[1])
		pin[Q] = 0;
	else if(POS_EDGE_PIN(9))
        pin[Q] = pin[D];
	else
        pin[Q] = prev_pin[Q];
}

// TODO: Fix delay from clear ?
CHIP_DESC( 74174 ) =
{
	CHIP_START( 74174<0> )
		INPUT_PINS( 1, 9, D_PINS[0] )
        OUTPUT_PIN( Q_PINS[0] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[0] )
		OUTPUT_DELAY_NS( 20.0, 21.0 ),

	CHIP_START( 74174<1> )
		INPUT_PINS( 1, 9, D_PINS[1] )
        OUTPUT_PIN( Q_PINS[1] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[1] )
		OUTPUT_DELAY_NS( 20.0, 21.0 ),

	CHIP_START( 74174<2> )
		INPUT_PINS( 1, 9, D_PINS[2] )
        OUTPUT_PIN( Q_PINS[2] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[2] )
		OUTPUT_DELAY_NS( 20.0, 21.0 ),

	CHIP_START( 74174<3> )
		INPUT_PINS( 1, 9, D_PINS[3] )
        OUTPUT_PIN( Q_PINS[3] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[3] )
		OUTPUT_DELAY_NS( 20.0, 21.0 ),

	CHIP_START( 74174<4> )
		INPUT_PINS( 1, 9, D_PINS[4] )
        OUTPUT_PIN( Q_PINS[4] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[4] )
		OUTPUT_DELAY_NS( 20.0, 21.0 ),

	CHIP_START( 74174<5> )
		INPUT_PINS( 1, 9, D_PINS[5] )
        OUTPUT_PIN( Q_PINS[5] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[5] )
		OUTPUT_DELAY_NS( 20.0, 21.0 ),

	CHIP_DESC_END
};



// TODO: Fix delay from clear ?
CHIP_DESC( 74S174 ) =
{
	CHIP_START( 74174<0> )
		INPUT_PINS( 1, 9, D_PINS[0] )
        OUTPUT_PIN( Q_PINS[0] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[0] )
		OUTPUT_DELAY_NS( 8.0, 11.5 ),

	CHIP_START( 74174<1> )
		INPUT_PINS( 1, 9, D_PINS[1] )
        OUTPUT_PIN( Q_PINS[1] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[1] )
		OUTPUT_DELAY_NS( 8.0, 11.5 ),

	CHIP_START( 74174<2> )
		INPUT_PINS( 1, 9, D_PINS[2] )
        OUTPUT_PIN( Q_PINS[2] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[2] )
		OUTPUT_DELAY_NS( 8.0, 11.5 ),

	CHIP_START( 74174<3> )
		INPUT_PINS( 1, 9, D_PINS[3] )
        OUTPUT_PIN( Q_PINS[3] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[3] )
		OUTPUT_DELAY_NS( 8.0, 11.5 ),

	CHIP_START( 74174<4> )
		INPUT_PINS( 1, 9, D_PINS[4] )
        OUTPUT_PIN( Q_PINS[4] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[4] )
		OUTPUT_DELAY_NS( 8.0, 11.5 ),

	CHIP_START( 74174<5> )
		INPUT_PINS( 1, 9, D_PINS[5] )
        OUTPUT_PIN( Q_PINS[5] )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( Q_PINS[5] )
		OUTPUT_DELAY_NS( 8.0, 11.5 ),

	CHIP_DESC_END
};
