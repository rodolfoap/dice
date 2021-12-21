#include <cstdio>
#include <cstdlib>

#include "chip.h"
#include "chip_desc.h"
#include "circuit.h"

#define DEBUG
#undef DEBUG

#ifdef DEBUG
//#define debug_printf(args...) printf(args)
//#define debug_printf(args...) do { printf(args); fflush(stdout); } while(0)
#define DEBUG_START_TIME (5268305775720)
#define debug_printf(args...) do { if(circuit->global_time > DEBUG_START_TIME ) printf(args); }while(0)
//#define debug_printf(args...) do { if(this && circuit && circuit->chips.size() > 187 && circuit->chips[187] && (this == circuit->chips[187] || this == circuit->chips[187]->input_links[0].chip) /*->input_links[0].chip*/) printf(args); }while(0)
//#define debug_printf(args...)
#else
#define debug_printf(args...)
//#define debug_printf(args...) printf(args)
#endif

extern CUSTOM_LOGIC( CLK_GATE );

void Chip::initialize()
{
	int new_out = output;

    // no clock edges during init
    inputs &= event_mask;

    debug_printf("init: %p in:%d out:%d lut:%llx\n", this, inputs, output, lut_data);

	switch(type) {
		case SIMPLE_CHIP: new_out = (lut_data >> inputs) & 1; break;
		case BASIC_CHIP:  new_out = (lut[inputs >> 5] >> (inputs & 0x1f)) & 1; break;
        case CUSTOM_CHIP: custom_update(this, 0); return;
	}

	if(new_out != output)
	{
		output = new_out;
        
        if(output)
            inputs |= prev_output_mask;
        else
            inputs &= ~prev_output_mask;

        for(LinkIterator it = output_links.begin(); it != output_links.end(); ++it)
            it->chip->inputs ^= it->mask;

        for(LinkIterator it = output_links.begin(); it != output_links.end(); ++it)
        {
            if(it->chip == this) continue;
            it->chip->initialize();
        }
	}
}

Chip::Chip(int QUEUE_SIZE, int SUBCYCLE_SIZE, Circuit* cir, const ChipDesc* desc, void* custom) : Cycle(QUEUE_SIZE, SUBCYCLE_SIZE),
	circuit(cir), custom_data(custom), inputs(0), output(0), event_mask(~0), prev_output_mask(0), /*deactive_inputs(0),*/ optimization_disabled(false),
    pending_event(0), state(PASSIVE), /*input_event_type(0),*/ sleep_time(0), current_cycle(this), last_output_event(0), visited(false), 
    total_event_count(0), activation_count(0), loop_count{{0}}, analog_output(0.0),
    input_events(QUEUE_SIZE), input_event_end_time(QUEUE_SIZE), first_input_event(QUEUE_SIZE), first_input_table_pos(QUEUE_SIZE),
    sub_cycles(SUBCYCLE_SIZE, nullptr)
{
    if(desc->logic_func == NULL && desc->custom_logic == NULL)
    {
        type = CUSTOM_CHIP;
        return;
    }

    int num_input_pins;
    // Count number of input pins
    for(num_input_pins = 0; desc->input_pins[num_input_pins] != 0; num_input_pins++);

    delay[0] = uint64_t(desc->output_delay[0] / Circuit::timescale);
	delay[1] = uint64_t(desc->output_delay[1] / Circuit::timescale);

    input_links.resize(num_input_pins);
    activation_cycles.resize(num_input_pins);
    last_input_event.resize(num_input_pins);

	// Don't set up LUT of custom chips
	if(desc->custom_logic)
	{
		type = CUSTOM_CHIP;
        custom_update = desc->custom_logic;
        return;
	}

    int lut_size = num_input_pins;

    // Setup event mask
    for(const uint8_t* p = &desc->event_pins[0]; *p != 0; p++) 
        event_mask &= ~(1 << lut_size++);

	if(desc->prev_output_pin)
	{
		prev_output_mask = (1 << lut_size);
        lut_size++;
	}

	if(lut_size <= 6)
	{
		type = SIMPLE_CHIP;
		lut_data = 0;
        //type = BASIC_CHIP;
		//lut = new uint32_t[2];
        //lut[0] = lut[1] = 0;
	}
	else
	{
		type = BASIC_CHIP;
		lut = new uint32_t[1 << (lut_size-5)];
		memset(lut, 0, sizeof(uint32_t)*(1 << (lut_size-5)));
	}

    //input_event_table.resize(1 << lut_size);

	// Fill the LUT
	for(int i = 0; i < (1 << lut_size); i++)
	{
		int pin[MAX_PINS+1] = { 0 };
		int prev_pin[MAX_PINS+1] = { 0 };
        int event_pin[MAX_PINS+1] = { 0 };
		int x = i;

		for(int j = 0; j < num_input_pins; j++)
		{
			pin[desc->input_pins[j]] = x & 1;
			x >>= 1;
		}
		
		for(int j = 0; desc->event_pins[j]; j++)
		{
			event_pin[desc->event_pins[j]] = x & 1;
			x >>= 1;
		}

		if(desc->prev_output_pin)
			prev_pin[desc->prev_output_pin] = x & 1;

		desc->logic_func(pin, prev_pin, event_pin, custom_data);
		
		if(type == SIMPLE_CHIP)
			lut_data |= ((pin[desc->output_pin] ? 1ull : 0) << i);
		else
            lut[i >> 5] |= ((pin[desc->output_pin] ? 1 : 0) << (i & 0x1f));
	}


    /*deactivation_table.resize((1 << lut_size));

    if(type == SIMPLE_CHIP) debug_printf("%p: %llx\n", this, lut_data);

    // Skip when lut_size > 14, algorithm is very slow. TODO: improve performance
    if(lut_size > 14) return;

    // Create deactivation table
    for(int i = 0; i < (1 << lut_size); i++)
    {
        int mask = 0;
        int num_mask_bits = 0;
        int out = (type == SIMPLE_CHIP) ? ((lut_data >> i) & 1) : (((lut[i >> 5]) >> (i & 0x1f)) & 1);
        
        for(int j = 0; j < num_input_pins; j++)
        {
            mask |= (1 << j);
            num_mask_bits++;

            int prev_input_mask = 0; // TODO: Rework algorithm to not use prev_input_mask
            for(int k = 0; desc->event_pins[k]; k++)
                if(desc->input_pins[j] == desc->event_pins[k])
                {
                    prev_input_mask = (1 << j) | (1 << (num_input_pins+k));
                    break;
                }

            if((1 << j) & prev_input_mask)
            {
                mask |= prev_input_mask;
                num_mask_bits++;
            }

            int val = 0;

            for(int k = 0; k < (1 << num_mask_bits); k++)
            {
                int in = (i & ~mask) | val;
                int new_out = (type == SIMPLE_CHIP) ? ((lut_data >> in) & 1) : (((lut[in >> 5]) >> (in & 0x1f)) & 1);

                if(new_out != out)
                {
                    mask &= ~(1 << j);
                    num_mask_bits--;

                    if((1 << j) & prev_input_mask)
                    {
                        mask &= ~prev_input_mask;
                        num_mask_bits--;
                    }

                    break;
                }

                val++;
                while(val & ~mask) val += (val & ~mask);
            }
        }

        deactivation_table[i] = mask & ((1 << num_input_pins) - 1);
        if(type == SIMPLE_CHIP) debug_printf("%d: %d\n", i, mask);
    }*/
}

