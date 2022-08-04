#include "stat_reader.h"

std::ostream &operator<<(std::ostream &out,
                         const TransportCatalogue::BusInfo &info) {
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

std::ostream &operator<<(std::ostream &out,
                         const TransportCatalogue::StopInfo &info) {

  if (info.buses.size()) {
    out << "Stop " << info.name << ": buses ";
    int flag = 0;

    for (const auto &bus : info.buses) {
      out << (flag++ ? " " : "") << bus;
    }
    out << std::endl;
  } else {
    out << "Stop " << info.name << ": no buses" << std::endl;
  }
  return out;
}

void Print(const TransportCatalogue::BusInfo &info) { std::cout << info; }

void Print(const TransportCatalogue::StopInfo &info) { std::cout << info; }

void Do(const TransportCatalogue &catalogue, std::istream &in,
        std::ostream &out) {
  int n;
  in >> n;
  // std::cerr << "N:" << n << std::endl;

  while (n-- > 0) {

    std::string type;
    in >> type;
    if (type == "Bus") {
      std::string name;
      std::getline(in, name);
      out << catalogue.GetBusInfo(strip(name));
    } else {
      std::string name;
      std::getline(in, name);
      name = strip(name);
      if (not catalogue.ContainsStop(name))
        out << "Stop " << name << ": not found" << std::endl;
      else
        std::cout << catalogue.GetStopInfo(strip(name));
    }
  }
}
