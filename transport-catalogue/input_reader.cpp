#include "input_reader.h"
#include <sstream>
#include <string>

using Relat = std::pair<Connection<std::string>, int>;

TransportCatalogue Read(std::istream &in) {
  TransportCatalogue catalogue;

  std::vector<Relat> stop_relations;
  std::vector<TempBus> bus_lines;

  int n;
  in >> n;
  in.ignore(1, '\n');

  while (n-- > 0) {
    // DBG("N", std::to_string(n));
    std::string full_line, type;
    std::getline(in, full_line);
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
          TempBus{name, (sep == '>' ? Bus_Type::ROUND : Bus_Type::FULL_PATH),
                  std::move(stops)});

    } else if (type == "Stop") {
      std::string name;
      std::getline(c_in, name, ':');
      name = strip(name);

      double lat, lgt;
      char c;
      c_in >> lat >> c >> lgt;

      if (full_line.find(" to ") != std::string::npos) {
        std::string relat;
        c_in >> c;
        while (std::getline(c_in, relat, ',')) {

          const auto to = relat.find(" to ");

          // DBG("amount", relat.substr(0, to - 1));
          int amount = std::stoi(relat.substr(0, to - 1));
          std::string to_name = strip(relat.substr(to + 3, std::string::npos));
          stop_relations.emplace_back(
              Connection<std::string>{name, strip(to_name)}, amount);
        }
      }
      catalogue.AddStop(Stop{std::move(name), Coordinates{lat, lgt}});
    }
  }

  for (const auto &line : bus_lines) {
    catalogue.AddBus(catalogue.ConstructBus(line.name, line.type, line.stops));
  }

  for (const auto &[pair, distance] : stop_relations) {
    catalogue.SetDistance(pair, distance);
  }
  return catalogue;
}