extern CUSTOM_LOGIC( deoptimize );

void Chip::connect(Chip* chip, const ChipDesc* desc, uint8_t pin)
{
    //this is output, chip is input

    for(int i = 0; desc->input_pins[i]; i++)
        if(desc->input_pins[i] == pin)
        {
			// If output links already contains a link to this chip,
            // OR with its mask, otherwise add new link
            int x;
            for(x = 0; x < output_links.size(); x++)
                if(output_links[x].chip == chip) break;

            if(x != output_links.size())
            {
                output_links[x].mask |= (1 << i);
            }
            else
            {
                output_links.push_back(ChipLink(chip, 1 << i));
                active_outputs |= (1ull << x);
            }

            // Add event bit to mask if this is an event pin
            for(int j = 0; desc->event_pins[j]; j++)
                if(desc->event_pins[j] == pin)
                    output_links[x].mask |= (1 << (chip->input_links.size() + j));

            // Don't connect deoptimizer to input
            if(type != CUSTOM_CHIP || custom_update != deoptimize)
                chip->input_links[i] = ChipLink(this, 1ull << x);

            return;
        }
}

int Chip::get_next_output(uint64_t time)
{
#ifdef DEBUG
    debug_printf("get next output c:%p t:%lld\n", this, time);
    if(circuit->global_time > DEBUG_START_TIME )
        print_output_events();
#endif
    
    current_output_event = output_events.begin();
    Cycle* c = this;

    // TODO: Always assumes next event is second event in lowest subcycle? Is this correct???
    if(output_events.empty()) return output ^ 1;
    
    while(c->current_output_event != c->output_events.end() && c->output_events[c->current_output_event].time < time)
    {
        Event& e = c->output_events[c->current_output_event];

        if(e.type == 1 && e.time + c->cycle_duration > time)
        {
            c = e.sub_cycle;
            c->current_output_event = c->output_events.begin();
        }
        else c->current_output_event++;
    }

    if(c->current_output_event == c->output_events.end()) return output ^ 1;

    while(c->output_events[c->current_output_event].type == 1)
    {
        c = c->output_events[c->current_output_event].sub_cycle;
        c->current_output_event = c->output_events.begin();
    }

    return c->output_events[c->current_output_event].state;
}

void Chip::wake_up()
{
    debug_printf("waking up %p\n", this);

    state = ACTIVE;

    // Get chip up to date
    const uint64_t global_time = circuit->global_time;
    uint64_t time = (global_time - sleep_time) % cycle_time;
    uint64_t delay = current_cycle->next_output_event_delay();
    last_output_event = global_time - time; // TODO: Is this correct?

    if(current_cycle != this) // current cycle is not root cycle, adjust activation and end time
    {
        uint64_t sleep_cycles = (global_time - sleep_time) / cycle_time;
        current_cycle->end_time += cycle_time*sleep_cycles; // TODO: is this correct?
        current_cycle->activation_time += cycle_time*sleep_cycles; // TODO: is this correct?
    }
    
    uint64_t act_out = current_cycle->active_outputs;

    while(global_time - time + delay >= current_cycle->end_time) // TODO: > or >= ?
    {
        if(current_cycle->parent_cycle->next_output_event_delay() + time > (global_time - current_cycle->activation_time)) // TODO: Hack, why is this needed?
            delay = current_cycle->parent_cycle->next_output_event_delay() - (global_time - current_cycle->activation_time) + time;
        current_cycle = current_cycle->parent_cycle;
    }

    while(time > delay)
    {
        time -= delay;

        //last_output_event = global_time - time; // TODO: Is this correct?
        output ^= 1;
        inputs ^= prev_output_mask;

        current_cycle = current_cycle->next_output_cycle(global_time - time);

        delay = current_cycle->next_output_event_delay();

        while(global_time - time + delay >= current_cycle->end_time) // TODO: > or >= ?
        {
            if(current_cycle->parent_cycle->next_output_event_delay() + time > (global_time - current_cycle->activation_time)) // TODO: Hack, why is this needed?
                delay = current_cycle->parent_cycle->next_output_event_delay() - (global_time - current_cycle->activation_time) + time;
            current_cycle = current_cycle->parent_cycle;
        }
    }

    last_output_event = global_time; // TODO: improve
    pending_event = circuit->queue_push(this, delay - time);
    current_cycle = current_cycle->next_output_cycle(global_time - time + delay); // TODO: is parameter correct?
    
    debug_printf("chip:%p curr:%p a1:%llx a2:%llx\n", this, current_cycle, act_out, current_cycle->active_outputs);

    for(uint64_t m = ~act_out & current_cycle->active_outputs; m;)
    {
        int i = Chip::next_bit64(m);
        output_links[i].chip->deactivate_outputs();
        m &= ~(1ull << i);
    }
}




