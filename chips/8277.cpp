#include "../chip_desc.h"

/*
8277 (aka 9328)
Dual 8-bit serial-in serial-out shift register.

     +---+--+---+
/RST |1  +--+ 16| VCC
/Q7A |2       15| /Q7B
 Q7A |3       14| Q7B
 DSA |4   82  13| DSB
 D1A |5   77  12| D1B
 D0A |6       11| D0B
CLKA |7       10| CLKB
 GND |8        9| COMMON CLK
     +----------+
*/


// TODO: Counts as 16 outputs on reset chip, change this?

static const uint8_t D_PINS[]  = { i3, i5, i6, i7, i8, i9, i10, i11, 
                                   i4, i12, i13, i14, i15, i16, i17, i18 };
static const uint8_t Q_PINS[]  = { i5, i6, i7, i8, i9, i10, i11, 3,
                                   i12, i13, i14, i15, i16, i17, i18, 14 };

static CHIP_LOGIC( CLK_A )
{
    pin[i1] = pin[7] || pin[9];
}

static CHIP_LOGIC( CLK_B )
{
    pin[i2] = pin[10] || pin[9];
}

static CHIP_LOGIC( D_A )
{
    pin[i3] = pin[4] ? pin[5] : pin[6];
}

static CHIP_LOGIC( D_B )
{
    pin[i4] = pin[13] ? pin[12] : pin[11];
}

template <int N>
CHIP_LOGIC( 8277 )
{
    int D = D_PINS[N]; // Serial input
    int Q = Q_PINS[N];
    int CLK = (N < 8) ? i1 : i2;
    
    if(!pin[1])
        pin[Q] = 0;
    else if(POS_EDGE_PIN(CLK))
        pin[Q] = pin[D];
    else
        pin[Q] = prev_pin[Q];
}

static CHIP_LOGIC( Q7A_Inv )
{
    pin[2] = pin[3] ^ 1;
}

static CHIP_LOGIC( Q7B_Inv )
{
    pin[15] = pin[14] ^ 1;
}

// TODO: Accurate delays, 8277 datasheet only lists max delay of 40 ns

CHIP_DESC( 8277 ) =
{
    // Shift Register A
    CHIP_START( CLK_A )
        INPUT_PINS( 7, 9 )
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_NS( 10.0, 10.0 ),

	CHIP_START( D_A )
        INPUT_PINS( 4, 5, 6 )
        OUTPUT_PIN( i3 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),


    CHIP_START( 8277<0> )
		INPUT_PINS( 1, i1, D_PINS[0] )
		OUTPUT_PIN( Q_PINS[0] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[0] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<1> )
		INPUT_PINS( 1, i1, D_PINS[1] )
		OUTPUT_PIN( Q_PINS[1] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[1] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<2> )
		INPUT_PINS( 1, i1, D_PINS[2] )
		OUTPUT_PIN( Q_PINS[2] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[2] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<3> )
		INPUT_PINS( 1, i1, D_PINS[3] )
		OUTPUT_PIN( Q_PINS[3] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[3] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<4> )
		INPUT_PINS( 1, i1, D_PINS[4] )
		OUTPUT_PIN( Q_PINS[4] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[4] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<5> )
		INPUT_PINS( 1, i1, D_PINS[5] )
		OUTPUT_PIN( Q_PINS[5] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[5] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<6> )
		INPUT_PINS( 1, i1, D_PINS[6] )
		OUTPUT_PIN( Q_PINS[6] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[6] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<7> )
		INPUT_PINS( 1, i1, D_PINS[7] )
		OUTPUT_PIN( Q_PINS[7] )
		EVENT_PINS( i1 )
		PREV_OUTPUT_PIN( Q_PINS[7] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),


    CHIP_START( Q7A_Inv )
        INPUT_PINS( 3 )
        OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),



    // Shift Register B
	CHIP_START( CLK_B )
        INPUT_PINS( 10, 9 )
        OUTPUT_PIN( i2 )
        OUTPUT_DELAY_NS( 10.0, 10.0 ),

	CHIP_START( D_B )
        INPUT_PINS( 13, 12, 11 )
        OUTPUT_PIN( i4 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),


    CHIP_START( 8277<8> )
		INPUT_PINS( 1, i2, D_PINS[8] )
		OUTPUT_PIN( Q_PINS[8] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[8] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<9> )
		INPUT_PINS( 1, i2, D_PINS[9] )
		OUTPUT_PIN( Q_PINS[9] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[9] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<10> )
		INPUT_PINS( 1, i2, D_PINS[10] )
		OUTPUT_PIN( Q_PINS[10] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[10] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<11> )
		INPUT_PINS( 1, i2, D_PINS[11] )
		OUTPUT_PIN( Q_PINS[11] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[11] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<12> )
		INPUT_PINS( 1, i2, D_PINS[12] )
		OUTPUT_PIN( Q_PINS[12] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[12] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<13> )
		INPUT_PINS( 1, i2, D_PINS[13] )
		OUTPUT_PIN( Q_PINS[13] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[13] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<14> )
		INPUT_PINS( 1, i2, D_PINS[14] )
		OUTPUT_PIN( Q_PINS[14] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[14] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),

    CHIP_START( 8277<15> )
		INPUT_PINS( 1, i2, D_PINS[15] )
		OUTPUT_PIN( Q_PINS[15] )
		EVENT_PINS( i2 )
		PREV_OUTPUT_PIN( Q_PINS[15] )
		OUTPUT_DELAY_NS( 20.0, 20.0 ),


    CHIP_START( Q7B_Inv )
        INPUT_PINS( 14 )
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_DESC_END
};

