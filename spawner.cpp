#include "spawner.h"

Vector2f Spawner::generator(Vector2u screen) {
  std::uniform_int_distribution<> rangeX(-200, screen.x + 200);
  std::uniform_int_distribution<> rangeY1(-200, screen.y + 200);
  std::uniform_int_distribution<> rangeY2(-200, -20);
  std::uniform_int_distribution<> rangeY3(screen.y, screen.y + 200);
  std::uniform_int_distribution<> choice(0, 1);
  std::mt19937 mersenne(rd());
  int x = rangeX(mersenne);
  int y;
  if ((screen.x - x) > screen.x || (screen.x - x) < 0)
    y = rangeY1(mersenne);
  else {
    int s = choice(mersenne);
    if (s == 0) y = rangeY2(mersenne);
    if (s == 1) y = rangeY3(mersenne);
  }
  Vector2f result(x, y);
  return result;
}
