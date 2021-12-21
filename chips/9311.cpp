#include "../chip_desc.h"

/*
74154 (9311)
1-of-16 inverting decoder/demultiplexer.

     +---+--+---+
 /Y0 |1  +--+ 24| VCC
 /Y1 |2       23| S0
 /Y2 |3       22| S1
 /Y3 |4       21| S2
 /Y4 |5       20| S3
 /Y5 |6   74  19| /EN2
 /Y6 |7  154  18| /EN1
 /Y7 |8       17| /Y15
 /Y8 |9       16| /Y14
 /Y9 |10      15| /Y13
/Y10 |11      14| /Y12
 GND |12      13| /Y11
     +----------+
*/


template <int N>
CHIP_LOGIC( 9311 )
{
    int P = N + ((N >= 11) ? 2 : 1);
    int S = 8*pin[20] + 4*pin[21] + 2*pin[22] + pin[23];

    if(!pin[18] && !pin[19] && S == N)
        pin[P] = 0;
    else
        pin[P] = 1;
}

// SEL->OUT: tPLH = 19 (or 20?), tPHL = 19
// ENABLE->OUT: tPLH = 12, tPHL = 14

CHIP_DESC( 9311 ) =
{
	CHIP_START( 9311<0> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<1> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<2> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<3> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<4> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<5> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<6> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<7> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),


	CHIP_START( 9311<8> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<9> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<10> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<11> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<12> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 14 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<13> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<14> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 16 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

	CHIP_START( 9311<15> )
        INPUT_PINS( 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 17 )
        OUTPUT_DELAY_NS( 19.0, 19.0 ),

   	CHIP_DESC_END
};

