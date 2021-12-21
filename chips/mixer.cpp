#include "mixer.h"
#include "../circuit.h"

/*
Mixer

Used for analog mixing of sound effects for final output.
Inputs: Pins 1-8
Output: Pin i1

       r[0]
1 ----/\/\/--------+
                   |
       r[1]        |
2 ----/\/\/--------+-----+
                   |     |            c2
.                  |     \<-----+-----||----- i1
.                  |     /      |
.                  |  r1 \     ---  
       r[7]        |     /     --- c1
8 ----/\/\/--------+     |      |
                         |      |
                        GND    GND

*/ 
 
MixerDesc::MixerDesc(std::vector<double> _r, double _r1, double _c1, double _c2) :
    r_in(_r), c1(_c1), r1(_r1), c2(_c2), v_c1(0.0), v_c2(0.0) 
{ 
    r_total = (r1 == 0.0) ? 0.0 : (1.0 / r1);
    for(double r : r_in) r_total += 1.0 / r;

    r_total = 1.0 / r_total;
}

CUSTOM_LOGIC( MixerDesc::init )
{
    MixerDesc* mixer = (MixerDesc*)chip->custom_data;
    
    mixer->v_c1 = 0.0;
    mixer->v_c2 = 0.0;

    mixer->c1_exp = (mixer->c1 == 0.0) ? 0.0 : chip->circuit->audio.rc_charge_exponent(mixer->r_total * mixer->c1);
    // Asumes 150k output impedence. TODO: Make configurable?
    mixer->c2_exp = (mixer->c2 == 0.0) ? 0.0 : chip->circuit->audio.rc_charge_exponent(K_OHM(150.0) * mixer->c2);

    chip->custom_update = MixerDesc::mixer;
}


CUSTOM_LOGIC( MixerDesc::mixer )
{
    MixerDesc* mixer = (MixerDesc*)chip->custom_data;

    double i = 0.0;
    for(int in = 0; in < mixer->r_in.size(); in++)
        i += chip->analog_input(in) / mixer->r_in[in];

    double v = i * mixer->r_total;

    // Low pass filtering
    if(mixer->c1 != 0.0)
    {
        mixer->v_c1 += (v - mixer->v_c1) * mixer->c1_exp;
        v = mixer->v_c1;
    }

    // High pass filtering
    mixer->v_c2 += (v - mixer->v_c2) * mixer->c2_exp;
    v -= mixer->v_c2;

    chip->analog_output = v;
}

CHIP_DESC( MIXER ) = 
{
	CUSTOM_CHIP_START(&MixerDesc::init)
        INPUT_PINS( 1, 2, 3, 4, 5, 6, 7, 8 )
        OUTPUT_PIN( i1 ),

    CHIP_DESC_END
};
