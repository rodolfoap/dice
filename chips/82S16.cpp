#include "../circuit.h"
#include "82S16.h"

/*
82S16
256-Bit Bipolar RAM (256x1)

       +------+
    A1 |1   16| Vcc
    A0 |2   15| A2
  /CE1 |3   14| A3 
  /CE2 |4   13| Din
  /CE3 |5   12| /WE
 /Dout |6   11| A7
    A4 |7   10| A6
   GND |8    9| A5
       +------+
*/

enum { ADDR_MASK = 0xff, CE_MASK = 0x700, WE_MASK = 0x800, D_SHIFT = 12 }; 

static CUSTOM_LOGIC( RAM_82S16 )
{
    if(chip->custom_data == NULL)
    {
        printf("ERROR: Missing Ram82S16Desc for chip %p\n", chip);\
        return;
    }

    Ram82S16Desc& data = *(Ram82S16Desc*)chip->custom_data;

    chip->state = PASSIVE;
    chip->inputs ^= mask;

    int addr = chip->inputs & ADDR_MASK;
    int new_out = 0;

    if((chip->inputs & CE_MASK) == 0)
    {
        if((chip->inputs & WE_MASK) == 0) //write enabled
        {
            data[addr] = (chip->inputs >> D_SHIFT) & 1;
            new_out = !data[addr];
            //printf("write addr:%d data:%d t:%lld inputs:%d\n", addr, data[addr], chip->circuit->global_time, chip->inputs);
        }
        else //read mode
        {
            new_out = !data[addr];
            //printf("read addr:%d data:%d t:%lld inputs:%d\n", addr, data[addr], chip->circuit->global_time, chip->inputs);
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

CHIP_DESC( 82S16 ) = 
{
	CUSTOM_CHIP_START( &RAM_82S16 )
	   INPUT_PINS( 2, 1, 15, 14, 7, 9, 10, 11, /* ADDR */
                   3, 4, 5, /* /CE */
                   12, 13 ) /* /WE, D */
	   OUTPUT_PIN( 6 )
	   OUTPUT_DELAY_NS( 30.0, 30.0 ),	//TODO: more accurate timing

   	CHIP_DESC_END
};

