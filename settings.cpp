#include "settings.h"

using namespace phoenix;

const char KeyAssignment::ScancodeName[][16] = {
    "None",
    "Escape", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
    "PrintScreen", "ScrollLock", "Pause",
    "Insert", "Delete", "Home", "End", "PageUp", "PageDown",
    "Up", "Down", "Left", "Right",
    
    "~", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace",
    "[", "]", "\\", ";", "'", ",", ".", "/",
    "Tab", "CapsLock", "Enter", "L Shift", "R Shift", "L Control", "R Control", "L Super", "R Super", "L Alt", "R Alt", "Spacebar", "Menu",
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "NumLock", "Keypad /", "Keypad *", "Keypad -", "Keypad +", "Keypad Enter", "Keypad .",      
    "Keypad 1", "Keypad 2", "Keypad 3", "Keypad 4", "Keypad 5", "Keypad 6", "Keypad 7", "Keypad 8", "Keypad 9", "Keypad 0"  
};

const KeyAssignment KeyAssignment::None(KeyAssignment::NONE, 0, 0);

const int Settings::Audio::FREQUENCIES[] = { 22500, 44100, 48000, 96000 };

nall::string KeyAssignment::name()
{
    if(type == KEYBOARD)
    {
        if(button > (unsigned)Keyboard::Scancode::Limit)
            button = (unsigned)Keyboard::Scancode::None;
        return ScancodeName[button];
    }
    
    if(type == JOYSTICK_AXIS)
    {
        unsigned axis = button >> 1;
        const char* dir = (button & 1) ? "+" : "-";
        if(axis == 0)
            return {"Joy ", joystick+1, " X-Axis", dir};
        else if(axis == 1)
            return {"Joy ", joystick+1, " Y-Axis", dir};
        else
            return {"Joy ", joystick+1, " Axis", axis+1, dir};
    }

    if(type == JOYSTICK_BUTTON)
        return {"Joy ", joystick+1, " Button ", button+1};

    return "None";
}


Keyboard::Scancode Settings::DefaultKeys::Up(int player)
{
    return player == 1 ? Keyboard::Scancode::Up :
           player == 2 ? Keyboard::Scancode::W :
           player == 3 ? Keyboard::Scancode::Keypad8 :
                         Keyboard::Scancode::I;
}

Keyboard::Scancode Settings::DefaultKeys::Down(int player)
{
    return player == 1 ? Keyboard::Scancode::Down :
           player == 2 ? Keyboard::Scancode::S :
           player == 3 ? Keyboard::Scancode::Keypad5 :
                         Keyboard::Scancode::K;
}


Keyboard::Scancode Settings::DefaultKeys::Left(int player)
{
    return player == 1 ? Keyboard::Scancode::Left :
           player == 2 ? Keyboard::Scancode::A :
           player == 3 ? Keyboard::Scancode::Keypad4 :
                         Keyboard::Scancode::J;
}

Keyboard::Scancode Settings::DefaultKeys::Right(int player)
{
    return player == 1 ? Keyboard::Scancode::Right :
           player == 2 ? Keyboard::Scancode::D :
           player == 3 ? Keyboard::Scancode::Keypad6 :
                         Keyboard::Scancode::L;
}

template <> Keyboard::Scancode Settings::DefaultKeys::Button<1>(int player)
{
    return player == 1 ? Keyboard::Scancode::ControlLeft :
           player == 2 ? Keyboard::Scancode::G :
           player == 3 ? Keyboard::Scancode::Keypad0 :
           player == 4 ? Keyboard::Scancode::ControlRight :
           player == 5 ? Keyboard::Scancode::Z :
                         Keyboard::Scancode::B;
}

template <> Keyboard::Scancode Settings::DefaultKeys::Button<2>(int player)
{
    return player == 1 ? Keyboard::Scancode::AltLeft :
           player == 2 ? Keyboard::Scancode::H :
           player == 3 ? Keyboard::Scancode::Point :
           player == 4 ? Keyboard::Scancode::ShiftRight :
           player == 5 ? Keyboard::Scancode::X :
                         Keyboard::Scancode::N;
}

template <> Keyboard::Scancode Settings::DefaultKeys::Button<3>(int player)
{
    return player == 1 ? Keyboard::Scancode::Space :
           player == 2 ? Keyboard::Scancode::J :
           player == 3 ? Keyboard::Scancode::Enter :
           player == 4 ? Keyboard::Scancode::Return :
           player == 5 ? Keyboard::Scancode::C :
                         Keyboard::Scancode::M;
}

void Settings::appendKey(KeyAssignment& k, string name)
{
    append(k.type, string{name, ".type"});
    append(k.button, string{name, ".button"});
    append(k.joystick, string{name, ".joystick"});
}

