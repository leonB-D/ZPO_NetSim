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

Factory load_factory_structure(std::istream& is) {
    std::string line;
    std::vector<ParsedLineData> data;
    Factory factory;

    while (std::getline(is, line)) {
        if (!(line.empty() || line[0] == ';')) {
            data.push_back(parse_line(line));
        }
    }

    if (data.empty()) return factory;

    int i = 0;

    while (data[i].type == ElementType::RAMP) {
        int id = std::stoi(data[i].parameters["id"]);
        int di = std::stoi(data[i].parameters["delivery-interval"]);
        factory.add_ramp(Ramp(id, di));
        i++;
    }

    while (data[i].type == ElementType::WORKER) {
        int id = std::stoi(data[i].parameters["id"]);
        int pd = std::stoi(data[i].parameters["processing-time"]);

        if (data[i].parameters["queue-type"] == "LIFO") {
            factory.add_worker(Worker(id, pd, std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
        }
        else if (data[i].parameters["queue-type"] == "FIFO") {
            factory.add_worker(Worker(id, pd, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
        }
        else throw std::invalid_argument("Wrong queue type in input file");

        i++;
    }

    while (data[i].type == ElementType::STOREHOUSE) {
        int id = std::stoi(data[i].parameters["id"]);
        factory.add_storehouse(Storehouse(id, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
        i++;
    }

    while (data[i].type == ElementType::LINK) {
        std::string src = data[i].parameters["src"];
        std::string dest = data[i].parameters["dest"];
        link_from_input(src, dest, factory);

        i++;
    }

    return factory;
}

void link_from_input(std::string& src, std::string& dest, Factory& factory) {
    int pos = src.find('-');
    std::string src_type = src.substr(0, pos);
    int src_id = std::stoi(src.substr(pos + 1, src.length()));

    pos = dest.find('-');
    std::string dest_type = dest.substr(0, pos);
    int dest_id = std::stoi(dest.substr(pos + 1, dest.length()));

    IPackageReceiver* dest_ptr;

    if (dest_type == "worker") dest_ptr = &*factory.find_worker_by_id(dest_id);

    else if (dest_type == "store") dest_ptr = &*factory.find_storehouse_by_id(dest_id);

    else throw std::invalid_argument("Wrong destination type in input file");

    if (src_type == "ramp") factory.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(dest_ptr);

    else if (src_type == "worker") factory.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(dest_ptr);

    else throw std::invalid_argument("Wrong source type in input file");
}