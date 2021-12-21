#ifndef WAV_LOG_H
#define WAV_LOG_H

#include "../chip_desc.h"

#include <cstdio>

/* 
    WAV Log:
    
    Outputs audio data in the WAV file format.
*/

class WavLogDesc
{
private:
    FILE* file;
    uint32_t sample_count;
    double gain;

public:
	WavLogDesc(const char* filename, double g = 10.0);

    ~WavLogDesc();

    static CUSTOM_LOGIC( wav_log );
};

extern CHIP_DESC( WAV_LOG );

#endif
