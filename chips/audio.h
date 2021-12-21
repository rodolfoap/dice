#ifndef AUDIO_H
#define AUDIO_H

#include <cmath>
#include "../chip_desc.h"
#include "../audio_desc.h"
#include "../cirque.h"
#include "../settings.h"

class Audio
{
public:
    const AudioDesc* desc;
    const Settings::Audio* settings;

    Audio();
    ~Audio();
    void audio_init(Circuit* circuit);
    void toggle_mute();
    static void callback(void* userdata, uint8_t* str, int len);

    static CUSTOM_LOGIC( audio_input );
    static CUSTOM_LOGIC( audio_output );
    static CUSTOM_LOGIC( audio );

    enum AudioPins { OUTPUT_MONO = i13, OUTPUT_LEFT = i13, OUTPUT_RIGHT = i14 };

    double sample_time() { return sample_period; }
    double rc_charge_exponent(double rc) { return 1.0 - exp(-sample_time() / rc); }
    double rc_discharge_exponent(double rc) { return exp(-sample_time() / rc); }

    static double rc_charge_exponent(double dt, double rc) { return 1.0 - exp(-dt / rc); }
    static double rc_discharge_exponent(double dt, double rc) { return exp(-dt / rc); }
private:
    std::vector<Chip*> audio_nodes;
    cirque<int16_t> audio_buffer;
    double gain;
    double sample_period;
};

extern CHIP_DESC( AUDIO );

#endif
