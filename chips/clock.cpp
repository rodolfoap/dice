#include "../chip_desc.h"
#include "../circuit.h"

/* 
	Clock

Inputs: None
Clock Output: Pin 1

*/

CUSTOM_LOGIC( clock )
{
    chip->state = ACTIVE;
    chip->activation_time = chip->circuit->global_time;

    chip->output_events.clear();
    chip->output_events.push_back(chip->circuit->global_time);
    chip->output_events.push_back(chip->circuit->global_time + chip->delay[0]);
    chip->cycle_time = chip->delay[0] + chip->delay[1];
    chip->first_output_event = chip->output_events.begin();
    chip->current_output_event = chip->output_events.begin();
    chip->end_time = ~0ull;
    
    chip->pending_event = chip->circuit->queue_push(chip, chip->delay[0]);
}

CHIP_DESC( CLOCK_14_318_MHZ ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_US( 0.5/14.31818, 0.5/14.31818 ),

	CHIP_DESC_END
};

CHIP_DESC( CLOCK_12_096_MHZ ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_US( 0.5/12.096, 0.5/12.096 ),

	CHIP_DESC_END
};

CHIP_DESC( CLOCK_10_733_MHZ ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_US( 0.5/10.733, 0.5/10.733 ),

	CHIP_DESC_END
};

CHIP_DESC( CLOCK_8_MHZ ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_US( 0.5/8.0, 0.5/8.0 ),

	CHIP_DESC_END
};

CHIP_DESC( CLOCK_6_MHZ ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_US( 0.5/6.0, 0.5/6.0 ),

	CHIP_DESC_END
};
