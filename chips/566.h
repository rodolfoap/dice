#ifndef NE566_H
#define NE566_H

#include "../chip_desc.h"
#include "audio.h"

class NE566Desc
{
public:
    NE566Desc(double _r1, double _c1);
    static CUSTOM_LOGIC( ne566 );

private:
    double r1;
    double c1;
    double v_cap;
    int flip_flop;

    struct NE566_DATA
    {
        double c_hi;
        double c_lo;
        double osc_stable;
        double osc_stop;
    };

    static const NE566_DATA data;
    static constexpr double VPOS = 5.0;
};

extern CHIP_DESC( 566 );

#endif
