#include "transport_catalogue.h"

Bus TransportCatalogue::ConstructBus(const std::string &name,
                                     const Bus_Type type,
                                     const std::vector<std::string> &stops) {
  Bus res;
  res.name = name;
  res.type = type;
  for (const auto &stop : stops) {
    const auto index = name_to_stop_.at(stop);
    res.route.push_back(index);
  }

  return res;
}

BusIndex TransportCatalogue::AddBus(Bus &&bus) {
  auto &ref = busses_.emplace_back(std::move(bus));
  name_to_bus_[ref.name] = &ref;

  for (const auto &stop : ref.route) {
    stops_to_busses_[stop].insert(&ref);
    busses_to_stops_[&ref].insert(stop);
  }

  return &ref;
}

StopIndex TransportCatalogue::AddStop(Stop &&stop) {
  stops_.emplace_back(std::move(stop));

  const auto &new_el = stops_.back();

  name_to_stop_[std::string_view(new_el.name.data(), new_el.name.size())] =
      &new_el;

  return &stops_.back();
}

StopRef TransportCatalogue::SearchStops(const std::string &name) const {
  return *name_to_stop_.at(name);
}

BusRef TransportCatalogue::SearchBus(const std::string &name) const {
  return *name_to_bus_.at(name);
}

bool TransportCatalogue::ContainsStop(const std::string &name) const {
  return name_to_stop_.count(name) != 0;
}

TransportCatalogue::BusInfo
TransportCatalogue::GetBusInfo(const std::string &name) const {

  if (not name_to_bus_.count(name)) {
    return {name, 0, 0, 0, 0};
  }

  double distance = 0.0;
  double real_distance = 0.0;

  const auto index = name_to_bus_.at(name);
  const auto &bus = *index;

  const auto &route = bus.route;
  auto route_size = route.size();
  for (size_t i = 0LU; i < route.size() - 1LU; ++i) {
    // std::cout << route[i]->coords.lat << route[i]->coords.lng;

    distance += ComputeDistance(route[i]->coords, route[i + 1]->coords);
    real_distance += GetDistance({route[i], route[i + 1]});
    if (bus.type == FULL_PATH)
      real_distance += GetDistance({route[i + 1], route[i]});
  }

  if (bus.type == FULL_PATH) {
    distance *= 2.0;
    route_size *= 2;
    route_size -= 1;
  }

  return {name, route_size, busses_to_stops_.at(index).size(), real_distance,
          real_distance / distance};
}

std::string strip(std::string_view line) {
  const auto a = line.find_first_not_of(' '), b = line.find_last_not_of(' ');

  return std::string(line.substr(a, b - a + 1));
}

auto TransportCatalogue::GetStopInfo(const std::string &name) const
    -> TransportCatalogue::StopInfo {
  if (not stops_to_busses_.count(name_to_stop_.at(name)))
    return {};
  std::set<std::string> res;

  for (const auto &bus : stops_to_busses_.at(name_to_stop_.at(name))) {
    res.insert(bus->name);
  }
  return {name, res};
}

double TransportCatalogue::GetDistance(const Connection<StopIndex> &stops) const {
  if (relations_.count(stops))
    return relations_.at({stops});
  else
    return relations_.at({stops.to, stops.from});
}

int TransportCatalogue::GetDistance(
    const Connection<std::string> &stops) const {
  const auto &first = name_to_stop_.at(stops.from),
             &second = name_to_stop_.at(stops.to);
  if (relations_.count({first, second}))
    return relations_.at({first, second});
  else
    return relations_.at({second, first});
}

void TransportCatalogue::SetDistance(const Connection<std::string> &stops,
                                     int distance) {

  const auto &from = name_to_stop_.at(stops.from),
             &to = name_to_stop_.at(stops.to);
  relations_[{from, to}] = distance;
}