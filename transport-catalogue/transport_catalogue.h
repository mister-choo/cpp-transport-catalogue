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

struct T1;
struct T2;
using Bus = T1;
using Stop = T2;
using Bus_Index = const Bus *;
using Stop_Index = const Stop *;

using Bus_Ref = const Bus &;
using Stop_Ref = const Stop &;

enum Bus_Type { CIRCULAR, DOUBLE };

struct T1 {
  std::string name;
  Bus_Type type;
  std::vector<Stop_Index> route;
  std::unordered_set<Stop_Index> stops;
};

struct T2 {
  std::string name;
  Coordinates coords;
};

struct IndexHasher {
  size_t operator()(const std::pair<Stop_Index, Stop_Index> &idx) const {
    size_t h_f = d_hasher_(idx.first);
    size_t h_s = d_hasher_(idx.second);

    return h_f + h_s * 37;
  }

private:
  std::hash<const void *> d_hasher_;
};

class TransportCatalogue {

  std::deque<Bus> busses_;
  std::deque<Stop> stops_;

  std::unordered_map<std::string_view, Bus_Index> name_to_bus_;
  std::unordered_map<std::string_view, Stop_Index> name_to_stop_;
  std::unordered_map<Stop_Index, std::unordered_set<Bus_Index>>
      stops_to_busses_;

  std::unordered_map<std::pair<Stop_Index, Stop_Index>, int, IndexHasher>
      relations_;
  // ADD BUS
public:
  Bus ConstructBus(const std::string &name, const Bus_Type type,
                   const std::vector<std::string> &stops);

  Bus_Index Add(Bus &&bus);

  // ADD STOP

  Stop_Index Add(Stop &&stop);

  // SEARCH PATH

  Stop_Ref Search_Stops(const std::string &name);

  // SEARCH BUS
  Bus_Ref Search_Bus(const std::string &name);
  // BUS INFO
  struct Info {
    std::string name;
    size_t stops_num;
    size_t unique_stops;
    double distance;
    double curvature;
  };
  bool Contains_Stop(const std::string &name) const;
  Info Bus_Info(const std::string &name);

  auto Stop_Info(const std::string &name) const -> std::set<std::string>;

  void Add_Relation(const std::pair<std::string, std::string> &stops,
                    int amount);
  int Get_Relation(const std::pair<std::string, std::string> &stops);
  double Get_Relation(const std::pair<Stop_Index, Stop_Index> &stops);
};
