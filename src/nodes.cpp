#include "nodes.hxx"

Ramp::Ramp(ElementID id, TimeOffset di)
    : PackageSender(this&), id_(id), time_offset_(di) {}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
    : PackageSender(this&), id_(id), time_offset_(pd), queue_(std::move(q)) {}

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageQueue> d)
    : id_(id), queue_(std::move(d)) {}

void PackageSender::push_package(Package&& p) {
    this->buffer = p;
}

void PackageSender::send_package() {
    this->buffer = std::nullopt;
}
