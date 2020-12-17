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
#define PI 3.14159265

using namespace sf;

int main(int args, char *argv[])
{
  ContextSettings windowSettings;
  windowSettings.antialiasingLevel = 16; //установка уровня сглаживания фигур
  RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Star wasteland", Style::Fullscreen, windowSettings);
  Vector2u screen = window.getSize();
  double FPS = 60;
  window.setFramerateLimit(FPS);
//---------------------------------ТЕКСТУРЫ И ШРИФТЫ------------------------------------------
    textureLoader tl;
    Texture texture;
    Texture dest_effect_planet;
    dest_effect_planet.loadFromFile("textures/Destroy_effects_planet/dest_effect.png");
    std::vector<String> paths;
    for (ptrdiff_t i = 0; i < 17; i++) {
      paths.push_back("textures/planet/planet" + std::to_string(i) + ".png");
    }
    std::vector<String> paths1;
    for (ptrdiff_t i = 0; i < 9; i++) {
      String s = "textures/Destroy_effects/regularExplosion0" + std::to_string(i) + ".png";
      paths1.push_back(s);
    }
    tl.loader(Objects::DEST_EFFECT, paths1);
    String path = "textures/player/player.png";
    tl.loader(Objects::PLAYER, path);
    texture.loadFromFile("textures/planet/Space.png");
    tl.loader(Objects::PLANET, paths);
    tl.background = texture;
    path = "textures/asteroid/meteorBrown_big4.png";
    tl.loader(Objects::ASTEROID, path);
    path = "textures/weapon/laserBlue03.png";
    tl.loader(Objects::WEAPON, path);
    path = "textures/UI/shield.png";
    tl.loader(Objects::UI, path);
    Font font, monoFont;
    font.loadFromFile("fonts/17734.otf");
    monoFont.loadFromFile("fonts/monosize.ttf");

    std::map<std::string, sf::String> menuSource;
    menuSource["mainTile"] = "textures/Menu/blueSheet.png";
    menuSource["panel"] = "textures/Menu/grey_panel.png";
    menuSource["logo"] = "textures/logo.png";
    menuSource["slider"] = "textures/Menu/slider.png";
    tl.loderMenu(menuSource);

    std::string settingsPath = "text/settings.ini";
    std::string recordsPath = "text/records.ini";
    settings settingsGame(settingsPath);
    settingsGame.read();
    TopResults records(recordsPath);
    records.read();

    Menu main_menu(settingsGame, records, tl, screen, font, monoFont);
    if (main_menu.menu(window)) return 0;
    settingsGame.read();
//---------------------------------МУЗЫКА И ЗВУКИ---------------------------------
    SoundBuffer boomBuffer;
    if (!boomBuffer.loadFromFile("sounds/Boom.ogg"))
      return -1;
    boomBuffer.loadFromFile("sounds/Boom.ogg");
    Sound boom(boomBuffer);
    SoundBuffer planetBuffer;
    if (!planetBuffer.loadFromFile("sounds/destroy_planet.ogg"))
      return -1;
    planetBuffer.loadFromFile("sounds/destroy_planet.ogg");
    Sound destroy_planet(planetBuffer);
    boom.setVolume(10);
    SoundBuffer shootBuffer;
    if (!shootBuffer.loadFromFile("sounds/shoot.ogg"))
      return -1;
    shootBuffer.loadFromFile("sounds/shoot.ogg");
    Sound shoot(shootBuffer);
    SoundBuffer bornBuffer;
    if (!bornBuffer.loadFromFile("sounds/born.ogg"))
      return -1;
    bornBuffer.loadFromFile("sounds/born.ogg");
    Sound bornSound(bornBuffer);
    Music music;
    if (!music.openFromFile("music/spaceMusic.ogg"))
      return -1;
    music.openFromFile("music/spaceMusic.ogg");
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
//---------------------------------ПЕРЕМЕННЫЕ ИГРЫ-------------------------------
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
    float sizeAsteroid = 1024/10; 
    int gameTime = 0; //прошедшое в секундах время игры
    int points = 0; //количество очков
    Clock gameTimeClock; //часы игры
    Clock clock; 
    Sprite background; //фон игры
    bool life = true; //жив ли игрок
    bool spaceClick = false; //нажата ли кнопка space
    float weaponKD = 1.5; //время, через которое бластер обновляется
    float asteroidKD = 5; //время, через которое астероид обновляется
    float currentFrame = 0; //текущий кадр
    int timeLastFrame = 0;
    int radius;
    int area;
    std::vector<Vector2f> spawnPointsPlanet;
    std::map<int, Vector2f> useSpawnPointsPlanet;

    background.setTexture(tl.background);

    //установка точек-границ спавна планет
    for (ptrdiff_t i = 0; i < 3; i++) {
      spawnPointsPlanet.push_back(Vector2f(screen.x / 3 * i, 0));
      spawnPointsPlanet.push_back(Vector2f(screen.x / 3 * i, screen.y / 2));
    }
//------------------------------СОЗДАНИЕ ОБЪЕКТОВ ИГРЫ-------------------------------------------------------
    Spawner spawner;
    Player player(tl.players[0], speed, angularSpeed, screen, w, h);
    Weapon weapon(tl.weapons[0], speed * 2.5, weaponKD);
    std::list<Asteroid*> asteroids;
    std::list<Asteroid*>::iterator it;
    std::list<Planet*> planets;
    std::list<Planet*>::iterator it_p;
    //установка позиции логов
    lifetime.setPosition(screen.x - 200, 0);
    pointCount.setPosition(screen.x - 200, 40);

    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 5, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 20, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 30, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 60, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, spawner.generator(speed, 0.2), asteroidKD, spawner.generatorAsteroids(screen), 120, tl.asteroids[0]));
    radius = spawner.generator(100, sizePlanet);
    area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
    planets.push_back(new Planet(radius, 5.0f, tl.planets[spawner.generator(0, tl.planets.size()-1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area), 
                                  radius/10, spawner.generator(5, 15), area, font, tl.ui[0]));
    radius = spawner.generator(100, sizePlanet);
    area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
    planets.push_back(new Planet(radius, 9.0f, tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area),
                                  radius/10, spawner.generator(5, 15), area, font, tl.ui[0]));
