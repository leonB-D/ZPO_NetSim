#include "types.hxx"
#include "Package.hxx"

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {

};
class IPackageQueue {
public:
    Package pop();
    PackageQueueType get_queue_type()

    }

};
class PackageQueue {

};