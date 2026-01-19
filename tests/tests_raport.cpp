//
// Created by Kacper on 19.01.2026.
//
#include <gtest/gtest.h>
#include "factory.hxx"
#include "raport.hpp"

#include <functional>
TEST(RaportTest, StructureReport_R1W1S1) {
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1,std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));


    std::vector<std::string> expected_report_lines{
        "",
        "== LOADING RAMPS ==",
        "",
        "LOADING RAMP #1",
        "  Delivery interval: 1",
        "  Receivers:",
        "    worker #1",
        "",
        "",
        "== WORKERS ==",
        "",
        "WORKER #1",
        "  Processing time: 1",
        "  Queue type: FIFO",
        "  Receivers:",
        "    storehouse #1",
        "",
        "",
        "== STOREHOUSES ==",
        "",
        "STOREHOUSE #1",
        "",
};
}

TEST(ReportsTest, TurnReportPackageInQueue) {

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

    std::vector<std::string> expected_report_lines{
        "=== [ Turn: " + std::to_string(t) + " ] ===",
        "",
        "== WORKERS ==",
        "",
        "WORKER #1",
        "  PBuffer: (empty)",
        "  Queue: #1",
        "  SBuffer: (empty)",
        "",
        "",
        "== STOREHOUSES ==",
        "",
        "STOREHOUSE #1",
        "  Stock: (empty)",
        "",
};
}