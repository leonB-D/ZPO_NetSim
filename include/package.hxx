#include "types.hxx"

class Package {
public:

    Package();
    explicit Package(ElementID id) : id_(id) {}
    Package(Package&& other) noexcept : id_(other.id_) {
        other.id_ = 0;
    }
    Package& operator=(Package&& other) noexcept;


    Package(const Package&) = delete;
    Package& operator=(const Package&) = delete;

    ElementID get_id() const { return id_; }

    ~Package() = default;

private:
    ElementID id_;

};
