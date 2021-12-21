#include "../chip_desc.h"

/*
74150
16-to-1 line inverting data selector/multiplexer.

    +---+--+---+
 D7 |1  +--+ 24| VCC
 D6 |2       23| D8
 D5 |3       22| D9
 D4 |4       21| D10
 D3 |5       20| D11
 D2 |6   74  19| D12
 D1 |7  150  18| D13
 D0 |8       17| D14
/EN |9       16| D15
 /Y |10      15| S0
 S3 |11      14| S1
GND |12      13| S2
    +----------+
*/

static CHIP_LOGIC( 74150 )
{
	if(pin[9])
    {
        pin[10] = 1;
    }
    else
    {
        switch(8*pin[11] + 4*pin[13] + 2*pin[14] + pin[15])
        {
            case 0:  pin[10] = pin[8] ^ 1;  break;
            case 1:  pin[10] = pin[7] ^ 1;  break;
            case 2:  pin[10] = pin[6] ^ 1;  break;
            case 3:  pin[10] = pin[5] ^ 1;  break;
            case 4:  pin[10] = pin[4] ^ 1;  break;
            case 5:  pin[10] = pin[3] ^ 1;  break;
            case 6:  pin[10] = pin[2] ^ 1;  break;
            case 7:  pin[10] = pin[1] ^ 1;  break;
            case 8:  pin[10] = pin[23] ^ 1; break;
            case 9:  pin[10] = pin[22] ^ 1; break;
            case 10: pin[10] = pin[21] ^ 1; break;
            case 11: pin[10] = pin[20] ^ 1; break;
            case 12: pin[10] = pin[19] ^ 1; break;
            case 13: pin[10] = pin[18] ^ 1; break;
            case 14: pin[10] = pin[17] ^ 1; break;
            case 15: pin[10] = pin[16] ^ 1; break;                    
        }
    }
}

// SEL->OUT: tPLH = 23, tPHL = 22
// ENABLE->OUT: tPLH = 15.5, tPHL = 21
// DATA->OUT: tPLH = 13, tPHL = 8.5
CHIP_DESC( 74150 ) =
{
	CHIP_START( 74150 )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 23.0, 22.0 ),

   	CHIP_DESC_END
};