#if 1
void Chip::update_inputs(uint32_t mask)
{
    total_event_count++;
    
    const uint64_t global_time = circuit->global_time;

    //debug_printf("UPDATE INPUTS: chip: %x mask: %d inp: %d t: %lld\n", this, mask, inputs, circuit->global_time);

    if(type == CUSTOM_CHIP)
	{
		custom_update(this, mask);
		return;
	}
    else if(optimization_disabled)
    {
        update_inputs_simple(this, mask);
        return;
    }

    if(state == ASLEEP)
    {
        wake_up();
    }

    /*for(int i = 0; mask >> i; i++)
    {
        i += Chip::next_bit(mask >> i);
        last_input_event[i] = global_time;
    }*/

    last_input_event[Chip::next_bit(mask)] = global_time;

    //Chip* input_chip = input_links[next_bit(mask)].chip;

    if(state == ACTIVE)
    {
        deactivate_outputs();
        input_event_table[input_events.back().state].pop_back();
        input_events.pop_back(); // TODO: possibly remove depending on deactivate_outputs!!!
    }

    // Make sure input chip is not ahead of us
    Chip* input_chip = input_links[next_bit(mask)].chip;

    //if(input_chip->state == PASSIVE)
    if(!input_chip->input_events.empty())
    {
        uint64_t time = input_chip->input_events.front().time;
        while(input_events.size() > 1 && input_events[input_events.begin() + 1].time < time)
        {
            input_event_table[input_events.front().state].pop_front();
            input_events.pop_front();
        }

        time = input_events.front().time;
        while(!output_events.empty() && output_events.front().time < time)
        {
            if(output_events.front().type)
                allocated_sub_cycles.deallocate(output_events.front().sub_cycle->allocated_sub_cycles);

            output_events.pop_front();
        }
    }

    /*uint64_t first_input_time = input_events.front().time;
    while(!output_events.empty() && output_events.front().time < first_input_time)
    {
        if(output_events.front().type)
            allocated_sub_cycles.deallocate(output_events.front().sub_cycle->allocated_sub_cycles);

        output_events.pop_front();
    }*/
    

    // Reconnect to deactive inputs
#if 0
    inputs &= ~deactive_inputs;
    //uint64_t last_input_time = input_events.size() ? input_events.back().time : 0;
    //uint64_t last_deactive_input_time = global_time;
    for(int i = 0; deactive_inputs >> i; i++)
    {
        i += Chip::next_bit(deactive_inputs >> i);
        
        Chip* c = input_links[i].chip;
        
        if(c->state == ASLEEP) c->wake_up();
        c->active_outputs |= input_links[i].mask;
        
        inputs |= c->output << i;
        last_input_event[i] = c->last_output_event;

        debug_printf("reconnecting %x to %p mask %d\n", this, c, input_links[i].mask);

        //if(c->last_output_event > last_input_time && c->last_output_event < last_deactive_input_time)
        //    last_deactive_input_time = c->last_output_event;
    }
    /*if(last_deactive_input_time != global_time && input_events.size())
    {
        input_events.back().type = 1;
        input_event_end_time[input_events.end() - 1] = last_deactive_input_time;
    }*/
#endif

    inputs ^= mask;

    int new_out;

	switch(type) {
		case SIMPLE_CHIP: new_out = (lut_data >> inputs) & 1; break;
	    default:          new_out = (lut[inputs >> 5] >> (inputs & 0x1f)) & 1; break; 
	}

    if(new_out != output && pending_event == 0)
    {
        pending_event = circuit->queue_push(this, delay[output]);
        inputs ^= prev_output_mask;

        if(output_events.full())
        {
            if(output_events.front().type)
                allocated_sub_cycles.deallocate(output_events.front().sub_cycle->allocated_sub_cycles);
                //allocated_sub_cycles &= ~output_events.front().sub_cycle->allocated_sub_cycles;
        }

        output_events.push_back(Event(global_time + delay[output], new_out));

        debug_printf("new_event: t:%lld %x %lld\n", global_time, this, output_events.back().time);
    }
    else if(pending_event && new_out == output)
    {
        /*if(pending_event == global_time) // Pending event for current time. Run event now?
        {
            update_output();

            // Now push new event
            pending_event = circuit->queue_push(this, delay[output]);
            inputs ^= prev_output_mask;

            if(output_events.full())
            {
                if(output_events.front().type)
                    allocated_sub_cycles.deallocate(output_events.front().sub_cycle->allocated_sub_cycles);
                    //allocated_sub_cycles &= ~output_events.front().sub_cycle->allocated_sub_cycles;
            }

            output_events.push_back(Event(global_time + delay[output], new_out));

            debug_printf("new_event: t:%lld %x %lld\n", global_time, this, output_events.back().time);
        }       
        else*/
        {
            debug_printf("remove_event: t:%lld %x %lld\n", global_time,  this, output_events.back().time);
        
            pending_event = 0;
            inputs ^= prev_output_mask;
        
            // TODO: Check allocated_sub_cycles of last event? (Should not have any though)

            if(!output_events.empty()) // TODO: !empty check still needed?
                output_events.pop_back();
        }
    }

    inputs &= event_mask;

    if(inputs >= input_event_table.size()) input_event_table.resize(inputs+1, cirque<uint16_t>(first_output_event.getQueueSize()));

    first_input_table_pos = input_event_table[inputs].begin();
    first_input_mask = active_inputs = (1 << input_links.size()) - 1;

    // TODO: use empty check? sort of not needed
    if(!input_events.empty() && activation_check(input_events.front().time, global_time) < global_time)
    {
        activate_inputs();
        return;
    }

    if(!input_events.empty() && input_events.back().time == global_time)
    {
        input_event_table[input_events.back().state].pop_back();
        input_events.pop_back();
    }
    else
    if(input_events.full())
        input_event_table[input_events.front().state].pop_front();

    //input_event_type &= ~(1ull << input_events.end());
    input_event_table[inputs].push_back(input_events.end().getRawIndex());
    input_events.push_back(Event(global_time, inputs));
    
    /*deactive_inputs = deactivation_table[inputs];
    
    for(int i = 0; deactive_inputs >> i; i++)
    {
        i += Chip::next_bit(deactive_inputs >> i);

        Chip* c = input_links[i].chip;
        // Handle multiple connections to a chip - TODO: do this better
        int m = c->output_links[Chip::next_bit(input_links[i].mask)].mask;
        if((deactive_inputs & m) == m)
        {
            c->active_outputs &= ~input_links[i].mask;
            debug_printf("disconnecting %x from %p mask %d\n", this, c, input_links[i].mask);
        }
    }*/
}

#else

void Chip::update_inputs(uint32_t mask)
{
    total_event_count++;

    if(type == CUSTOM_CHIP)
	{
		custom_update(this, mask);
		return;
	}

    update_inputs_simple(this, mask);
    return;
}

