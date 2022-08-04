#pragma once

#include <deque>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <string>
#include <string_view>
#include <vector>

#include "geo.h"

std::string strip(std::string_view line);

struct Bus;
struct Stop;

using BusIndex = const Bus *;
using StopIndex = const Stop *;

// clang-format off

enum Bus_Type { 
  ROUND, 
  FULL_PATH 
};

// clang-format on

struct Bus {
  std::string name;
  Bus_Type type;
  std::vector<StopIndex> route;
};

struct Stop {
  std::string name;
  Coordinates coords;
};

using BusRef = const Bus &;
using StopRef = const Stop &;
template <typename T> struct Connection {
  T from, to;
  inline bool operator==(const Connection<T> &oth) const {
    return from == oth.from and to == oth.to;
  }
};

struct IndexHasher {
  size_t operator()(const Connection<StopIndex> &index_pair) const {
    size_t h_f = d_hasher_(index_pair.from);
    size_t h_s = d_hasher_(index_pair.to);

    return h_f + h_s * 37;
  }

private:
  std::hash<const void *> d_hasher_;
};

class TransportCatalogue {

  std::deque<Bus> busses_;
  std::deque<Stop> stops_;

  std::unordered_map<std::string_view, BusIndex> name_to_bus_;
  std::unordered_map<std::string_view, StopIndex> name_to_stop_;

  std::unordered_map<StopIndex, std::unordered_set<BusIndex>> stops_to_busses_;
  std::unordered_map<BusIndex, std::unordered_set<StopIndex>> busses_to_stops_;

  std::unordered_map<Connection<StopIndex>, int, IndexHasher> relations_;
  // ADD BUS
public:
  Bus ConstructBus(const std::string &name, const Bus_Type type,
                   const std::vector<std::string> &stops);

  BusIndex AddBus(Bus &&bus);

  // ADD STOP

  StopIndex AddStop(Stop &&stop);

  // SEARCH PATH

  StopRef SearchStops(const std::string &name) const;

  // SEARCH BUS
  BusRef SearchBus(const std::string &name) const;
  // BUS INFO
  struct BusInfo {
    std::string name;
    size_t stops_num;
    size_t unique_stops;
    double distance;
    double curvature;
  };

  bool ContainsStop(const std::string &name) const;

  BusInfo GetBusInfo(const std::string &name) const;

  struct StopInfo {
    std::string name;
    std::set<std::string> buses;
  };

  auto GetStopInfo(const std::string &name) const -> StopInfo;

  void SetDistance(const Connection<std::string> &stops, int amount);

  int GetDistance(const Connection<std::string> &stops) const;

  double GetDistance(const Connection<StopIndex> &stops) const;
};
