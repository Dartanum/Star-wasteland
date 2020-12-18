#include "Menu.h"

Menu::Menu(settings& _setting, TopResults& records, textureLoader& textures_, Vector2u& ScreenSize, Font& font_, Font& recordFont) {
  textures = textures_.menu["mainTile"];
  logo.setTexture(textures_.menu["logo"]);
  sliderTexture = textures_.menu["slider"];
  back.setTexture(&textures_.menu["panel"]);
  back.setTextureRect(IntRect(88, 33, 433, 538));
  background.setSize(Vector2f(ScreenSize.x, ScreenSize.y));
  background.setTexture(&textures_.backgrounds[0]);
  screenSize = ScreenSize;
  font = font_;
  topFont = recordFont;
  buttonSize = Vector2f(screenSize.x / 10, screenSize.y / 20);
  settings_ = _setting;
  top = records;
  delayFrame = 10;
  clickBuffer.loadFromFile("sounds/Menu/click1.ogg");
  rolloverBuffer.loadFromFile("sounds/Menu/rollover2.ogg");

  std::string line;
  std::ifstream reader("text/rules.ini");
  if (reader.is_open()) {
    while (getline(reader, line)) {
      rules += line + '\n';
    }
  }
  reader.close();
}

bool Menu::menu(RenderWindow& window) {
  bool isMenu = true;
  int interval = 10;
  int countBut = 5;
  int butNum = -1;

  String names[] = {L"play", L"rules", L"records", L"settings", L"exit"};
  for (ptrdiff_t i = 0; i < countBut; i++) {
    buttons.push_back(MenuButton(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 49), buttonSize, names[i], font));
  }

  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);

  back.setSize(Vector2f(buttonSize.x + interval * 2, (buttonSize.y + interval) * countBut + interval));
  back.setOrigin(back.getSize().x / 2, back.getSize().y / 2);
  back.setPosition(screenSize.x / 2, screenSize.y / 2 + back.getSize().y / 2);
  Vector2f subMenuPos(back.getPosition().x, back.getPosition().y);
  logo.setOrigin(logo.getGlobalBounds().width/2, logo.getGlobalBounds().height / 2);
  logo.setPosition(screenSize.x/2, screenSize.y/3);
  buttons[0].setPos(back, interval);
  for (ptrdiff_t i = 1; i < buttons.size(); i++) {
    buttons[i].setPos(buttons[i-1], interval);
  }
  int currentFrame = 0;
//------------------------------Ã≈Õﬁ-------------------------------------
  while (isMenu) {
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
    }
    currentFrame = clock.getElapsedTime().asMilliseconds() / 16;
    if (currentFrame > delayFrame) {
      window.clear();
      window.draw(background);
      window.draw(logo);
      window.draw(back);
      for (ptrdiff_t i = 0; i < buttons.size(); i++) {
        if (buttons[i].listen(window, clickSound, rolloverSound))
          butNum = i;
        else butNum = -1;
        window.draw(buttons[i].button);
        window.draw(buttons[i].text);
        switch (butNum) {
        case -1: continue;
        case 0:
          isMenu = false;
          break;
        case 1:
          Rules(window);
          clock.restart();
          break;
        case 2:
          Records(window);
          clock.restart();
          break;
        case 3:
          Settings(window);
          clock.restart();
          break;
        case 4:
          window.close();
          return true;
        }
        if (butNum != -1) break;
      }
      window.display();
    }
  }
  return false;
}

void Menu::Rules(RenderWindow& window) {
  bool isOpen = true;
  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);
  Text ruless;
  ruless.setCharacterSize(20);
  ruless.setFillColor(Color::Black);
  ruless.setFont(font);
  ruless.setString(rules);
  MenuButton ButtonClose(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 49), buttonSize, L"close", font);
  Vector2f interval(ruless.getGlobalBounds().width / 10, ruless.getGlobalBounds().height / 10);
  RectangleShape listBack(Vector2f(ruless.getGlobalBounds().width + interval.x, ruless.getGlobalBounds().height + interval.y));
  listBack.setTexture(back.getTexture());
  listBack.setTextureRect(back.getTextureRect());
  listBack.setOrigin(listBack.getGlobalBounds().width / 2, listBack.getGlobalBounds().height / 2);
  listBack.setPosition(screenSize.x / 2, screenSize.y / 2);
  ruless.setOrigin(ruless.getGlobalBounds().width / 2, ruless.getGlobalBounds().height / 2);
  ruless.setPosition(listBack.getPosition());
  ButtonClose.setPos(Vector2f(listBack.getPosition().x + listBack.getGlobalBounds().width / 2 - buttonSize.x / 2, listBack.getPosition().y + listBack.getGlobalBounds().height / 2 + buttonSize.y / 2));

  while (isOpen) {
    isOpen = !ButtonClose.listen(window, clickSound, rolloverSound);
    window.clear();
    window.draw(background);
    window.draw(listBack);
    window.draw(ruless);
    window.draw(ButtonClose.button);
    window.draw(ButtonClose.text);
    window.display();
  }
}

