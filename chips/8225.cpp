#include "../circuit.h"
#include "8225.h"

/*
Signetics 8225 (aka TI 7489)
64-Bit Bipolar RAM (16x4)

       +--------+
    A0 |1     16| VCC
   /CE |2     15| A2
   /WE |3     14| A3 
    D0 |4  82 13| A4
   /Q0 |5  25 12| D4
    D1 |6     11| /Q4
   /Q1 |7     10| D3
   GND |8      9| /Q3
       +--------+
*/

// TODO: Optimize

enum { ADDR_MASK = 0xf, CE_MASK = 0x10, WE_MASK = 0x20, D_SHIFT = 6 }; 

template<int BIT> CUSTOM_LOGIC( RAM_8225 )
{
    N8225Desc& data = *(N8225Desc*)chip->custom_data;
    
    chip->state = PASSIVE;
    chip->inputs ^= mask;

    int addr = chip->inputs & ADDR_MASK;
    int new_out = 0;

    if((chip->inputs & CE_MASK) == 0)
    {
        if((chip->inputs & WE_MASK) == 0) // write enabled
        {
            data[addr][BIT] = (chip->inputs >> D_SHIFT) & 1;
            new_out = ~data[addr][BIT] & 1;
            //printf("write addr:%d data:%d t:%lld inputs:%d\n", addr, data[addr][BIT], chip->circuit->global_time, chip->inputs);
        }
        else // read mode
        {
            new_out = ~data[addr][BIT] & 1;
            //printf("read addr:%d data:%d t:%lld inputs:%d\n", addr, data[addr][BIT], chip->circuit->global_time, chip->inputs);
        }
    }
    else
        new_out = 1;

    if(new_out != chip->output && chip->pending_event == 0)
    {
        // Schedule event
        chip->pending_event = chip->circuit->queue_push(chip, chip->delay[chip->output]);
    }
    else if(chip->pending_event && new_out == chip->output)
    {
        // Cancel event
        chip->pending_event = 0;
    }
}

CHIP_DESC( 8225 ) = 
{
	CUSTOM_CHIP_START( &RAM_8225<0> )
        INPUT_PINS( /* ADDR: */ 1, 15, 14, 13, /* /CE, /WE, D: */ 2, 3, 4 )
	    OUTPUT_PIN( 5 )
	    OUTPUT_DELAY_NS( 30.0, 35.0 ),

	CUSTOM_CHIP_START( &RAM_8225<1> )
        INPUT_PINS( /* ADDR: */ 1, 15, 14, 13, /* /CE, /WE, D: */ 2, 3, 6 )
	    OUTPUT_PIN( 7 )
	    OUTPUT_DELAY_NS( 30.0, 35.0 ),

	CUSTOM_CHIP_START( &RAM_8225<2> )
        INPUT_PINS( /* ADDR: */ 1, 15, 14, 13, /* /CE, /WE, D: */ 2, 3, 10 )
	    OUTPUT_PIN( 9 )
	    OUTPUT_DELAY_NS( 30.0, 35.0 ),

	CUSTOM_CHIP_START( &RAM_8225<3> )
        INPUT_PINS( /* ADDR: */ 1, 15, 14, 13, /* /CE, /WE, D: */ 2, 3, 12 )
	    OUTPUT_PIN( 11 )
	    OUTPUT_DELAY_NS( 30.0, 35.0 ),

   	CHIP_DESC_END
};

