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
#include <list>
#include <map>
#define PI 3.14159265

using namespace sf;

int main(int args, char *argv[])
{
  ContextSettings settings;
  settings.antialiasingLevel = 16; //��������� ������ ����������� �����
  RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Test shapes", Style::Fullscreen, settings);
//---------------------------------�������� � ������------------------------------------------
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
    Font font;
    font.loadFromFile("fonts/17734.otf");
//---------------------------------������ � �����---------------------------------
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
//---------------------------------���������� ����-------------------------------
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
    int sizePlanet = 150;
    float sizeAsteroid = 1024/10; 
    int gameTime = 0; //��������� � �������� ����� ����
    int points = 0; //���������� �����
    Clock gameTimeClock; //���� ����
    Clock clock; 
    Sprite background; //��� ����
    bool life = true; //��� �� �����
    bool spaceClick = false; //������ �� ������ space
    float weaponKD = 1.5; //�����, ����� ������� ������� �����������
    float asteroidKD = 5; //�����, ����� ������� �������� �����������
    float currentFrame = 0; //������� ����
    int radius;
    int area;
    std::vector<Vector2f> spawnPointsPlanet;
    std::map<int, Vector2f> useSpawnPointsPlanet;

    background.setTexture(tl.background);

    Vector2u screen = window.getSize();
    //��������� �����-������ ������ ������
    for (ptrdiff_t i = 0; i < 3; i++) {
      spawnPointsPlanet.push_back(Vector2f(screen.x / 3 * i, 0));
      spawnPointsPlanet.push_back(Vector2f(screen.x / 3 * i, screen.y / 2));
    }
//------------------------------�������� �������� ����-------------------------------------------------------
    Spawner spawner;
    Player player(tl.players[0], speed, angularSpeed, screen, w, h);
    Weapon weapon(tl.weapons[0], speed * 2.5, weaponKD);
    std::list<Asteroid*> asteroids;
    std::list<Asteroid*>::iterator it;
    std::list<Planet*> planets;
    std::list<Planet*>::iterator it_p;
    //��������� ������� �����
    lifetime.setPosition(screen.x - 200, 0);
    pointCount.setPosition(screen.x - 200, 40);

    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 100, asteroidKD, spawner.generatorAsteroids(screen), 10, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 93, asteroidKD, spawner.generatorAsteroids(screen), 20, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 90, asteroidKD, spawner.generatorAsteroids(screen), 30, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 102, asteroidKD, spawner.generatorAsteroids(screen), 60, tl.asteroids[0]));
    asteroids.push_back(new Asteroid(sizeAsteroid, speed + 95, asteroidKD, spawner.generatorAsteroids(screen), 120, tl.asteroids[0]));
    radius = spawner.generator(100, sizePlanet);
    area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
    planets.push_back(new Planet(radius, 5.0f, tl.planets[spawner.generator(0, tl.planets.size()-1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area), 3, 5, area, font, tl.ui[0]));
    radius = spawner.generator(100, sizePlanet);
    area = spawner.chooseArea(spawnPointsPlanet, useSpawnPointsPlanet);
    planets.push_back(new Planet(radius, 9.0f, tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area), 15, 8, area, font, tl.ui[0]));
//-------------------------------������� �������---------------------------------------------------------------
    while (window.isOpen())
    {
      //������������ ������
      if (!music.getStatus()) {
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
//--------------------------------����������--------------------------------------------------
          std::ostringstream gameTimeString, playerPointsString;
          gameTimeString << gameTime;
          playerPointsString << points;
          lifetime.setString("Time: " + gameTimeString.str());
          pointCount.setString("Points: " + playerPointsString.str());

//------------------------------��������� ���������-------------------------------------------
          if (gameTime != 0 && gameTime % 30 == 0) {
            for (it = asteroids.begin(); it != asteroids.end(); it++) {
              if((*it)->getSpeed() < 320)
                (*it)->setSpeed((*it)->getSpeed() + 10);
              if((*it)->getKD() > 4)
                (*it)->setKD((*it)->getKD() - 0.25);
            }
          }
//---------------------------------������ ���������-------------------------------------------
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
//-----------------------------------�������----------------------------------------------------
          for (it_p = planets.begin(); it_p != planets.end(); it_p++) {
            //������ �������
            if ((*it_p)->currentHp <= 0 && !(*it_p)->destroy) {
              (*it_p)->deathTime = gameTime;
              (*it_p)->destroy = true;
              (*it_p)->dest_anim_clock.restart();
              (*it_p)->born = false;
              (*it_p)->texture_planet = dest_effect_planet;
              //(*it_p)->planet.setTextureRect(IntRect(0, 122, 130, 130));
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
              (*it_p)->Die(radius, 5.0f, tl.planets[spawner.generator(0, tl.planets.size() - 1)], spawner.generatorPlanets(screen, spawnPointsPlanet, radius, area), 4, 5, area);
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
            if((*it_p)->born == true || (*it_p)->destroy)
              window.draw((*it_p)->planet);
            //����� �� ������� � ������ ���������
            if ((*it_p)->born) {
              window.draw((*it_p)->icon);
              (*it_p)->hpView.setString(std::to_string((*it_p)->currentHp) + " / " + std::to_string((*it_p)->hp));
              window.draw((*it_p)->hpView);
            }
          }
//---------------------------------------------------------------------------------------------------          
          if (life) {
            //������������ ������ � ����������
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
                (*it)->update(spawner.generatorAsteroids(screen));
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
                (*it)->update(spawner.generatorAsteroids(screen));
              }
              //������������ ��������� � ��������
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
