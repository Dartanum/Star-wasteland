#include "settings.h"

settings::settings(std::string& Path, int nickLength) : path(Path), maxLengthNick(nickLength) {}

bool settings::read() {
  std::string line;
  std::vector<std::string> lines;
  std::ifstream reader(path);
  if (reader.is_open()) {
    if (reader.peek() != EOF) {
      while (getline(reader, line)) {
        lines.push_back(line);
      }
      reader.close();
      soundIsOn = std::stoi(lines[0]);
      musicIsOn = std::stoi(lines[1]);
      soundVolume = std::stoi(lines[2]);
      musicVolume = std::stoi(lines[3]);
      nickname = lines[4];
      return true;
    }
    else {
      setDefault();
      write();
    }
  }
  else throw std::exception("settings file closed");
  return false;
}

bool settings::write() {
  std::ofstream writer(path);
  if (writer.is_open()) {
    writer << soundIsOn << '\n';
    writer << musicIsOn << '\n';
    writer << soundVolume << '\n';
    writer << musicVolume << '\n';
    writer << nickname << '\n';
    writer.close();
    return true;
  }
  else throw std::exception("settings file closed");
  return false;
}

bool settings::write(std::string line) {
  std::ofstream writer(path);
  std::vector<std::string> params;
  std::string temp;
  std::istringstream stream(line);
  while (stream >> temp)
    params.push_back(temp);
  if (writer.is_open()) {
    for (ptrdiff_t i = 0; i < params.size(); i++)
      writer << params[i] << '\n';
  }
  else throw std::exception("settings file closed");
  return false;
}

void settings::setDefault() {
  soundIsOn = 1;
  musicIsOn = 1;
  soundVolume = 20;
  musicVolume = 20;
  nickname = "Player";
}