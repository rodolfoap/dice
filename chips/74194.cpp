#include "../chip_desc.h"

/*
74194
4-bit bidirectional universal shift register with asynchronous reset.

     +---+--+---+            +---+---*---------------+
/RST |1  +--+ 16| VCC        | S1| S0| Function      |
   R |2       15| Q0         +===+===*===============+
  P0 |3       14| Q1         | 0 | 0 | Hold          |
  P1 |4 74194 13| Q2         | 0 | 1 | Shift right   |
  P2 |5       12| Q3         | 1 | 0 | Shift left    |
  P3 |6       11| CLK        | 1 | 1 | Parallel load |
   L |7       10| S1         +---+---*---------------+
 GND |8        9| S0
     +----------+
*/

static const uint8_t SR_PINS[] = { 2, 15, 14, 13 };
static const uint8_t SL_PINS[] = { 14, 13, 12, 7 };

template <int N>
CHIP_LOGIC( 74194 )
{
    int P = N + 3; // Parallel input
    int SR = SR_PINS[N]; // Shift right serial input
    int SL = SL_PINS[N]; // Shift left serial input
    int Q = 15 - N;
    
    if(!pin[1])
        pin[Q] = 0;
    else if(POS_EDGE_PIN(11))
    {
        if(pin[10] && pin[9])
            pin[Q] = pin[P];
        else if(pin[10] && !pin[9])
            pin[Q] = pin[SL];
        else if(!pin[10] && pin[9])
            pin[Q] = pin[SR];
        else
            pin[Q] = prev_pin[Q];
    }
    else
        pin[Q] = prev_pin[Q];
}

// TODO: Fix delay from clear?

CHIP_DESC( 74194 ) =
{
	CHIP_START( 74194<0> )
		INPUT_PINS( 1, 9, 10, 11, 3, SR_PINS[0], SL_PINS[0] )
		OUTPUT_PIN( 15 )
		EVENT_PINS( 11 )
		PREV_OUTPUT_PIN( 15 )
		OUTPUT_DELAY_NS( 14.0, 17.0 ),

    CHIP_START( 74194<1> )
		INPUT_PINS( 1, 9, 10, 11, 4, SR_PINS[1], SL_PINS[1] )
		OUTPUT_PIN( 14 )
		EVENT_PINS( 11 )
		PREV_OUTPUT_PIN( 14 )
		OUTPUT_DELAY_NS( 14.0, 17.0 ),

	CHIP_START( 74194<2> )
		INPUT_PINS( 1, 9, 10, 11, 5, SR_PINS[2], SL_PINS[2] )
		OUTPUT_PIN( 13 )
		EVENT_PINS( 11 )
		PREV_OUTPUT_PIN( 13 )
		OUTPUT_DELAY_NS( 14.0, 17.0 ),

	CHIP_START( 74194<3> )
	    INPUT_PINS( 1, 9, 10, 11, 6, SR_PINS[3], SL_PINS[3] )
		OUTPUT_PIN( 12 )
		EVENT_PINS( 11 )
		PREV_OUTPUT_PIN( 12 )
		OUTPUT_DELAY_NS( 14.0, 17.0 ),

	CHIP_DESC_END
};

