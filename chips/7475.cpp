#include "../chip_desc.h"

/*
7475
Dual 2-bit transparent latches with complementary outputs.

     +---+--+---+
/1Q1 |1  +--+ 16| 1Q1
 1D1 |2       15| 1Q2
 1D2 |3       14| /1Q2
 2LE |4       13| 1LE
 VCC |5  7475 12| GND
 2D1 |6       11| /2Q1
 2D2 |7       10| 2Q1
/2Q2 |8        9| 2Q2
     +----------+
*/

static const uint8_t D_PINS[]  = { 2, 3, 6, 7 };
static const uint8_t G_PINS[]  = { 13, 13, 4, 4 };
static const uint8_t Q_PINS[]  = { 16, 15, 10, 9 };
static const uint8_t Qn_PINS[] = { 1, 14, 11, 8 };

template <int N>
CHIP_LOGIC( 7475_Q )
{
    int D = D_PINS[N];
    int G = G_PINS[N];
    int Q = Q_PINS[N];

    if(pin[G])
		pin[Q] = pin[D];
	else
        pin[Q] = prev_pin[Q];
}

template <int N>
CHIP_LOGIC( 7475_Qn )
{
    int D = D_PINS[N];
    int G = G_PINS[N];
    int Qn = Qn_PINS[N];

    if(pin[G])
		pin[Qn] = pin[D] ^ 1;
	else
        pin[Qn] = prev_pin[Qn];
}

#define CHIP_START_7475_Q( BIT ) \
    CHIP_START( 7475_Q<BIT> ) \
        INPUT_PINS( D_PINS[BIT], G_PINS[BIT] ) \
        OUTPUT_PIN( Q_PINS[BIT] ) \
        PREV_OUTPUT_PIN( Q_PINS[BIT] ) \
        OUTPUT_DELAY_NS( 16.0, 7.0 )

#define CHIP_START_7475_Qn( BIT ) \
    CHIP_START( 7475_Qn<BIT> ) \
        INPUT_PINS( D_PINS[BIT], G_PINS[BIT] ) \
        OUTPUT_PIN( Qn_PINS[BIT] ) \
        PREV_OUTPUT_PIN( Qn_PINS[BIT] ) \
        OUTPUT_DELAY_NS( 16.0, 7.0 )

// TODO: Fix delay from data? (tPLH = 16/24, tPHL = 14/7)
CHIP_DESC( 7475 ) =
{
    CHIP_START_7475_Q(0),
    CHIP_START_7475_Q(1),
    CHIP_START_7475_Q(2),
    CHIP_START_7475_Q(3),

    CHIP_START_7475_Qn(0),
    CHIP_START_7475_Qn(1),
    CHIP_START_7475_Qn(2),
    CHIP_START_7475_Qn(3),

	CHIP_DESC_END
};

