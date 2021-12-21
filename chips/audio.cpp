#include "audio.h"
#include "../circuit.h"

#include <SDL/SDL.h>
#include <algorithm>
#include <set>

/* 
	Inputs:
		1-8: AUDIO

	Outputs:
		None

    TODO: Handle simulation running slower than real time better?
    TODO: Audio can cause program to hang? If audio device isn't working properly? 
*/

static const double TTL_VOLTAGES[] = { 0.2, 3.4 };

static CUSTOM_LOGIC( audio_timer )
{
    chip->state = ACTIVE;
    chip->activation_time = chip->circuit->global_time;

    chip->delay[0] = chip->delay[1] = uint64_t(chip->circuit->audio.sample_time() / Circuit::timescale);

    chip->output_events.clear();
    chip->output_events.push_back(chip->circuit->global_time);
    chip->output_events.push_back(chip->circuit->global_time + chip->delay[0]);
    chip->cycle_time = chip->delay[0] + chip->delay[1];
    chip->first_output_event = chip->output_events.begin();
    chip->current_output_event = chip->output_events.begin();
    chip->end_time = ~0ull;

    chip->pending_event = chip->circuit->queue_push(chip, chip->delay[0]);
}

CUSTOM_LOGIC( Audio::audio_input )
{
    chip->inputs ^= mask;
    chip->analog_output = TTL_VOLTAGES[chip->inputs & 1];
}

CUSTOM_LOGIC( Audio::audio_output )
{
    Audio* audio = (Audio*)chip->custom_data;
    double volume = chip->circuit->settings.audio.volume * audio->gain;
    double v = chip->input_links[0].chip->analog_output * volume;
    int16_t sample = (v > INT16_MAX) ? INT16_MAX : (v < INT16_MIN) ? INT16_MIN : v;

    SDL_LockAudio();
    audio->audio_buffer.push_back(sample);
    SDL_UnlockAudio();
}

CHIP_DESC( AUDIO ) = 
{
	CUSTOM_CHIP_START(&audio_timer)
        OUTPUT_DELAY_S( 1.0 / 48000.0, 1.0 / 48000.0 ) // Sample rate, will be overwritten
        INPUT_PINS( i10 ) OUTPUT_PIN( i11 ),

	CUSTOM_CHIP_START(&Audio::audio)
        INPUT_PINS( i11 ) OUTPUT_PIN( i12 ),

    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 1, i12 ) OUTPUT_PIN( i1 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 2, i12 ) OUTPUT_PIN( i2 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 3, i12 ) OUTPUT_PIN( i3 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 4, i12 ) OUTPUT_PIN( i4 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 5, i12 ) OUTPUT_PIN( i5 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 6, i12 ) OUTPUT_PIN( i6 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 7, i12 ) OUTPUT_PIN( i7 ),
    CUSTOM_CHIP_START(&Audio::audio_input) INPUT_PINS( 8, i12 ) OUTPUT_PIN( i8 ),

	CUSTOM_CHIP_START(&Audio::audio_output)
        INPUT_PINS( Audio::OUTPUT_MONO ),

	CHIP_DESC_END
};

CUSTOM_LOGIC( Audio::audio )
{
    Audio* audio = (Audio*)chip->custom_data;
    std::vector<Chip*>& audio_nodes = audio->audio_nodes;

    if(mask == 0) // init
    {
        //printf("audio init %d\n", chip->output_links.size());
        std::set<Chip*> input_nodes, unqueued_nodes;
        for(const ChipLink& cl : chip->output_links) 
        {
            input_nodes.insert(cl.chip);
            for(const ChipLink& cl2 : cl.chip->output_links)
                unqueued_nodes.insert(cl2.chip);
        }
        // Add VCC & GND to input_nodes. TODO: Make less ugly
        input_nodes.insert(chip->circuit->chips[0]);
        input_nodes.insert(chip->circuit->chips[1]);
        
        // Enque all outputs of the DAC inputs in topological order.
        // This algorithm probably sucks?
        // TODO: Can infinite loop if there is a problem in the netlist?
        while(!unqueued_nodes.empty())
        for(Chip* c : unqueued_nodes)
        {
            bool can_insert = true;
            for(const ChipLink& cl : c->input_links)
                if(input_nodes.count(cl.chip) == 0 && std::find(audio_nodes.begin(), audio_nodes.end(), cl.chip) == audio_nodes.end())
                {
                    //printf("cannot insert %p\n", cl.chip);
                    can_insert = false;
                    break;
                }

            if(can_insert)
            {
                //printf("inserted %p\n", c);
                audio_nodes.push_back(c);
                
                for(const ChipLink& cl : c->output_links)
                    unqueued_nodes.insert(cl.chip);
                
                unqueued_nodes.erase(c);
                break;
            }
        }
    }

    // Process all nodes
    for(Chip* c : audio_nodes) c->custom_update(c, 0);
}

Audio::Audio() : gain(10.0), desc(NULL), settings(NULL), audio_buffer(8192)
{ }

void Audio::audio_init(Circuit* circuit)
{
    settings = &circuit->settings.audio;
    
    //int buffer_size = FREQUENCY[circuit->settings.audio.frequency] / 50; // 20 ms, TODO: Make configurable?
    // Hardcoded 2048 buffer size now TODO: Make configurable?
    int buffer_size = 2048;

    SDL_AudioSpec as;
	as.freq = Settings::Audio::FREQUENCIES[settings->frequency];
	as.format = AUDIO_S16SYS;
	as.channels = 1;
	as.samples = buffer_size;
	as.callback = &Audio::callback;
	as.userdata = (void*)this;

    SDL_CloseAudio();

	if(SDL_OpenAudio(&as, NULL) < 0)
	{
		printf("Unable to open audio:\n%s\n", SDL_GetError());
		exit(1);
	}

    if(desc) gain = desc->gain;
    sample_period = 1.0 / double(as.freq);

    SDL_PauseAudio(settings->mute);
}

void Audio::toggle_mute()
{
    SDL_PauseAudio(settings->mute);
}

Audio::~Audio()
{
	SDL_PauseAudio(1); // TODO: move?
	SDL_CloseAudio();
}

void Audio::callback(void* userdata, uint8_t* str, int len)
{
    Audio* audio = (Audio*)userdata;
    cirque<int16_t>& buffer = audio->audio_buffer;
   
    int16_t* stream = (int16_t*)str;
    int length = len >> 1;
    
    static int16_t last_val = 0;

    for(int i = 0; i < length; i++)
    {
        if(!buffer.empty())
        {
            last_val = stream[i] = buffer.front();
            buffer.pop_front();

            /*static int max_sample = 0;
            if(last_val > max_sample) 
            { printf("Max volume:%d\n", last_val); max_sample = last_val; }*/
        }
        else 
        {
            stream[i] = last_val;
        }
    }
}

