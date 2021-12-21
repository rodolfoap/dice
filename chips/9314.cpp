#include "../chip_desc.h"

/*
9314
Quad /S-/R latches with enable and asynchronous reset.

    +---+--+---+
 /E |1  +--+ 16| VCC
/S0 |2       15| Q0
 D0 |3       14| /S1
 D1 |4   93  13| Q1
/S2 |5   14  12| Q2
 D2 |6       11| /S3
 D3 |7       10| Q3
GND |8        9| /RST
    +----------+

*/

static const uint8_t S_PINS[] = {  2, 14,  5, 11 };
static const uint8_t D_PINS[] = {  3,  4,  6,  7 };
static const uint8_t Q_PINS[] = { 15, 13, 12, 10 };

template<int N> CHIP_LOGIC( 9314 )
{
    int S = S_PINS[N];
    int D = D_PINS[N];
    int Q = Q_PINS[N];

    if(!pin[9])
        pin[Q] = 0;
    else if(!pin[1] && !pin[D] && !pin[S])
        pin[Q] = 0;
    else if(!pin[1] && pin[D] && !pin[S])
        pin[Q] = 1;
    else if(!pin[1] && !pin[D] && pin[S])
        pin[Q] = 0;
    else
        pin[Q] = prev_pin[Q];
}

// TODO: Accurate timing?

CHIP_DESC( 9314 ) =
{
	CHIP_START( 9314<0> )
        INPUT_PINS( 1, 9, S_PINS[0], D_PINS[0] )
        OUTPUT_PIN( Q_PINS[0] )
        PREV_OUTPUT_PIN( Q_PINS[0] )
        OUTPUT_DELAY_NS( 18.0, 18.0 ),

	CHIP_START( 9314<1> )
        INPUT_PINS( 1, 9, S_PINS[1], D_PINS[1] )
        OUTPUT_PIN( Q_PINS[1] )
        PREV_OUTPUT_PIN( Q_PINS[1] )
        OUTPUT_DELAY_NS( 18.0, 18.0 ),

	CHIP_START( 9314<2> )
        INPUT_PINS( 1, 9, S_PINS[2], D_PINS[2] )
        OUTPUT_PIN( Q_PINS[2] )
        PREV_OUTPUT_PIN( Q_PINS[2] )
        OUTPUT_DELAY_NS( 18.0, 18.0 ),

	CHIP_START( 9314<3> )
        INPUT_PINS( 1, 9, S_PINS[3], D_PINS[3] )
        OUTPUT_PIN( Q_PINS[3] )
        PREV_OUTPUT_PIN( Q_PINS[3] )
        OUTPUT_DELAY_NS( 18.0, 18.0 ),

	CHIP_DESC_END
};
