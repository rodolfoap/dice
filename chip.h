#ifndef CHIP_H
#define CHIP_H

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <array>
#include "cirque.h"

// TODO: remove
#include <cstdio>

class Circuit;
class ChipDesc;
class Chip;

enum ChipType { SIMPLE_CHIP = 0, BASIC_CHIP, CUSTOM_CHIP };

enum ChipState { ACTIVE = 0, PASSIVE, ASLEEP };

struct ChipLink
{
    Chip* chip;
    uint64_t mask;

    ChipLink(Chip* c = NULL, uint64_t m = 0) : chip(c), mask(m) {}
};

struct Cycle;

struct Event
{
    enum EventType { SUBCYCLE_MASK = 1 };
    
    uint64_t time;
    //union { int state; Cycle* sub_cycle; };
    union {
        //struct { uint32_t state; uint32_t type; };
        uint32_t state;
        Cycle* sub_cycle;
    };
    uint32_t type;

    //Event(uint64_t t = 0, int s = 0) : time(t), state(s) { }
    Event(uint64_t t = 0, uint32_t s = 0, int e = 0) : time(t), state(s), type(e) { }

    bool operator<(const Event& e) const { return time < e.time; }
};

template <unsigned N> class big_int
{
private:
    uint64_t bits[N];

public:
    big_int() : bits{{0}} { }

    class biterator
    {
    private:
        const uint64_t* bits;
        unsigned pos;
    
    public:  
        biterator(const uint64_t* b) : bits(b), pos(0) { }
        
        bool operator==(const biterator& b) const
        { return bits == b.bits && pos == b.pos; }

        bool operator!=(const biterator& b) const
        { return bits != b.bits || pos != b.pos; }

        bool has_bit()
        {
            if(pos == 64*N) return false;

            while(!(bits[pos >> 6] >> (pos & 63)))
            {
                pos = (pos & ~63) + 64;
                if(pos == 64*N) return false;
            }
            return true;
        }

        int next_bit() // DO NOT call if no bits are set
        {
            // not needed if has_bit always called first ??
            //while(!(bits[pos >> 6] >> (pos & 63))) pos = (pos & ~63) + 64;
            pos += __builtin_ctzll(bits[pos >> 6] >> (pos & 63)); // TODO: non-gcc version
            return pos;
        }
    };

    biterator begin() { return biterator(&bits[0]); }

    void operator|=(const big_int& a)
    { for(int i = 0; i < N; i++) bits[i] |= a.bits[i]; }

    void operator&=(const big_int& a)
    { for(int i = 0; i < N; i++) bits[i] &= a.bits[i]; }

    void operator^=(const big_int& a)
    { for(int i = 0; i < N; i++) bits[i] ^= a.bits[i]; }

};

struct SubcycleAllocator
{
    uint64_t* bits;
    int N;

    SubcycleAllocator(int SIZE) : N(SIZE >> 6)
    { 
        bits = new uint64_t[N]; 
        memset(bits, 0, sizeof(uint64_t)*N);
    }
    ~SubcycleAllocator() { delete[] bits; }

    int size() const { return N << 6; }
    bool full() const
    {
        uint64_t result = bits[0];
        for(int i = 1; i < N; i++) result &= bits[i];
        return ~result == 0;
    }
    void deallocate(const SubcycleAllocator& a)
    {
        for(int i = 0; i < N; i++) bits[i] &= ~a.bits[i];
    }
    int allocate() // Set a bit and return that bit also
    {
        for(int i = 0; i < N-1; i++)
            if(~bits[i]) 
            {
                int b = __builtin_ctzll(~bits[i]); // TODO: gcc alternative
                bits[i] |= (1ull << b);
                return 64*i + b;
            }

         int b = __builtin_ctzll(~bits[N-1]);
         bits[N-1] |= (1ull << b);
         return 64*(N-1) + b;
    }
    void operator |=(const SubcycleAllocator& a)
    {
        for(int i = 0; i < N; i++) bits[i] |= a.bits[i];
    }
    void clear()
    {
        for(int i = 0; i < N; i++) bits[i] = 0;
    }
    void set(int n)
    {
        bits[n >> 6] |= (1ull << (n & 63));
    }
};

