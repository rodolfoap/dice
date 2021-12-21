#include "566.h"
#include "../circuit.h"

/*
566
Function Generator

        +---+--+---+    
    GND |1  +--+  8| VCC
     NC |2        7| C1
 SQ_WAV |3   566  6| R1
TRI_WAV |4        5| MOD 
        +----------+    
*/

// For VPOS = 5V, VNEG = -5V ONLY!
const NE566Desc::NE566_DATA NE566Desc::data = { 3.364 - 5.0, 1.940 - 5.0, 4.885 - 5.0, 4.495 - 5.0 };

NE566Desc::NE566Desc(double _r1, double _c1) : r1(_r1), c1(_c1), v_cap(0.0), flip_flop(0)
{

}

CUSTOM_LOGIC( NE566Desc::ne566 )
{
    NE566Desc* desc = (NE566Desc*)chip->custom_data;

    double sample_time = chip->circuit->audio.sample_time();
    double v_mod = chip->analog_input(0);
    double i = 0.0;

    if(v_mod > NE566Desc::data.osc_stop)
    {
        double v_charge = NE566Desc::VPOS - v_mod - 0.1;
        if(v_charge > 0.0)
        {
            i = (v_charge * 0.95) / desc->r1;
            double i_rise;

            if(v_mod < NE566Desc::data.osc_stable)
            {
                // TODO: improve nonlinear range calculation
                i_rise = (NE566Desc::VPOS - NE566Desc::data.osc_stable - 0.1) * 0.95 / desc->r1;
                i_rise *= 1.0 - (NE566Desc::data.osc_stable - v_mod) / (NE566Desc::data.osc_stable - NE566Desc::data.osc_stop);
            }
            else i_rise = i;
            
            // Loop during sample period
            for(double dt = sample_time; dt > 0.0;)
            {
                if(desc->flip_flop) // Discharging
                {
                    desc->v_cap -= i * sample_time / desc->c1;
                    dt = 0;
                    
                    // Discharged past lower limit?
                    if(desc->v_cap < NE566Desc::data.c_lo)
                    {
                        dt = desc->c1 * (NE566Desc::data.c_lo - desc->v_cap) / i;
                        desc->v_cap = NE566Desc::data.c_lo;
                        desc->flip_flop = 0;
                    }
                    break;
                }
                else // Charging
                {
                    desc->v_cap += i_rise * dt / desc->c1;
                    dt = 0;

                    if(desc->v_cap > v_mod) desc->v_cap = v_mod;

                    // Charged past upper limit?
                    if(desc->v_cap > NE566Desc::data.c_hi)
                    {
                        dt = desc->c1 * (desc->v_cap - NE566Desc::data.c_hi) / i;
                        desc->v_cap = NE566Desc::data.c_hi;
                        desc->flip_flop = 1;
                    }
                }
            }
        }
    }

    // Triangle output
    chip->analog_output = desc->v_cap;
}

CHIP_DESC( 566 ) = 
{
    // Triangle wave
	CUSTOM_CHIP_START(&NE566Desc::ne566)
        INPUT_PINS( 5 )
        OUTPUT_PIN( 4 ),

	CHIP_DESC_END
};