void Menu::Records(RenderWindow& window) {
  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);
  int charSize = 25;
  top.makeView(charSize, topFont, Color::Black);
  Text text = top.text;
  int interval = 50;
  int intervalBetweenButton = 10;
  RectangleShape listBack(Vector2f(text.getGlobalBounds().width + interval, text.getGlobalBounds().height + interval));
  listBack.setOrigin(listBack.getSize().x / 2, listBack.getSize().y / 2);
  listBack.setPosition(screenSize.x / 2, screenSize.y / 2);
  listBack.setTexture(back.getTexture());
  listBack.setTextureRect(back.getTextureRect());
  text.setPosition(listBack.getPosition());
  MenuButton ButtonClose(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 49), buttonSize, L"close", font);
  ButtonClose.setPos(Vector2f(listBack.getPosition().x, listBack.getPosition().y + listBack.getSize().y / 2 + ButtonClose.button.getSize().y / 2 + intervalBetweenButton));
  bool isOpen = true;
  while (isOpen) {
    isOpen = !ButtonClose.listen(window, clickSound, rolloverSound);
    window.clear();
    window.draw(background);
    window.draw(listBack);
    window.draw(text);
    window.draw(ButtonClose.button);
    window.draw(ButtonClose.text);
    window.display();
  }
}

