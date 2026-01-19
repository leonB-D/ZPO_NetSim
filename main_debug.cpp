//test
#include "nodes.hxx"
#include "input_output.hxx"
#include <iostream>
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

    auto tokens = parse_line("Ala ma kota.");
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }

    return 0;
}