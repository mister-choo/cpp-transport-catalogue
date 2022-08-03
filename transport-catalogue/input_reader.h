#pragma once

#include "transport_catalogue.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Temp_bus {
  std::string name;
  Bus_Type type;
  std::vector<std::string> stops;
};

TransportCatalogue Read();