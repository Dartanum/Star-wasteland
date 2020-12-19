#ifndef TOPRESULTS_H
#define TOPRESULTS_H

#include <SFML/Graphics.hpp>
#include <iosfwd>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

struct Result
{
  bool operator==(const Result rhs) const;
  bool operator!=(const Result rhs) const { return !operator==(rhs); }
  std::string name;
  int points = 0;
  int time = 0;
};

class TopResults
{
public:
  TopResults(std::string& Path, int SizeTop);
  TopResults() = default;
  TopResults(TopResults&) = default;

  std::vector<Result> top;
  sf::Text text;
  bool read();
  bool write();
  bool write(std::string name, int point, int time);
  bool isRecord(int point);
  void makeView(int charSize, sf::Font& font, sf::Color color);
private:
  std::string makeSpace(int count);
  std::string path;
  Result defaultResult;
  int sizeTop;
};
#endif
