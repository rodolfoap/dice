#include "../chip_desc.h"

/*
74155
2-of-8 inverting decoder/demultiplexer with separate enables.

      +---+--+---+
 1EN1 |1  +--+ 16| VCC
/1EN2 |2       15| /2EN1
   S1 |3       14| /2EN2
 /1Y3 |4   74  13| S0
 /1Y2 |5  155  12| /2Y3
 /1Y1 |6       11| /2Y2
 /1Y0 |7       10| /2Y1
  GND |8        9| /2Y0
      +----------+
*/

template<int N> CHIP_LOGIC( 74155A )
{
    int S = 2*pin[3] + pin[13];
    
    if(!pin[1] || pin[2])
        pin[7-N] = 1;
    else if(S == N)
        pin[7-N] = 0;
    else
        pin[7-N] = 1;
}

template<int N> CHIP_LOGIC( 74155B )
{
    int S = 2*pin[3] + pin[13];
    
    if(pin[14] || pin[15])
        pin[9+N] = 1;
    else if(S == N)
        pin[9+N] = 0;
    else
        pin[9+N] = 1;
}

CHIP_DESC( 74155 ) =
{
	CHIP_START( 74155A<0> )
        INPUT_PINS( 1, 2, 3, 13 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

	CHIP_START( 74155A<1> )
        INPUT_PINS( 1, 2, 3, 13 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

	CHIP_START( 74155A<2> )
        INPUT_PINS( 1, 2, 3, 13 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

	CHIP_START( 74155A<3> )
        INPUT_PINS( 1, 2, 3, 13 )
        OUTPUT_PIN( 4 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),    


	CHIP_START( 74155B<0> )
        INPUT_PINS( 3, 13, 14, 15 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

    CHIP_START( 74155B<1> )
        INPUT_PINS( 3, 13, 14, 15 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

	CHIP_START( 74155B<2> )
        INPUT_PINS( 3, 13, 14, 15 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

	CHIP_START( 74155B<3> )
        INPUT_PINS( 3, 13, 14, 15 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 21.0, 21.0 ),

   	CHIP_DESC_END
};

