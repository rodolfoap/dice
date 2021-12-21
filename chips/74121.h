#ifndef MONO_74121_H
#define MONO_74121_H

#include "../chip_desc.h"

class Mono74121Desc
{
public:
	double r, c;

    static const double LN_2;

	Mono74121Desc(double res, double cap) : r(res), c(cap) { }

	static CUSTOM_LOGIC( mono_74121 );
};

extern CHIP_DESC( 74121 );

#endif
