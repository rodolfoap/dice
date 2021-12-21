#include "../chip_desc.h"
#include "../circuit.h"

/*
	Electronic Latch: Provides power on reset.
	At power on, the latch output will be low.
	Moments later, it will go high, once the capacitors finish charging.
	After that, the latch can be set to either high or low
	by the rest of the circuit.
	
    TODO: Is this accurate?

	Inputs: 1 = Set, 2 = Reset
	Outputs: 3 = Transistor Output (the state of the latch)
	
	Set and reset are active low.
*/

static CUSTOM_LOGIC( latch_init )
{
    chip->state = PASSIVE;
    chip->active_outputs = (1 << chip->output_links.size()) - 1;   
    
    // Generate output event, called once at init
    chip->pending_event = chip->circuit->queue_push(chip, chip->delay[0]);
}

static CHIP_LOGIC( latch )
{
    if(!pin[i1])
        pin[3] = 0;
    else if(POS_EDGE_PIN(i1))
        pin[3] = 1;
    else if(!pin[2])
        pin[3] = 0;
    else if(!pin[1])
        pin[3] = 1;
    else
        pin[3] = prev_pin[3];
}

CHIP_DESC( LATCH ) = 
{
	CUSTOM_CHIP_START( &latch_init )
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_US( 1.0, 1.0 ),

    CHIP_START( latch )
        INPUT_PINS( 1, 2, i1 )
        EVENT_PINS( i1 )
        OUTPUT_PIN( 3 )
        PREV_OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 100.0, 100.0 ),

	CHIP_DESC_END
};
