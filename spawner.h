#ifndef SPAWNER_H
#define SPAWNER_H
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>

using namespace sf;
class Spawner
{
  std::random_device rd;
public:
  Vector2f generator(Vector2u screen);
};
#endif