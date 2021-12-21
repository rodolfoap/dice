#include "../chip_desc.h"

/*
7485
4-bit noninverting magnitude comparator with cascade inputs.

     +---+--+---+
  B3 |1  +--+ 16| VCC
IA<B |2       15| A3
IA=B |3       14| B2
IA>B |4       13| A2
OA>B |5  7485 12| A1
OA=B |6       11| B1
OA<B |7       10| A0
 GND |8        9| B0
     +----------+

*/

static CHIP_LOGIC( 7485_G )
{
	int A = pin[10] + 2*pin[12] + 4*pin[13] + 8*pin[15];
	int B = pin[9]  + 2*pin[11] + 4*pin[14] + 8*pin[1];
	
	if(A > B)
		pin[5] = 1;
	else if(A < B)
		pin[5] = 0;		
	else if(!pin[2] && !pin[3])
		pin[5] = 1;	
    else
        pin[5] = 0;
}

static CHIP_LOGIC( 7485_EQ )
{
	int A = pin[10] + 2*pin[12] + 4*pin[13] + 8*pin[15];
	int B = pin[9]  + 2*pin[11] + 4*pin[14] + 8*pin[1];
	
	if(A != B)
		pin[6] = 0;
	else if(pin[3])
		pin[6] = 1;	
    else
        pin[6] = 0;
}


static CHIP_LOGIC( 7485_L )
{
	int A = pin[10] + 2*pin[12] + 4*pin[13] + 8*pin[15];
	int B = pin[9]  + 2*pin[11] + 4*pin[14] + 8*pin[1];
	
	if(A > B)
		pin[7] = 0;
	else if(A < B)
		pin[7] = 1;		
	else if(!pin[3] && !pin[4])
		pin[7] = 1;	
    else
        pin[7] = 0;
}



CHIP_DESC( 7485 ) =
{
	CHIP_START( 7485_G )
        INPUT_PINS( 1, 2, 3, 9, 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 5 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 7485_EQ )
        INPUT_PINS( 1, 3, 9, 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 23.0, 20.0 ),

    CHIP_START( 7485_L )
        INPUT_PINS( 1, 3, 4, 9, 10, 11, 12, 13, 14, 15 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_DESC_END
};

