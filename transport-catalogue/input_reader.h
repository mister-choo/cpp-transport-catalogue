#pragma once

#include "transport_catalogue.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct TempBus {
  std::string name;
  Bus_Type type;
  std::vector<std::string> stops;
};

TransportCatalogue Read(std::istream& in = std::cin);