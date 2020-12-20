#include "player.h"
#include "textureLoader.h"
#include "planet.h"
#include "asteroid.h"
#include "weapon.h"
#include "spawner.h"
#include "Menu.h"
#include "settings.h"
#include "TopResults.h"

#include <math.h>
#include <sstream>
#include <list>
#include <map>
#include <Windows.h>
#define PI 3.14159265

using namespace sf;

int main(int args, char* argv[])
{
  ContextSettings windowSettings;
  windowSettings.antialiasingLevel = 16; //��������� ������ ����������� �����
  RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Star wasteland", Style::Fullscreen, windowSettings);
  Vector2u screen = window.getSize();
  double FPS = 60;
  window.setFramerateLimit(FPS);
  //---------------------------------�������� � ������------------------------------------------
  textureLoader tl;
  Font font, monoFont;
  font.loadFromFile("src/fonts/17734.otf");
  monoFont.loadFromFile("src/fonts/monosize.ttf");
  std::vector<String> textures;
  for (ptrdiff_t i = 0; i < 2; i++) {
    textures.push_back("src/textures/Backgrounds/back" + std::to_string(i + 1) + ".jpg");
  }
  tl.loader(Objects::BACKGROUND, textures);
  std::map<std::string, sf::String> menuSource;
  menuSource["mainTile"] = "src/textures/Menu/blueSheet.png";
  menuSource["panel"] = "src/textures/Menu/panel.png";
  menuSource["logo"] = "src/textures/logo.png";
  menuSource["slider"] = "src/textures/Menu/slider.png";
  tl.loderMenu(menuSource);
  Texture dest_effect_planet;
  std::vector<String> paths;
  std::vector<String> paths1;
  std::vector<String> paths2;
  dest_effect_planet.loadFromFile("src/textures/Destroy_effects_planet/dest_effect.png");
  for (ptrdiff_t i = 0; i < 18; i++) {
    paths.push_back("src/textures/planet/planet" + std::to_string(i) + ".png");
  }
  for (ptrdiff_t i = 0; i < 9; i++) {
    String s = "src/textures/Destroy_effects/regularExplosion0" + std::to_string(i) + ".png";
    paths1.push_back(s);
  }
  for (ptrdiff_t i = 0; i < 8; i++) {
    paths2.push_back("src/textures/asteroid/meteor" + std::to_string(i) + ".png");
  }
  tl.loader(Objects::DEST_EFFECT, paths1);
  String path = "src/textures/player/player.png";
  tl.loader(Objects::PLAYER, path);
  tl.loader(Objects::PLANET, paths);
  tl.loader(Objects::ASTEROID, paths2);
  path = "src/textures/weapon/laserBlue.png";
  tl.loader(Objects::WEAPON, path);
  path = "src/textures/UI/shield.png";
  tl.loader(Objects::UI, path);

  std::string settingsPath = "src/text/settings.ini";
  std::string recordsPath = "src/text/records.ini";
  settings settingsGame(settingsPath, 15);
  TopResults records(recordsPath, 5);
  settingsGame.read();
  records.read();
  Menu main_menu(settingsGame, records, tl, screen, font, monoFont);
  if (main_menu.menu(window)) return 0;
  //---------------------------------������ � �����---------------------------------
  SoundBuffer boomBuffer;
  if (!boomBuffer.loadFromFile("src/sounds/Boom.ogg"))
    return -1;
  boomBuffer.loadFromFile("src/sounds/Boom.ogg");
  Sound boom(boomBuffer);
  SoundBuffer planetBuffer;
  if (!planetBuffer.loadFromFile("src/sounds/destroy_planet.ogg"))
    return -1;
  planetBuffer.loadFromFile("src/sounds/destroy_planet.ogg");
  Sound destroy_planet(planetBuffer);
  boom.setVolume(10);
  SoundBuffer shootBuffer;
  if (!shootBuffer.loadFromFile("src/sounds/shoot.ogg"))
    return -1;
  shootBuffer.loadFromFile("src/sounds/shoot.ogg");
  Sound shoot(shootBuffer);
  SoundBuffer bornBuffer;
  if (!bornBuffer.loadFromFile("src/sounds/born.ogg"))
    return -1;
  bornBuffer.loadFromFile("src/sounds/born.ogg");
  Sound bornSound(bornBuffer);
  Music music;
  if (!music.openFromFile("src/music/spaceMusic.ogg"))
    return -1;
  music.openFromFile("src/music/spaceMusic.ogg");

  //---------------------------------���������� ����-------------------------------
  Text lifetime;
  Text pointCount;
  pointCount.setFont(font);
  lifetime.setFont(font);
  int width = 50;
  int heigth = 75;
  int w = 112;
  int h = 107;
  double speed = FPS / (FPS * 10);
  double angularSpeed = 3;
  int sizePlanet = 150;
  float sizeAsteroid = 1024 / 10;
  int gameTime = 0; //��������� � �������� ����� ����
  int points = 0; //���������� �����
  Clock gameTimeClock; //���� ����
  Clock clock;
  RectangleShape background; //��� ����
  background.setSize(Vector2f(screen.x, screen.y));
  bool life = true; //��� �� �����
  bool spaceClick = false; //������ �� ������ space
  float weaponKD = 1.5; //�����, ����� ������� ������� �����������
  float asteroidKD = 5; //�����, ����� ������� �������� �����������
  float currentFrame = 0; //������� ����
  int timeLastFrame = 0;
  int radius;
  int area;
  //��������� �����-������ ������ ������
  std::vector<Vector2f> spawnPointsPlanet;
  std::map<int, Vector2f> useSpawnPointsPlanet;
  for (ptrdiff_t i = 0; i < 3; i++) {
    spawnPointsPlanet.push_back(Vector2f(screen.x / 3 * i, 0));
    spawnPointsPlanet.push_back(Vector2f(screen.x / 3 * i, screen.y / 2));
  }
  background.setTexture(tl.backgrounds[1]);

  Spawner spawner;
  Player player(tl.players[0], speed, angularSpeed, screen, w, h);
  Weapon weapon(tl.weapons[0], speed * 2.5, weaponKD);
  std::list<Asteroid*> asteroids;
  std::list<Asteroid*>::iterator it;
  std::list<Planet*> planets;
  std::list<Planet*>::iterator it_p;
  int choice = 0;
  do {
    if (!life) {
      music.stop();
      if (choice == 0)
        choice = main_menu.endGameMenu(window, points, gameTime, background);
      switch (choice) {
      case 1:
        return 0;
        break;
      case 2:
        if (main_menu.menu(window))
          return 0;
      case 3:
        choice = 0;
        life = true;
        gameTimeClock.restart();
        spaceClick = false;
        gameTime = 0;
        points = 0;
        clock.restart();
        spaceClick = false;
        player.standartCondition();
        player.player.setPosition(screen.x / 2, screen.y / 2);
        player.player.setRotation(0);
        break;
      }
    }
    settingsGame.read();
    records.read();
    music.setVolume(settingsGame.musicVolume);
    if (settingsGame.soundIsOn) {
      boom.setVolume(settingsGame.soundVolume);
      destroy_planet.setVolume(settingsGame.soundVolume);
      shoot.setVolume(settingsGame.soundVolume);
      bornSound.setVolume(settingsGame.soundVolume);
    }
    else {
      boom.setVolume(0);
      destroy_planet.setVolume(0);
      shoot.setVolume(0);
      bornSound.setVolume(0);
    }
    //-------------------------------------------------START GAME------------------------------------------------------------------------------

        //��������� ������� �����
    lifetime.setPosition(screen.x - 200, 0);
    pointCount.setPosition(screen.x - 200, 40);

    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 5, tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 20, tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 30, tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 60, tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 120, tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]));
    radius = spawner.generator(100, sizePlanet);
    area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
    planets.push_back(new Planet(radius, 5.0f, *tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area),
      radius / 10, spawner.generator(5, 15), area, font, *tl.ui[0]));
    radius = spawner.generator(100, sizePlanet);
    area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
    planets.push_back(new Planet(radius, 9.0f, *tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area),
      radius / 10, spawner.generator(5, 15), area, font, *tl.ui[0]));
    //-------------------------------������� �������---------------------------------------------------------------
    while (window.isOpen())
    {
      Minimizer::minimizeWindow(window);
      //������������ ������
      if (!music.getStatus() && settingsGame.musicIsOn) {
        music.play();
      }
      float time = clock.getElapsedTime().asMicroseconds();
      clock.restart();
      time /= 800;
      player.standartCondition();
      Event event;
      if (life) {
        gameTime = gameTimeClock.getElapsedTime().asSeconds();
      }
      while (window.pollEvent(event))
      {
        if (event.type == Event::Closed)
          window.close();
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
          life = false;
          choice = 2;
        }
      }
      if (Keyboard::isKeyPressed(Keyboard::A) && life) {
        player.flyLeft(time);
      }
      if (Keyboard::isKeyPressed(Keyboard::D) && life) {
        player.flyRight(time);
      }
      if (Keyboard::isKeyPressed(Keyboard::W) && life) {
        player.flyForward(time, currentFrame);
      }
      if (Keyboard::isKeyPressed(Keyboard::S) && life) {
        player.flyBack(time);
      }
      if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::S) && life) {
        player.flyForwardAndBack();
      }
      if (Keyboard::isKeyPressed(Keyboard::Space) && life) {
        if (!spaceClick && weapon.getIsMove()) {
          spaceClick = true;
          weapon.clock.restart();
          shoot.play();
        }
      }
      //--------------------------------����������--------------------------------------------------
      std::ostringstream gameTimeString, playerPointsString;
      gameTimeString << gameTime;
      playerPointsString << points;
      lifetime.setString("Time: " + gameTimeString.str());
      pointCount.setString("Points: " + playerPointsString.str());

      //------------------------------��������� ���������-------------------------------------------
      if (gameTime != 0 && gameTime % 30 == 0 && timeLastFrame != gameTime) {
        timeLastFrame = gameTime;
        for (it = asteroids.begin(); it != asteroids.end(); it++) {
          if ((*it)->getSpeed() < speed * 3)
            (*it)->setSpeed((*it)->getSpeed() * 1.1);
          if ((*it)->getKD() > asteroidKD / 2)
            (*it)->setKD((*it)->getKD() * 0.9);
        }
      }
      //---------------------------------������ ���������-------------------------------------------
      window.clear();
      window.draw(background);
      if (life)
        window.draw(player.player);
      window.draw(lifetime);
      window.draw(pointCount);

      if (!life && !player.anim_end) {
        player.Destroy(tl.dest_effect);
        window.draw(player.dest_sprite);
      }
      //-----------------------------------�������----------------------------------------------------
      for (it_p = planets.begin(); it_p != planets.end(); it_p++) {
        //����� �� ������� � ������ ���������
        if ((*it_p)->born) {
          window.draw((*it_p)->icon);
          (*it_p)->hpView.setString(std::to_string((*it_p)->currentHp) + " / " + std::to_string((*it_p)->hp));
          window.draw((*it_p)->hpView);
        }
        //������ �������
        if ((*it_p)->currentHp <= 0 && !(*it_p)->destroy) {
          (*it_p)->deathTime = gameTime;
          (*it_p)->destroy = true;
          (*it_p)->dest_anim_clock.restart();
          (*it_p)->born = false;
          (*it_p)->texture_planet = dest_effect_planet;
          destroy_planet.play();
        }
        if ((*it_p)->destroy == true) {
          (*it_p)->Destroy(5, 4, Vector2f(0, 122), 130);
          window.draw((*it_p)->planet);
        }
        if ((*it_p)->currentHp <= 0 && !(*it_p)->destroy) {
          radius = spawner.generator(100, sizePlanet);
          useSpawnPointsPlanet.erase((*it_p)->occupiedArea);
          area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
          (*it_p)->Die(radius, 5.0f, *tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area), radius / 10, 5, area);
          continue;
        }
        //����� �������
        if (!(*it_p)->born && (*it_p)->deathTime + (*it_p)->spawnKD < gameTime) {
          (*it_p)->Spawn();
          window.draw((*it_p)->planet);
          //������������ ����� ������
          if ((*it_p)->beginSpawn)
            bornSound.play();
          continue;
        }
        else {
          (*it_p)->spawnClock.restart();
        }
        (*it_p)->Rotation();
        if ((*it_p)->born == true || (*it_p)->destroy)
          window.draw((*it_p)->planet);
      }
      //------------------------------------�������-------------------------------------------------
      //����������� �������
      if (weapon.clock.getElapsedTime().asSeconds() > weaponKD) {
        spaceClick = false;
        weapon.update();
        weapon.setIsMove(true);
      }
      //����������� � ��������� �������
      if (spaceClick && weapon.getIsMove()) {
        window.draw(weapon.weapon);
        weapon.Move(player);
      }
      //--------------------------------------���������---------------------------------------------
      for (it = asteroids.begin(); it != asteroids.end(); it++) {
        //����� ������� ��� ������ ��������� ���������
        if ((*it)->getSpawnTime() <= gameTime && !(*it)->getExist()) {
          (*it)->clock.restart();
          (*it)->setExist(true);
        }
        //������ ��������� ���������
        if ((*it)->getSpawnTime() <= gameTime) {
          (*it)->setAsteroidMove(true);
          window.draw((*it)->asteroid);
        }
        //����������� ���������
        if ((*it)->getAsteroidMove()) (*it)->Move(player);
        //������������ ������� � ����������
        if (weapon.weapon.getGlobalBounds().intersects((*it)->asteroid.getGlobalBounds()) && (*it)->getAsteroidMove()) {
          (*it)->dest_pos = (*it)->asteroid.getPosition();
          (*it)->dest_anim_clock.restart();
          (*it)->destroy = true;
          (*it)->update(spawner.generatorAsteroids(screen), tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]);
          weapon.update();
          weapon.setIsMove(false);
          (*it)->setAsteroidMove(false);
          points += 100;
          boom.play();
        }
        //�������� ����������� ���������
        if ((*it)->destroy) {
          (*it)->Destroy(tl.dest_effect);
          window.draw((*it)->dest_sprite);
        }
        //����������� ���������
        if ((*it)->clock.getElapsedTime().asSeconds() > (*it)->getKD() && (*it)->getExist()) {
          (*it)->setAsteroidMove(true);
          (*it)->update(spawner.generatorAsteroids(screen), tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]);
        }
        //������������ ��������� � ��������
        for (it_p = planets.begin(); it_p != planets.end(); it_p++) {
          if ((*it_p)->Intersects((*it)->asteroid) && (*it_p)->born) {
            (*it)->dest_pos = (*it)->asteroid.getPosition();
            (*it)->dest_anim_clock.restart();
            (*it)->destroy = true;
            (*it)->update(spawner.generatorAsteroids(screen), tl.asteroids[spawner.generator(0, tl.asteroids.size() - 1)]);
            (*it)->setAsteroidMove(false);
            (*it_p)->currentHp -= 1;
            boom.play();
          }
        }
      }
      //������������ ������ � ����������
      RectangleShape body(Vector2f(player.player.getLocalBounds().width / 1.5, player.player.getLocalBounds().height / 1.5));
      body.setOrigin(body.getGlobalBounds().width / 2, body.getGlobalBounds().height / 2);
      body.setRotation(player.player.getRotation());
      body.setPosition(player.getPos());
      for (it = asteroids.begin(); it != asteroids.end(); it++) {
        if (body.getGlobalBounds().intersects((*it)->asteroid.getGlobalBounds())) {
          life = false;
          player.dest_anim_clock.restart();
          player.pos_die = Vector2f(player.getPos().x - player.player.getGlobalBounds().width / 2, player.getPos().y - player.player.getGlobalBounds().height / 2);
          boom.play();
          break;
        }
      }
      window.display();
      if (!life) {
        for (it = asteroids.begin(); it != asteroids.end(); it++) {
          delete (*it);
        }
        for (it_p = planets.begin(); it_p != planets.end(); it_p++) {
          delete (*it_p);
        }
        asteroids.clear();
        planets.clear();
        useSpawnPointsPlanet.clear();
        break;
      }
    }
  } while (!life);
  return 0;
}
