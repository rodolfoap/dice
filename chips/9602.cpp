#include "9602.h"
#include "../circuit.h"

/*
9602/74123
Dual retriggerable monostable multivibrators with overriding reset.

       +---+--+---+                        +---+--+---+
 1Cext |1  +--+ 16| VCC               /1TR |1  +--+ 16| VCC
1RCext |2       15| 2RCext             1TR |2       15| 1RCext
 /1RST |3       14| 2Cext            /1RST |3       14| 1Cext
   1TR |4  9602 13| /2RST              /1Q |4   74  13| 1Q
  /1TR |5       12| 2TR                 2Q |5  123  12| /2Q
    1Q |6       11| /2TR             2Cext |6       11| /2RST
   /1Q |7       10| Q2              2RCext |7       10| 2TR
   GND |8        9| /2Q                GND |8        9| /2TR
       +----------+                        +----------+        
*/

/*
A 9602 is composed of two latches.
The first latch is set when output of trigger OR gate goes high, and is cleared
shortly after. (Differentiator circuit). This causes capacitor to discharge rapidly (through approx. 1 Ohm?),
and then begin charging through external resistor.
The second latch is set by first latch, and cleared when capacitor finishes charging, or by reset signal.

TODO: Fix delay equations, only hold for C > 1000 pF
*/

struct A { enum { TRIG1 = 4, TRIG2 = 5, RST = 3, Q = 6, Q_n = 7, TRIG = i3, LATCH1 = i4, BUF = i5, CAP = 2 }; };
struct B { enum { TRIG1 = 12, TRIG2 = 11, RST = 13, Q = 10, Q_n = 9, TRIG = i6, LATCH1 = i7, BUF = i8, CAP = 14 }; };

struct C { enum { TRIG1 = 2, TRIG2 = 1, RST = 3, Q = 13, Q_n = 4, TRIG = i3, LATCH1 = i4, BUF = i5, CAP = 14 }; };
struct D { enum { TRIG1 = 10, TRIG2 = 9, RST = 11, Q = 5, Q_n = 12, TRIG = i6, LATCH1 = i7, BUF = i8, CAP = 6 }; };

const double Mono9602Desc::TIME_CONSTANT = 0.31;
const double Mono74123Desc::TIME_CONSTANT = 0.28;

CUSTOM_LOGIC( Mono9602Desc::mono_9602A )
{
    Mono9602Desc* desc = (Mono9602Desc*)chip->custom_data;

    // Update tp_hl in standard portion based on rc value
    if(chip->output_links.size())
    {
        //chip->output_links[0].chip->delay[1] = uint64_t(TIME_CONSTANT*(desc->r1*desc->c1 + desc->c1) / Circuit::timescale);
        // Attempted curve fit - needs to be made more accurate
        chip->output_links[0].chip->delay[0] = uint64_t((TIME_CONSTANT*(desc->r1*desc->c1 + desc->c1) + 15.0e-12*desc->r1) / Circuit::timescale);
    }
}

CUSTOM_LOGIC( Mono9602Desc::mono_9602B )
{
    Mono9602Desc* desc = (Mono9602Desc*)chip->custom_data;

    // Update tp_hl in standard portion based on rc value
    if(chip->output_links.size())
    {
        chip->output_links[0].chip->delay[0] = uint64_t((TIME_CONSTANT*(desc->r2*desc->c2 + desc->c2) + 15.0e-12*desc->r2) / Circuit::timescale);
    }
}

template<typename T> CHIP_LOGIC( 9602_OR )
{
    pin[T::TRIG] = pin[T::TRIG1] || !pin[T::TRIG2];
}

template<typename T> CHIP_LOGIC( 9602_LATCH1 )
{
    if(POS_EDGE_PIN(T::TRIG))
        pin[T::LATCH1] = 1;
    else if(pin[T::BUF])
        pin[T::LATCH1] = 0;
    else
        pin[T::LATCH1] = prev_pin[T::LATCH1];
}

template<typename T> CHIP_LOGIC( 9602_BUF )
{
    pin[T::BUF] = pin[T::LATCH1];
}

template<typename T> CHIP_LOGIC( 9602_CAP )
{
    pin[T::CAP] = pin[T::LATCH1] ^ 1;
}

