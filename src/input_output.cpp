#include <input_output.hxx>

void load_factory_structure(std::istream& is) {}

void save_factory_structure(Factory& factory, std::ostream& os) {}

std::vector<std::string> parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}