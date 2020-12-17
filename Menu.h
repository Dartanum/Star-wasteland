#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <string>

#include "settings.h"
#include "MenuButton.h"
#include "CheckBox.h"
#include "Rollover.h"
#include "textureLoader.h"
#include "TopResults.h"

using namespace sf;
class Menu
{
public:
  Menu(settings& setting, TopResults& records, textureLoader& textures, Vector2u& ScreenSize, Font& font, Font& recordFont);
  bool menu(RenderWindow& window);
  //bool endGameMenu(std::string pathTop, );
private:
  Clock clock;
  float delayFrame;
  String rules;
  SoundBuffer clickBuffer;
  SoundBuffer rolloverBuffer;
  Texture textures;
  Texture sliderTexture;
  Sprite background;
  Sprite logo;
  Vector2u screenSize;
  Vector2f buttonSize;
  Font font;
  Font topFont;
  std::vector<MenuButton> buttons;
  RectangleShape back;
  settings settings_;
  TopResults top;

  void Rules(RenderWindow& window);
  void Settings(RenderWindow& window);
  void Records(RenderWindow& window);
  std::vector<Vector2f> chooseStateCheckbox(bool isSound);
};
#endif