//-------------------------------ИГРОВОЙ ПРОЦЕСС---------------------------------------------------------------
    while (window.isOpen())
    {
      //проигрывание музыки
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
        }
        if (Keyboard::isKeyPressed(Keyboard::Left) && life) { 
            player.flyLeft(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && life) {
            player.flyRight(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) && life) {
            player.flyForward(time, currentFrame);    
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) && life) {
            player.flyBack(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Down) && life) {
            player.flyForwardAndBack();
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && life) {
          if (!spaceClick && weapon.getIsMove()) {
            spaceClick = true;
            weapon.clock.restart();
            shoot.play();
          }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
          //пауза
        }
//--------------------------------ИНФОРМАЦИЯ--------------------------------------------------
          std::ostringstream gameTimeString, playerPointsString;
          gameTimeString << gameTime;
          playerPointsString << points;
          lifetime.setString("Time: " + gameTimeString.str());
          pointCount.setString("Points: " + playerPointsString.str());

//------------------------------ПОВЫШЕНИЕ СЛОЖНОСТИ-------------------------------------------
          if (gameTime != 0 && gameTime % 30 == 0 && timeLastFrame != gameTime) {
             timeLastFrame = gameTime;
            for (it = asteroids.begin(); it != asteroids.end(); it++) {
              if((*it)->getSpeed() < speed * 3)
                (*it)->setSpeed((*it)->getSpeed()*1.1);
              if((*it)->getKD() > asteroidKD / 2)
                (*it)->setKD((*it)->getKD()*0.9);
            }
          }
//---------------------------------НАЧАЛО ОТРИСОВКИ-------------------------------------------
          window.clear();
          window.draw(background);
          if(life)
           window.draw(player.player);
          window.draw(lifetime);
          window.draw(pointCount);

          if (!life && !player.anim_end) {
            player.Destroy(tl.dest_effect);
            window.draw(player.dest_sprite);
          }
//-----------------------------------ПЛАНЕТЫ----------------------------------------------------
          for (it_p = planets.begin(); it_p != planets.end(); it_p++) {
            //вывод хп планеты и значка прочности
            if ((*it_p)->born) {
              window.draw((*it_p)->icon);
              (*it_p)->hpView.setString(std::to_string((*it_p)->currentHp) + " / " + std::to_string((*it_p)->hp));
              window.draw((*it_p)->hpView);
            }
            //смерть планеты
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
              (*it_p)->Die(radius, 5.0f, tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area), radius/10, 5, area);
              continue;
            }
            //спавн планеты
            if (!(*it_p)->born && (*it_p)->deathTime + (*it_p)->spawnKD < gameTime) {
              (*it_p)->Spawn();
              window.draw((*it_p)->planet);
              //проигрывание звука спавна
              if ((*it_p)->beginSpawn)
                bornSound.play();
              continue;
            }
            else {
              (*it_p)->spawnClock.restart();
            }
            (*it_p)->Rotation();
            if((*it_p)->born == true || (*it_p)->destroy)
              window.draw((*it_p)->planet);
          }
