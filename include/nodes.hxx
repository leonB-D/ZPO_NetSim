#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP


#include "types.hxx"
#include "package.hxx"
#include "helpers.hxx"
#include "storage_types.hxx"
#include <optional>
#include <memory>
#include <map>
#include <utility>




enum class ReceiverType {
    WORKER, STOREHOUSE
};

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);
bool Factory::is_consistent();

class IPackageReceiver
{
public:

    virtual ~IPackageReceiver() = default;
    virtual void receive_package(Package&& p) = 0;

    virtual ElementID get_id() const = 0;

    virtual ReceiverType get_receiver_type() const = 0;


    using const_iterator = IPackageStockpile::const_iterator;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
};



class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : generate_probability_(
            std::move(pg)) {};

    void add_receiver(IPackageReceiver* receiver);

    void remove_receiver(IPackageReceiver* receiver);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const {
        return this->preferences_;
    };

    const_iterator cbegin() const noexcept { return preferences_.cbegin(); };

    const_iterator cend() const noexcept { return preferences_.cend(); };

    const_iterator begin() const noexcept { return preferences_.begin(); };

    const_iterator end() const noexcept { return preferences_.end(); };

private:
    preferences_t preferences_;

    ProbabilityGenerator generate_probability_;
};



class PackageSender
{
public:
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    virtual ~PackageSender() = default;

    void send_package();

    virtual ElementID get_id() const = 0;

    ReceiverPreferences receiver_preferences_;

protected:
    std::optional<Package> push_buffer_;
    void push_package(Package&& package);
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), delivery_interval_(di) {}

    void deliver_goods(Time t);
    ElementID get_id() const override { return id_; }
    TimeOffset get_delivery_interval() const { return delivery_interval_; }

private:
    ElementID id_;
    TimeOffset delivery_interval_;
    Time last_delivery_time_ = 0;
}



class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
        : id_(id), processing_duration_(pd), queue_(std::move(q)) {}

    void receive_package(Package&& p) override;
    ElementID get_id() const override { return id_; }

    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }

    void do_work(Time t);

    TimeOffset get_processing_duration() const { return processing_duration_; }
    Time get_package_processing_start_time() const { return package_processing_start_time_; }

    const_iterator begin() const override { return queue_->begin(); }
    const_iterator end() const override { return queue_->end(); }
    const_iterator cbegin() const override { return queue_->cbegin(); }
    const_iterator cend() const override { return queue_->cend(); }

private:
    ElementID id_;
    TimeOffset processing_duration_;
    std::unique_ptr<IPackageQueue> queue_;

    std::optional<Package> processing_buffer_;
    Time package_processing_start_time_ = 0;
};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = nullptr)
        : id_(id), d_(std::move(d)) {}

    void receive_package(Package&& p) override;
    ElementID get_id() const override { return id_; }

    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }

    const_iterator begin() const override { return d_->begin(); }
    const_iterator end() const override { return d_->end(); }
    const_iterator cbegin() const override { return d_->cbegin(); }
    const_iterator cend() const override { return d_->cend(); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

#endif //NETSIM_NODES_HPP