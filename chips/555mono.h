#ifndef MONO_555_H
#define MONO_555_H

#include "../chip_desc.h"

class Mono555Desc
{
public:
	double r, c;

    static const double LN_3;

	Mono555Desc(double res, double cap) : r(res), c(cap) { }
	static CUSTOM_LOGIC( mono_555 );
};

extern CHIP_DESC( 555_Mono );

#endif
