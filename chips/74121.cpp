#include "74121.h"
#include "../circuit.h"

/*
74121
Monostable multivibrator with Schmitt-trigger inputs. 
Programmable output pulse width from 40 ns to 20 seconds.

     +---+--+---+
  /Q |1  +--+ 14| VCC
     |2       13|
/TR1 |3   74  12|
/TR2 |4  121  11| RCext
  TR |5       10| Cext
   Q |6        9| Rint
 GND |7        8|
     +----------+

*/

const double Mono74121Desc::LN_2 = 0.6931471805599453;

CUSTOM_LOGIC( Mono74121Desc::mono_74121 )
{
    Mono74121Desc* desc = (Mono74121Desc*)chip->custom_data;

    // Update tp_hl in standard portion based on rc value
    if(chip->output_links.size())
    {
        chip->output_links[0].chip->delay[1] = uint64_t((LN_2 * (desc->r + K_OHM(2.0)) * desc->c) / Circuit::timescale);
    }
}

static CHIP_LOGIC( 74121_TRIG )
{
    pin[i1] = (!pin[3] || !pin[4]) && pin[5];
}

static CHIP_LOGIC( 74121_Q )
{
    if(POS_EDGE_PIN(i1))
        pin[6] = 1;
    else if(pin[i2])
        pin[6] = 0;
    else
        pin[6] = prev_pin[6];
}

static CHIP_LOGIC( 74121_BUF )
{
    pin[i2] = pin[6];
}

static CHIP_LOGIC( 74121_Q_n )
{
    pin[1] = pin[6] ^ 1;
}


CHIP_DESC( 74121 ) = 
{
	CUSTOM_CHIP_START(&Mono74121Desc::mono_74121)
        OUTPUT_PIN( i1 ),

    CHIP_START( 74121_TRIG )
        INPUT_PINS( 3, 4, 5 )
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_NS( 40.0, 40.0 ),
        // pulses < 40-50 ns should not trigger the chip?

	CHIP_START( 74121_Q )
        INPUT_PINS( i1, i2 )
        EVENT_PINS( i1 )
        OUTPUT_PIN( 6 )
        PREV_OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ), 

    CHIP_START( 74121_BUF )
        INPUT_PINS( 6 )
        OUTPUT_PIN( i2 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),

    CHIP_START( 74121_Q_n )
        INPUT_PINS( 6 )
        OUTPUT_PIN( 1 )
        OUTPUT_DELAY_NS( 5.0, 5.0 ),

	CHIP_DESC_END
};

