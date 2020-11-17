#include "player.h"
#include "textureLoader.h"
#include "planet.h"
#include "asteroid.h"
#include "weapon.h"
#include "spawner.h"

#include <SFML/Graphics.hpp>
#include <math.h>
#include <sstream>
#include <random>
#include <list>
#define PI 3.14159265

using namespace sf;

int main(int args, char *argv[])
{
    textureLoader tl;
    Texture texture;

    std::vector<String> paths = { "textures/planet/Orange-Planet.png", "textures/planet/Lava-PLanet.png" };
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
    Text lifetime;
    Text pointCount;
    pointCount.setFont(font);
    lifetime.setFont(font);

    int width = 50;
    int heigth = 75;
    int w = 112;
    int h = 107;
    double speed = 165;
    double angularSpeed = 1;
    float sizePlanet = 25.0f;
    float sizeAsteroid = 10.0f;
    int gameTime = 0;
    int points = 0;
    Clock gameTimeClock;
    Clock weaponClock;
    Clock clock;
    Clock asteroidClock;
    Clock gameClock;
    Sprite background;
    Sprite sprite;
    bool spaceClick = false;
    bool weaponMove = true;
    bool asteroidMove = true;
    bool life = true;
    float weaponKD = 2.5;
    float asteroidKD = 6;

    background.setTexture(tl.background);
    sprite.setTexture(tl.players[0]);

    ContextSettings settings;
    settings.antialiasingLevel = 16; //установка уровня сглаживания фигур
    RenderWindow window(VideoMode(1920, 1080), "Test shapes", Style::Fullscreen, settings);

    Vector2u screen = window.getSize();
    //создание объектов игры
    Spawner spawnAsteroid;
    Planet planet1(sizePlanet, 5.0f, tl.planets[0], Vector2f(140.0f, 267.0f));
    Planet planet2(sizePlanet, 9.0f, tl.planets[1], Vector2f(1094.0f, 872.0f));
    Player player(sprite, speed, angularSpeed, screen, w, h);
    std::list<Asteroid*> asteroids;
    std::list<Asteroid*>::iterator it;
    Weapon weapon(tl.weapons[0], speed * 2.0);
    //установка позиции логов
    lifetime.setPosition(screen.x - 200, 0);
    pointCount.setPosition(screen.x - 200, 40);
    float currentFrame = 0; //текущий кадр

    asteroids.push_back(new Asteroid(1.0f, angularSpeed, speed, asteroidKD, spawnAsteroid.generator(screen), 1, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(1.0f, angularSpeed, speed + 125, asteroidKD, spawnAsteroid.generator(screen), 5, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(1.0f, angularSpeed, speed + 143, asteroidKD, spawnAsteroid.generator(screen), 15, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(1.0f, angularSpeed, speed + 117, asteroidKD, spawnAsteroid.generator(screen), 30, tl.asteroids[0]));
    while (window.isOpen())
    {
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
        if (Keyboard::isKeyPressed(Keyboard::Left)) { 
            player.flyLeft(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player.flyRight(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            player.flyForward(time, currentFrame);    
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) { 
            player.flyBack(time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Down)) {
            player.flyForwardAndBack();
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && weaponMove) {
                spaceClick = true;
        }
        //body - границы спрайта игрока без пламени
          //обработка логов игры
          std::ostringstream gameTimeString, playerPointsString;
          gameTimeString << gameTime;
          playerPointsString << points;
          lifetime.setString("Time: " + gameTimeString.str());
          pointCount.setString("Points: " + playerPointsString.str());
          FloatRect body(player.player.getGlobalBounds().left, player.player.getGlobalBounds().top, player.player.getGlobalBounds().width, player.player.getGlobalBounds().height / 2);
          for (it = asteroids.begin(); it != asteroids.end(); it++) {
            if (body.intersects((*it)->asteroid.getGlobalBounds())) { //столкновение игрока с астероидом
              player.player.setColor(Color::Transparent);
              life = false;
              lifetime.setPosition(screen.x / 2 - 120, screen.y / 2 - 50);
              lifetime.setScale(2.0, 2.0);
              pointCount.setPosition(screen.x / 2 - 120, screen.y / 2 - 100);
              pointCount.setScale(2.0, 2.0);
            }
          }
          window.clear();
          window.draw(background);
          window.draw(player.player);
          window.draw(planet1.planet);
          window.draw(planet2.planet);
          window.draw(lifetime);
          window.draw(pointCount);
          int weaponX = weapon.weapon.getPosition().x;
          int weaponY = weapon.weapon.getPosition().y;
          if (weaponClock.getElapsedTime().asSeconds() > weaponKD && (weaponX > screen.x || weaponX < 0 || weaponY > screen.y || weaponY < 0)) { //перезарядка снаряда       ПРИКРУТИТЬ УСЛОВИЕ ВЫХОДА СНАРЯДА ЗА ЭКРАН!!!
            weaponClock.restart();
            spaceClick = false;
            weapon.update(screen);
            weaponMove = true;
          }
          if (spaceClick && weaponMove) { //если нажата кнопка стрельбы, то запуск снаряда
            window.draw(weapon.weapon);
            weapon.Move(player);
          }
          for (it = asteroids.begin(); it != asteroids.end(); it++) {
            if ((*it)->getSpawnTime() <= gameTime) {
              (*it)->setAsteroidMove(true);
              window.draw((*it)->asteroid);
            }
            if ((*it)->getAsteroidMove()) (*it)->Move(player);
            if (weapon.weapon.getGlobalBounds().intersects((*it)->asteroid.getGlobalBounds()) && (*it)->getAsteroidMove()) { //столкновение снаряда с астероидом
              (*it)->update(spawnAsteroid.generator(screen));
              weapon.update(screen);
              spaceClick = false;
              weaponMove = false;
              (*it)->setAsteroidMove(false);
              (*it)->clock.restart();
              points += 100;
            }
            if ((*it)->clock.getElapsedTime().asSeconds() > (*it)->getKD() && (*it)->getSpawnTime() <= gameTime) { //перезарядка астероида
              (*it)->setAsteroidMove(true);
              (*it)->update(spawnAsteroid.generator(screen));
              (*it)->clock.restart();
            }
          }
          window.display();
    }

    return 0;
}
