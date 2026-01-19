#ifndef INPUT_OUTPUT_HXX
#define INPUT_OUTPUT_HXX

#include <iostream>
#include <sstream>
#include <vector>
#include <factory.hxx>

void load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);
std::vector<std::string> parse_line(std::string line);

#endif //INPUT_OUTPUT_HXX