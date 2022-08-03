#pragma once

#include "transport_catalogue.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &out,
                         const TransportCatalogue::Info info);

void Do(TransportCatalogue &C);