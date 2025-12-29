#ifndef NODES_HXX
#define NODES_HXX

#include <optional>
#include <memory>
#include <map>
#include "helpers.hxx"
#include "package.hxx"
#include "storage_types.hxx"

using ElementID = int;
using Time = int;
using TimeOffset = int;


class IPackageReceiver {
public:
    void receive_package(Package&& p);
    ElementID get_id();
};

class ReceiverPreferences {
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
public:
    preferences_t preferences_;

    ReceiverPreferences(ProbabilityGenerator pg);
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences();
};

class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender(PackageSender&&) = default;
    void send_package();
    std::optional<Package>& get_sending_buffer();
protected:
    void push_package(Package&&);
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval();
    ElementID get_id();
};

class Worker : public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration();
    Time get_package_processing_start_time();
};

class Storehouse {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageQueue> d);
};

#endif