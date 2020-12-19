#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iosfwd>

using namespace sf;

class MenuButton
{
public:
  Text text;
  RectangleShape button;
  Vector2f size;
  //button
  MenuButton(Texture* texture_, Vector2f textureRectSize, Vector2f beginPosTexture, Vector2f NextRect, Vector2f size, String name_, Font& font); 
  void setPos(MenuButton& buttonAbove, int interval);
  void setPos(RectangleShape& back, int interval);
  void setPos(Vector2f position);
  void setSizeRelativeText();
  bool listen(RenderWindow& window, Sound& soundClick, Sound& soundRoll);
private:
  String name;
  bool isClick;
  Vector2f beginRect;
  Vector2f sizeRect;
  Vector2f nextRect;
  IntRect container;
  void standart();
  void click();
  Clock clock;
  bool choose;
};
#endif