#endif

void Chip::update_inputs_simple(Chip* chip, int mask)
{
    //debug_printf("UPDATE INPUTS: chip: %x mask: %d inp: %d t: %lld\n", chip, chip->mask, chip->inputs, chip->circuit->global_time);

	chip->inputs ^= mask;

	int new_out;
	
	switch(chip->type) {
		case SIMPLE_CHIP: new_out = (chip->lut_data >> chip->inputs) & 1; break;
		case BASIC_CHIP:  new_out = (chip->lut[chip->inputs >> 5] >> (chip->inputs & 0x1f)) & 1; break; 
	}

    if(new_out != chip->output && chip->pending_event == 0)
    {
        chip->pending_event = chip->circuit->queue_push(chip, chip->delay[chip->output]);
        chip->inputs ^= chip->prev_output_mask;

        //debug_printf("new_event: %p %lld\n", chip, chip->pending_event);
    }
    else if(chip->pending_event && new_out == chip->output)
	{
		//debug_printf("remove_event: %p %lld\n", chip, chip->pending_event);
        
        chip->pending_event = 0;
        chip->inputs ^= chip->prev_output_mask;
    }

    chip->inputs &= chip->event_mask;
}


void Chip::update_output()
{
    debug_printf("update output: %p t:%lld o:%d\n", this, circuit->global_time, output ^ 1);
    
    uint64_t global_time = circuit->global_time;

    if(state == ASLEEP)
    {
        state = ACTIVE;
        
        Cycle* parent = current_cycle->parent_cycle;
        
        debug_printf("was asleep1: sleep:%lld\n", sleep_time);

        //print_output_events();

        uint64_t act_out = current_cycle->active_outputs;
        
        // TODO: Use simplified & faster version of wake_up()
#if 0
        wake_up();
#else
        //debug_printf("waking up %x\n", this);
        
        uint64_t delay = 0;

        while(global_time + delay >= current_cycle->end_time) // TODO: > or >= ?
        {
            if(current_cycle->parent_cycle->next_output_event_delay() > (global_time - current_cycle->activation_time)) // TODO: Hack, why is this needed?
                delay = current_cycle->parent_cycle->next_output_event_delay() - (global_time - current_cycle->activation_time);
            
            current_cycle = current_cycle->parent_cycle;
        }

        current_cycle = current_cycle->next_output_cycle(global_time + delay); // TODO: is parameter correct?
        pending_event = circuit->queue_push(this, delay);
        
        if(current_cycle->output_events[current_cycle->current_output_event].type)
        {
            Cycle* c = current_cycle->output_events[current_cycle->current_output_event].sub_cycle;
            while(c->output_events.front().type)
                c = c->output_events.front().sub_cycle;

            output = c->output_events.front().state; // ^ 1; // TODO: is this correct?
        }
        else
            output = current_cycle->output_events[current_cycle->current_output_event].state;//^ 1; // TODO: is this correct?
        
        last_output_event = global_time; // TODO: FIX!
#endif
        for(uint64_t m = ~act_out & current_cycle->active_outputs; m;)
        {
            int i = Chip::next_bit64(m);
            output_links[i].chip->deactivate_outputs();
            m &= ~(1ull << i);
        }

        debug_printf("Was asleep, out:%d, pend:%lld\n", output, pending_event);

        return;
    }

    last_output_event = global_time;

    // Don't just iterate through this the normal way -
    // active_outputs can be modified through update_inputs().
    for(uint64_t mask = ~0ull; current_cycle->active_outputs & mask;)
    {
        int i = Chip::next_bit64(current_cycle->active_outputs & mask);
        ChipLink* link = &output_links[i];
        link->chip->update_inputs(link->mask);
        mask &= ~(1ull << i);
    }

	output ^= 1;
	pending_event = 0;

    if(state == ACTIVE)
    {
        if(current_cycle->active_outputs == 0)// && current_cycle == this)
        {
            debug_printf("going to sleep:%x\n", this);
            state = ASLEEP;
            sleep_time = global_time;

            //while(current_cycle->parent_cycle && current_cycle->parent_cycle->active_outputs == 0) // TODO: make this work?
            //    current_cycle = current_cycle->parent_cycle;
            
            // If current cycle isn't root cycle, schedule an event to wake up
            //if(current_cycle != this)
            //    pending_event = circuit->queue_push(this, current_cycle->end_time - global_time);

                                        //TODO: this part increases total event count, why???
            if(current_cycle != this && (current_cycle->parent_cycle != this || active_outputs))
                pending_event = circuit->queue_push(this, current_cycle->end_time - global_time);

            return;
        }

        uint64_t delay = current_cycle->next_output_event_delay();
    
        while(global_time + delay >= current_cycle->end_time) // TODO: > or >= ?
        {
            if(current_cycle->parent_cycle->next_output_event_delay() > (global_time - current_cycle->activation_time)) // TODO: Hack, why is this needed?
                delay = current_cycle->parent_cycle->next_output_event_delay() - (global_time - current_cycle->activation_time);
            
            // Deactivate all outputs only disconnected from sub-cycle
            for(uint64_t m = ~current_cycle->active_outputs & current_cycle->parent_cycle->active_outputs; m;)
            {
                int i = Chip::next_bit64(m);
                output_links[i].chip->deactivate_outputs();
                m &= ~(1ull << i);
            }
            
            // May have been deactivated above?
            if(state != ACTIVE) 
            {
                debug_printf("DEACT %p\n", this);
                return;
            }

            current_cycle = current_cycle->parent_cycle;
        }

        current_cycle = current_cycle->next_output_cycle(global_time + delay); // TODO: is parameter correct?
        pending_event = circuit->queue_push(this, delay);
	}
}




