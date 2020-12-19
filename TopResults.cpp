#include "TopResults.h"

TopResults::TopResults(std::string& Path, int SizeTop) : path(Path), sizeTop(SizeTop), defaultResult({"", 0, 0}) {}

bool Result::operator==(const Result rhs) const {
  return (name == rhs.name && points == rhs.points && time == rhs.time);
}

void TopResults::makeView(int charSize, sf::Font& font, sf::Color color) {
  text.setFont(font);
  text.setFillColor(color);
  text.setCharacterSize(charSize);
  int lName, lPoint, lTime, totalLength;
  lName = lPoint = lTime = 0;
  int minName, minPoint, minTime, minLength, titleLength, countSpace;
  std::string title, name, points, time;
  title = "RECORDS"; name = "NAME"; points = "POINTS"; time = "TIME";
  minName = name.length();
  minPoint = points.length();
  minTime = time.length();
  titleLength = title.length();
  countSpace = 4;
  minLength = minName + minPoint + minTime + countSpace;
  for (ptrdiff_t i = 0; i < top.size(); i++) {
    if (top[i].name.length() > lName) lName = top[i].name.length();
    if (std::to_string(top[i].points).length() > lPoint) lPoint = std::to_string(top[i].points).length();
    if (std::to_string(top[i].time).length() > lTime) lTime = std::to_string(top[i].time).length();
  }
  totalLength = lName + lPoint + lTime + countSpace;
  if (totalLength <= minLength) totalLength = minLength;
  if (lName <= minName) lName = minName;
  if (lPoint <= minPoint) lPoint = minPoint;
  if (lTime <= minTime) lTime = minTime;
  std::string str;
  std::stringstream stream;
  stream << makeSpace(totalLength / 2 - titleLength / 2) << title << "\n";
  stream << makeSpace(2) << name << makeSpace(1 + lName - minName) << points << makeSpace(1 + lPoint - minPoint) << time << "\n";
  if(top[0] != defaultResult)
    for (ptrdiff_t i = 0; i < top.size(); i++) {
      stream << std::to_string(i + 1) << makeSpace(1) << top[i].name << makeSpace(lName - top[i].name.length() + 1) <<
        std::to_string(top[i].points) << makeSpace(lPoint - std::to_string(top[i].points).length() + 1) << std::to_string(top[i].time);
      if (i != top.size() - 1) stream << "\n";
    }
  str = stream.str();
  text.setString(str);
  text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
}

bool TopResults::read() {
  std::string line, name, total;
  std::ifstream reader(path);
  std::stringstream lines;
  int point, time;
  if (reader.is_open()) {
    if (reader.peek() != EOF) {
      while (getline(reader, line)) {
        lines << line;
        lines >> name >> point >> time;
        top.push_back({ name, point, time });
        lines.clear();
      }
      reader.close();
    }
    else {
      top.push_back({"", 0, 0});
    }
    return true;
  }
  return false;
}

bool TopResults::write(std::string name, int point, int time) {
  top.push_back({ name, point, time });
  std::sort(top.begin(), top.end(), [](const Result& lhs, const Result& rhs) { return lhs.points > rhs.points; });
  if(top.size() > sizeTop)
    top.pop_back();
  return write();
}

bool TopResults::write() {
  std::ofstream writer(path);
  std::stringstream stream;
  if (writer.is_open()) {
    writer.clear();
    for (Result i : top) {
      stream << i.name << " " << i.points << " " << i.time << '\n';
    }
    writer << stream.str();
    writer.close();
    return true;
  }
  return false;
}


bool TopResults::isRecord(int point) {
  for (Result i : top) {
    if (i.points < point) return true;
  }
  return false;
}

std::string TopResults::makeSpace(int count) {
  std::string str = "";
  for (ptrdiff_t i = 0; i < count; i++) {
    str += " ";
  }
  return str;
}
