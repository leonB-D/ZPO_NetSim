#include <input_output.hxx>

ParsedLineData parse_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::string type_token;

    std::istringstream token_stream(line);

    std::getline(token_stream, type_token, ' ');

    ParsedLineData data;

    if (type_token == "LOADING_RAMP") data.type = ElementType::RAMP;
    else if (type_token == "WORKER") data.type = ElementType::WORKER;
    else if (type_token == "STOREHOUSE") data.type = ElementType::STOREHOUSE;
    else if (type_token == "LINK") data.type = ElementType::LINK;
    else throw std::invalid_argument("Wrong type in input file");

    std::string key;
    std::string value;

    while (std::getline(token_stream, key, '=') && std::getline(token_stream, value, ' ')) {
        data.parameters.insert(std::pair<std::string, std::string>(key, value));
    }

    return data;
}

void load_factory_structure(std::istream& is) {
    std::string line;
    std::vector<ParsedLineData> data;

    while (std::getline(is, line)) {
        if (line.empty() || line[0] == ';') continue;

        data.push_back(parse_line(line));
    }
}

void save_factory_structure(Factory& factory, std::ostream& os) {}