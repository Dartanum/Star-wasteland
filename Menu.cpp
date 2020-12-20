#include "Menu.h"

Menu::Menu(settings& _setting, TopResults& records, textureLoader& textures_, Vector2u& ScreenSize, Font& font_, Font& recordFont) {
  textures = textures_.menu["mainTile"];
  logo.setTexture(*textures_.menu["logo"]);
  sliderTexture = textures_.menu["slider"];
  back.setTexture(textures_.menu["panel"]);
  back.setTextureRect(IntRect(88, 33, 433, 538));
  background.setSize(Vector2f(ScreenSize.x, ScreenSize.y));
  background.setTexture(textures_.backgrounds[0]);
  screenSize = ScreenSize;
  font = font_;
  topFont = recordFont;
  buttonSize = Vector2f(screenSize.x / 10, screenSize.y / 20);
  settings_ = _setting;
  top = records;
  delayFrame = 10;
  clickBuffer.loadFromFile("src/sounds/Menu/click1.ogg");
  rolloverBuffer.loadFromFile("src/sounds/Menu/rollover2.ogg");

  std::string line;
  std::ifstream reader("src/text/rules.ini");
  if (reader.is_open()) {
    while (getline(reader, line)) {
      rules += line + '\n';
    }
  }
  reader.close();
  buildMainMenu();
}

void Menu::buildMainMenu() {
  int interval = 10;
  int countBut = 5;
  String names[] = { L"play", L"rules", L"records", L"settings", L"exit" };
  for (ptrdiff_t i = 0; i < countBut; i++) {
    buttons.push_back(MenuButton(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 49), buttonSize, names[i], font));
  }
  back.setSize(Vector2f(buttonSize.x + interval * 2, (buttonSize.y + interval) * countBut + interval));
  back.setOrigin(back.getSize().x / 2, back.getSize().y / 2);
  back.setPosition(screenSize.x / 2, screenSize.y / 2 + back.getSize().y / 2);
  Vector2f subMenuPos(back.getPosition().x, back.getPosition().y);
  logo.setOrigin(logo.getGlobalBounds().width / 2, logo.getGlobalBounds().height / 2);
  logo.setPosition(screenSize.x / 2, screenSize.y / 3);
  buttons[0].setPos(back, interval);
  for (ptrdiff_t i = 1; i < buttons.size(); i++) {
    buttons[i].setPos(buttons[i - 1], interval);
  }
}

bool Menu::menu(RenderWindow& window) {
  bool isMenu = true;
  int countBut = 5;
  int butNum = -1;
  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);
  int currentFrame = 0;
  clock.restart();
