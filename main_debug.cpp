//test
#include "factory.hxx"
#include "raport.hpp"


int main() {
    std::cout<<"Hello World!"<<std::endl;

    Factory factory;

    factory.add_ramp(Ramp(1, 10));
    factory.add_worker(Worker(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1,std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    Time t = 1;
    r.deliver_goods(t);
    r.send_package();
    generate_structure_report(factory,std::cout);
}