#include "../chip_desc.h"

/*
7495
4-bit shift register with serial and parallel synchronous operating modes.

     +---+--+---+            
  Ds |1  +--+ 14| VCC        
  P0 |2       13| Q0         
  P1 |3       12| Q1         
  P2 |4 7495  11| Q2         
  P3 |5       10| Q3         
   S |6        9| /CP1       
 GND |7        8| /CP2       
     +----------+

S     mode control input
Ds    serial data input
P#    parallel data inputs
/CP1  serial clock (active Low going edge) input
/CP2  parallel clock (active Low going edge) input
Q#    parallel outputs


L = low
H = high
X = don't care
l = low one set-up time prior to the high to low clock transition
h = high one set-up time prior to the high to low clock transition
Pn = lower case letters indicate the state of the references input/output one set-up time prior to the high to low clock transition
l>h = low to high
h<l = high to low


MODE            INPUTS               OUTPUTS
===============+===+====+====+==+==++===========+    
               | S |/CP1|/CP2|Ds|Pn||Q0|Q1|Q2|Q3|
---------------+---+----+----+--+--||--+--+--+--+        
Shift          | L |h>l | X  | l| X|| L|q0|q1|q2|
               | L |h>l | X  | h| X|| H|q0|q1|q2|
---------------+---+----+----+--+--++--+--+--+--+
Parallel load  | H | X  |h>l | X|Pn||p0|p1|p2|p3|
---------------+---+----+----+--+--++-----------+
Mode Change    |h>l| L  | L  | X| X|| no change |
               |l>h| L  | L  | X| X|| no change |
               |h>l| H  | L  | X| X|| no change |
               |l>h| H  | L  | X| X|| ?         |
               |h>l| L  | H  | X| X|| ?         |
               |l>h| L  | H  | X| X|| no change |
               |h>l| H  | H  | X| X|| ?         |
               |l>h| H  | H  | X| X|| no change |
---------------+---+----+----+--+--++-----------+

*/

static const uint8_t SR_PINS[] = { 1, 13, 12, 11 };

template <int N>
CHIP_LOGIC( 7495 )
{
    int P = N + 2; // Parallel input
    int SR = SR_PINS[N]; // Shift right serial input
    int Q = 13 - N;
    
    if(pin[6] && NEG_EDGE_PIN(8)) // Parallel load
    {
        pin[Q] = pin[P];
    }
    else if(!pin[6] && NEG_EDGE_PIN(9)) // Shift right
    {
        pin[Q] = pin[SR];
    }
    else
    {
        pin[Q] = prev_pin[Q];
    }
}

CHIP_DESC( 7495 ) =
{
	CHIP_START( 7495<0> )
		INPUT_PINS( 6, 8, 9, SR_PINS[0], 2 )
		OUTPUT_PIN( 13 )
		EVENT_PINS( 8, 9 )
		PREV_OUTPUT_PIN( 13 )
		OUTPUT_DELAY_NS( 18.0, 21.0 ),

    CHIP_START( 7495<1> )
		INPUT_PINS( 6, 8, 9, SR_PINS[1], 3 )
		OUTPUT_PIN( 12 )
		EVENT_PINS( 8, 9 )
		PREV_OUTPUT_PIN( 12 )
		OUTPUT_DELAY_NS( 18.0, 21.0 ),

	CHIP_START( 7495<2> )
		INPUT_PINS( 6, 8, 9, SR_PINS[2], 4 )
		OUTPUT_PIN( 11 )
		EVENT_PINS( 8, 9 )
		PREV_OUTPUT_PIN( 11 )
		OUTPUT_DELAY_NS( 18.0, 21.0 ),

	CHIP_START( 7495<3> )
		INPUT_PINS( 6, 8, 9, SR_PINS[3], 5 )
		OUTPUT_PIN( 10 )
		EVENT_PINS( 8, 9 )
		PREV_OUTPUT_PIN( 10 )
		OUTPUT_DELAY_NS( 18.0, 21.0 ),

	CHIP_DESC_END
};

