#ifndef SPAWNER_H
#define SPAWNER_H

#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;
class Spawner
{
  std::random_device rd;
public:
  Vector2f generatorAsteroids(Vector2u screen);
  Vector2f generatorPlanets(Vector2u screen, std::vector<Vector2f>& spawnPoints, int radius, int& area);
  int chooseArea(std::vector<Vector2f>& spawnPoints, std::map<int, Vector2f>& useSpawnPoints);
  int generator(int min, int max);
};
#endif