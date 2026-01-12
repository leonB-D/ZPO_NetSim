
#include "Package.hxx"
#include <list>
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
class IPackageQueue {

};
class PackageQueue {

};