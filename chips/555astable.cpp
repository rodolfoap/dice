#include "555astable.h"
#include "../circuit.h"

/*
555
Universal timer (Astable mode).
When CV is not connected it is held at 2/3.VCC through a 5k/10k divider.

     +---+--+---+            +------+--------+------*---+-----+
 GND |1  +--+  8| VCC        | /RST |  /TR   | Thr  | Q | Dis |
 /TR |2        7| Dis        +======+========+======*===+=====+
   Q |3   555  6| Thr        |  0   |   X    |  X   | 0 |  0  |
/RST |4        5| CV         |  1   | < CV/2 |  X   | 1 |  Z  |
     +----------+            |  1   | > CV/2 | < CV | - |  -  |
                             |  1   | > CV/2 | > CV | 0 |  0  |
                             +------+--------+------*---+-----+
*/

const double Astable555Desc::LN_2 = 0.6931471805599453;

CUSTOM_LOGIC( Astable555Desc::astable_555 )
{
    Astable555Desc* desc = (Astable555Desc*)chip->custom_data;

    if(chip->state != PASSIVE)
        chip->deactivate_outputs();

    chip->inputs ^= mask;

    // Update tp_lh, tp_hl in standard portion based on r and c values
    if(mask == 1 && !(chip->inputs & 1)) // Negedge /RST (reset being asserted)
    {
        if(chip->output)
            chip->pending_event = chip->circuit->queue_push(chip, 100.0e-9 / Circuit::timescale);
    }
    else if(chip->inputs & 1) // Posedge /RST (reset being de-asserted)
    {
        double TC = -log((5.0 - desc->ctrl) / (5.0 - 0.5*desc->ctrl));
        uint64_t hi_time = uint64_t(TC * (desc->r1 + desc->r2) * desc->c / Circuit::timescale);
        uint64_t lo_time = uint64_t(LN_2 * desc->r2 * desc->c / Circuit::timescale);

        chip->output_events.clear();

        uint64_t delay = 0;
        if(mask == 1) // Coming out of reset
        {
            TC = -log((5.0 - desc->ctrl) / 5.0);
            delay = uint64_t(TC * (desc->r1 + desc->r2) * desc->c / Circuit::timescale);
            //printf("first:%lld hi:%lld\n", delay, hi_time);

            chip->update_output(); // TODO: Should be delayed by several ns

            chip->activation_time = chip->circuit->global_time + delay;
            chip->output_events.push_back(Event(chip->activation_time, 0));
            chip->output_events.push_back(Event(chip->activation_time + hi_time, 1));
        }
        else if(chip->output)
        {
            if(chip->last_output_event + hi_time > chip->circuit->global_time)
                delay = chip->last_output_event + hi_time - chip->circuit->global_time;
            
            // TODO: Adjust delay exponentially based on time remaining?

            chip->activation_time = chip->circuit->global_time + delay;
            chip->output_events.push_back(Event(chip->activation_time, 0));
            chip->output_events.push_back(Event(chip->activation_time + hi_time, 1));
        }
        else
        {
            if(chip->last_output_event + lo_time > chip->circuit->global_time)
                delay = chip->last_output_event + lo_time - chip->circuit->global_time;

            chip->activation_time = chip->circuit->global_time + delay;
            chip->output_events.push_back(Event(chip->activation_time, 0));
            chip->output_events.push_back(Event(chip->activation_time + lo_time, 1));
        }
        
        chip->state = ACTIVE;
        chip->cycle_time = hi_time + lo_time;
        chip->end_time = ~0ull;
        chip->first_output_event = chip->output_events.begin();
        chip->current_output_event = chip->output_events.begin();

        chip->pending_event = chip->circuit->queue_push(chip, delay);

        //printf("t:%lld pend:%lld del:%lld 1:%lld 2:%lld hi:%lld lo:%lld TC:%g\n", chip->circuit->global_time, chip->pending_event, delay, chip->output_events.front().time, chip->output_events.back().time, hi_time, lo_time, TC);
    }
}

CHIP_DESC( 555_Astable ) = 
{
	CUSTOM_CHIP_START(&Astable555Desc::astable_555)
        INPUT_PINS( 4, i3 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

	CHIP_DESC_END
};
