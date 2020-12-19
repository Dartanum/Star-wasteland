#include "CheckBox.h"

CheckBox::CheckBox(Texture* texture, Vector2f SizeRect, Vector2f BeginRect, Vector2f NextRect, Vector2f Size, bool State) {
  size = Size;
  state = State;
  button.setSize(size);
  nextRect = NextRect;
  button.setTexture(texture);
  button.setOrigin(size.x / 2, size.y / 2);
  beginRect = BeginRect;
  sizeRect = SizeRect;
  button.setTextureRect(IntRect(beginRect.x, beginRect.y, sizeRect.x, sizeRect.y));
  container = IntRect(button.getPosition().x - size.x / 2, button.getPosition().y - size.y / 2, size.x, size.y);
  beginState = IntRect(beginRect.x, beginRect.y, sizeRect.x, sizeRect.y);
  nextState = IntRect(nextRect.x, nextRect.y, sizeRect.x, sizeRect.y);
  button.setTextureRect(beginState);
}

void CheckBox::setPos(Vector2f position) {
  button.setPosition(position);
  container = IntRect(button.getPosition().x - size.x / 2, button.getPosition().y - size.y / 2, size.x, size.y);
}

void CheckBox::click() {
  state = !state;
  if(button.getTextureRect() == beginState) button.setTextureRect(nextState); 
  else button.setTextureRect(beginState);
}

bool CheckBox::listen(RenderWindow& window, Sound& sound) {
  int currentFrame = clock.getElapsedTime().asMilliseconds() / 16;
  bool chooseClose = false;
  if (container.contains(Mouse::getPosition(window))) {
    chooseClose = true;
  }
  else chooseClose = false;
  if (Mouse::isButtonPressed(Mouse::Left) && chooseClose && currentFrame > 30) {
    click();
    sound.play();
    clock.restart();
    return true;
  }
  else return false;
}