#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iosfwd>

using namespace sf;

class MenuButton
{
public:
  String name;
  Text text;
  Vector2f size;
  Vector2f beginRect;
  Vector2f sizeRect;
  Vector2f nextRect;
  IntRect container;
  RectangleShape button;
  bool isClick;
  //button
  MenuButton(Texture& texture_, Vector2f textureRectSize, Vector2f beginPosTexture, Vector2f NextRect, Vector2f size, String name_, Font& font); 
  void setPos(MenuButton& buttonAbove, int interval);
  void setPos(RectangleShape& back, int interval);
  void setPos(Vector2f position);
  void setSizeRelativeText();
  void standart();
  void click();
  bool listen(RenderWindow& window, Sound& sound);
private:
  Clock clock;
};
#endif