struct Cycle
{
    Cycle* parent_cycle;
    //uint64_t allocated_sub_cycles; // or 128 bit?
    SubcycleAllocator allocated_sub_cycles;

    cirque<Event> output_events;
    //uint64_t output_event_type;
    cirque<Event>::index first_output_event;
    cirque<Event>::index current_output_event;
    
    uint64_t activation_time;
    uint64_t cycle_time;
    uint64_t cycle_duration;
    uint64_t end_time;
    uint64_t active_outputs;

    Cycle(int QUEUE_SIZE, int SUBCYCLE_SIZE) : parent_cycle(NULL), /*allocated_sub_cycles(0),*/ /*output_event_type(0),*/ activation_time(0), cycle_time(0), active_outputs(0), end_time(0), cycle_duration(0),
        output_events(QUEUE_SIZE), first_output_event(QUEUE_SIZE), current_output_event(QUEUE_SIZE), allocated_sub_cycles(SUBCYCLE_SIZE)
    { }
        
    uint64_t next_output_event_delay()
    {
        if(current_output_event == first_output_event)
            return output_events[first_output_event].time + cycle_time - output_events.back().time;
        else
            return output_events[current_output_event].time - output_events[current_output_event-1].time;
    }

    Cycle* next_output_cycle(uint64_t time) // time = time of next event?
    {
        // TODO: Make iterative?
        if(output_events[current_output_event].type) // Sub-cycle
        {
            Cycle* cycle = output_events[current_output_event].sub_cycle;
            
            current_output_event++;
            if(current_output_event == output_events.end()) current_output_event = first_output_event;
            
            //cycle->end_time = time + (cycle->end_time - cycle->activation_time);
            cycle->end_time = time + cycle->cycle_duration;
            if(cycle->end_time > end_time) // TODO: is this correct?
                cycle->end_time = end_time;
            cycle->activation_time = time;
            cycle->active_outputs = active_outputs;
            cycle->current_output_event = cycle->first_output_event;
            
            //printf("decend act:%lld end:%lld\n", cycle->activation_time, cycle->end_time);
                
            return cycle->next_output_cycle(time);
        }

        current_output_event++;
        if(current_output_event == output_events.end()) current_output_event = first_output_event;
        
        return this;
    }

    void print_output_events(int level = 0)
    {
        /*if(level == 0) printf("Print Output Events:%p\n", this);
        
        for(int i = 0; i < level*4; i++) printf(" ");
        printf("cycle:%p cycle_time:%lld act:%lld end:%lld\n", this, cycle_time, activation_time, end_time);

        for(cirque<Event>::index i = output_events.begin(); i != output_events.end(); i++)
        {
            for(int i = 0; i < level*4; i++) printf(" ");

            if(output_events[i].type) // Sub-cycle
            {
                printf("i:%d t:%lld, sub-cycle\n", int(i.getRawIndex()), output_events[i].time);
                output_events[i].sub_cycle->print_output_events(level+1);
            }
            else
                printf("i:%d t:%lld, state:%d\n", int(i.getRawIndex()), output_events[i].time, output_events[i].state);
        }*/
    }
};

typedef typename std::vector<ChipLink>::iterator LinkIterator;

#if 0
class debug_var
{
    uint64_t val;
public:
    debug_var(uint64_t v = 0) : val(v) { }
    void operator++(int) { val++; }
    void operator=(uint64_t x) { val = x; }
    operator uint64_t() const { return val; } 
};
#else
class debug_var
{
public:
    debug_var(uint64_t v = 0) { }
    void operator++(int) { }
    void operator=(uint64_t x) { }
    operator uint64_t() const { return 0; } 
};
#endif

