#ifndef AUDIODESC_H
#define AUDIODESC_H

enum SpeakerConfiguration { MONO = 0, STEREO };

struct AudioDesc
{
    double gain;
    SpeakerConfiguration speaker_config;

    AudioDesc() : gain(10.0), speaker_config(MONO) { }

    AudioDesc& set_gain(double g) 
    { 
        gain = g;
        return *this;
    }
    AudioDesc& set_speaker(SpeakerConfiguration s) 
    { 
        speaker_config = s;
        return *this;
    }
};

#define AUDIO_DESC( name ) AudioDesc audio_##name = AudioDesc()
#define AUDIO_GAIN( g ) .set_gain(g)
#define AUDIO_SPEAKER_CONFIG( s ) .set_speaker(s)
#define AUDIO_DESC_END ;

#endif
