#ifndef SETTINGS_H
#define SETTINGS_H

#include <phoenix.hpp>

using namespace nall;

struct KeyAssignment
{
    enum InputType { NONE = 0, KEYBOARD, JOYSTICK_AXIS, JOYSTICK_BUTTON };

    unsigned type;
    unsigned button;
    unsigned joystick;

    KeyAssignment(unsigned t = NONE, unsigned b = 0, unsigned j = 0) :
        type(t), button(b), joystick(j) { }

    KeyAssignment(unsigned t, phoenix::Keyboard::Scancode s) :
        type(t), button((unsigned)s), joystick(0) { }

    nall::string name();

    static const char ScancodeName[][16];
    static const KeyAssignment None;
};

struct Settings : configuration
{
    nall::string filename;
    unsigned num_mice;

    bool pause, throttle;
    bool fullscreen;
    
    struct Audio
    {
        enum Frequency { FREQ_22500 = 0, FREQ_44100, FREQ_48000, FREQ_96000 };
        static const int FREQUENCIES[];

        unsigned frequency;
        unsigned volume; // Fixed point, tenths of percent
        bool mute;

    } audio;

    struct Video
    {
        enum Multisampling { NONE = 0, TWO_X, FOUR_X, EIGHT_X };

        unsigned multisampling;
        unsigned brightness, contrast;
        bool keep_aspect;
        bool vsync;
        bool status_visible;
    } video;

    struct Input
    {
        struct MouseAxis
        {
            unsigned mouse;
            unsigned axis;
        };

        struct JoystickAxis
        {
            unsigned joystick;
            unsigned axis;
        };

        enum JoystickMode { JOYSTICK_RELATIVE = 0, JOYSTICK_ABSOLUTE = 1 };

        struct Paddle
        {
            bool use_mouse;
            MouseAxis x_axis, y_axis;
            unsigned mouse_sensitivity;

            bool use_keyboard;
            unsigned keyboard_sensitivity;
            KeyAssignment up, down, left, right;

            bool use_joystick;
            JoystickAxis joy_x_axis, joy_y_axis;
            unsigned joystick_mode;
            unsigned joystick_sensitivity;
        } paddle[4];

        struct Wheel
        {
            bool use_mouse;
            MouseAxis axis;
            unsigned mouse_sensitivity;

            bool use_keyboard;
            unsigned keyboard_sensitivity;

            KeyAssignment left, right;
        } wheel[4];

        struct Throttle
        {
            KeyAssignment key;
            unsigned keyboard_sensitivity;
        } throttle[1];

        struct Joystick
        {
            KeyAssignment up, down, left, right;
        } joystick[2];

        struct Button
        {
            KeyAssignment button1, button2, button3;
        } buttons[6];

        struct CoinStart
        {
            KeyAssignment coin1, coin2, coin3, coin4, dollar, start1, start2;
        } coin_start;

        struct UserInterface
        {
            KeyAssignment pause, throttle, fullscreen, quit;
        } ui;

    } input;

    struct DefaultKeys
    {
        static phoenix::Keyboard::Scancode Up(int player);
        static phoenix::Keyboard::Scancode Down(int player);
        static phoenix::Keyboard::Scancode Left(int player);
        static phoenix::Keyboard::Scancode Right(int player);
        template <int> static phoenix::Keyboard::Scancode Button(int player);
    };

    Settings();

    void appendKey(KeyAssignment& k, string name);

    bool load()
    {
        configuration::load(filename);
        return save();  // Create file if it doesn't exist
    }

    bool save()
    {
        return configuration::save(filename);
    }

    template<size_t N> const Input::Joystick& joystick() const { return input.joystick[N]; }
    template<size_t N> const Input::Button& buttons() const { return input.buttons[N]; }
    const Input::CoinStart& coinStart() const { return input.coin_start; }
    const Input::UserInterface& ui() const { return input.ui; }
};

#endif
