#ifndef NODES_HXX
#define NODES_HXX

#include <optional>
#include <memory>
#include <map>
#include <utility>
#include "helpers.hxx"
#include "storage_types.hxx"

class PackageSender;

enum class ReceiverType {
    WORKER, STOREHOUSE
};

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

class IPackageReceiver {
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;
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
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator)
        : pg_(std::move(pg)) {};

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const {return preferences_;}
    const_iterator begin() const {return preferences_.begin();}
    const_iterator end() const {return preferences_.end();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator cend() const {return preferences_.cend();}
private:
    preferences_t preferences_;
    ProbabilityGenerator pg_;
};

class PackageSender {
public:
    PackageSender() = default;
    PackageSender(PackageSender&& other) = default;
    virtual ~PackageSender() = default;

    void send_package();
    void push_package(Package&& p);
    std::optional<Package>& get_sending_buffer() {return buffer;}

    ReceiverPreferences receiver_preferences_;
protected:
    std::optional<Package> buffer = std::nullopt;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di)
        : PackageSender(), id_(id), time_offset_(di) {};

    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() {return time_offset_;}
    ElementID get_id() const {return id_;}

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
    std::optional<Package>& get_processing_buffer() {return p_buffer;}
    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }
    IPackageQueue* get_queue() const { return queue_.get(); }

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
    std::optional<Package> p_buffer = std::nullopt;
};

class Storehouse: public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageQueue> d)
        : id_(id), queue_(std::move(d)) {};

    void receive_package(Package&& p) override {queue_->push(std::move(p));}
    ElementID get_id() const override {return id_;}
    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }

    IPackageStockpile::const_iterator begin() const override {return queue_->begin();}
    IPackageStockpile::const_iterator end() const override {return queue_->end();}
    IPackageStockpile::const_iterator cbegin() const override {return queue_->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return queue_->cend();}

private:
    ElementID id_;
    std::unique_ptr<IPackageQueue> queue_;
};

#endif