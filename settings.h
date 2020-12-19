#ifndef SETTINGS_H
#define SETTINGS_H
#include <SFML/Graphics.hpp>
#include <iosfwd>
#include <fstream>
#include <sstream>

class settings
{
public:
  settings(std::string& Path, int nickLength);
  settings() = default;
  settings(settings& set) = default;
  bool soundIsOn;
  bool musicIsOn;
  int soundVolume;
  int musicVolume;
  std::string nickname;
  int maxLengthNick;

  bool read();
  bool write();
  bool write(std::string line);
private:
  std::string path;
  void setDefault();
};
#endif

