namespace phoenix {

struct pKeyboard {
  static map<Keyboard::Scancode, unsigned> keymap;

  static void initialize();
  static bool pressed(Keyboard::Scancode scancode);
  static vector<bool> state();
};

}
