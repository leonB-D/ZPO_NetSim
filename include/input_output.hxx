#ifndef INPUT_OUTPUT_HXX
#define INPUT_OUTPUT_HXX

#include <iostream>
#include <sstream>
#include <vector>
#include <factory.hxx>

struct ParsedLineData {
    ElementType type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(const std::string& line);
void load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);

#endif //INPUT_OUTPUT_HXX