#include "../circuit.h"
#include "2533.h"

/*
2533
1024-Bit Static Shift Register

       +--------+
   OUT |1      8| Vcc
   Vgg |2  25  7| IN2
   SEL |3  33  6| CLK
   GND |4      5| IN1
       +--------+
*/

static CHIP_LOGIC( 2533_DATA )
{
    pin[i1] = pin[3] ? pin[7] : pin[5];
}

enum { DATA_MASK = 1, CLK_MASK = 2 }; 

static CUSTOM_LOGIC( RAM_2533 )
{
    if(chip->custom_data == NULL)
    {
        printf("ERROR: Missing Ram2533Desc for chip %p\n", chip);
        return;
    }

    Ram2533Desc* desc = (Ram2533Desc*)chip->custom_data;

    chip->state = PASSIVE;
    chip->inputs ^= mask;

    if(!(chip->inputs & CLK_MASK) && (mask & CLK_MASK)) // CLK falling edge, shift and store data
    {
        desc->data[desc->addr] = chip->inputs & DATA_MASK;
        desc->addr = (desc->addr + 1) & 1023;
    }

    int new_out = desc->data[desc->addr];

    if(new_out != chip->output && chip->pending_event == 0)
    {
        // Schedule event
        chip->pending_event = chip->circuit->queue_push(chip, chip->delay[chip->output]);
    }
    else if(chip->pending_event && new_out == chip->output)
    {
        // Cancel event
        chip->pending_event = 0;
    }
}

CHIP_DESC( 2533 ) = 
{
	CHIP_START( 2533_DATA )
        INPUT_PINS( 3, 5, 7 )
        OUTPUT_PIN ( i1 )
        OUTPUT_DELAY_NS( 25.0, 25.0 ),

    CUSTOM_CHIP_START( &RAM_2533 )
	   INPUT_PINS( i1, 6 )
	   OUTPUT_PIN( 1 )
       OUTPUT_DELAY_NS( 200.0, 200.0 ),

   	CHIP_DESC_END
};

