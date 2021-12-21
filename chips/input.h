#ifndef INPUT_H
#define INPUT_H

#include "../chip_desc.h"
#include "555mono.h"
#include <SDL/SDL_joystick.h>

struct KeyAssignment; // in settings.h

struct Joystick
{
    enum JOYSTICK { UP = 1, DOWN, LEFT, RIGHT };
    enum LIMIT { BUTTON_THRESHOLD = 8192 }; // TODO: make configurable?
};

struct Wheel
{
    enum OUTPUT { A = 1, B = 2 };
};

struct Coin
{
    enum COINS { COIN1_LO =  1, COIN2_LO =  2, COIN3_LO =  3, COIN4_LO =  4, 
                 COIN1_HI = i1, COIN2_HI = i2, COIN3_HI = i3, COIN4_HI = i4 };
    enum DOLLARS { DOLLAR_LO = 5, DOLLAR_HI = i5 };
};

template <unsigned PADDLE, bool HORIZONTAL>
class AnalogInputDesc
{
    double min_val, max_val, current_val;
    Mono555Desc* mono_555;

public:
    AnalogInputDesc(double min, double max, Mono555Desc* m) : min_val(min), max_val(max),
        current_val((max+min) / 2.0), mono_555(m) { }

    static CUSTOM_LOGIC( analog_input );
};

template <unsigned WHEEL>
class WheelDesc
{
public:
    double angle; // Wheel position, 0.0 to 360.0
    cirque<double> wheel_events[2];

    WheelDesc() : angle(0.0), wheel_events{cirque<double>(32), cirque<double>(32)} { }
};

template <unsigned THROTTLE>
class ThrottleDesc
{
    double* pos; // Throttle position, 0.0 to 100.0

public:
    ThrottleDesc(double* p) : pos(p) { }

    static CUSTOM_LOGIC( throttle_input );
};


typedef AnalogInputDesc<0, true> Paddle1HorizontalDesc; 
typedef AnalogInputDesc<1, true> Paddle2HorizontalDesc;
typedef AnalogInputDesc<2, true> Paddle3HorizontalDesc;
typedef AnalogInputDesc<3, true> Paddle4HorizontalDesc;

typedef AnalogInputDesc<0, false> Paddle1VerticalDesc; 
typedef AnalogInputDesc<1, false> Paddle2VerticalDesc; 
typedef AnalogInputDesc<2, false> Paddle3VerticalDesc; 
typedef AnalogInputDesc<3, false> Paddle4VerticalDesc; 

typedef WheelDesc<0> Wheel1Desc;
typedef WheelDesc<1> Wheel2Desc;
typedef WheelDesc<2> Wheel3Desc;
typedef WheelDesc<3> Wheel4Desc;

typedef ThrottleDesc<0> Throttle1Desc;

extern CHIP_DESC( PADDLE1_HORIZONTAL_INPUT );
extern CHIP_DESC( PADDLE2_HORIZONTAL_INPUT );
extern CHIP_DESC( PADDLE3_HORIZONTAL_INPUT );
extern CHIP_DESC( PADDLE4_HORIZONTAL_INPUT );

extern CHIP_DESC( PADDLE1_VERTICAL_INPUT );
extern CHIP_DESC( PADDLE2_VERTICAL_INPUT );
extern CHIP_DESC( PADDLE3_VERTICAL_INPUT );
extern CHIP_DESC( PADDLE4_VERTICAL_INPUT );

extern CHIP_DESC( THROTTLE1_INPUT );

extern CHIP_DESC( COIN_INPUT );
extern CHIP_DESC( START_INPUT );

extern CHIP_DESC( JOYSTICK1_INPUT );
extern CHIP_DESC( JOYSTICK2_INPUT );

extern CHIP_DESC( WHEEL1_INPUT );
extern CHIP_DESC( WHEEL2_INPUT );
extern CHIP_DESC( WHEEL3_INPUT );
extern CHIP_DESC( WHEEL4_INPUT );

extern CHIP_DESC( BUTTONS1_INPUT );
extern CHIP_DESC( BUTTONS2_INPUT );
extern CHIP_DESC( BUTTONS3_INPUT );
extern CHIP_DESC( BUTTONS4_INPUT );
extern CHIP_DESC( BUTTONS5_INPUT );
extern CHIP_DESC( BUTTONS6_INPUT );

// Creates a connection from the paddle to the 555 timer being adjusted
#define PADDLE_CONNECTION( c1, c2 ) CONNECTION(c1, i3, c2, i3)
#define THROTTLE_CONNECTION( c1, c2 ) CONNECTION(c1, i3, c2, i3)

// Suffix operators! For Buttons -- NO = Normally Open, NC = Normally Closed
inline constexpr uint8_t operator"" _NO ( unsigned long long pin ) { return pin; }
inline constexpr uint8_t operator"" _NC ( unsigned long long pin ) { return pin + i1 - 1; }

class Input
{
private:
    std::vector<int> mouse_rel_x, mouse_rel_y;
    std::vector<SDL_Joystick*> joysticks;
public:
    Input();
    ~Input();
    void poll_input();
    
    int getRelativeMouseX(unsigned mouse);
    int getRelativeMouseY(unsigned mouse);
    bool getKeyboardState(unsigned scancode);
    bool getJoystickButton(unsigned joystick, unsigned button);
    int16_t getJoystickAxis(unsigned joystick, unsigned axis);
    int getNumJoysticks();
    int getNumJoystickAxes(int joystick);
    bool getKeyPressed(const KeyAssignment& key_assignment);
};

#endif
