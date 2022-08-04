#include <iostream>

#include <string>
#include <string_view>
#include <vector>

#include "input_reader.h"
#include "stat_reader.h"
#include "transport_catalogue.h"


int main() {

  TransportCatalogue catalogue = Read();

  Do(catalogue);

  return 0;
}