template<typename T> CHIP_LOGIC( 9602_Q )
{
    if(!pin[T::RST])
        pin[T::Q] = 0;
    else if(POS_EDGE_PIN(T::CAP))
        pin[T::Q] = 0;
    else if(pin[T::LATCH1])
        pin[T::Q] = 1;
    else
        pin[T::Q] = prev_pin[T::Q];
}

template<typename T> CHIP_LOGIC( 9602_Q_n )
{
    pin[T::Q_n] = pin[T::Q] ^ 1;
}


CHIP_DESC( 9602 ) = 
{
	CUSTOM_CHIP_START(&Mono9602Desc::mono_9602A)
        OUTPUT_PIN( i1 ),

    CHIP_START( 9602_OR<A> )
        INPUT_PINS( A::TRIG1, A::TRIG2 )
        OUTPUT_PIN( A::TRIG )
        OUTPUT_DELAY_NS( 20.0, 20.0 ),
        // pulses < 25 ns should not trigger the chip? Or is it < 40 ns?

	CHIP_START( 9602_LATCH1<A> )
        INPUT_PINS( A::TRIG, A::BUF )
        EVENT_PINS( A::TRIG )
        OUTPUT_PIN( A::LATCH1 )
        PREV_OUTPUT_PIN( A::LATCH1 )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_BUF<A> )
        INPUT_PINS( A::LATCH1 )
        OUTPUT_PIN( A::BUF )
        OUTPUT_DELAY_NS( 30.0, 30.0 ),

    CHIP_START( 9602_CAP<A> )
        INPUT_PINS( A::LATCH1, i1 )
        OUTPUT_PIN( A::CAP )
        OUTPUT_DELAY_NS( 20.0, 20.0 ), // tp_lh will be overwritten

    CHIP_START( 9602_Q<A> )
        INPUT_PINS( A::RST, A::LATCH1, A::CAP )
        EVENT_PINS( A::CAP )
        OUTPUT_PIN( A::Q )
        PREV_OUTPUT_PIN( A::Q )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_Q_n<A> )
        INPUT_PINS( A::Q )
        OUTPUT_PIN( A::Q_n )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),



	CUSTOM_CHIP_START(&Mono9602Desc::mono_9602B)
        OUTPUT_PIN( i2 ),

    CHIP_START( 9602_OR<B> )
        INPUT_PINS( B::TRIG1, B::TRIG2 )
        OUTPUT_PIN( B::TRIG )
        OUTPUT_DELAY_NS( 20.0, 20.0 ),
        // pulses < 25 ns should not trigger the chip? Or is it < 40 ns?

	CHIP_START( 9602_LATCH1<B> )
        INPUT_PINS( B::TRIG, B::BUF )
        EVENT_PINS( B::TRIG )
        OUTPUT_PIN( B::LATCH1 )
        PREV_OUTPUT_PIN( B::LATCH1 )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_BUF<B> )
        INPUT_PINS( B::LATCH1 )
        OUTPUT_PIN( B::BUF )
        OUTPUT_DELAY_NS( 30.0, 30.0 ),

    CHIP_START( 9602_CAP<B> )
        INPUT_PINS( B::LATCH1, i2 )
        OUTPUT_PIN( B::CAP )
        OUTPUT_DELAY_NS( 20.0, 20.0 ), // tp_lh will be overwritten

    CHIP_START( 9602_Q<B> )
        INPUT_PINS( B::RST, B::LATCH1, B::CAP )
        EVENT_PINS( B::CAP )
        OUTPUT_PIN( B::Q )
        PREV_OUTPUT_PIN( B::Q )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_Q_n<B> )
        INPUT_PINS( B::Q )
        OUTPUT_PIN( B::Q_n )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

	CHIP_DESC_END
};




CUSTOM_LOGIC( Mono74123Desc::mono_74123A )
{
    Mono74123Desc* desc = (Mono74123Desc*)chip->custom_data;

    // Update tp_hl in standard portion based on rc value
    if(chip->output_links.size())
    {
        // Attempted curve fit - needs to be made more accurate
        chip->output_links[0].chip->delay[0] = uint64_t((TIME_CONSTANT*desc->r1*desc->c1*(1.0 + 700.0 / desc->r1) + 8.0e-12*desc->r1) / Circuit::timescale);
    }
}

