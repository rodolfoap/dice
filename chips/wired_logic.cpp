#include "../chip_desc.h"

/* 
	Wired-AND circuit    

	Inputs: Pins 1-9
    Output: Pin 10

       VCC
        |
        \
        /
        \
        /
        |
 1 -----+
        |
 2 -----+
        |
 ...    +----- 10 
        |
 9 -----+

*/

static CHIP_LOGIC( wired_and )
{
    uintptr_t NUM = (uintptr_t)custom_data;

    pin[10] = 1;

    for(int i = 1; i <= NUM; i++)
    {
        if(!pin[i])
        {
            pin[10] = 0;
            break;
        }
    }
}

CHIP_DESC( WIRED_AND ) = 
{
    CHIP_START( wired_and )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7, 8, 9 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 2.0, 2.0 ), //TODO: Is delay accurate?

   	CHIP_DESC_END
};



/* 
	Wired-OR circuit    

	Inputs: Pins 1-9
    Output: Pin 10
*/

static CHIP_LOGIC( wired_or )
{
    uintptr_t NUM = (uintptr_t)custom_data;

    pin[10] = 0;

    for(int i = 1; i <= NUM; i++)
    {
        if(pin[i])
        {
            pin[10] = 1;
            break;
        }
    }
}

CHIP_DESC( WIRED_OR ) = 
{
    CHIP_START( wired_or )
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7, 8, 9 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 2.0, 2.0 ), //TODO: Is delay accurate?

   	CHIP_DESC_END
};

