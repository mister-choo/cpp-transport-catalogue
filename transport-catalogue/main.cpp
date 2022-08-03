#include <iostream>

#include <string>
#include <string_view>
#include <vector>

#include "input_reader.h"
#include "stat_reader.h"
#include "transport_catalogue.h"

/*std::ostream &operator<<(std::ostream &out,
                         const std::vector<std::string> vec) {
  for (const auto &el : vec)
    out << "[" << el << "] ";

  return out;
}*/

int main() {

  TransportCatalogue C = Read();

  Do(C);

  return 0;
}