//test
#include "factory.hxx"
#include "raport.hpp"


int main()
{
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1,std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));


    for (Time t = 1; t <= 10; t++)
    {
        factory.do_work(t);
        factory.do_deliveries(t);
        factory.do_package_passing();
        generate_simulation_turn_report(factory, std::cout, t);
    }

    return 0;
}