CUSTOM_LOGIC( Mono74123Desc::mono_74123B )
{
    Mono74123Desc* desc = (Mono74123Desc*)chip->custom_data;

    // Update tp_hl in standard portion based on rc value
    if(chip->output_links.size())
    {
        chip->output_links[0].chip->delay[0] = uint64_t((TIME_CONSTANT*desc->r2*desc->c2*(1.0 + 700.0 / desc->r2) + 8.0e-12*desc->r2) / Circuit::timescale);
    }
}

template<typename T> CHIP_LOGIC( 74123_AND )
{
    pin[T::TRIG] = pin[T::TRIG1] && !pin[T::TRIG2];
}

CHIP_DESC( 74123 ) = 
{
	CUSTOM_CHIP_START(&Mono74123Desc::mono_74123A)
        OUTPUT_PIN( i1 ),

    CHIP_START( 74123_AND<C> )
        INPUT_PINS( C::TRIG1, C::TRIG2 )
        OUTPUT_PIN( C::TRIG )
        OUTPUT_DELAY_NS( 20.0, 20.0 ),
        // pulses < 25 ns should not trigger the chip? Or is it < 40 ns?

	CHIP_START( 9602_LATCH1<C> )
        INPUT_PINS( C::TRIG, C::BUF )
        EVENT_PINS( C::TRIG )
        OUTPUT_PIN( C::LATCH1 )
        PREV_OUTPUT_PIN( C::LATCH1 )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_BUF<C> )
        INPUT_PINS( C::LATCH1 )
        OUTPUT_PIN( C::BUF )
        OUTPUT_DELAY_NS( 30.0, 30.0 ),

    CHIP_START( 9602_CAP<C> )
        INPUT_PINS( C::LATCH1, i1 )
        OUTPUT_PIN( C::CAP )
        OUTPUT_DELAY_NS( 20.0, 20.0 ), // tp_lh will be overwritten

    CHIP_START( 9602_Q<C> )
        INPUT_PINS( C::RST, C::LATCH1, C::CAP )
        EVENT_PINS( C::CAP )
        OUTPUT_PIN( C::Q )
        PREV_OUTPUT_PIN( C::Q )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_Q_n<C> )
        INPUT_PINS( C::Q )
        OUTPUT_PIN( C::Q_n )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),



	CUSTOM_CHIP_START(&Mono74123Desc::mono_74123B)
        OUTPUT_PIN( i2 ),

    CHIP_START( 74123_AND<D> )
        INPUT_PINS( D::TRIG1, D::TRIG2 )
        OUTPUT_PIN( D::TRIG )
        OUTPUT_DELAY_NS( 20.0, 20.0 ),
        // pulses < 25 ns should not trigger the chip? Or is it < 40 ns?

	CHIP_START( 9602_LATCH1<D> )
        INPUT_PINS( D::TRIG, D::BUF )
        EVENT_PINS( D::TRIG )
        OUTPUT_PIN( D::LATCH1 )
        PREV_OUTPUT_PIN( D::LATCH1 )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_BUF<D> )
        INPUT_PINS( D::LATCH1 )
        OUTPUT_PIN( D::BUF )
        OUTPUT_DELAY_NS( 30.0, 30.0 ),

    CHIP_START( 9602_CAP<D> )
        INPUT_PINS( D::LATCH1, i2 )
        OUTPUT_PIN( D::CAP )
        OUTPUT_DELAY_NS( 20.0, 20.0 ), // tp_lh will be overwritten

    CHIP_START( 9602_Q<D> )
        INPUT_PINS( D::RST, D::LATCH1, D::CAP )
        EVENT_PINS( D::CAP )
        OUTPUT_PIN( D::Q )
        PREV_OUTPUT_PIN( D::Q )
        OUTPUT_DELAY_NS( 2.5, 2.5 ), 

    CHIP_START( 9602_Q_n<D> )
        INPUT_PINS( D::Q )
        OUTPUT_PIN( D::Q_n )
        OUTPUT_DELAY_NS( 4.0, 4.0 ),

	CHIP_DESC_END
};

