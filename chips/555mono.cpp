#include "555mono.h"
#include "../circuit.h"

/*
555
Universal timer (Monostable mode).
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

const double Mono555Desc::LN_3 = 1.0986122886681;

CUSTOM_LOGIC( Mono555Desc::mono_555 )
{
    Mono555Desc* desc = (Mono555Desc*)chip->custom_data;

    // Update tp_hl in standard portion based on rc value
    if(chip->output_links.size())
    {
        if(chip->state != PASSIVE)
            chip->deactivate_outputs(); // TODO: is this necessary?

        chip->output_links[0].chip->delay[0] = uint64_t(LN_3*desc->r*desc->c / Circuit::timescale);

        // Generate event to standard portion of chip
        chip->pending_event = chip->circuit->queue_push(chip, 0);
    }
}

static CHIP_LOGIC( 555_BUF ) 
{
    pin[i2] = pin[2];
}

static CHIP_LOGIC( 555_LATCH )
{
    if(!pin[4])
        pin[3] = 0;
    else if(!pin[i2])
        pin[3] = 1;
    else if(pin[i4])
        pin[3] = 0;
    else
        pin[3] = prev_pin[3];
}

static CHIP_LOGIC( 555_THRESH ) 
{
    pin[i4] = pin[3];
}

CHIP_DESC( 555_Mono ) = 
{
	CUSTOM_CHIP_START(&Mono555Desc::mono_555)
        INPUT_PINS( i3 )
        OUTPUT_PIN( i1 ),

    CHIP_START( 555_BUF )
        INPUT_PINS( 2 )
        OUTPUT_PIN( i2 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),
        // TODO: Currently pulses < 50 ns will not trigger the chip, is this accurate?

	CHIP_START( 555_LATCH )
        INPUT_PINS( i2, 4, i4 )
        OUTPUT_PIN( 3 )
        PREV_OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_START( 555_THRESH )
        INPUT_PINS( i1, 3 )
        OUTPUT_PIN( i4 )
        OUTPUT_DELAY_NS( 10.0, 10.0 ), // tp_lh will be overwritten

	CHIP_DESC_END
};

