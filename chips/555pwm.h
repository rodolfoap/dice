#ifndef PWM_555_H
#define PWM_555_H

#include "../chip_desc.h"

class PWM555Desc
{
public:
	double r, c;
    uint64_t start_time, pulse_len;

    static const double LN_3;

	PWM555Desc(double res, double cap) : r(res), c(cap) { };
	static CUSTOM_LOGIC( pwm_555 );
};

extern CHIP_DESC( 555_PWM );

#endif
