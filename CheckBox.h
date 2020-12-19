#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iosfwd>

using namespace sf;

class CheckBox
{
public:
  Vector2f size;
  Vector2f beginRect;
  Vector2f sizeRect;
  Vector2f nextRect;
  IntRect container;
  IntRect beginState;
  IntRect nextState;
  RectangleShape button;
  Clock clock;
  bool state;

  CheckBox(Texture* texture, Vector2f SizeRect, Vector2f BeginRect, Vector2f NextRect, Vector2f Size, bool State);
  void setPos(Vector2f position);
  void click();
  bool listen(RenderWindow& window, Sound& sound);
};
#endif