uint64_t Chip::activation_check(uint64_t min_time, uint64_t max_time)
{
#ifdef DEBUG
    debug_printf("activation check new: %p in:%d min:%lld max:%lld\n", this, inputs, min_time, max_time);
    print_input_events();
#endif

    visited = true;      
    //int active_inputs = (1 << input_links.size()) - 1;

    const uint64_t global_time = circuit->global_time;
    
    int curr_loop_count = 0;

    const cirque<uint16_t>& input_event_list = input_event_table[inputs];

#if 0
    // Update last input event from deactive inputs
    //inputs &= ~deactive_inputs;
    for(int i = 0; deactive_inputs >> i; i++)
    {
        i += Chip::next_bit(deactive_inputs >> i);
        
        Chip* c = input_links[i].chip;
        
        if(c->state == ASLEEP) c->wake_up();
        //c->active_outputs |= input_links[i].mask;

        //inputs |= c->output << i;
        last_input_event[i] = c->last_output_event;

        //debug_printf("reconnecting %x to %p mask %d\n", this, c, input_links[i].mask);
    }
#endif

    //for(cirque<uint64_t>::index i = input_events.begin() + (input_events.size() & 1); i != input_events.end(); i += 2)//++i)
    //for(cirque<Event, 64>::index i = input_events.begin(); i != input_events.end(); i++)
    //for(cirque<Event>::index i = first_input_table_pos; i != input_events.end(); i++, first_input_table_pos++)
    //for(cirque<uint8_t>::index x = input_event_list.begin(); x != input_event_list.end(); x++)
    for(cirque<uint16_t>::index x = first_input_table_pos; x != input_event_list.end(); x++, first_input_table_pos++)
    {
        //if(curr_loop_count++ >= 64) break;
        
        loop_count[0]++;

        //cirque<Event>::index i = input_event_list[x];
        cirque<Event>::index i = x.makeIndex(input_event_list[x]);

        //if(global_time < next_check_time[i]) continue;

        //if(input_events[i].type == 2) continue;

        uint64_t min, max;

        uint64_t start_time, end_time;

        if(input_events[i].type) // sub cycle
        {
            //debug_printf("subcycle, in:%d, state:%d t:%lld end:%lld\n", inputs, input_events[i].state, input_events[i].time, input_event_end_time[i]);
            start_time = input_events[i].time;
            end_time = input_event_end_time[i];
            //end_time = (cirque<Event>::index(i+1) != input_events.end()) ? input_events[i+1].time : global_time;
        }
        else
        {
            //debug_printf("cycle, in:%d, state:%d t:%lld\n", inputs, input_events[i].state, input_events[i].time);

            start_time = input_events[i].time;
            end_time = (cirque<Event>::index(i+1) != input_events.end()) ? input_events[i+1].time : global_time;
        }

        min = min_time > start_time ? min_time : start_time;
        max = max_time < end_time ? max_time : end_time;

        /*if(input_events[i].type) // sub cycle
        //if(input_event_type & (1ull << i)) // sub cycle
        {
            debug_printf("subcycle, in:%d, state:%d t:%lld end:%lld\n", inputs, input_events[i].state, input_events[i].time, input_event_end_time[i]);
            
            //check only first state in cycle (for now?)
            min = min_time > input_events[i].time ? min_time : input_events[i].time;
            max = max_time < input_event_end_time[i] ? max_time : input_event_end_time[i];
        }
        else
        {
            debug_printf("cycle, in:%d, state:%d t:%lld\n", inputs, input_events[i].state, input_events[i].time);

            min = min_time > input_events[i].time ? min_time : input_events[i].time;
            if(i+1 != input_events.end())
                max = max_time < input_events[i+1].time ? max_time : input_events[i+1].time;
            else
                max = max_time;
        }*/

        //debug_printf("min:%lld max:%lld\n", min, max);

        if(min >= max_time) 
        {
            max_time = min;
            break;
        }

        if(min >= max) continue;

        bool result = true;

        activation_time = min;

        loop_count[1]++;

        //for(int j = 0; j < input_links.size(); j++)
        for(int j = 0; active_inputs >> j; j++)
        {
            loop_count[2]++;
            
            j += Chip::next_bit(active_inputs >> j);
            Chip* chip = input_links[j].chip;
            activation_cycles[j] = chip;

            if(last_input_event[j] <= min)
            {
                active_inputs &= ~(1 << j);
                //if(input_links[j].mask == 0) active_inputs &= ~(1 << j);
                //else active_inputs &= ~input_links[j].chip->output_links[Chip::next_bit(input_links[j].mask)].mask;
            }
            else if(chip->state == ACTIVE)
            {
                loop_count[3]++;
                
                // TODO: Keep a range during this check, instead of setting max = min ???????
                // TODO: Iterate multiple times, if cycle is significantly less than (min,max) range?
                // Actually, don't need to do this? doesn't seem to ever be possible?
                /*if(chip->current_cycle == chip)
                {
                    if(chip->activation_time > min)
                        min = chip->activation_time;
                    
                    activation_time = min_time = min > last_input_event[j] ? last_input_event[j] : min;

                    if(min >= max)
                    {
                        debug_printf("act_check fail1: active chip %p, act:%lld, cyc:%lld\n", chip, chip->activation_time, chip->cycle_time);
                        result = false;
                        break;
                    }
                    
                    activation_time = min + ((global_time - min) % chip->cycle_time);
                    min_time = activation_time > last_input_event[j] ? last_input_event[j] : activation_time;
                    
                    if(activation_time >= max)
                    {
                        debug_printf("act_check fail2: active chip %p, act:%lld, cyc:%lld\n", chip, chip->activation_time, chip->cycle_time);

                        result = false;
                        break;
                    }
                    
                    min = activation_time;
                    max = min + 1;
                    debug_printf("act_check pass1: active chip %p, act:%lld, cyc:%lld\n", chip, chip->activation_time, chip->cycle_time);
                }
                else */ // TODO: Skipping the above increases check counts, why?
                {    
                    result = false;
                    uint64_t min_save = min;

                    uint64_t act_min = ~0ull;

                    for(Cycle* c = chip->current_cycle; c != NULL; c = c->parent_cycle)
                    {
                        loop_count[7]++;
                        
                        min = min_save;

                        if(c->activation_time > min)
                            min = c->activation_time;
                        
                        if(min < act_min) act_min = min;

                        if(min >= max)
                        {
                            debug_printf("act_check fail3: active chip %p, cyc:%p, act:%lld, cyc:%lld\n", chip, c, c->activation_time, c->cycle_time);
                            loop_count[6]++;
                            continue;
                        }

                        activation_time = min + ((global_time - min) % c->cycle_time);
                        
                        if(activation_time < act_min) act_min = activation_time;

                        if(activation_time >= max)
                        {
                            debug_printf("act_check fail4: active chip %p, act:%lld, cyc:%lld\n", c, c->activation_time, c->cycle_time);
                            continue;
                        }
                        
                        min = activation_time;
                        max = min + 1;
                        result = true;
                        activation_cycles[j] = c;
                        break;
                    }

                    min_time = act_min > last_input_event[j] ? last_input_event[j] : act_min;

                    if(result == false) break;

                    debug_printf("act_check pass2: active chip %p, act:%lld, cyc:%lld\n", chip, chip->activation_time, chip->cycle_time);    
                }

            }
            else if(chip->state == PASSIVE && (chip->type == CUSTOM_CHIP || chip->optimization_disabled))
            {
                min_time = last_input_event[j];
                result = false;
                break;
            }
            else if(chip->state == PASSIVE && chip->visited == false) // TODO: Passive checks needed?
            {
                loop_count[4]++;

                if((first_input_mask >> j) & 1)
                {
                    //chip->first_input_table_pos = chip->input_events.begin();
                    chip->first_input_table_pos = chip->input_event_table[chip->inputs].begin();
                    //chip->first_input_table_pos = chip->input_events.binary_search(chip->input_events.begin(), chip->input_events.end(), min);
                    chip->first_input_mask = chip->active_inputs = (1 << chip->input_links.size()) - 1;
                    first_input_mask &= ~(1 << j);
                }
                
                activation_time = chip->activation_check(min, max);
                
                min_time = activation_time > last_input_event[j] ? last_input_event[j] : activation_time;

                if(activation_time >= max)
                {
                    result = false;
                    break;
                }
                else
                {
                    min = activation_time;
                    max = min + 1;
                }
            }
        }

        if(result == false) 
        {
            // NOTE: this icreases total event count, skips some checks
            //i = input_events.binary_search(i, input_events.end(), min_time);
            //if(i == input_events.end()) { max_time = global_time; break; }
            
            //cirque<Event>::index i2 = input_events.binary_search(i, input_events.end(), min_time);
            //if(i2 != i) i2 = i2 - 1;
            continue;
        }

        loop_count[5]++;

        debug_printf("chip %p: input check passed\n", this);

        //print_output_events();

        // Don't allow cycles less than total delay? For 9602 etc.
        // TODO: Find a better way to handle these situations?
        debug_printf("chip:%p cyc: %lld del:%lld %lld\n", this, global_time - activation_time, delay[0], delay[1]);
        if(global_time - activation_time < delay[0] + delay[1]) { debug_printf("CYC_LESS\n"); continue; }

#if 0
        // Inputs match, perform output check
        current_output_event = output_events.begin();

        // Scan output events for all events that occured during this time
        // TODO: Currently does not allow activation in the middle of a sub-cycle,
        // only on the very first event. Update this?
        while(current_output_event != output_events.end() && output_events[current_output_event].time < activation_time)
        {    current_output_event++; /*loop_count[6]++;*/ }

        if(current_output_event != output_events.end())
        {
            // Next event should not equal current output
            int next_out;

            if(output_events[current_output_event].type) 
                next_out = output_events[current_output_event].sub_cycle->output_events.front().state;
            else
                next_out = output_events[current_output_event].state;
            
            debug_printf("output check: curr:%d next:%d out:%d pend:%lld time:%lld time2:%lld\n", int(current_output_event.getRawIndex()), next_out, output, pending_event, output_events[current_output_event].time, output_events[current_output_event].time + global_time - activation_time);

            if(next_out == output) continue;
        
            // Check that pending event matches first event in cycle
            if(pending_event && pending_event != output_events[current_output_event].time + global_time - activation_time) continue;

            // TODO: Check cases where propagation delays have been modified, i.e., 555 mono timers?
            //if(cycle_time >= output_events[first_output_event] + global_time - activation_time)
            //    continue;
        }
#endif
        if(get_next_output(activation_time) == output) 
        {
            debug_printf("act_check fail5: chip %p, act:%lld out:%d\n", this, activation_time, output);
            continue;
        }

         // Check that pending event matches first event in cycle
         if(pending_event && pending_event != current_cycle->output_events[current_cycle->current_output_event].time + global_time - activation_time) continue;

         // TODO: Check cases where propagation delays have been modified, i.e., 555 mono timers?
         //if(cycle_time >= output_events[first_output_event] + global_time - activation_time)
         //    continue;


        debug_printf("activation check new: %p result:%d\n", this, (int)true);

        // Activation check passed
        first_input_event = i;
        visited = false;
        return activation_time;
    }

    //debug_printf("activation check new: %p result:%d %lld\n", this, (int)false, max_time);
    
    visited = false;
    //if(input_events.size() && first_input_event == input_events.end()) return global_time;
    //if(input_events.size() && input_events[first_input_event].time > max_time) return input_events[first_input_event].time;
    //if(input_event_list.size() && first_input_event == input_event_list.end()) return global_time;
    //if(input_event_list.size() && input_events[input_event_list[first_input_event]].time > max_time) return input_events[input_event_list[first_input_event]].time;
    return max_time;
}




