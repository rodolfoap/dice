#include "../chip_desc.h"

/*
74175
4-bit D flip-flop with complementary outputs and reset.

     +---+--+---+            +----+---+---*---+---+
/RST |1  +--+ 16| VCC        |/RST|CLK| D | Q |/Q |
  Q1 |2       15| Q4         +====+===+===*===+===+
 /Q1 |3       14| /Q4        |  0 | X | X | 0 | 1 |
  D1 |4   74  13| D4         |  1 | / | 0 | 0 | 1 |
  D2 |5  175  12| D3         |  1 | / | 1 | 1 | 0 |
 /Q2 |6       11| /Q3        |  1 |!/ | X | - | - |
  Q2 |7       10| Q3         +----+---+---*---+---+
 GND |8        9| CLK
     +----------+

*/

static const uint8_t D_PINS[]  = { 4, 5, 12, 13 };
static const uint8_t Q_PINS[]  = { 2, 7, 10, 15 };
static const uint8_t QN_PINS[] = { 3, 6, 11, 14 };

template <int N>
CHIP_LOGIC( 74175 )
{
    int D = D_PINS[N];
    int Q = i1 + N;

    if(!pin[1])
		pin[Q] = 0;
	else if(POS_EDGE_PIN(9))
        pin[Q] = pin[D];
	else
        pin[Q] = prev_pin[Q];
}

template <int N>
CHIP_LOGIC( 74175_Q )
{
    int IN = i1 + N;
    int OUT = Q_PINS[N]; 
    
    pin[OUT] = pin[IN];
}

template <int N>
CHIP_LOGIC( 74175_Qn )
{
    int IN = i1 + N;
    int OUT = QN_PINS[N]; 

    pin[OUT] = pin[IN] ^ 1;
}

// TODO: Fix delay from clear ?
CHIP_DESC( 74175 ) =
{
	CHIP_START( 74175<0> )
		INPUT_PINS( 1, 9, D_PINS[0] )
		OUTPUT_PIN( i1 )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( i1 )
		OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 74175_Q<0> )
        INPUT_PINS( i1 )
        OUTPUT_PIN( Q_PINS[0] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),

    CHIP_START( 74175_Qn<0> )
        INPUT_PINS( i1 )
        OUTPUT_PIN( QN_PINS[0] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),


	CHIP_START( 74175<1> )
		INPUT_PINS( 1, 9, D_PINS[1] )
		OUTPUT_PIN( i2 )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( i2 )
		OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 74175_Q<1> )
        INPUT_PINS( i2 )
        OUTPUT_PIN( Q_PINS[1] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),

    CHIP_START( 74175_Qn<1> )
        INPUT_PINS( i2 )
        OUTPUT_PIN( QN_PINS[1] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),


	CHIP_START( 74175<2> )
		INPUT_PINS( 1, 9, D_PINS[2] )
		OUTPUT_PIN( i3 )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( i3 )
		OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 74175_Q<2> )
        INPUT_PINS( i3 )
        OUTPUT_PIN( Q_PINS[2] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),

    CHIP_START( 74175_Qn<2> )
        INPUT_PINS( i3 )
        OUTPUT_PIN( QN_PINS[2] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),


	CHIP_START( 74175<3> )
		INPUT_PINS( 1, 9, D_PINS[3] )
		OUTPUT_PIN( i4 )
		EVENT_PINS( 9 )
		PREV_OUTPUT_PIN( i4 )
		OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 74175_Q<3> )
        INPUT_PINS( i4 )
        OUTPUT_PIN( Q_PINS[3] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),

    CHIP_START( 74175_Qn<3> )
        INPUT_PINS( i4 )
        OUTPUT_PIN( QN_PINS[3] )
        OUTPUT_DELAY_NS( 4.0, 8.0 ),

	CHIP_DESC_END
};