//------------------------------Ã≈Õﬁ-------------------------------------
  while (isMenu) {
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
    }
    currentFrame = clock.getElapsedTime().asMilliseconds() / 16;
    butNum = -1;
    if (currentFrame > delayFrame) {
      window.clear();
      window.draw(background);
      window.draw(logo);
      window.draw(back);
      for (ptrdiff_t i = 0; i < buttons.size(); i++) {
        if (buttons[i].listen(window, clickSound, rolloverSound))
          butNum = i;
        window.draw(buttons[i].button);
        window.draw(buttons[i].text);
      }
      window.display();
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
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
    }
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
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
    }
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
  String soundS, musicS, volumeS, name, nick;
  soundS = "sound"; musicS = "music"; volumeS = "volume"; name = "Name:"; nick = settings_.nickname;
  int charSize = 20;
  std::map<String, Text> texts;
  std::map<String, Text>::iterator it;
  texts[soundS] = Text();
  texts[musicS] = Text();
  texts[volumeS] = Text();
  texts[name] = Text();
  texts[nick] = Text();
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
  Rollover lineS(sliderTexture, textures, IntRect(388, 72, 28, 39), Vector2f(listBack.getSize().x / 3, sliderTexture->getSize().y), settings_.soundVolume, font, charSize);
  Rollover lineM(sliderTexture, textures, IntRect(388, 72, 28, 39), Vector2f(listBack.getSize().x / 3, sliderTexture->getSize().y), settings_.musicVolume, font, charSize);
  rects = chooseStateCheckbox(true);
  CheckBox checkboxS(textures, Vector2f(36, 36), rects[0], rects[1], Vector2f(texts[soundS].getGlobalBounds().width / 2, texts[soundS].getGlobalBounds().width / 2), settings_.soundIsOn);
  rects = chooseStateCheckbox(false);
  CheckBox checkboxM(textures, Vector2f(36, 36), rects[0], rects[1], checkboxS.size, settings_.musicIsOn);
  listBack.setTexture(back.getTexture());
  listBack.setTextureRect(back.getTextureRect());
  listBack.setOrigin(listBack.getLocalBounds().width / 2, listBack.getLocalBounds().height / 2);
  listBack.setPosition(screenSize.x / 2, screenSize.y / 2);
  Vector2f interval(listBack.getSize().x / 20, listBack.getSize().y / 20);
  texts[name].setPosition(listBack.getPosition().x - listBack.getSize().x / 2 + texts[name].getLocalBounds().width / 2 + interval.x,
    listBack.getPosition().y - listBack.getSize().y / 2 + texts[name].getLocalBounds().height / 2 + interval.y);
  texts[nick].setPosition(texts[name].getPosition().x + texts[name].getGlobalBounds().width + 40, texts[name].getPosition().y);
  ButtonSave.setPos(Vector2f(listBack.getPosition().x - listBack.getSize().x / 2 + ButtonSave.size.x / 2 + interval.x, listBack.getPosition().y + listBack.getSize().y / 2 - ButtonSave.size.y / 2 - interval.y));
  ButtonClose.setPos(Vector2f(listBack.getPosition().x + listBack.getSize().x / 2 - ButtonClose.size.x / 2 - interval.x, ButtonSave.button.getPosition().y));
  //Õ¿—“–Œ… » «¬” ¿
  texts[soundS].setPosition(texts[name].getPosition().x, texts[name].getPosition().y + texts[soundS].getGlobalBounds().height + interval.y * 2);
  checkboxS.setPos(Vector2f(texts[soundS].getPosition().x, texts[soundS].getPosition().y + checkboxS.size.y + interval.y));
  texts[volumeS].setPosition(listBack.getPosition().x + listBack.getSize().x / 2 - texts[volumeS].getLocalBounds().width / 2 - interval.x, texts[soundS].getPosition().y);
  lineS.setPos(Vector2f(listBack.getPosition().x, checkboxS.button.getPosition().y));
  //Õ¿—“–Œ… » Ã”«€ »
  int height = (checkboxS.button.getPosition().y + checkboxS.size.y / 2) - (texts[soundS].getPosition().y - texts[soundS].getGlobalBounds().height / 2);
  int intervalY = (ButtonSave.button.getPosition().y - ButtonSave.size.y - interval.y - height) - texts[soundS].getPosition().y + texts[soundS].getGlobalBounds().height / 2;
  texts[musicS].setPosition(texts[soundS].getPosition().x, texts[soundS].getPosition().y + intervalY);
  checkboxM.setPos(Vector2f(checkboxS.button.getPosition().x, checkboxS.button.getPosition().y + intervalY));
  lineM.setPos(Vector2f(lineS.position.x, lineS.position.y + intervalY));

  bool isOpen = true;
  settings newSettings(settings_);
  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);
  IntRect nickArea = buildNickArea(texts[nick]);
  Vector2i mousePos;
  bool isClick = false;
  bool isEdit = false;
  Text oldNick = texts[nick];
  while (isOpen)
  {
    Event event;
    window.clear();
    window.draw(background);
    window.draw(listBack);
    texts[volumeS].setPosition(texts[volumeS].getPosition().x, texts[volumeS].getPosition().y + intervalY);
    window.draw(texts[volumeS]);
    texts[volumeS].setPosition(texts[volumeS].getPosition().x, texts[volumeS].getPosition().y - intervalY);
    while (window.pollEvent(event)) {
      mousePos = Mouse::getPosition(window);
      isClick = Mouse::isButtonPressed(Mouse::Left);
      if (isClick && nickArea.contains(mousePos) && !isEdit) {
        isEdit = true;
      }
      if (isEdit) {
        isEdit = enterText(window, event, texts[nick]);
        if (!isEdit && texts[nick].getString().getSize() < 2) {
          texts[nick] = oldNick;
        }
        if (!isEdit) buildNickArea(texts[nick]);
      }
      lineS.listen(window, event);
      lineM.listen(window, event);
      if (event.type == Event::Closed)
        window.close();
    }
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
      newSettings.nickname = texts[nick].getString();
      settings_ = newSettings;
      settings_.write();
    }
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
    window.display();
  }
}

