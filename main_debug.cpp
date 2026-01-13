//test
#include "nodes.hxx"
#include <iostream>
int main() {
    std::cout<<"Hello World!"<<std::endl;

    ReceiverPreferences rp;
    Worker ip1(1, 5, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Worker ip2(2, 10, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Storehouse ip3(3, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    rp.add_receiver(&ip1);
    rp.add_receiver(&ip2);
    rp.add_receiver(&ip3);


    return 0;
}