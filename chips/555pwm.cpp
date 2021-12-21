#include "555pwm.h"
#include "../circuit.h"

/*
555
Universal timer (PWM mode).
555 with control input driven by the threshold output of a second 555.
Assuming linear voltage ramp due to transistors, or something.

NOTE: In netlist, connect output (pin 3) of first 555 to pin 5 of this one.

TODO: Make more accurate, mostly guesswork. Hook up reset

     +---+--+---+            +------+--------+------*---+-----+
 GND |1  +--+  8| VCC        | /RST |  /TR   | Thr  | Q | Dis |
 /TR |2        7| Dis        +======+========+======*===+=====+
   Q |3   555  6| Thr        |  0   |   X    |  X   | 0 |  0  |
/RST |4        5| CV         |  1   | < CV/2 |  X   | 1 |  Z  |
     +----------+            |  1   | > CV/2 | < CV | - |  -  |
                             |  1   | > CV/2 | > CV | 0 |  0  |
                             +------+--------+------*---+-----+
*/

const double PWM555Desc::LN_3 = 1.0986122886681;

CUSTOM_LOGIC( PWM555Desc::pwm_555 )
{
    PWM555Desc* desc = (PWM555Desc*)chip->custom_data;

    chip->state = PASSIVE;

    chip->inputs ^= mask;

    if(mask == 1 && (chip->inputs & 1)) // Update start time, pulse length
    {
        desc->start_time = chip->circuit->global_time;
        desc->pulse_len = chip->input_links[0].chip->input_links[2].chip->delay[0];
    }
    else if(mask == 2 && !(chip->inputs & 2)) // Falling edge trigger
    {
        // Generate rising edge event
        chip->pending_event = chip->circuit->queue_push(chip, chip->delay[0]);
    }
    else if(mask == 4 && (chip->inputs & 4)) // Rising edge output
    {
        // Generate falling edge event based on input voltage
        double scale = 0.2 + 0.8*((chip->circuit->global_time - desc->start_time) / double(desc->pulse_len));
        chip->pending_event = chip->circuit->queue_push(chip, uint64_t(LN_3*desc->r*desc->c*scale / Circuit::timescale));
    }
}

static CHIP_LOGIC( 555_Buf ) // Needed to generate an event to pwm_555
{
    pin[3] = pin[i2];
}


CHIP_DESC( 555_PWM ) = 
{
	CUSTOM_CHIP_START(&PWM555Desc::pwm_555)
        INPUT_PINS( 5, 2, 3 )
        OUTPUT_PIN( i2 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ), // tp_hl will be overwritten

    CHIP_START( 555_Buf )
        INPUT_PINS( i2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 1.0, 1.0 ),

	CHIP_DESC_END
};