Settings::Settings() : num_mice(0), pause(false), throttle(true), fullscreen(false)
{
    append(audio.frequency = Audio::Frequency::FREQ_48000, "audio.frequency");
    append(audio.volume = 500, "audio.volume");
    append(audio.mute = false, "audio.mute");

    append(video.status_visible = true, "video.status_visible");
    append(video.keep_aspect = true, "video.keep_aspect");
    append(video.multisampling = Video::FOUR_X, "video.multisampling");
    append(video.vsync = false, "video.vsync");

    // Paddles
    unsigned num = 1;
    for(Input::Paddle& paddle : input.paddle)
    {
        string paddle_name = {"input.paddle", num};

        append(paddle.use_mouse = (num == 1),  string{paddle_name, ".use_mouse"});
        append(paddle.x_axis.mouse = 0,        string{paddle_name, ".x_axis.mouse"});
        append(paddle.x_axis.axis  = 0,        string{paddle_name, ".x_axis.axis"});
        append(paddle.y_axis.mouse = 0,        string{paddle_name, ".y_axis.mouse"});
        append(paddle.y_axis.axis  = 1,        string{paddle_name, ".y_axis.axis"});
        append(paddle.mouse_sensitivity = 500, string{paddle_name, ".mouse_sensitivity"});

        append(paddle.use_keyboard = true, string{paddle_name, ".use_keyboard"});
        appendKey(paddle.up =    KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Up(num)),    string{paddle_name, ".up"});
        appendKey(paddle.down =  KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Down(num)),  string{paddle_name, ".down"});
        appendKey(paddle.left =  KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Left(num)),  string{paddle_name, ".left"});
        appendKey(paddle.right = KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Right(num)), string{paddle_name, ".right"});
        append(paddle.keyboard_sensitivity = 250, string{paddle_name, ".keyboard_sensitivity"});

        append(paddle.use_joystick = false,       string{paddle_name, ".use_joystick"});
        append(paddle.joy_x_axis.joystick = 0,    string{paddle_name, ".joy_x_axis.joystick"});
        append(paddle.joy_x_axis.axis = 0,        string{paddle_name, ".joy_x_axis.axis"});
        append(paddle.joy_y_axis.joystick = 0,    string{paddle_name, ".joy_y_axis.joystick"});
        append(paddle.joy_y_axis.axis = 1,        string{paddle_name, ".joy_y_axis.axis"});
        append(paddle.joystick_mode = 0,          string{paddle_name, ".joystick_mode"});
        append(paddle.joystick_sensitivity = 500, string{paddle_name, ".joystick_sensitivity"});

        num++;
    }

    // Throttle 1
    appendKey(input.throttle[0].key = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Up), "input.throttle1.key");
    append(input.throttle[0].keyboard_sensitivity = 250, "input.throttle1.keyboard_sensitivity");

    // Joysticks
    num = 1;
    for(Input::Joystick& joystick : input.joystick)
    {
        string joystick_name = {"input.joystick", num};

        appendKey(joystick.up =    KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Up(num)),    string{joystick_name, ".up"});
        appendKey(joystick.down =  KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Down(num)),  string{joystick_name, ".down"});
        appendKey(joystick.left =  KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Left(num)),  string{joystick_name, ".left"});
        appendKey(joystick.right = KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Right(num)), string{joystick_name, ".right"});

        num++;
    }

    // Wheels
    num = 1;
    for(Input::Wheel& wheel : input.wheel)
    {
        string wheel_name = {"input.wheel", num};

        append(wheel.use_mouse = (num == 1),  string{wheel_name, ".use_mouse"});
        append(wheel.axis.mouse = 0,          string{wheel_name, ".axis.mouse"});
        append(wheel.axis.axis  = 0,          string{wheel_name, ".axis.axis"});
        append(wheel.mouse_sensitivity = 500, string{wheel_name, ".mouse_sensitivity"});

        append(wheel.use_keyboard = true, string{wheel_name, ".use_keyboard"});
        appendKey(wheel.left =  KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Left(num)),  string{wheel_name, ".left"});
        appendKey(wheel.right = KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Right(num)), string{wheel_name, ".right"});
        append(wheel.keyboard_sensitivity = 500, string{wheel_name, ".keyboard_sensitivity"});

        num++;
    }

    // Buttons
    num = 1;
    for(Input::Button& buttons : input.buttons)
    {
        string button_name = {"input.player", num};
        appendKey(buttons.button1 = KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Button<1>(num)), string{button_name, ".button1"});
        appendKey(buttons.button2 = KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Button<2>(num)), string{button_name, ".button2"});
        appendKey(buttons.button3 = KeyAssignment(KeyAssignment::KEYBOARD, DefaultKeys::Button<3>(num)), string{button_name, ".button3"});
        num++;
    }

    appendKey(input.coin_start.coin1 = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Number5), "input.coin1");
    appendKey(input.coin_start.coin2 = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Number6), "input.coin2");
    appendKey(input.coin_start.coin3 = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Number7), "input.coin3");
    appendKey(input.coin_start.coin4 = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Number8), "input.coin4");
    appendKey(input.coin_start.dollar = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Backspace), "input.dollar");

    appendKey(input.coin_start.start1 = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Number1), "input.start1");
    appendKey(input.coin_start.start2 = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Number2), "input.start2");

    // User Interface
    appendKey(input.ui.pause = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::P), "input.ui.pause");
    appendKey(input.ui.throttle = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Insert), "input.ui.throttle");
    appendKey(input.ui.fullscreen = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::F11), "input.ui.fullscreen");
    appendKey(input.ui.quit = KeyAssignment(KeyAssignment::KEYBOARD, Keyboard::Scancode::Escape), "input.ui.quit");
} 

