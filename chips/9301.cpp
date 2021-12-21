#include "../chip_desc.h"

/*
9301
1-of-10 inverting decoder/demultiplexer.

    +---+--+---+             +---+---+---+---*---+---+---+---+
 S2 |1  +--+ 16| VCC         | S3| S2| S1| S0|/Y0|/Y1|...|/Y9|
 S3 |2       15| S0          +===+===+===+===*===+===+===+===+
/Y5 |3       14| S1          | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 1 |
/Y6 |4       13| /Y0         | 0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 |
/Y7 |5  9301 12| /Y1         | . | . | . | . | 1 | 1 | . | 1 |
/Y8 |6       11| /Y2         | 1 | 0 | 0 | 1 | 1 | 1 | 1 | 0 |
/Y9 |7       10| /Y3         | 1 | 0 | 1 | X | 1 | 1 | 1 | 1 |
GND |8        9| /Y4         | 1 | 1 | X | X | 1 | 1 | 1 | 1 |
    +----------+             +---+---+---+---*---+---+---+---+
*/

template <int N>
CHIP_LOGIC( 9301 )
{
    int P = (N <= 4) ? 13-N : N-2;
    int S = 8*pin[2] + 4*pin[1] + 2*pin[14] + pin[15];

    if(S == N)
        pin[P] = 0;
    else
        pin[P] = 1;
}


CHIP_DESC( 9301 ) =
{
	CHIP_START( 9301<0> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<1> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<2> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<3> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<4> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),


	CHIP_START( 9301<5> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<6> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<7> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<8> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

	CHIP_START( 9301<9> )
        INPUT_PINS( 1, 2, 14, 15 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

    CHIP_DESC_END
};

