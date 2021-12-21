#include "../chip_desc.h"

/*
7430
8-input NAND gate.

    +---+--+---+                 ________
  A |1  +--+ 14| VCC        /Y = ABCDEFGH
  B |2       13|
  C |3       12| H
  D |4  7430 11| G
  E |5       10|
  F |6        9|
GND |7        8| /Y
    +----------+
*/

static CHIP_LOGIC( 7430 )
{
	pin[8] = (pin[1] & pin[2] & pin[3] & pin[4] & pin[5] & pin[6] & pin[11] & pin[12]) ^ 1;
}

CHIP_DESC( 7430 ) =
{
	CHIP_START( 7430 )
		INPUT_PINS( 1, 2, 3, 4, 5, 6, 11, 12 )
		OUTPUT_PIN( 8 )
		OUTPUT_DELAY_NS( 13.0, 8.0 ),
	CHIP_DESC_END
};