void Chip::activate_inputs()
{
    //visited = true;

    activation_count++;

    debug_printf("activating: %p t:%lld act:%lld\n", this, circuit->global_time, activation_time);

    state = ACTIVE;

    current_cycle = this;
    
    cycle_time = circuit->global_time - activation_time;
    end_time = ~0ull;

    // Reconnect to deactive inputs
#if 0
    inputs &= ~deactive_inputs;
    for(int i = 0; deactive_inputs >> i; i++)
    {
        i += Chip::next_bit(deactive_inputs >> i);
        
        Chip* c = input_links[i].chip;
        
        if(c->state == ASLEEP) c->wake_up();
        c->active_outputs |= input_links[i].mask;

        inputs |= c->output << i;
        last_input_event[i] = c->last_output_event;

        debug_printf("reconnecting %x to %p mask %d\n", this, c, input_links[i].mask);
    }
    deactive_inputs = 0;
#endif
    
#ifdef DEBUG
    cirque<Event>::index diff = input_events.end() - first_input_event.getRawIndex();
    if(diff.getRawIndex() > max_cycle_length)
        max_cycle_length = diff.getRawIndex();
#endif

    for(int i = 0; i < input_links.size(); i++)
    {
        if(last_input_event[i] > activation_time) // TODO: Store active_inputs in chip to avoid this check?
        {
            Chip* chip = input_links[i].chip;

            for(Cycle* c = chip->current_cycle; c != activation_cycles[i]->parent_cycle; c = c->parent_cycle)
            {
                debug_printf("disconnecting %p from chip:%p cyc:%p mask %d\n", this, chip, c, input_links[i].mask);
                c->active_outputs &= ~input_links[i].mask;
            }

            if(chip->state == PASSIVE)
                chip->activate_inputs();
        }
    }

#if 0
    // Scan output events for all events that occured during this time
    // TODO: Does this need to be done? Also done in activation_check
    current_output_event = output_events.begin();

    // TODO: Currently does not allow activation in the middle of a sub-cycle,
    // only on the very first event. Update this?
    while(current_output_event != output_events.end() && output_events[current_output_event].time < activation_time)
    {
        current_output_event++;
    }
#endif
    first_output_event = current_output_event;

    if(first_output_event != output_events.end())
    {
        if(pending_event)
        {
            output_events.pop_back(); // Skip most recent event
            //pending_event = circuit->queue_push(this, output_events[first_output_event].time - activation_time); // TODO: is this correct?
            debug_printf("activation2 %p t:%lld %lld %lld %lld\n", this, circuit->global_time, pending_event - circuit->global_time, delay[0], delay[1]);
        }
        else
        {
            pending_event = circuit->queue_push(this, output_events[current_output_event].time - activation_time);
            debug_printf("push %lld\n", output_events[current_output_event].time - activation_time);
        }

        current_cycle = next_output_cycle(cycle_time + output_events[current_output_event].time);

#ifdef DEBUG
        if(circuit->global_time > DEBUG_START_TIME )
            print_output_events();
#endif
    }

    debug_printf("first:%d end:%d\n", (int)first_output_event.getRawIndex(), (int)output_events.end().getRawIndex());
}



