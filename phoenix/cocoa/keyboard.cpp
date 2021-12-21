namespace phoenix {

#include <Carbon/Carbon.h>

map<Keyboard::Scancode, unsigned> pKeyboard::keymap;

bool pKeyboard::pressed(Keyboard::Scancode scancode) {
  KeyMap keys;
  GetKeys(keys);
  uint32_t* k = (uint32_t*)keys;
  unsigned id = keymap.find(scancode)();
  return (k[id >> 5] >> (id & 31)) & 1;
}

vector<bool> pKeyboard::state() {
  vector<bool> output;
  output.resize((unsigned)Keyboard::Scancode::Limit);
  for(auto& n : output) n = false;

  KeyMap keys;
  GetKeys(keys);
  uint32_t* k = (uint32_t*)keys;

  for(auto node : keymap)
    output[(unsigned)node.key] = (k[node.value >> 5] >> (node.value & 31)) & 1;

  return output;
}

void pKeyboard::initialize() {
  auto append = [](Keyboard::Scancode scancode, unsigned keysym) {
    keymap.insert(scancode, keysym);
  };

    #define map(id, name) append(name, id)
    map(0x35, Keyboard::Scancode::Escape);

    map(0x7a, Keyboard::Scancode::F1);
    map(0x78, Keyboard::Scancode::F2);
    map(0x63, Keyboard::Scancode::F3);
    map(0x76, Keyboard::Scancode::F4);
    map(0x60, Keyboard::Scancode::F5);
    map(0x61, Keyboard::Scancode::F6);
    map(0x62, Keyboard::Scancode::F7);
    map(0x64, Keyboard::Scancode::F8);
    map(0x65, Keyboard::Scancode::F9);
    map(0x6d, Keyboard::Scancode::F10);
    map(0x67, Keyboard::Scancode::F11);
    map(0x6f, Keyboard::Scancode::F12);

    map(0x69, Keyboard::Scancode::PrintScreen);
  //map(0x??, Keyboard::Scancode::ScrollLock);
    map(0x71, Keyboard::Scancode::Pause);

    map(0x32, Keyboard::Scancode::Grave);
    map(0x12, Keyboard::Scancode::Number1);
    map(0x13, Keyboard::Scancode::Number2);
    map(0x14, Keyboard::Scancode::Number3);
    map(0x15, Keyboard::Scancode::Number4);
    map(0x17, Keyboard::Scancode::Number5);
    map(0x16, Keyboard::Scancode::Number6);
    map(0x1a, Keyboard::Scancode::Number7);
    map(0x1c, Keyboard::Scancode::Number8);
    map(0x19, Keyboard::Scancode::Number9);
    map(0x1d, Keyboard::Scancode::Number0);

    map(0x1b, Keyboard::Scancode::Minus);
    map(0x18, Keyboard::Scancode::Equal);
    map(0x33, Keyboard::Scancode::Backspace);

    map(0x72, Keyboard::Scancode::Insert);
    map(0x75, Keyboard::Scancode::Delete);
    map(0x73, Keyboard::Scancode::Home);
    map(0x77, Keyboard::Scancode::End);
    map(0x74, Keyboard::Scancode::PageUp);
    map(0x79, Keyboard::Scancode::PageDown);

    map(0x00, Keyboard::Scancode::A);
    map(0x0b, Keyboard::Scancode::B);
    map(0x08, Keyboard::Scancode::C);
    map(0x02, Keyboard::Scancode::D);
    map(0x0e, Keyboard::Scancode::E);
    map(0x03, Keyboard::Scancode::F);
    map(0x05, Keyboard::Scancode::G);
    map(0x04, Keyboard::Scancode::H);
    map(0x22, Keyboard::Scancode::I);
    map(0x26, Keyboard::Scancode::J);
    map(0x28, Keyboard::Scancode::K);
    map(0x25, Keyboard::Scancode::L);
    map(0x2e, Keyboard::Scancode::M);
    map(0x2d, Keyboard::Scancode::N);
    map(0x1f, Keyboard::Scancode::O);
    map(0x23, Keyboard::Scancode::P);
    map(0x0c, Keyboard::Scancode::Q);
    map(0x0f, Keyboard::Scancode::R);
    map(0x01, Keyboard::Scancode::S);
    map(0x11, Keyboard::Scancode::T);
    map(0x20, Keyboard::Scancode::U);
    map(0x09, Keyboard::Scancode::V);
    map(0x0d, Keyboard::Scancode::W);
    map(0x07, Keyboard::Scancode::X);
    map(0x10, Keyboard::Scancode::Y);
    map(0x06, Keyboard::Scancode::Z);

    map(0x21, Keyboard::Scancode::BracketLeft);
    map(0x1e, Keyboard::Scancode::BracketRight);
    map(0x2a, Keyboard::Scancode::Backslash);
    map(0x29, Keyboard::Scancode::Semicolon);
    map(0x27, Keyboard::Scancode::Apostrophe);
    map(0x2b, Keyboard::Scancode::Comma);
    map(0x2f, Keyboard::Scancode::Period);
    map(0x2c, Keyboard::Scancode::Slash);

    map(0x53, Keyboard::Scancode::Keypad1);
    map(0x54, Keyboard::Scancode::Keypad2);
    map(0x55, Keyboard::Scancode::Keypad3);
    map(0x56, Keyboard::Scancode::Keypad4);
    map(0x57, Keyboard::Scancode::Keypad5);
    map(0x58, Keyboard::Scancode::Keypad6);
    map(0x59, Keyboard::Scancode::Keypad7);
    map(0x5b, Keyboard::Scancode::Keypad8);
    map(0x5c, Keyboard::Scancode::Keypad9);
    map(0x52, Keyboard::Scancode::Keypad0);

    map(0x41, Keyboard::Scancode::Point);
    map(0x4c, Keyboard::Scancode::Enter);
    map(0x45, Keyboard::Scancode::Add);
    map(0x4e, Keyboard::Scancode::Subtract);
    map(0x43, Keyboard::Scancode::Multiply);
    map(0x4b, Keyboard::Scancode::Divide);

    map(0x47, Keyboard::Scancode::NumLock);
    map(0x39, Keyboard::Scancode::CapsLock);

    map(0x7e, Keyboard::Scancode::Up);
    map(0x7d, Keyboard::Scancode::Down);
    map(0x7b, Keyboard::Scancode::Left);
    map(0x7c, Keyboard::Scancode::Right);

    map(0x30, Keyboard::Scancode::Tab);
    map(0x24, Keyboard::Scancode::Return);
    map(0x31, Keyboard::Scancode::Space);
  //map(0x??, Keyboard::Scancode::Menu);

    map(0x38, Keyboard::Scancode::ShiftLeft);
    map(0x3b, Keyboard::Scancode::ControlLeft);
    map(0x3a, Keyboard::Scancode::AltLeft);
    map(0x37, Keyboard::Scancode::SuperLeft);

    map(0x3c, Keyboard::Scancode::ShiftRight);
    map(0x3e, Keyboard::Scancode::ControlRight);
    map(0x3d, Keyboard::Scancode::AltRight);
    //map(0x37, Keyboard::Scancode::SuperRight);
    #undef map
  }
}
