#include "dipswitch.h"
#include "../circuit.h"

/*
Standard 1-bit DIP Switch. Can be SPDT or DPDT.

Inputs: Pin 1 (connected to output pin 3 when state = 0)
        Pin 2 (connected to output pin 3 when state = 1)
        Pin 4 (connected to output pin 6 when state = 0)
        Pin 5 (connected to output pin 6 when state = 1)

Outputs: Pin 3 (First pole)
         Pin 6 (Second pole, optional)

 1 ---O\
        \
        |O---- 3    
        |
 2 ---O |
        |
        |          (when state = 0)
        |
 4 ---O\|
        \
         O---- 6
 
 5 ---O

*/

extern CUSTOM_LOGIC( clock );

CUSTOM_LOGIC( DipswitchDesc::logic )
{
    DipswitchDesc* desc = (DipswitchDesc*)chip->custom_data;

    chip->state = PASSIVE;

    if(desc->state != chip->output)
        chip->pending_event = chip->circuit->queue_push(chip, 0);
}

static CHIP_LOGIC( dipswitch1 )
{
    pin[3] = pin[i2] ? pin[2] : pin[1];
}

static CHIP_LOGIC( dipswitch2 )
{
    pin[6] = pin[i2] ? pin[5] : pin[4];
}

CHIP_DESC( DIPSWITCH ) = 
{
    // Timer to refresh dipswitches if state has been changed in GUI.
    CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 250.0, 250.0 ),
    
    CUSTOM_CHIP_START(&DipswitchDesc::logic)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i2 ),

	CHIP_START( dipswitch1 )
        INPUT_PINS( 1, 2, i2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( dipswitch2 )
        INPUT_PINS( 4, 5, i2 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_DESC_END
};




/*
SP4T Switch. Assumes outputs are tied high when not selected by switch.

Inputs: Pin 1 (connected to output pin 2 when state = 0,
               connected to output pin 3 when state = 1,
               connected to output pin 4 when state = 2,
               connected to output pin 5 when state = 3)

Outputs: Pin 2 (First throw)
         Pin 3 (Second throw)
         Pin 4 (Third throw)
         Pin 5 (Forth throw)

 2 ---O\
        \
         \
 3 ---O   \
           O---- 1      (when state = 0)
 
 4 ---O        
                   
         
 5 ---O 
*/


template<int T> CUSTOM_LOGIC( DipswitchSP4TDesc::logic )
{
    DipswitchSP4TDesc* desc = (DipswitchSP4TDesc*)chip->custom_data;

    chip->state = PASSIVE;

    int x = desc->state == T;

    if(x != chip->output)
        chip->pending_event = chip->circuit->queue_push(chip, 0);
}

template<int T> CHIP_LOGIC( sp4t )
{
    pin[2+T] = pin[i2+T] ? pin[1] : 1;
}

CHIP_DESC( DIPSWITCH_SP4T ) = 
{
    // Timer to refresh dipswitches if state has been changed in GUI.
    CUSTOM_CHIP_START(&clock) OUTPUT_PIN(i1) OUTPUT_DELAY_MS(250.0, 250.0),
    
    CUSTOM_CHIP_START(DipswitchSP4TDesc::logic<0>) INPUT_PINS(i1) OUTPUT_PIN(i2),
    CUSTOM_CHIP_START(DipswitchSP4TDesc::logic<1>) INPUT_PINS(i1) OUTPUT_PIN(i3),
    CUSTOM_CHIP_START(DipswitchSP4TDesc::logic<2>) INPUT_PINS(i1) OUTPUT_PIN(i4),
    CUSTOM_CHIP_START(DipswitchSP4TDesc::logic<3>) INPUT_PINS(i1) OUTPUT_PIN(i5),

	CHIP_START(sp4t<0>) INPUT_PINS(1, i2) OUTPUT_PIN(2) OUTPUT_DELAY_NS(0.1, 0.1),
    CHIP_START(sp4t<1>) INPUT_PINS(1, i3) OUTPUT_PIN(3) OUTPUT_DELAY_NS(0.1, 0.1),
    CHIP_START(sp4t<2>) INPUT_PINS(1, i4) OUTPUT_PIN(4) OUTPUT_DELAY_NS(0.1, 0.1),
    CHIP_START(sp4t<3>) INPUT_PINS(1, i5) OUTPUT_PIN(5) OUTPUT_DELAY_NS(0.1, 0.1),

	CHIP_DESC_END
};





/*
AMP 53137
4-bit Rotary Hex DIP Switch

      +---+--+---+
   A0 |1  +--+ 16| 
  /A0 |2       15| Y0
  /A1 |3       14| Y1
   A1 |4   53  13| 
      |5  137  12| A2
   Y2 |6       11| /A2
   Y3 |7       10| /A3
      |8        9| A3
      +----------+
*/

template<int BIT>
CUSTOM_LOGIC( Dipswitch53137Desc::logic )
{
    Dipswitch53137Desc* desc = (Dipswitch53137Desc*)chip->custom_data;

    chip->state = PASSIVE;

    if(((desc->state >> BIT) & 1) != chip->output)
        chip->pending_event = chip->circuit->queue_push(chip, 0);
}

template<int BIT>
CHIP_LOGIC( 53137 )
{
    static const int in_low[] = { 2, 3, 11, 10 };
    static const int in_hi[] = { 1, 4, 12, 9 };
    static const int out[] = { 15, 14, 6, 7 };
    static const int sw[] = { i2, i3, i4, i5 };

    pin[out[BIT]] = pin[sw[BIT]] ? pin[in_hi[BIT]] : pin[in_low[BIT]];
}

CHIP_DESC( 53137 ) = 
{
    // Timer to refresh dipswitches if state has been changed in GUI.
    CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 250.0, 250.0 ),
    

    CUSTOM_CHIP_START(Dipswitch53137Desc::logic<0>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i2 ),

    CUSTOM_CHIP_START(Dipswitch53137Desc::logic<1>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i3 ),

    CUSTOM_CHIP_START(Dipswitch53137Desc::logic<2>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i4 ),

    CUSTOM_CHIP_START(Dipswitch53137Desc::logic<3>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i5 ),


    CHIP_START( 53137<0> )
        INPUT_PINS( 1, 2, i2 )
        OUTPUT_PIN( 15 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( 53137<1> )
        INPUT_PINS( 3, 4, i3 )
        OUTPUT_PIN( 14 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( 53137<2> )
        INPUT_PINS( 11, 12, i4 )
        OUTPUT_PIN( 6 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( 53137<3> )
        INPUT_PINS( 9, 10, i5 )
        OUTPUT_PIN( 7 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_DESC_END
};




/*
4SPST 4-bit Hex DIP Switch.

Inputs: Pin 1 (connected to output pin 9  when state = 0)
        Pin 2 (connected to output pin 9  when state = 1)
        Pin 3 (connected to output pin 10 when state = 0)
        Pin 4 (connected to output pin 10 when state = 1)
        Pin 5 (connected to output pin 11 when state = 0)
        Pin 6 (connected to output pin 11 when state = 1)
        Pin 7 (connected to output pin 12 when state = 0)
        Pin 8 (connected to output pin 12 when state = 1)

Outputs: Pin 9  (First pole)
         Pin 10 (Second pole)
         Pin 11 (Third pole)
         Pin 12 (Fourth pole)

 1 ---O
        
         O---- 9    
        /
 2 ---O/ 
         
                   
         
 3 ---O\
        \
         O---- 10
 
 4 ---O

                   (when state = 5)

 5 ---O  
         
         O---- 11
        /
 6 ---O/



 7 ---O\
        \
         O---- 12
 
 8 ---O

*/
template<int BIT>
CHIP_LOGIC( DIPSWITCH_4SPST )
{
    static const int in_low[] = { 1, 3, 5, 7 };
    static const int in_hi[] = { 2, 4, 6, 8 };
    static const int out[] = { 9, 10, 11, 12 };
    static const int sw[] = { i2, i3, i4, i5 };

    pin[out[BIT]] = pin[sw[BIT]] ? pin[in_hi[BIT]] : pin[in_low[BIT]];
}

CHIP_DESC( DIPSWITCH_4SPST ) = 
{
    // Timer to refresh dipswitches if state has been changed in GUI.
    CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 250.0, 250.0 ),
    

    CUSTOM_CHIP_START(Dipswitch4SP4TDesc::logic<0>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i2 ),

    CUSTOM_CHIP_START(Dipswitch4SP4TDesc::logic<1>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i3 ),

    CUSTOM_CHIP_START(Dipswitch4SP4TDesc::logic<2>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i4 ),

    CUSTOM_CHIP_START(Dipswitch4SP4TDesc::logic<3>)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i5 ),


    CHIP_START( DIPSWITCH_4SPST<0> )
        INPUT_PINS( 1, 2, i2 )
        OUTPUT_PIN( 9 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( DIPSWITCH_4SPST<1> )
        INPUT_PINS( 3, 4, i3 )
        OUTPUT_PIN( 10 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( DIPSWITCH_4SPST<2> )
        INPUT_PINS( 5, 6, i4 )
        OUTPUT_PIN( 11 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_START( DIPSWITCH_4SPST<3> )
        INPUT_PINS(7, 8, i5 )
        OUTPUT_PIN( 12 )
        OUTPUT_DELAY_NS( 0.1, 0.1 ),

	CHIP_DESC_END
};





/* Potentiometer */
template <typename T, double (T::*r)>
void PotentiometerDesc<T, r>::logic(Chip* chip, int mask)
{
    PotentiometerDesc<T, r>* desc = (PotentiometerDesc<T, r>*)chip->custom_data;

    chip->state = PASSIVE;

    if(desc->current_val != desc->output.*r) // Update resistance value in attached chip
    {
        desc->output.*r = desc->current_val;
        
        chip->deactivate_outputs(); // TODO: is this necessary?
        //chip->state = PASSIVE;
        //chip->active_outputs = (1 << chip->output_links.size()) - 1;
        
        chip->pending_event = chip->circuit->queue_push(chip, 0);
    }
}


CHIP_DESC( POT_555_ASTABLE ) = 
{
    // Timer to refresh potentiometer if state has been changed in GUI.
    CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 250.0, 250.0 ),
    
    ChipDesc(&PotentimeterAstable555Desc::logic)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i3 ),

	CHIP_DESC_END
};

CHIP_DESC( POT_555_MONO ) = 
{
    // Timer to refresh potentiometer if state has been changed in GUI.
    CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 250.0, 250.0 ),
    
    ChipDesc(&PotentimeterMono555Desc::logic)
        INPUT_PINS( i1 )
        OUTPUT_PIN( i3 ),

	CHIP_DESC_END
};

