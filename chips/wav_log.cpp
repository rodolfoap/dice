
#include <ctime>

#include "wav_log.h"
#include "../circuit.h"

/* 
    WAV Log:
    
    Outputs audio data in the WAV file format.
*/

template <typename T> void write(FILE* file, const T& t) 
{
    fwrite(&t, sizeof(T), 1, file);
}

WavLogDesc::WavLogDesc(const char* filename, double g) : gain(g * 1000.0)
{
    sample_count = 0;
    file = fopen(filename, "w+b");
	
    fwrite("RIFF", 1, 4, file);
    write<uint32_t>(file, 36 + (sample_count * 1 * 2));
    fwrite("WAVE", 1, 4, file);

    fwrite("fmt ", 1, 4, file);
    write<uint32_t>(file, 16);
    write<uint16_t>(file, 1); // PCM Audio Format
    write<uint16_t>(file, 1); // Num channels (Mono)
    write<uint32_t>(file, 48000); // Sample rate, TODO: configurable
    write<uint32_t>(file, 48000 * 1 * 2); // Byte rate, 16-bit samples
    write<uint16_t>(file, 1 * 2); // Block Align
    write<uint16_t>(file, 16); // Bits per sample

    fwrite("data", 1, 4, file);
    write<uint32_t>(file, sample_count * 1 * 2);
}
	
WavLogDesc::~WavLogDesc()
{
	// Write chunk sizes
    fseek(file, 4, SEEK_SET);
    write<uint32_t>(file, 36 + (sample_count * 1 * 2));
    fseek(file, 32, SEEK_CUR);
    write<uint32_t>(file, sample_count * 1 * 2);

    fclose(file);
}

CUSTOM_LOGIC( WavLogDesc::wav_log )
{
    WavLogDesc* desc = (WavLogDesc*)chip->custom_data;
    double v = chip->input_links[0].chip->analog_output * desc->gain;
    int16_t sample = (v > INT16_MAX) ? INT16_MAX : (v < INT16_MIN) ? INT16_MIN : v;
    write<int16_t>(desc->file, sample);
    desc->sample_count++;
}

CHIP_DESC( WAV_LOG ) = 
{
    CUSTOM_CHIP_START(&WavLogDesc::wav_log)
	    INPUT_PINS(1),

	CHIP_DESC_END
};
