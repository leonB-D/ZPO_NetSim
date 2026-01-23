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

void save_factory_structure(const Factory& factory,std::ostream& os) {
    // RAMPY
	os << "; == LOADING RAMPS ==\n\n";
    for (auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
    	os << "LOADING_RAMP id=" << std::to_string(ramp->get_id()) << " delivery-interval="
    	<< std::to_string(ramp->get_delivery_interval()) << std::endl;
	}

    // PRACOWNICY
    os << "\n; == WORKERS ==\n\n";
    std::string queue_type;
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
    	switch (worker->get_queue()->get_queue_type()) {
        	case PackageQueueType::LIFO: {
            	queue_type = "LIFO";
                break;
    		}
            case PackageQueueType::FIFO: {
              queue_type = "FIFO";
              break;
            }
            default: break;
    	}
        os << "WORKER id=" << std::to_string(worker->get_id()) << " processing-time="
        << std::to_string(worker->get_processing_duration()) << " queue-type=" << queue_type << std::endl;
    }
    // MAGAZYNY
    os << "\n;== STOREHOUSES ==\n\n";
    for (auto storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++) {
    	os << "STOREHOUSE id=" << std::to_string(storehouse->get_id()) << std::endl;
    }
    // POŁĄCZENIA
    os << "\n;== LINKS ==\n\n";
    for (auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
        for (auto reciever = ramp->receiver_preferences_.cbegin(); reciever != ramp->receiver_preferences_.cend(); ++reciever) {
            std::string dest_type;
            if (reciever->first->get_receiver_type() == ReceiverType::WORKER) {
                dest_type = "worker-";
            }
            else if (reciever->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                dest_type = "store-";
            }
            os << "LINK src=ramp-" << std::to_string(ramp->get_id()) << " dest=" << dest_type << std::to_string(reciever->first->get_id()) << std::endl;
        }
    }
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        for (auto reciever = worker->receiver_preferences_.cbegin(); reciever != worker->receiver_preferences_.cend(); ++reciever) {
            std::string dest_type;
            if (reciever->first->get_receiver_type() == ReceiverType::WORKER) {
                dest_type = "worker-";
            }
            else if (reciever->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                dest_type = "store-";
            }
            os << "LINK src=worker-" << std::to_string(worker->get_id()) << " dest=" << dest_type << std::to_string(reciever->first->get_id()) << std::endl;
        }
    }
}