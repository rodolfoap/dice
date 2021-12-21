#include "diode_matrix.h"

/* 
	Diode Matrix: An array of diodes connected to form a Wired-AND circuit.
	See Space Race netlist for an example.
	
	Inputs: RO01-ROW15
	Outputs: COL0-COL7
*/

template <int C>
CHIP_LOGIC( diode_matrix )
{
    DIODE_LAYOUT* d_l = (DIODE_LAYOUT*)custom_data;

    pin[COL0 + C] = 1;

    for(int i = 0; i < 16; i++)
    {
        if((*d_l)[i][C] && !pin[ROW0 + i])
        {
            pin[COL0 + C] = 0;
            break;
        }
    }
}


CHIP_DESC( DIODE_MATRIX ) = 
{
    CHIP_START( diode_matrix<0> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL0 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ), //TODO: Is delay accurate?

    CHIP_START( diode_matrix<1> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL1 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

    CHIP_START( diode_matrix<2> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL2 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

    CHIP_START( diode_matrix<3> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL3 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

    CHIP_START( diode_matrix<4> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL4 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

    CHIP_START( diode_matrix<5> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL5 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

    CHIP_START( diode_matrix<6> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL6 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

    CHIP_START( diode_matrix<7> )
        INPUT_PINS( ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7,
                    ROW8, ROW9, ROW10, ROW11, ROW12, ROW13, ROW14, ROW15 )
        OUTPUT_PIN( COL7 )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

   	CHIP_DESC_END
};

