#include "../chip_desc.h"
#include "../circuit.h"

/*
Clock Gate

Speed hack, used to disconnect high frequency signals from a chip 
when they cannot cause a change in the node's chip.

(For example, could be used to disconnect a high frequency signal from an AND gate
when the other input to the AND gate is low)

Input pins:
    1: Clock gate enable signal (low frequency)
    2: Clock or other high frequency signal, to be gated


Output pins: 
    3: Gated clock output

TODO: Find some way to do this automatically?
TODO: Doesn't always work correctly
*/

#if 0
static CUSTOM_LOGIC( CLK_GATE )
{
    if(mask & 2)
    {
        // Clock input deactivated, so disconnect again
        chip->input_links[1].chip->active_outputs &= ~chip->input_links[1].mask;
    }
    else
    {
        chip->inputs ^= mask;
    }

    if(chip->output_links.size()) // Probably can assume this is true
    {
        Chip* c = chip->output_links[0].chip;
        
        if(c->state != PASSIVE)
            c->deactivate_outputs();
        
        c->last_input_event[1] = chip->circuit->global_time;

        //c->inputs ^= 2;
        //c->input_event_table[c->inputs].push_back(c->input_events.end());
        //c->input_events.push_back(Event(c->circuit->global_time, c->inputs));

        if(chip->inputs & 1)
        {
            // Reconnect to clock
            //c->deactivate_outputs();
        }
        else // Disconnect from clock
        {
            //c->deactivate_outputs();
            c->state = ACTIVE;
            c->activation_time = c->circuit->global_time;
            c->input_links[0].chip->active_outputs &= ~c->input_links[0].mask;

            c->active_inputs = 0;
            c->cycle_time = 1;
            c->pending_event = 0;
        }
    }
}


static CHIP_LOGIC( CLK_BUF )
{
    pin[3] = pin[2];
}

CHIP_DESC( CLK_GATE ) = 
{
	CUSTOM_CHIP_START( CLK_GATE )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( i1 ),

	CHIP_START( CLK_BUF )
        INPUT_PINS( 2, i1 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 0.0, 0.0 ),

	CHIP_DESC_END
};
#endif

#if 0
static CHIP_LOGIC( CLK_GATE )
{
    pin[3] = pin[2] & pin[1];
}

CHIP_DESC( CLK_GATE ) = 
{
	CHIP_START( CLK_GATE )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 0.0, 0.0 ),

	CHIP_DESC_END
};
#endif

#if 1
static CHIP_LOGIC( CLK_GATE )
{
    if(POS_EDGE_PIN(2) && pin[1])
        pin[3] = 1;
    else if(NEG_EDGE_PIN(2))
        pin[3] = 0;
    else
        pin[3] = prev_pin[3];
}

CHIP_DESC( CLK_GATE ) = 
{
	CHIP_START( CLK_GATE )
        INPUT_PINS( 1, 2 )
        EVENT_PINS( 2 )
        OUTPUT_PIN( 3 )
        PREV_OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 0.0, 0.0 ),

	CHIP_DESC_END
};
#endif

static CHIP_LOGIC( CLK_GATE_n )
{
    pin[3] = pin[2] & ~pin[1];
}

CHIP_DESC( CLK_GATE_n ) = 
{
	CHIP_START( CLK_GATE_n )
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 0.0, 0.0 ),

	CHIP_DESC_END
};
