#include "player.h"
#include "textureLoader.h"
#include "planet.h"
#include "asteroid.h"
#include "weapon.h"
#include "spawner.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <sstream>
#include <random>
#include <list>
#define PI 3.14159265

using namespace sf;

int main(int args, char *argv[])
{
//---------------------------------ТЕКСТУРЫ И ШРИФТЫ------------------------------------------
    textureLoader tl;
    Texture texture;

    std::vector<String> paths = { "textures/planet/Orange-Planet.png", "textures/planet/Lava-PLanet.png" };
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
    Font font;
    font.loadFromFile("fonts/17734.otf");
//---------------------------------МУЗЫКА И ЗВУКИ---------------------------------
    SoundBuffer boomBuffer;
    if (!boomBuffer.loadFromFile("sounds/Boom.ogg"))
      return -1;
    boomBuffer.loadFromFile("sounds/Boom.ogg");
    Sound boom(boomBuffer);
    boom.setVolume(20);
    SoundBuffer shootBuffer;
    if (!shootBuffer.loadFromFile("sounds/shoot.ogg"))
      return -1;
    shootBuffer.loadFromFile("sounds/shoot.ogg");
    Sound shoot(shootBuffer);
    Music music;
    if (!music.openFromFile("music/spaceMusic.ogg"))
      return -1;
    music.openFromFile("music/spaceMusic.ogg");

    Text lifetime;
    Text pointCount;
    pointCount.setFont(font);
    lifetime.setFont(font);
    int width = 50;
    int heigth = 75;
    int w = 112;
    int h = 107;
    double speed = 165;
    double angularSpeed = 1.5;
    float sizePlanet = 150;
    float sizeAsteroid = 1024/10; 
    int gameTime = 0; //прошедшое в секундах время игры
    int points = 0; //количество очков
    Clock gameTimeClock; //часы игры
    Clock clock; 
    Sprite background; //фон игры
    Sprite sprite; //вспомогательный спрайт для передачи в параметры создания объекта
    bool life = true; //жив ли игрок
    bool spaceClick = false; //нажата ли кнопка space
    float weaponKD = 1.5; //время, через которое бластер обновляется
    float asteroidKD = 5; //время, через которое астероид обновляется

    background.setTexture(tl.background);
    //sprite.setTexture(tl.players[0]);

    ContextSettings settings;
    settings.antialiasingLevel = 16; //установка уровня сглаживания фигур
    RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Test shapes", Style::Fullscreen, settings);

    Vector2u screen = window.getSize();
    //создание объектов игры
    Spawner spawnAsteroid;
    Planet planet1(sizePlanet, 5.0f, tl.planets[0], Vector2f(140.0f, 267.0f));
    Planet planet2(sizePlanet, 9.0f, tl.planets[1], Vector2f(1094.0f, 872.0f));
    Player player(tl.players[0], speed, angularSpeed, screen, w, h);
    Weapon weapon(tl.weapons[0], speed * 2.5, weaponKD);
    std::list<Asteroid*> asteroids;
    std::list<Asteroid*>::iterator it;
    //установка позиции логов
    lifetime.setPosition(screen.x - 200, 0);
    pointCount.setPosition(screen.x - 200, 40);
    float currentFrame = 0; //текущий кадр

    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 100, asteroidKD, spawnAsteroid.generator(screen), 1, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 93, asteroidKD, spawnAsteroid.generator(screen), 5, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 90, asteroidKD, spawnAsteroid.generator(screen), 15, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 102, asteroidKD, spawnAsteroid.generator(screen), 30, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 95, asteroidKD, spawnAsteroid.generator(screen), 60, tl.asteroids[0]));
    while (window.isOpen())
    {
      if (!music.getStatus()) {
        music.play();
      }
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 800;
        player.standartCondition();
        planet1.Rotation();
        planet2.Rotation();
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
          //обработка логов игры
          std::ostringstream gameTimeString, playerPointsString;
          gameTimeString << gameTime;
          playerPointsString << points;
          lifetime.setString("Time: " + gameTimeString.str());
          pointCount.setString("Points: " + playerPointsString.str());

          //усложнение игры каждые 30 секунд
          if (gameTime != 0 && gameTime % 30 == 0) {
            for (it = asteroids.begin(); it != asteroids.end(); it++) {
              if((*it)->getSpeed() < 320)
                (*it)->setSpeed((*it)->getSpeed() + 10);
              if((*it)->getKD() > 4)
                (*it)->setKD((*it)->getKD() - 0.25);
            }
          }
          window.clear();
          window.draw(background);
          if(life)
           window.draw(player.player);
          window.draw(planet1.planet);
          window.draw(planet2.planet);
          window.draw(lifetime);
          window.draw(pointCount);
          if (!life && !player.anim_end) {
            player.Destroy(tl.dest_effect);
            window.draw(player.dest_sprite);
          }
          //столкновение игрока с астероидом
          if (life) {
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
            //перезарядка снаряда
            if (weapon.clock.getElapsedTime().asSeconds() > weaponKD) {
              spaceClick = false;
              weapon.update(screen);
              weapon.setIsMove(true);
            }
            //перемещение и рисование снаряда
            if (spaceClick && weapon.getIsMove()) {
              window.draw(weapon.weapon);
              weapon.Move(player);
            }
            //логика астероидов
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
              if ((*it)->getAsteroidMove()) (*it)->Move(player);
              //столкновение снаряда с астероидом
              if (weapon.weapon.getGlobalBounds().intersects((*it)->asteroid.getGlobalBounds()) && (*it)->getAsteroidMove()) {
                (*it)->dest_pos = (*it)->asteroid.getPosition();
                (*it)->dest_anim_clock.restart();
                (*it)->destroy = true;
                (*it)->update(spawnAsteroid.generator(screen));
                weapon.update(screen);
                weapon.setIsMove(false);
                (*it)->setAsteroidMove(false);
                points += 100;
                boom.play();
              }
              if ((*it)->destroy) {
                (*it)->Destroy(tl.dest_effect);
                window.draw((*it)->dest_sprite);
              }
              //перезарядка астероида
              if ((*it)->clock.getElapsedTime().asSeconds() > (*it)->getKD() && (*it)->getExist()) {
                (*it)->setAsteroidMove(true);
                (*it)->update(spawnAsteroid.generator(screen));
              }
              //столкновение астероида с планетой
              if (planet1.Intersects((*it)->asteroid)) {
                (*it)->dest_pos = (*it)->asteroid.getPosition();
                (*it)->dest_anim_clock.restart();
                (*it)->destroy = true;
                (*it)->update(spawnAsteroid.generator(screen));
                (*it)->setAsteroidMove(false);
                boom.play();
              }
            }
          }
          window.display();
    }

    return 0;
}
