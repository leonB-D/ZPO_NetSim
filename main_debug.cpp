//test
#include "nodes.hxx"
#include "input_output.hxx"
#include <iostream>
#include <fstream>

int main() {
    // std::cout<<"Hello World!"<<std::endl;
    //
    // ReceiverPreferences rp;
    // Worker ip1(1, 5, std::make_unique<PackageQueue>(PackageQueueType::LIFO));
    // Worker ip2(2, 10, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    // Storehouse ip3(3, std::make_unique<PackageQueue>(PackageQueueType::LIFO));
    // rp.add_receiver(&ip1);
    // rp.add_receiver(&ip2);
    // rp.add_receiver(&ip3);
    //
    // for (auto item : rp.get_preferences()) {
    //     std::cout<<item.first->get_id()<<" "<<item.second<<std::endl;
    // }
    //
    // std::vector<int> sums = {0, 0, 0};
    // for (int i=0; i<100; i++) {
    //     int id = rp.choose_receiver()->get_id();
    //     sums[id-1] += 1;
    // }
    // std::cout<<"1: "<<sums[0]<<"\n2: "<<sums[1]<<"\n3: "<<sums[2];

    std::string line = "WORKER id=<worker-id> processing-time=<processing-time> queue-type=<queue-type>";

    ParsedLineData data = parse_line(line);

    for (auto it : data.parameters) {
        std::cout << it.first << " : " << it.second << std::endl;
    }

    std::ifstream input_file("struct-input.txt");

    Factory factory = load_factory_structure(input_file);

    std::cout << factory.find_ramp_by_id(1)->get_delivery_interval() << std::endl;
    std::cout << factory.find_worker_by_id(2)->receiver_preferences_.begin()->first->get_id() << std::endl;

    input_file.close();

    return 0;
}