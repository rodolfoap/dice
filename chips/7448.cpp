#include "../chip_desc.h"

/*
7448
BCD to 7-segment decoder/common-cathode LED driver with ripple blank input and output.

        +---+--+---+
     A1 |1  +--+ 16| VCC
     A2 |2       15| YF
    /LT |3       14| YG
/BI/RBO |4       13| YA
   /RBI |5  7448 12| YB
     A3 |6       11| YC
     A0 |7       10| YD
    GND |8        9| YE
        +----------+
*/

static uint8_t segments[7][16] = 
{
    { 1, 0, 1, 1,  0, 1, 0, 1,  1, 1, 0, 0,  0, 1, 0, 0 },
    { 1, 1, 1, 1,  1, 0, 0, 1,  1, 1, 0, 0,  1, 0, 0, 0 },
    { 1, 1, 0, 1,  1, 1, 1, 1,  1, 1, 0, 1,  0, 0, 0, 0 },
    { 1, 0, 1, 1,  0, 1, 1, 0,  1, 0, 1, 0,  0, 1, 1, 0 },
    { 1, 0, 1, 0,  0, 0, 1, 0,  1, 0, 1, 0,  0, 0, 1, 0 },
    { 1, 0, 0, 0,  1, 1, 1, 0,  1, 1, 0, 0,  1, 1, 1, 0 },
    { 0, 0, 1, 1,  1, 1, 1, 0,  1, 1, 1, 1,  1, 1, 1, 0 }
};

static CHIP_LOGIC( 7448A )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[13] = 0;
    else if(!pin[3])              pin[13] = 1;
    else if(val == 0 && !pin[5])  pin[13] = 0;
    else                          pin[13] = segments[0][val];
}

static CHIP_LOGIC( 7448B )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[12] = 0;
    else if(!pin[3])              pin[12] = 1;
    else if(val == 0 && !pin[5])  pin[12] = 0;
    else                          pin[12] = segments[1][val];
}

static CHIP_LOGIC( 7448C )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[11] = 0;
    else if(!pin[3])              pin[11] = 1;
    else if(val == 0 && !pin[5])  pin[11] = 0;
    else                          pin[11] = segments[2][val];
}

static CHIP_LOGIC( 7448D )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[10] = 0;
    else if(!pin[3])              pin[10] = 1;
    else if(val == 0 && !pin[5])  pin[10] = 0;
    else                          pin[10] = segments[3][val];
}

static CHIP_LOGIC( 7448E )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[9] = 0;
    else if(!pin[3])              pin[9] = 1;
    else if(val == 0 && !pin[5])  pin[9] = 0;
    else                          pin[9] = segments[4][val];
}

static CHIP_LOGIC( 7448F )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[15] = 0;
    else if(!pin[3])              pin[15] = 1;
    else if(val == 0 && !pin[5])  pin[15] = 0;
    else                          pin[15] = segments[5][val];
}

static CHIP_LOGIC( 7448G )
{
	int val = (pin[6] << 3) | (pin[2] << 2) | (pin[1] << 1) | pin[7];
    
    if(!pin[4])                   pin[14] = 0;
    else if(!pin[3])              pin[14] = 1;
    else if(val == 0 && !pin[5])  pin[14] = 0;
    else                          pin[14] = segments[6][val];
}

// TODO: Schematic says 100 ns delay, seems a bit long? Verify.

CHIP_DESC( 7448 ) =
{
	CHIP_START( 7448A )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 13 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 7448B )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 7448C )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 7448D )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 7448E )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 7448F )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 7448G )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7 )
        OUTPUT_PIN( 14 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

	CHIP_DESC_END
};