void Menu::Settings(RenderWindow& window) {
  settings_.read();
  std::vector<Vector2f> rects;
  //“≈ —“
  String soundS, musicS, volumeS;
  soundS = "sound"; musicS = "music"; volumeS = "volume";
  int charSize = 20;
  std::map<String, Text> texts;
  std::map<String, Text>::iterator it;
  texts[soundS] = Text();
  texts[musicS] = Text();
  texts[volumeS] = Text();
  for (it = texts.begin(); it != texts.end(); it++) {
    it->second.setCharacterSize(charSize);
    it->second.setFillColor(Color::Black);
    it->second.setFont(font);
    it->second.setString(it->first);
    it->second.setOrigin(it->second.getLocalBounds().width / 2, it->second.getLocalBounds().height / 2);
  }
  //›ÎÂÏÂÌÚ˚ ËÌÚÂÙÂÈÒ‡
  RectangleShape listBack(Vector2f(screenSize.x / 4, screenSize.y / 4));
  Vector2f sizeBut = Vector2f(listBack.getSize().x / 4, listBack.getSize().y / 8);
  MenuButton ButtonClose(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 50), sizeBut, L"close", font);
  MenuButton ButtonSave(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 50), sizeBut, L"save", font);
  Rollover lineS(sliderTexture, textures, IntRect(388, 72, 28, 39), Vector2f(listBack.getSize().x / 3, sliderTexture.getSize().y), settings_.soundVolume, font, charSize);
  Rollover lineM(sliderTexture, textures, IntRect(388, 72, 28, 39), Vector2f(listBack.getSize().x / 3, sliderTexture.getSize().y), settings_.musicVolume, font, charSize);
  rects = chooseStateCheckbox(true);
  CheckBox checkboxS(textures, Vector2f(36, 36), rects[0], rects[1], Vector2f(texts[soundS].getGlobalBounds().width / 2, texts[soundS].getGlobalBounds().width / 2), settings_.soundIsOn);
  rects = chooseStateCheckbox(false);
  CheckBox checkboxM(textures, Vector2f(36, 36), rects[0], rects[1], checkboxS.size, settings_.musicIsOn);
  listBack.setTexture(back.getTexture());
  listBack.setTextureRect(back.getTextureRect());
  listBack.setOrigin(listBack.getLocalBounds().width / 2, listBack.getLocalBounds().height / 2);
  listBack.setPosition(screenSize.x / 2, screenSize.y / 2);
  Vector2f interval(listBack.getSize().x / 20, listBack.getSize().y / 20);
  ButtonSave.setPos(Vector2f(listBack.getPosition().x - listBack.getSize().x / 2 + ButtonSave.size.x / 2 + interval.x, listBack.getPosition().y + listBack.getSize().y / 2 - ButtonSave.size.y / 2 - interval.y));
  ButtonClose.setPos(Vector2f(listBack.getPosition().x + listBack.getSize().x / 2 - ButtonClose.size.x / 2 - interval.x, ButtonSave.button.getPosition().y));
  //Õ¿—“–Œ… » «¬” ¿
  texts[soundS].setPosition(listBack.getPosition().x - listBack.getSize().x / 2 + texts[soundS].getLocalBounds().width / 2 + interval.x,
                            listBack.getPosition().y - listBack.getSize().y / 2 + texts[soundS].getLocalBounds().height / 2 + interval.y);
  checkboxS.setPos(Vector2f(texts[soundS].getPosition().x, texts[soundS].getPosition().y + checkboxS.size.y + interval.y));
  texts[volumeS].setPosition(listBack.getPosition().x + listBack.getSize().x / 2 - texts[volumeS].getLocalBounds().width / 2 - interval.x, texts[soundS].getPosition().y);
  lineS.setPos(Vector2f(listBack.getPosition().x, listBack.getPosition().y - listBack.getSize().y / 2 + texts[volumeS].getLocalBounds().height * 4));
  //Õ¿—“–Œ… » Ã”«€ »
  int intervalY = listBack.getSize().y / 2 - interval.y;
  texts[musicS].setPosition(texts[soundS].getPosition().x, texts[soundS].getPosition().y + intervalY);
  checkboxM.setPos(Vector2f(checkboxS.button.getPosition().x, checkboxS.button.getPosition().y + intervalY));
  lineM.setPos(Vector2f(lineS.position.x, lineS.position.y + intervalY));

  bool isOpen = true;
  settings newSettings(settings_);
  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);
  while (isOpen)
  {
    if (checkboxS.listen(window, clickSound)) {
      newSettings.soundIsOn = !newSettings.soundIsOn;
    }
    if (checkboxM.listen(window, clickSound)) {
      newSettings.musicIsOn = !newSettings.musicIsOn;
    }
    if (ButtonClose.listen(window, clickSound, rolloverSound)) {
      isOpen = false;
    }
    else if (ButtonSave.listen(window, clickSound, rolloverSound)) {
      newSettings.soundVolume = lineS.process;
      newSettings.musicVolume = lineM.process;
      settings_ = newSettings;
      settings_.write();
    }
    if(Mouse::isButtonPressed && lineS.containerSlider.contains(Mouse::getPosition(window)))
      lineS.listen(window);
    else if(Mouse::isButtonPressed && lineM.containerSlider.contains(Mouse::getPosition(window)))
    lineM.listen(window);
    window.clear();
    window.draw(background);
    window.draw(listBack);
    for (it = texts.begin(); it != texts.end(); it++) {
      window.draw(it->second);
    }
    window.draw(ButtonSave.button);
    window.draw(ButtonSave.text);
    window.draw(ButtonClose.button);
    window.draw(ButtonClose.text);
    window.draw(checkboxS.button);
    window.draw(checkboxM.button);
    window.draw(lineS.line);
    window.draw(lineM.line);
    window.draw(lineS.slider);
    window.draw(lineM.slider);
    window.draw(lineS.processText);
    window.draw(lineM.processText);
    texts[volumeS].setPosition(texts[volumeS].getPosition().x, texts[volumeS].getPosition().y + intervalY);
    window.draw(texts[volumeS]);
    texts[volumeS].setPosition(texts[volumeS].getPosition().x, texts[volumeS].getPosition().y - intervalY);
    window.display();
  }
}

std::vector<Vector2f> Menu::chooseStateCheckbox(bool isSound) {
  Vector2f beginRect;
  Vector2f nextRect;
  std::vector<Vector2f> result;
  if (isSound) {
    if (settings_.soundIsOn) {
      beginRect = Vector2f(381, 36);
      nextRect = Vector2f(381, 0);
    }
    else {
      beginRect = Vector2f(381, 0);
      nextRect = Vector2f(381, 36);
    }
  }
  else {
    if (settings_.musicIsOn) {
      beginRect = Vector2f(381, 36);
      nextRect = Vector2f(381, 0);
    }
    else {
      beginRect = Vector2f(381, 0);
      nextRect = Vector2f(381, 36);
    }
  }
  result.push_back(beginRect);
  result.push_back(nextRect);
  return result;
}

bool Menu::endGameMenu(RenderWindow& window, int gameTime, int pointsCount) {
  //Text text;
  //text.setFont(font);
  //text.setCharacterSize(20);
  //text.setFillColor(Color::Black);
  //String str;
  //str = "points: " + std::to_string(pointsCount);
}