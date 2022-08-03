#include "input_reader.h"
#include <sstream>
#include <string>

/*std::string strip(std::string_view line) {
  const auto a = line.find_first_not_of(' '), b = line.find_last_not_of(' ');

  return std::string(line.substr(a, b - a + 1));
}*/

using Relat = std::pair<std::pair<std::string, std::string>, int>;

/*template <bool new_line = true> void DBG(const std::string &str) {
  if constexpr (new_line)
    std::cerr << "{" << str << "}"
              << "\n";
  else
    std::cerr << "{" << str << "}"
              << " ";
}

template <bool new_line = true>
void DBG(const std::string &str1, const std::string &str2) {
  if constexpr (new_line)
    std::cerr << str1 << " {" << str2 << "}"
              << "\n";
  else
    std::cerr << str1 << " {" << str2 << "}"
              << " ";
}*/


TransportCatalogue Read() {
  TransportCatalogue C;

  std::vector<Relat> stop_relations;
  std::vector<Temp_bus> bus_lines;

  int n;
  std::cin >> n;
  std::cin.ignore(1, '\n');

  while (n-- > 0) {
    // DBG("N", std::to_string(n));
    std::string full_line, type;
    std::getline(std::cin, full_line);
    std::istringstream c_in;
    c_in.str(full_line);

    // DBG("Full_line", full_line);
    c_in >> type;

    if (type == "Bus") {
      std::string name;
      std::getline(c_in, name, ':');
      name = strip(name);
      // DBG("Bus: ", name);

      char sep = [&full_line]() {
        if (full_line.find('>') != std::string::npos)
          return '>';
        else
          return '-';
      }();

      std::vector<std::string> stops;
      for (std::string stop_name; std::getline(c_in, stop_name, sep);) {

        stops.push_back(strip(stop_name));
      }

      bus_lines.push_back(
          Temp_bus{name, (sep == '>' ? Bus_Type::CIRCULAR : Bus_Type::DOUBLE),
                   std::move(stops)});

    } else if (type == "Stop") {
      std::string name;
      std::getline(c_in, name, ':');
      name = strip(name);
      // DBG("Stop: ", name);

      double lat, lgt;
      char c;
      c_in >> lat >> c >> lgt;

      // DBG("lat", std::to_string(lat));
      // DBG("lgt", std::to_string(lgt));

      if (full_line.find(" to ") != std::string::npos) {
        std::string relat;
        c_in >> c;
        while (std::getline(c_in, relat, ',')) {
          // DBG("relat", relat);
          const auto to = relat.find(" to ");

          // //DBG("linepart", linepart);
          // DBG("amount", relat.substr(0, to - 1));
          int amount = std::stoi(relat.substr(0, to - 1));
          std::string to_name = strip(relat.substr(to + 3, std::string::npos));
          // DBG("to name ", to_name);
          // DBG("name ", name);

          stop_relations.emplace_back(std::pair{name, strip(to_name)}, amount);
        }
      }
      C.Add(Stop{std::move(name), Coordinates{lat, lgt}});
    }
  }

  for (const auto &line : bus_lines) {
    C.Add(C.ConstructBus(line.name, line.type, line.stops));
  }

  for (const auto &[p, m] : stop_relations) {
    C.Add_Relation(p, m);
  }
  return C;
}