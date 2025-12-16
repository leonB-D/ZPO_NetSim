#include "types.hxx"

class Package {
public:
    Package(ElementID);
    Package(Package&&);
    ElementID get_id();
};
