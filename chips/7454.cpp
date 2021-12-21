#include "../chip_desc.h"

/*
7454
4-wide 2-input AND-NOR gate.

    +---+--+---+                 _______________________
  A |1  +--+ 14| VCC        /Y = (A.B)+(C.D)+(E.F)+(G.H)
  C |2       13| B
  D |3       12| NC
  E |4  7454 11| NC
  F |5       10| H
 NC |6        9| G
GND |7        8| Y
    +----------+

*/

static CHIP_LOGIC( 7454 )
{
	pin[8] = ((pin[1] & pin[13]) | (pin[2] & pin[3]) | (pin[4] & pin[5]) | (pin[9] & pin[10])) ^ 1;
}

CHIP_DESC( 7454 ) =
{
	CHIP_START( 7454 )
        INPUT_PINS( 1, 2, 3, 4, 5, 9, 10, 13 )
        OUTPUT_PIN( 8 )
        OUTPUT_DELAY_NS( 13.0, 8.0 ),

	CHIP_DESC_END
};

