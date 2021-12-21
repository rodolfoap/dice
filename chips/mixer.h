#ifndef MIXER_H
#define MIXER_H

#include "../chip_desc.h"
#include "audio.h"
#include <vector>

class MixerDesc
{
public:
    std::vector<double> r_in;
    double c1;
    double r1;
    double c2; // optional
    
    MixerDesc(std::vector<double> _r, double _r1 = 0.0, double _c1 = 0.0, double _c2 = 0.0);
    static CUSTOM_LOGIC( mixer );
    static CUSTOM_LOGIC( init );

private:
    double r_total;
    double c1_exp;
    double c2_exp;

    double v_c1 = 0.0;
    double v_c2 = 0.0;
};

extern CHIP_DESC( MIXER );

#endif
