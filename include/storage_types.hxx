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
};

class IPackageQueue {

};