void Chip::deactivate_outputs()
{
    if(state == ASLEEP)
    {
        wake_up();
    }

    debug_printf("deactivating: %p t:%lld pend:%lld in:%d out:%d\n", this, pending_event, circuit->global_time, inputs, output);

    // TODO: Update all this for new algorithm????
    if((pending_event >= circuit->global_time) && ((pending_event - circuit->global_time) <= delay[output]))
    {
        if((pending_event - circuit->global_time) == delay[output])
        {
            debug_printf("super awkward deactivation t:%lld %lld %p\n",  circuit->global_time, pending_event - circuit->global_time, this);

            // Determine if deactivation is due to an input to this chip, which will still have a pending event,
            // in which case no event should be scheduled by this chip. Otherwise it is due to a chip further down stream,
            // in which case the event should stay scheduled. TODO: is this correct?
            bool is_input = false;
            
            for(int i = 0; i < input_links.size(); i++)
            {
                if(input_links[i].chip->pending_event == circuit->global_time)
                {
                    is_input = true;
                    debug_printf("is input = TRUE, c:%p\n", input_links[i].chip);
                    break;
                }
            }

            if(is_input)
            {
                pending_event = 0;
                inputs = output ? prev_output_mask : 0;
            }
            else
            {
                inputs = output ? 0 : prev_output_mask;
            }
        }
        else
        {            
            debug_printf("awkward deactivation t:%lld %lld %p %d\n",  circuit->global_time, pending_event - circuit->global_time, this, delay[output]);
            inputs = output ? 0 : prev_output_mask;
        }
    }
    else
    {
        pending_event = 0;
        inputs = output ? prev_output_mask : 0;
    }

    state = PASSIVE;

    for(int i = 0; i < input_links.size(); i++)
    {
        for(Cycle* c = input_links[i].chip->current_cycle; c; c = c->parent_cycle)
            c->active_outputs |= input_links[i].mask;
    }
    for(int i = 0; i < input_links.size(); i++)
    {
        if(input_links[i].chip->state == ASLEEP)
            input_links[i].chip->wake_up();

        inputs |= input_links[i].chip->output << i;
    }
    /*for(int i = 0; i < input_links.size(); i++)
    {
        //for(Cycle* c = input_links[i].chip->current_cycle; c != activation_cycles[i]->parent_cycle; c = c->parent_cycle)
        //    c->active_outputs |= input_links[i].mask;
        
        for(Cycle* c = input_links[i].chip->current_cycle; c; c = c->parent_cycle)
        {
            c->active_outputs |= input_links[i].mask;
            debug_printf("cyc:%p act:%lld m:%lld\n", c, c->active_outputs, input_links[i].mask);
        }

        if(input_links[i].chip->state == ASLEEP)
            input_links[i].chip->wake_up();

        inputs |= input_links[i].chip->output << i;
    }*/
    inputs &= event_mask;

    debug_printf("inputs_after:%d\n", inputs);

    if(type == CUSTOM_CHIP || optimization_disabled || input_events.empty())
    {
        // TODO: use iterator?
        for(int i = 0; i < output_links.size(); i++)
        {
            //if((output_links[i].chip->active_inputs & output_links[i].mask) && output_links[i].chip->event_count)
            if(!(current_cycle->active_outputs & (1ull << i)) && output_links[i].chip->state != PASSIVE) // output_links[i].chip->state != PASSIVE)
            //if(output_links[i].chip->state != PASSIVE)
                output_links[i].chip->deactivate_outputs();
        }       
       
        return;
    }

#ifdef DEBUG
    debug_printf("BEFORE:\n");
    if(circuit->global_time > DEBUG_START_TIME )
    {
        print_output_events();
        print_input_events();
    }
#endif

#ifdef DEBUG
    cirque<Event>::index diff = input_events.end() - first_input_event.getRawIndex();
    if(diff.getRawIndex() > max_cycle_length)
        max_cycle_length = diff.getRawIndex();
#endif

    if(!input_events[first_input_event].type)
    //if(!(input_event_type & (1ull << first_input_event)))
    {
        //input_event_type |= (1ull << first_input_event);
        input_events[first_input_event].type = 1;
        input_event_end_time[first_input_event] = input_events[first_input_event+1].time;
    }

    //input_events.erase_back(first_input_event);
    for(cirque<Event>::index i = input_events.end() - 1; i != first_input_event; i--)
    {
        input_event_table[input_events.back().state].pop_back();
        input_events.pop_back();
    }

    // Save 1 full cycle of input events?
   /* cirque<Event>::index idx = input_events.end() - 1;
    input_event_type |= (1ull << idx);
    input_event_end_time[idx] = activation_time + cycle_time;*/

    //input_event_type &= ~(1ull << input_events.end());
    
    if(inputs >= input_event_table.size()) input_event_table.resize(inputs+1, cirque<uint16_t>(first_output_event.getQueueSize()));

    input_event_table[inputs].push_back(input_events.end().getRawIndex());
    input_events.push_back(Event(circuit->global_time, inputs));

    //while(~allocated_sub_cycles == 0)
    while(allocated_sub_cycles.full())
    {
        //debug_printf("allocated:%lld\n", allocated_sub_cycles);
        
        //printf("full t:%lld\n", circuit->global_time); fflush(stdout);

        if(output_events.begin() == first_output_event) // Shouldn't happen?
        {
            debug_printf("ERROR out of subcycles!\n");
            
            input_events.clear();
            output_events.clear();
            first_output_event = output_events.begin();

            // TODO: Do this differently?
            for(int i = 0; i < input_event_table.size(); i++) input_event_table[i].clear();

            //allocated_sub_cycles = 0;
            allocated_sub_cycles.clear();
            //input_event_type = 0;
            //output_event_type = 0;
            cycle_time = 0;

            break;
        }

        if(output_events.front().type)
            allocated_sub_cycles.deallocate(output_events.front().sub_cycle->allocated_sub_cycles);
            //allocated_sub_cycles &= ~output_events.front().sub_cycle->allocated_sub_cycles;

        //output_event_type &= ~(1ull << output_events.begin());
        output_events.pop_front();

        // TODO: Fix all this
        uint64_t event_delay;
        /*if(output_event_type & (1ull << output_events.begin()))
        {
            Cycle* c = output_events.front().sub_cycle;
            while(c->output_event_type & (1ull << c->output_events.begin()))
                c = c->output_events.front().sub_cycle;

            event_delay = delay[c->output_events.front().state]; // ^ 1; // TODO: is this correct?
        }
        else
            event_delay = delay[output_events.front().state]; //^ 1; // TODO: is this correct?
        */
        event_delay = delay[1] < delay[0] ? delay[1] : delay[0];

        // Clear all input events occurring before first output event. TODO: is this correct?        
        while(!input_events.empty() && input_events.front().time + event_delay <= output_events.front().time)
        {
            input_event_table[input_events.front().state].pop_front();
            input_events.pop_front();
        }
    }

    if(first_output_event != output_events.end())
    {
        //uint64_t cycle_num = Chip::next_bit64(~allocated_sub_cycles);
        //allocated_sub_cycles |= 1ull << cycle_num;
        int cycle_num = allocated_sub_cycles.allocate();
        
#ifdef DEBUG
        if(cycle_num > max_subcycle_length)
            max_subcycle_length = cycle_num;
#endif

        if(sub_cycles[cycle_num] == NULL)
            sub_cycles[cycle_num] = new Cycle(first_output_event.getQueueSize(), allocated_sub_cycles.size());

        //Cycle& cycle = sub_cycles[cycle_num];
        Cycle& cycle = *sub_cycles[cycle_num];

        cycle.output_events.clear();
        //cycle.output_event_type = 0;
        //cycle.allocated_sub_cycles = 1ull << cycle_num;
        cycle.allocated_sub_cycles.clear();
        cycle.allocated_sub_cycles.set(cycle_num);
        cycle.parent_cycle = this;
    
        for(cirque<Event>::index i = first_output_event; i != output_events.end(); i++)
        {
            if(output_events[i].type) // Sub-cycle
            {
                //cycle.output_event_type |= (1ull << cycle.output_events.end());
                cycle.allocated_sub_cycles |= output_events[i].sub_cycle->allocated_sub_cycles;
                output_events[i].sub_cycle->parent_cycle = &cycle;
            }

            cycle.output_events.push_back(output_events[i]);
        }
        cycle.first_output_event = cycle.output_events.begin();
    
        cycle.cycle_time = cycle_time;
        cycle.activation_time = cycle.output_events.front().time;
        cycle.cycle_duration = circuit->global_time - cycle.activation_time + 1; // TODO: Plus 1 ok?
    
        //output_event_type |= (1ull << first_output_event);
        output_events[first_output_event].type = 1;
        output_events[first_output_event].sub_cycle = &cycle;
        output_events.erase_back(first_output_event);
        debug_printf("chip:%p first:%d end:%d\n", this, (int)first_output_event.getRawIndex(), int(output_events.end().getRawIndex()));
    }

    // TODO: Clear all output events occuring before first input event?

#ifdef DEBUG
    debug_printf("AFTER:\n");
    if(circuit->global_time > DEBUG_START_TIME )
    {
        print_output_events();
        print_input_events();
    }
#endif

    // TODO: use iterator?
    for(int i = 0; i < output_links.size(); i++)
    {
        //if((output_links[i].chip->active_inputs & output_links[i].mask) && output_links[i].chip->event_count)
        if(!(current_cycle->active_outputs & (1ull << i)) && output_links[i].chip->state != PASSIVE) // output_links[i].chip->state != PASSIVE)
        //if(output_links[i].chip->state != PASSIVE)
            output_links[i].chip->deactivate_outputs();
    }

    current_cycle = this;
    active_outputs = (1ull << output_links.size()) - 1;

    // If there is a pending event, add to output events
    if(pending_event && pending_event != circuit->global_time) // TODO: is skip when pending event == global_time accurate??? 
    {
        //output_event_type &= ~(1ull << output_events.end());
        output_events.push_back(Event(pending_event, output ^ 1));
    }
    else if(pending_event) // Pending event for current time. Run event now?
    {
        update_output();
    }

    // Set last_input_event to global time, if it occurred during the cycle
    // TODO: Adjust time accurately?
    /*for(uint64_t& x : last_input_event) 
        if(x >= activation_time) 
            x = circuit->global_time;*/

    for(int i = 0; i < last_input_event.size(); i++)
        if(last_input_event[i] >= activation_time)
            last_input_event[i] = input_links[i].chip->last_output_event;
}



inline int Chip::next_bit(uint32_t x)
{
#if 0
    // Computes number of trailing zeros
    static const uint8_t de_bruijn_bit_position[32] = 
    {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    return de_bruijn_bit_position[((uint32_t)((x & -x) * 0x077CB531U)) >> 27];
#else
    return __builtin_ctz(x);
#endif  
}

inline int Chip::next_bit64(uint64_t x)
{
#if 0
    // TODO: 64-bit gcc alternative
#else
    return __builtin_ctzll(x);
#endif  
}

