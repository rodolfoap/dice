#include "../chip_desc.h"

/*
7442
1-of-10 inverting decoder/demultiplexer.

    +---+--+---+             +---+---+---+---*---+---+---+---+
/Y0 |1  +--+ 16| VCC         | S3| S2| S1| S0|/Y0|/Y1|...|/Y9|
/Y1 |2       15| S0          +===+===+===+===*===+===+===+===+
/Y2 |3       14| S1          | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 1 |
/Y3 |4       13| S2          | 0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 |
/Y4 |5  7442 12| S3          | . | . | . | . | 1 | 1 | . | 1 |
/Y5 |6       11| /Y9         | 1 | 0 | 0 | 1 | 1 | 1 | 1 | 0 |
/Y6 |7       10| /Y8         | 1 | 0 | 1 | X | 1 | 1 | 1 | 1 |
GND |8        9| /Y7         | 1 | 1 | X | X | 1 | 1 | 1 | 1 |
    +----------+             +---+---+---+---*---+---+---+---+
*/


template <int N>
CHIP_LOGIC( 7442 )
{
    int P = N + ((N >= 7) ? 2 : 1);
    int S = 8*pin[12] + 4*pin[13] + 2*pin[14] + pin[15];

    if(S == N)
        pin[P] = 0;
    else
        pin[P] = 1;
}


// TODO: More accurate timing based on # of logic levels?
CHIP_DESC( 7442 ) =
{
	CHIP_START( 7442<0> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<1> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<2> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<3> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<4> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),


	CHIP_START( 7442<5> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<6> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<7> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<8> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

	CHIP_START( 7442<9> )
        INPUT_PINS( 12, 13, 14, 15 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 10.0, 14.0 ),

    CHIP_DESC_END
};

