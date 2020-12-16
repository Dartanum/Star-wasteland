#ifndef MENU_H
#define MENU_H

#include "MenuButton.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <string>

#include "settings.h"
#include "CheckBox.h"
#include "Rollover.h"

using namespace sf;
class Menu
{
public:
  Menu(settings& setting, Texture& texture, Texture& backText, Texture& background_, Vector2u& ScreenSize, Font& font);
  bool menu(RenderWindow& window, Texture& logotip);
private:
  String rules;
  SoundBuffer clickBuffer;
  SoundBuffer rolloverBuffer;
  Texture textures;
  Sprite background;
  Sprite logo;
  Vector2u screenSize;
  Vector2f buttonSize;
  Font font;
  std::vector<MenuButton> buttons;
  RectangleShape back;
private:
  void Rules(RenderWindow& window);
  void Settings(RenderWindow& window);
  double calcDistanceVolume(RectangleShape& slider, int volume);
  std::vector<Vector2f> chooseStateCheckbox(bool isSound);
  settings settings_;
};
#endif

