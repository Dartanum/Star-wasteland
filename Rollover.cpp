#include "Rollover.h"

Rollover::Rollover(Texture* textureLine, Texture* textureSlider, IntRect rectSlider, Vector2f sizeLine, int beginProcess, Font& font, int charSize) {
  line.setSize(sizeLine);
  line.setOrigin(0, sizeLine.y / 2);
  line.setTexture(textureLine);
  length = sizeLine.x;
  slider.setTexture(textureSlider);
  slider.setTextureRect(rectSlider);
  slider.setSize(Vector2f(line.getSize().x / 10, line.getSize().x / 8));
  slider.setOrigin(slider.getSize().x / 2, 0);
  process = beginProcess;
  processText.setFont(font);
  processText.setFillColor(Color::Black);
  processText.setString(std::to_string(process));
  processText.setCharacterSize(charSize);
  processText.setOrigin(processText.getGlobalBounds().width / 2, processText.getGlobalBounds().height / 2);
  isPressed = false;
}

void Rollover::setPos(Vector2f pos) {
  position = pos;
  line.setPosition(pos);
  processText.setPosition(position.x + length + slider.getSize().x * 2, position.y);
  slider.setPosition(setCurrentVolume(process), position.y);
  containerSlider = IntRect(slider.getPosition().x - slider.getSize().x / 2, slider.getPosition().y, slider.getSize().x, slider.getSize().y);
  containerLine = IntRect(line.getPosition().x, line.getPosition().y - line.getSize().y / 2, line.getSize().x, line.getSize().y);
  finishLineX = position.x + length;
}

void Rollover::updateContainer() {
  containerSlider = IntRect(slider.getPosition().x - slider.getSize().x / 2, slider.getPosition().y, slider.getSize().x, slider.getSize().y);
}

void Rollover::listen(RenderWindow& window, Event& event) {
  if (event.type == Event::MouseButtonPressed && containerSlider.contains(Mouse::getPosition(window)) && event.mouseButton.button == Mouse::Left) {
    slider.setFillColor(Color::Green);
    isPressed = true;
  }
  if (event.type == Event::MouseButtonReleased) {
    slider.setFillColor(Color::White);
    isPressed = false;
  }
  if (event.type == Event::MouseMoved && isPressed) {
    if (event.mouseMove.x >= finishLineX) {
      slider.setPosition(finishLineX, position.y);
    }
    else if (event.mouseMove.x <= position.x) {
      slider.setPosition(position.x, position.y);
    }
    else slider.setPosition(event.mouseMove.x, position.y);
    updateContainer();
    updateProcess();
  }
}

float Rollover::setCurrentVolume(int volume) {
  return line.getPosition().x + (length / 100.0 * volume);
}

int Rollover::getCurrentVolume(float currentPos) {
  return 100.0 / length * (slider.getPosition().x - position.x);
}

void Rollover::updateProcess() {
  process = getCurrentVolume(slider.getPosition().x);
  processText.setString(std::to_string(process));
}