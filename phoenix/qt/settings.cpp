namespace phoenix {

static Settings *settings = nullptr;

void Settings::load() {
  string path = {userpath(), ".config/phoenix/"};
  configuration::load({path, "qt.cfg"});
}

void Settings::save() {
  string path = {userpath(), ".config/phoenix/"};
  directory::create(path, 0755);
  configuration::save({path, "qt.cfg"});
}

Settings::Settings() {
  append(geometry.frameX = 4, "FrameX");
  append(geometry.frameY = 24, "FrameY");
  append(geometry.frameWidth = 8, "FrameWidth");
  append(geometry.frameHeight = 28, "FrameHeight");
  append(geometry.menuHeight = 20, "MenuHeight");
  append(geometry.statusHeight = 20, "StatusHeight");
}

}
