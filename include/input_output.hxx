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
void link_from_input(std::string& src, std::string& dest, Factory& factory);
Factory load_factory_structure(std::istream& is);

#endif //INPUT_OUTPUT_HXX