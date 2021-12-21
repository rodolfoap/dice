#include "../chip_desc.h"

/*
7483
4-bit binary full adder with fast carry.

    +---+--+---+
 A4 |1  +--+ 16| B4          S=A+B+CIN
 S3 |2       15| S4
 A3 |3       14| COUT
 B3 |4       13| CIN
VCC |5  7483 12| GND
 S2 |6       11| B1
 B2 |7       10| A1
 A2 |8        9| S1
    +----------+
*/

static CHIP_LOGIC( 7483_S0 )
{
	pin[9] = pin[10] ^ pin[11] ^ pin[13];
}

static CHIP_LOGIC( 7483_S1 )
{
	int a = 2*pin[8] + pin[10];
    int b = 2*pin[7] + pin[11];
    int s = a + b + pin[13];

    pin[6] = (s >> 1) & 1;
}

static CHIP_LOGIC( 7483_S2 )
{
	int a = 4*pin[3] + 2*pin[8] + pin[10];
    int b = 4*pin[4] + 2*pin[7] + pin[11];
    int s = a + b + pin[13];

    pin[2] = (s >> 2) & 1;
}

static CHIP_LOGIC( 7483_S3 )
{
	int a = 8*pin[1]  + 4*pin[3] + 2*pin[8] + pin[10];
    int b = 8*pin[16] + 4*pin[4] + 2*pin[7] + pin[11];
    int s = a + b + pin[13];

    pin[15] = (s >> 3) & 1;
}

static CHIP_LOGIC( 7483_COUT )
{
	int a = 8*pin[1]  + 4*pin[3] + 2*pin[8] + pin[10];
    int b = 8*pin[16] + 4*pin[4] + 2*pin[7] + pin[11];
    int s = a + b + pin[13];

    pin[14] = (s >> 4) & 1;
}

CHIP_DESC( 7483 ) =
{
	CHIP_START( 7483_S0 )
        INPUT_PINS( 10, 11, 13 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 16.0, 16.0 ),

	CHIP_START( 7483_S1 )
        INPUT_PINS( 7, 8, 10, 11, 13 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 7483_S2 )
        INPUT_PINS( 3, 4, 7, 8, 10, 11, 13 )
        OUTPUT_PIN( 2 )
        OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 7483_S3 )
        INPUT_PINS( 1, 3, 4, 7, 8, 10, 11, 13, 16 )
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 16.0, 16.0 ),

    CHIP_START( 7483_COUT )
        INPUT_PINS( 1, 3, 4, 7, 8, 10, 11, 13, 16 )
        OUTPUT_PIN( 14 )
        OUTPUT_DELAY_NS( 9.0, 11.0 ),

	CHIP_DESC_END
};

