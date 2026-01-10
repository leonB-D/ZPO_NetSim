#include "types.hxx"
#include <set>
class Package {
public:

    Package() = default;
    explicit Package(ElementID id) : id_(id) { assigned_ids.insert(id); }
    Package(Package&& package) : id_(package.id_) {}
    Package& operator=(Package&& package) noexcept;
    ElementID get_id() const { return id_; }
    ~Package() = default;

private:
    ElementID id_;
    static std::set<ElementID> assigned_ids;
    static std::set<ElementID> freed_ids;
};
