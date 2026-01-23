#ifndef STORAGE_TYPES_HXX
#define STORAGE_TYPES_HXX

#include <list>
#include "types.hxx"
#include "package.hxx"

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual ~IPackageStockpile() = default;
    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
};

class IPackageQueue: public IPackageStockpile {
public:
    ~IPackageQueue() override = default;
    virtual Package pop() = 0;
    virtual PackageQueueType get_QueueType() const = 0;
};

class PackageQueue: public IPackageQueue {
public:
    PackageQueue() = delete;
    ~PackageQueue() override = default;
    explicit PackageQueue(PackageQueueType queueType): package_QueueType_(queueType), package_list_() {};

    void push(Package&& package) override {
    this->package_list_.emplace_back(std::move(package));
    }
    bool empty() const override {
    return this->package_list_.empty();
    }
    size_t size() const override{
        return this->package_list_.size();
    }
    const_iterator begin() const override
    {
        return this->package_list_.cbegin();
    }
    const_iterator end() const override
    {
        return this->package_list_.cend();
    }
    const_iterator cbegin() const override
    {
        return this->package_list_.cbegin();
    }
    const_iterator cend() const override
    {
        return this->package_list_.cend();
    }
    Package pop() override;
    PackageQueueType get_QueueType() const override
    {
        return this->package_QueueType_;
    };
private:
    PackageQueueType package_QueueType_;
    std::list<Package> package_list_;
};

#endif