bool Menu::enterText(RenderWindow& window, Event& event, Text& text) {
  std::string str = text.getString();
  if (Keyboard::isKeyPressed(Keyboard::Enter)) {
    return false;
  } 
  else
    if (event.type == sf::Event::TextEntered) {
      if (event.text.unicode < 128)
      {
        if (event.text.unicode == 8 && str.size() > 0) {
          str.resize(str.size() - 1);
          text.setString(str);
        }
        else if (str.size() != settings_.maxLengthNick && event.text.unicode != 32) {
          str += static_cast<char>(event.text.unicode);
          text.setString(str);
        }
      }
    }
  return true;
}

IntRect Menu::buildNickArea(Text& text) {
  return IntRect (text.getPosition().x - text.getGlobalBounds().width / 2, text.getPosition().y - text.getGlobalBounds().height / 2,
    text.getGlobalBounds().width, text.getGlobalBounds().height);
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

int Menu::endGameMenu(RenderWindow& window, int pointsCount, int gameTime, RectangleShape& Background) {
  Sound clickSound(clickBuffer);
  Sound rolloverSound(rolloverBuffer);
  Text text;
  text.setFont(font);
  text.setCharacterSize(40);
  text.setFillColor(Color::Black);
  String str = "Name: " + settings_.nickname + '\n' + "points: " + std::to_string(pointsCount) + '\n' + "time: " + std::to_string(gameTime);
  text.setString(str);
  text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
  MenuButton ButtonExit(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 50), buttonSize, L"exit", font);
  MenuButton ButtonMenu(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 50), buttonSize, L"menu", font);
  MenuButton ButtonRestart(textures, Vector2f(190, 50), Vector2f(0, 0), Vector2f(0, 50), buttonSize, L"restart", font);
  int interval = 20;
  float width = ButtonExit.size.x + ButtonMenu.size.x + ButtonRestart.size.x + interval * 4;
  float height = ButtonExit.size.y + text.getGlobalBounds().height + interval * 4;
  RectangleShape listBack(Vector2f(width, height));
  listBack.setOrigin(listBack.getSize().x / 2, listBack.getSize().y / 2);
  listBack.setPosition(screenSize.x / 2, screenSize.y / 2);
  listBack.setTexture(back.getTexture());
  listBack.setTextureRect(back.getTextureRect());
  ButtonExit.setPos(Vector2f(listBack.getPosition().x - listBack.getSize().x / 2 + interval + ButtonExit.size.x / 2, listBack.getPosition().y + listBack.getSize().y / 2 - interval - ButtonExit.size.y / 2));
  ButtonMenu.setPos(Vector2f(ButtonExit.button.getPosition().x + ButtonExit.size.x + interval, ButtonExit.button.getPosition().y));
  ButtonRestart.setPos(Vector2f(ButtonMenu.button.getPosition().x + ButtonMenu.size.x + interval, ButtonMenu.button.getPosition().y));
  text.setPosition(ButtonMenu.button.getPosition().x, ButtonMenu.button.getPosition().y - ButtonMenu.size.y / 2 - text.getGlobalBounds().height / 2 - interval * 2);
  if (top.isRecord(pointsCount))
    top.write(settings_.nickname, pointsCount, gameTime);
  while (true) {
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
    }
    window.clear();
    window.draw(Background);
    window.draw(listBack);
    window.draw(text);
    if (ButtonExit.listen(window, clickSound, rolloverSound))
      return 1;
    if (ButtonMenu.listen(window, clickSound, rolloverSound))
      return 2;
    if (ButtonRestart.listen(window, clickSound, rolloverSound))
      return 3;
    window.draw(ButtonExit.button);
    window.draw(ButtonExit.text);
    window.draw(ButtonMenu.button);
    window.draw(ButtonMenu.text);
    window.draw(ButtonRestart.button);
    window.draw(ButtonRestart.text);
    window.display();
  }
}