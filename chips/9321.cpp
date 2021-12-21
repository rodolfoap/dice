#include "../chip_desc.h"

/*
9321
Dual 1-of-4 inverting decoders/demultiplexers with separate enables.

      +---+--+---+
 /1EN |1  +--+ 16| VCC
  1S0 |2       15| /2EN
  1S1 |3       14| 2S0
 /1Y0 |4   93  13| 2S1
 /1Y1 |5   21  12| /2Y0
 /1Y2 |6       11| /2Y1
 /1Y3 |7       10| /2Y2
  GND |8        9| /2Y3
      +----------+
*/

template<int N> CHIP_LOGIC( 9321A )
{
    int S = 2*pin[3] + pin[2];
    
    if(pin[1])
        pin[4+N] = 1;
    else if(S == N)
        pin[4+N] = 0;
    else
        pin[4+N] = 1;
}

template<int N> CHIP_LOGIC( 9321B )
{
    int S = 2*pin[13] + pin[14];
    
    if(pin[15])
        pin[12-N] = 1;
    else if(S == N)
        pin[12-N] = 0;
    else
        pin[12-N] = 1;
}

// TODO: Timing
// A Input to output: 13, 14
// E Input to output: 9, 12

CHIP_DESC( 9321 ) =
{
	CHIP_START( 9321A<0> )
        INPUT_PINS( 1, 2, 3 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

	CHIP_START( 9321A<1> )
        INPUT_PINS( 1, 2, 3 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

	CHIP_START( 9321A<2> )
        INPUT_PINS( 1, 2, 3 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

	CHIP_START( 9321A<3> )
        INPUT_PINS( 1, 2, 3 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),    


	CHIP_START( 9321B<0> )
        INPUT_PINS( 13, 14, 15 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

    CHIP_START( 9321B<1> )
        INPUT_PINS( 13, 14, 15 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

	CHIP_START( 9321B<2> )
        INPUT_PINS( 13, 14, 15 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

	CHIP_START( 9321B<3> )
        INPUT_PINS( 13, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 13.0, 14.0 ),

   	CHIP_DESC_END
};

