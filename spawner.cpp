#include "spawner.h"

Vector2f Spawner::generatorAsteroids(Vector2u screen) {
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

Vector2f Spawner::generatorPlanets(Vector2u screen, std::vector<Vector2f>& spawnPoints, int radius, int& area) {
  std::mt19937 mersenne(rd());
  std::uniform_int_distribution<> rangeX(spawnPoints[area].x + radius, spawnPoints[area].x + (screen.x / 3 - radius));
  std::uniform_int_distribution<> rangeY(spawnPoints[area].y + radius, spawnPoints[area].y + (screen.y / 2 - radius));
  int x = rangeX(mersenne);
  int y = rangeY(mersenne);
  Vector2f result(x, y);
  return result;
}

int Spawner::chooseArea(std::vector<Vector2f>& spawnPoints, std::map<int, Vector2f>& useSpawnPoints) {
  std::mt19937 mersenne(rd());
  std::vector<int> availablePoints;
  int point;
  bool flag = true;
  for (ptrdiff_t i = 0; i < spawnPoints.size(); i++) {
    flag = true;
    try {
      if (spawnPoints[i] == useSpawnPoints.at(i)) {
        flag = false;
      }
    }
    catch (std::out_of_range&) { }
    if (flag) 
      availablePoints.push_back(i);
  }
  std::uniform_int_distribution<> index(0, availablePoints.size() - 1);
  point = availablePoints[index(mersenne)];
  useSpawnPoints[point] = spawnPoints[point];
  return point;
}

int Spawner::generator(int min, int max) {
  std::uniform_int_distribution<> range(min, max);
  std::mt19937 mersenne(rd());
  return range(mersenne);
}