//---------------------------------------------------------------------------------------------------          
          if (life) {
            //столкновение игрока с астероидом
            RectangleShape body(Vector2f(player.player.getLocalBounds().width / 1.5, player.player.getLocalBounds().height / 1.5));
            body.setOrigin(body.getGlobalBounds().width / 2, body.getGlobalBounds().height / 2);
            body.setRotation(player.player.getRotation());
            body.setPosition(player.getPos());
            for (it = asteroids.begin(); it != asteroids.end(); it++) {
              if (body.getGlobalBounds().intersects((*it)->asteroid.getGlobalBounds())) {
                player.player.setFillColor(Color::Transparent);
                life = false;
                lifetime.setPosition(screen.x / 2 - 120, screen.y / 2 - 50);
                lifetime.setScale(2.0, 2.0);
                pointCount.setPosition(screen.x / 2 - 120, screen.y / 2 - 100);
                pointCount.setScale(2.0, 2.0);
                player.dest_anim_clock.restart();
                player.pos_die = Vector2f(player.getPos().x - player.player.getGlobalBounds().width/2, player.getPos().y - player.player.getGlobalBounds().height / 2);
                boom.play();
                break;
              }
            }
//------------------------------------БЛАСТЕР-------------------------------------------------
            //перезарядка снаряда
            if (weapon.clock.getElapsedTime().asSeconds() > weaponKD) {
              spaceClick = false;
              weapon.update();
              weapon.setIsMove(true);
            }
            //перемещение и рисование снаряда
            if (spaceClick && weapon.getIsMove()) {
              window.draw(weapon.weapon);
              weapon.Move(player);
            }
//--------------------------------------АСТЕРОИДЫ---------------------------------------------
            for (it = asteroids.begin(); it != asteroids.end(); it++) {
              //сброс времени при первом появлении астероида
              if ((*it)->getSpawnTime() <= gameTime && !(*it)->getExist()) {
                (*it)->clock.restart();
                (*it)->setExist(true);
              }
              //первое появление астероида
              if ((*it)->getSpawnTime() <= gameTime) {
                (*it)->setAsteroidMove(true);
                window.draw((*it)->asteroid);
              }
              //перемещение астероида
              if ((*it)->getAsteroidMove()) (*it)->Move(player);
              //столкновение снаряда с астероидом
              if (weapon.weapon.getGlobalBounds().intersects((*it)->asteroid.getGlobalBounds()) && (*it)->getAsteroidMove()) {
                (*it)->dest_pos = (*it)->asteroid.getPosition();
                (*it)->dest_anim_clock.restart();
                (*it)->destroy = true;
                (*it)->update(spawner.generatorAsteroids(screen));
                weapon.update();
                weapon.setIsMove(false);
                (*it)->setAsteroidMove(false);
                points += 100;
                boom.play();
              }
              //анимация уничтожения астероида
              if ((*it)->destroy) {
                (*it)->Destroy(tl.dest_effect);
                window.draw((*it)->dest_sprite);
              }
              //перезарядка астероида
              if ((*it)->clock.getElapsedTime().asSeconds() > (*it)->getKD() && (*it)->getExist()) {
                (*it)->setAsteroidMove(true);
                (*it)->update(spawner.generatorAsteroids(screen));
              }
              //столкновение астероида с планетой
              for (it_p = planets.begin(); it_p != planets.end(); it_p++) {
                if ((*it_p)->Intersects((*it)->asteroid) && (*it_p)->born) {
                  (*it)->dest_pos = (*it)->asteroid.getPosition();
                  (*it)->dest_anim_clock.restart();
                  (*it)->destroy = true;
                  (*it)->update(spawner.generatorAsteroids(screen));
                  (*it)->setAsteroidMove(false);
                  (*it_p)->currentHp -= 1;
                  boom.play();
                }
              }
            }
          }
          window.display();
    }
    return 0;
}
