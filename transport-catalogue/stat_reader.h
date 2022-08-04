#pragma once

#include "transport_catalogue.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &out,
                         const TransportCatalogue::BusInfo &info);

std::ostream &operator<<(std::ostream &out,
                         const TransportCatalogue::StopInfo &info);

void Print(const TransportCatalogue::BusInfo &info);

void Print(const TransportCatalogue::StopInfo &info);

void Do(const TransportCatalogue &C, std::istream &in = std::cin,
        std::ostream &out = std::cout);