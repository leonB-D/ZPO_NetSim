#include <gtest/gtest.h>
#include <nodes.hxx>

TEST(WorkerTest, HasBuffer) {
    // Test scenariusza opisanego na stronie:
    // http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_nodes#bufor_aktualnie_przetwarzanego_polproduktu

    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

TEST(WorkerTest, WorksCorrectly) {
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Storehouse s(2, std::make_unique<PackageQueue>(PackageQueueType::LIFO));
    w.receiver_preferences_.add_receiver(&s);
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    ++t;
    w.do_work(t);

    EXPECT_EQ(s.cbegin()->get_id(), 1);
    EXPECT_EQ(w.get_sending_buffer().value().get_id(), 2);
}

TEST(ReceiverPreferencesTest, CorrectlyScaling) {
    Worker w1(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Worker w2(2, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Worker w3(3, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));

    ReceiverPreferences rp = ReceiverPreferences();

    rp.add_receiver(&w1);
    rp.add_receiver(&w2);
    rp.add_receiver(&w3);

    double p_sum = 0;

    for (auto item : rp.get_preferences()) {
        p_sum += item.second;
    }

    EXPECT_NEAR(p_sum, 1, 0.01);

    rp.remove_receiver(&w1);

    p_sum = 0;

    for (auto item : rp.get_preferences()) {
        p_sum += item.second;
    }

    EXPECT_NEAR(p_sum, 1, 0.01);
}