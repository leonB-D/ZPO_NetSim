//
// Created by Kacper on 19.01.2026.
//
#include <gtest/gtest.h>
#include "factory.hxx"
#include "raport.hpp"

#include <functional>
#include <gmock/gmock-matchers.h>
using ::testing::ContainerEq;

// Funkcje potrzebne do porównania linii wygenerowanych raportów z liniami, jakie powinny się w nim znaleźć
void perform_report_check(std::function<void(std::ostringstream&)>& reporting_function,
                          std::vector<std::string>& expected_report_lines) {
    std::ostringstream report_oss;
    reporting_function(report_oss);

    std::vector<std::string> output_lines;
    std::istringstream iss;
    iss.str(report_oss.str());
    std::string line;
    while (std::getline(iss, line, '\n')) {
        output_lines.push_back(line);
    }

    std::ostringstream expected_report_oss;
    for (auto& line : expected_report_lines) {
        expected_report_oss << line << "\n";
    }
    ASSERT_EQ(output_lines.size(), output_lines.size());
    EXPECT_THAT(output_lines, ContainerEq(expected_report_lines));


}

void perform_turn_report_check(const Factory& factory, Time t, std::vector<std::string>& expected_report_lines) {
    std::function<void(std::ostringstream&)> reporting_function = [&factory, t](
            std::ostringstream& oss) { generate_simulation_turn_report(factory, oss, t); };
    perform_report_check(reporting_function, expected_report_lines);
}

void perform_structure_report_check(const Factory& factory, std::vector<std::string>& expected_report_lines) {
    std::function<void(std::ostringstream&)> reporting_function = [&factory](
            std::ostringstream& oss) { generate_structure_report(factory, oss); };
    perform_report_check(reporting_function, expected_report_lines);
}

//TESTY

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
    perform_structure_report_check(factory, expected_report_lines);
}
TEST(ReportsTest, StructureReport_R2W2S2) {
    // Utwórz fabrykę.
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_ramp(Ramp(2, 2));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(2, 2, std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
    factory.add_storehouse(Storehouse(1,std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(2,std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r1 = *(factory.find_ramp_by_id(1));
    r1.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Ramp& r2 = *(factory.find_ramp_by_id(2));
    r2.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));
    r2.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));

    Worker& w1 = *(factory.find_worker_by_id(1));
    w1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    Worker& w2 = *(factory.find_worker_by_id(2));
    w2.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    w2.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(2)));

    // -----------------------------------------------------------------------

    std::vector<std::string> expected_report_lines{
            "",
            "== LOADING RAMPS ==",
            "",
            "LOADING RAMP #1",
            "  Delivery interval: 1",
            "  Receivers:",
            "    worker #1",
            "",
            "LOADING RAMP #2",
            "  Delivery interval: 2",
            "  Receivers:",
            "    worker #1",
            "    worker #2",
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
            "WORKER #2",
            "  Processing time: 2",
            "  Queue type: LIFO",
            "  Receivers:",
            "    storehouse #1",
            "    storehouse #2",
            "",
            "",
            "== STOREHOUSES ==",
            "",
            "STOREHOUSE #1",
            "",
            "STOREHOUSE #2",
            "",
    };

    perform_structure_report_check(factory, expected_report_lines);
}
TEST(ReportsTest, TurnReportPackageInProcessingBuffer) {
    // Utwórz fabrykę.
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1,std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    // Ustaw warunki początkowe symulacji.
    Time t = 1;
    r.deliver_goods(t);
    r.send_package();
    w.do_work(t);

    // -----------------------------------------------------------------------

    std::vector<std::string> expected_report_lines{
        "=== [ Turn: " + std::to_string(t) + " ] ===",
        "",
        "== WORKERS ==",
        "",
        "WORKER #1",
        "  PBuffer: #1 (pt = 1)",
        "  Queue: (empty)",
        "  SBuffer: (empty)",
        "",
        "",
        "== STOREHOUSES ==",
        "",
        "STOREHOUSE #1",
        "  Stock: (empty)",
        "",
};

    perform_turn_report_check(factory, t, expected_report_lines);
}
TEST(ReportsTest, TurnReportPackageInQueue) {

    Factory factory;

    factory.add_ramp(Ramp(1, 1));
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
    perform_turn_report_check(factory, t, expected_report_lines);
}
