#ifndef NODES_HXX
#define NODES_HXX

#include <optional>
#include <memory>
#include <map>
#include "helpers.hxx"
#include "storage_types.hxx"

using ElementID = int;
using Time = int;
using TimeOffset = int;
enum class RecieverType {
    WORKER, STOREHOUSE
};


class IPackageReceiver {
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
public:
    preferences_t preferences_;
    ProbabilityGenerator pg_;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator)
        : pg_(std::move(pg)) {};

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences() {return preferences_;}
    const_iterator begin() const {return preferences_.begin();}
    const_iterator end() const {return preferences_.end();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator cend() const {return preferences_.cend();}
};

class PackageSender {
public:
    PackageSender() = default;
    PackageSender(PackageSender&& other) = default;

    void send_package();
    std::optional<Package>& get_sending_buffer() {return buffer;}

    ReceiverPreferences receiver_preferences_;
protected:
    std::optional<Package> buffer = std::nullopt;
    void push_package(Package&& p);
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di)
        : PackageSender(), id_(id), time_offset_(di) {};

    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() {return time_offset_;}
    ElementID get_id() {return id_;}

private:
    ElementID id_;
    TimeOffset time_offset_;
    Time start_time_ = 0;
};

class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
        : PackageSender(), id_(id), time_offset_(pd), queue_(std::move(q)) {};

    void do_work(Time t);
    TimeOffset get_processing_duration() {return time_offset_;}
    Time get_package_processing_start_time() {return start_time_;}

    void receive_package(Package&& p) override {queue_->push(std::move(p));}
    ElementID get_id() const override {return id_;}
    IPackageStockpile::const_iterator begin() const override {return queue_->begin();}
    IPackageStockpile::const_iterator end() const override {return queue_->end();}
    IPackageStockpile::const_iterator cbegin() const override {return queue_->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return queue_->cend();}

private:
    ElementID id_;
    TimeOffset time_offset_;
    Time start_time_ = 0;
    std::unique_ptr<IPackageQueue> queue_;
};

class Storehouse: public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageQueue> d)
        : id_(id), queue_(std::move(d)) {};

    void receive_package(Package&& p) override {queue_->push(std::move(p));}
    ElementID get_id() const override {return id_;}
    IPackageStockpile::const_iterator begin() const override {return queue_->begin();}
    IPackageStockpile::const_iterator end() const override {return queue_->end();}
    IPackageStockpile::const_iterator cbegin() const override {return queue_->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return queue_->cend();}

private:
    ElementID id_;
    std::unique_ptr<IPackageQueue> queue_;
};

#endif