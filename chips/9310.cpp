#include "../chip_desc.h"

/*
74160 (9310)
4-bit synchronous BCD decade counter with load, asynchronous reset, and ripple carry output.

     +---+--+---+
/RST |1  +--+ 16| VCC
 CLK |2       15| RCO
  P0 |3       14| Q0
  P1 |4       13| Q1
  P2 |5  9310 12| Q2
  P3 |6       11| Q3
 ENP |7       10| ENT
 GND |8        9| /LOAD
     +----------+
*/

static CHIP_LOGIC( 9310A )
{
	if(!pin[1])						
		pin[14] = 0;					
	else if(POS_EDGE_PIN(2) && !pin[9])		
		pin[14] = pin[3];				
	else if(POS_EDGE_PIN(2) && pin[7] && pin[10])	 
		pin[14] = prev_pin[14] ^ 1;			
	else							
		pin[14] = prev_pin[14];				
}


static CHIP_LOGIC( 9310B )
{
	if(!pin[1])
		pin[13] = 0;
	else if(POS_EDGE_PIN(2) && !pin[9])
		pin[13] = pin[4];
	else if(POS_EDGE_PIN(2) && pin[7] && pin[10])
		pin[13] = !pin[11] & (prev_pin[13] ^ pin[14]);
	else
        pin[13] = prev_pin[13];
}

static CHIP_LOGIC( 9310C )
{
	if(!pin[1])
		pin[12] = 0;
	else if(POS_EDGE_PIN(2) && !pin[9])
		pin[12] = pin[5];
    else if(POS_EDGE_PIN(2) && pin[7] && pin[10])
		pin[12] = ((pin[14] & pin[13]) ^ prev_pin[12]);		
    else
       	pin[12] = prev_pin[12];
}

static CHIP_LOGIC( 9310D )
{
	if(!pin[1])
		pin[11] = 0;
	else if(POS_EDGE_PIN(2) && !pin[9])
		pin[11] = pin[6];
	else if(POS_EDGE_PIN(2) && pin[7] && pin[10])
		pin[11] = (((pin[14] & pin[13] & pin[12]) | (pin[14] & prev_pin[11])) ^ prev_pin[11]);
	else
        pin[11] = prev_pin[11];
}

static CHIP_LOGIC( 9310_TC )
{
    pin[15] = pin[10] & pin[11] & !pin[12] & !pin[13] & pin[14];
}

CHIP_DESC( 9310 ) =
{
	CHIP_START( 9310A )
        INPUT_PINS( 1, 2, 3, 7, 9, 10 )
        OUTPUT_PIN( 14 )
		EVENT_PINS( 2 )
		PREV_OUTPUT_PIN( 14 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 9310B )
        INPUT_PINS( 1, 2, 4, 7, 9, 10, 11, 14 )
        OUTPUT_PIN( 13 )
		EVENT_PINS( 2 )
		PREV_OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 9310C )
        INPUT_PINS( 1, 2, 5, 7, 9, 10, 14, 13 )
        OUTPUT_PIN( 12 )
		EVENT_PINS( 2 )
		PREV_OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

	CHIP_START( 9310D )
        INPUT_PINS( 1, 2, 6, 7, 9, 10, 14, 13, 12 )
        OUTPUT_PIN( 11 )
		EVENT_PINS( 2 )
		PREV_OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 12.0, 15.0 ),

    CHIP_START( 9310_TC )
        INPUT_PINS( 10, 14, 13, 12, 11)
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 6.0, 2.0 ), // Somewhat arbitrary

    CHIP_DESC_END
};

