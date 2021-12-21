#ifndef MONO_9602_H
#define MONO_9602_H

#include "../chip_desc.h"

class Mono9602Desc
{
public:
	double r1, c1, r2, c2;

    static const double TIME_CONSTANT;

	Mono9602Desc(double res1, double cap1, double res2, double cap2) 
        : r1(res1), c1(cap1), r2(res2), c2(cap2) { };

	static CUSTOM_LOGIC( mono_9602A );
    static CUSTOM_LOGIC( mono_9602B );
};

class Mono74123Desc
{
public:
	double r1, c1, r2, c2;

    static const double TIME_CONSTANT;

	Mono74123Desc(double res1, double cap1, double res2, double cap2) 
        : r1(res1), c1(cap1), r2(res2), c2(cap2) { };

	static CUSTOM_LOGIC( mono_74123A );
    static CUSTOM_LOGIC( mono_74123B );
};

extern CHIP_DESC( 9602 );
extern CHIP_DESC( 74123 );

#endif