/* 
class Chip
{
public:
    Circuit* circuit;
	
    uint32_t inputs;
    uint32_t output;

    uint64_t active_outputs;

    std::vector<ChipLink> output_links;
    std::vector<ChipLink> input_links;

    virtual void update_inputs(uint32_t mask) = 0;
	virtual void update_output() = 0;

    virtual void activate_inputs() = 0;
    virtual void deactivate_outputs() = 0;
    virtual void wake_up(uint32_t mask) = 0;
    uint64_t (Chip::*activation_check)(uint64_t min, uint64_t max);
    // void (Chip::*wake_up)(uint32_t mask); // ??????????

    static int next_bit(uint32_t x);
    static int next_bit64(uint64_t x);
};

class BasicChip : public Chip, public Cycle
{

}

class CustomChip : public Chip
{
    virtual void update_inputs(uint32_t mask) = 0;
}

*/

class Chip : public Cycle
{
public:
    Circuit* circuit;
	
    int inputs;
    int output;

	int event_mask;
	int prev_output_mask;

	uint64_t delay[2];

  	uint64_t pending_event;

    std::vector<ChipLink> output_links;
    std::vector<ChipLink> input_links;

	ChipType type;
	
	union {
		uint64_t lut_data;
		uint32_t* lut;
        void (*custom_update)(Chip* chip, int mask);
	};

    void* custom_data;
    bool optimization_disabled;
	
    // Begin new stuff
    ChipState state;

    cirque<Event> input_events;
    cirque<uint64_t> input_event_end_time;
    //cirque<uint64_t> next_check_time; 
    //std::vector<cirque<uint8_t, EVENT_QUEUE_SIZE/2>> input_event_table;
    std::vector<cirque<uint16_t>> input_event_table;
    cirque<uint16_t>::index first_input_table_pos;
    //std::unordered_map<int,cirque<uint8_t>> input_event_table;
    
    //std::vector<int> deactivation_table;
    //int deactive_inputs;

    //uint64_t input_event_type;
    cirque<Event>::index first_input_event;
    int first_input_mask;
    int active_inputs;

    uint64_t sleep_time;

    std::vector<Cycle*> sub_cycles;
    ~Chip()
    {
        for(Cycle* c : sub_cycles) if(c != NULL) delete c;
    }
    //std::vector<Cycle> sub_cycles;
    //Cycle sub_cycles[sizeof(allocated_sub_cycles) * 8]; // TODO: Make into vector and autosize to reduce memory usage
    Cycle* current_cycle;

    std::vector<Cycle*> activation_cycles;

    std::vector<uint64_t> last_input_event; // Bitmask of cirque indexes where input was updated? or just time? (just time for now)
    uint64_t last_output_event;

    bool visited;
    // End new stuff
	
	Chip(int QUEUE_SIZE, int SUBCYCLE_SIZE, Circuit* cir, const ChipDesc* desc, void* custom = NULL);
    void connect(Chip* chip, const ChipDesc* desc, uint8_t pin);
	void initialize();

    void update_inputs(uint32_t mask);
	void update_output();

    //virtual void custom_update(uint32_t mask) = 0;

    uint64_t activation_check(uint64_t min_time, uint64_t max_time);
    void activate_inputs();
    //void activate_outputs();
    void deactivate_outputs();
    void wake_up();
    int get_next_output(uint64_t time);

    static int next_bit(uint32_t x);
    static int next_bit64(uint64_t x);

    static void update_inputs_simple(Chip* chip, int mask);

    double analog_output;

    //For debugging
    debug_var total_event_count;
    debug_var activation_count;
    debug_var loop_count[8];
    debug_var max_cycle_length;
    debug_var max_subcycle_length;
    
    double analog_input(int n) { return input_links[n].chip->analog_output; }

    void print_input_events()
    {
        /*printf("Print Input Events:%p\n", this);

        for(cirque<Event>::index i = input_events.begin(); i != input_events.end(); i++)
        {
            if(input_events[i].type) // Sub-cycle
            {
                printf("i:%d t:%lld end:%lld state:%d, sub-cycle\n", int(i.getRawIndex()), input_events[i].time, input_event_end_time[i], input_events[i].state);
            }
            else
                printf("i:%d t:%lld, state:%d\n", int(i.getRawIndex()), input_events[i].time, input_events[i].state);
        }*/
    }
};

#endif
