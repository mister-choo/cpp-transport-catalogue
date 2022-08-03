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
    res.stops.insert(index);
  }

  return res;
}
Bus_Index TransportCatalogue::Add(Bus &&bus) {
  auto &ref = busses_.emplace_back(std::move(bus));
  name_to_bus_[ref.name] = &ref;

  for (const auto &stop : ref.stops) {
    stops_to_busses_[stop].insert(&ref);
  }

  return &ref;
}
Stop_Index TransportCatalogue::Add(Stop &&stop) {
  stops_.emplace_back(std::move(stop));

  const auto &new_el = stops_.back();

  name_to_stop_[std::string_view(new_el.name.data(), new_el.name.size())] =
      &new_el;

  return &stops_.back();
}
Stop_Ref TransportCatalogue::Search_Stops(const std::string &name) {
  return *name_to_stop_.at(name);
}
Bus_Ref TransportCatalogue::Search_Bus(const std::string &name) {
  return *name_to_bus_.at(name);
}

bool TransportCatalogue::Contains_Stop(const std::string &name) const {
  return name_to_stop_.count(name);
}

TransportCatalogue::Info TransportCatalogue::Bus_Info(const std::string &name) {

  if (not name_to_bus_.count(name)) {
    return Info{name, 0, 0, 0, 0};
  }

  double distance = 0.0;
  double true_distance = 0.0;
  const auto &bus = *name_to_bus_.at(name);
  const auto &route = bus.route;
  auto route_size = route.size();
  for (size_t i = 0LU; i < route.size() - 1LU; ++i) {
    // std::cout << route[i]->coords.lat << route[i]->coords.lng;

    distance += ComputeDistance(route[i]->coords, route[i + 1]->coords);
    true_distance += Get_Relation({route[i], route[i + 1]});
    if (bus.type == DOUBLE)
      true_distance += Get_Relation({route[i + 1], route[i]});
  }

  if (bus.type == DOUBLE) {
    distance *= 2.0;
    route_size *= 2;
    route_size -= 1;
  }

  return Info{name, route_size, bus.stops.size(), true_distance,
              true_distance / distance};
}
std::string strip(std::string_view line) {
  const auto a = line.find_first_not_of(' '), b = line.find_last_not_of(' ');

  return std::string(line.substr(a, b - a + 1));
}

auto TransportCatalogue::Stop_Info(const std::string &name) const
    -> std::set<std::string> {
  if (not stops_to_busses_.count(name_to_stop_.at(name)))
    return {};
  std::set<std::string> res;

  for (const auto &bus : stops_to_busses_.at(name_to_stop_.at(name))) {
    res.insert(bus->name);
  }
  return res;
}
double TransportCatalogue::Get_Relation(
    const std::pair<Stop_Index, Stop_Index> &stops) {
  if (relations_.count(stops))
    return relations_.at({stops});
  else
    return relations_.at({stops.second, stops.first});
}
int TransportCatalogue::Get_Relation(
    const std::pair<std::string, std::string> &stops) {
  const auto &first = name_to_stop_.at(stops.first),
             &second = name_to_stop_.at(stops.second);
  if (relations_.count({first, second}))
    return relations_.at({first, second});
  else
    return relations_.at({second, first});
}

void TransportCatalogue::Add_Relation(
    const std::pair<std::string, std::string> &stops, int amount) {

  const auto &first = name_to_stop_.at(stops.first),
             &second = name_to_stop_.at(stops.second);
  relations_[std::pair{first, second}] = amount;
}