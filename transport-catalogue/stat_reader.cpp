#include "stat_reader.h"

std::ostream &operator<<(std::ostream &out,
                         const TransportCatalogue::Info info) {
  out << "Bus " << info.name << ": ";
  if (info.stops_num == 0) {
    out << "not found" << std::endl;
  } else {
    out << info.stops_num << " stops on route, " << info.unique_stops
        << " unique stops, " << info.distance << " route length, "
        << info.curvature << " curvature" << std::endl;
  }
  return out;
}

void Do(TransportCatalogue &C) {

  int n;
  std::cin >> n;
  // std::cerr << "N:" << n << std::endl;

  while (n-- > 0) {

    std::string type;
    std::cin >> type;
    if (type == "Bus") {
      std::string name;
      std::getline(std::cin, name);
      std::cout << C.Bus_Info(strip(name));
    } else {
      std::string name;
      std::getline(std::cin, name);
      name = strip(name);
      if (not C.Contains_Stop(name)) {
        std::cout << "Stop " << name << ": not found" << std::endl;
      } else {
        const auto busses = C.Stop_Info(strip(name));
        if (busses.size()) {
          std::cout << "Stop " << name << ": buses ";
          int flag = 0;

          for (const auto &bus : busses) {
            std::cout << (flag++ ? " " : "") << bus;
          }
          std::cout << std::endl;
        } else {
          std::cout << "Stop " << name << ": no buses" << std::endl;
        }
      }
    }
  }
}