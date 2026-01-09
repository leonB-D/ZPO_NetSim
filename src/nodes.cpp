#include <utility>

#include "nodes.hxx"

void PackageSender::push_package(Package&& p) {
    this->buffer = p;
}

void PackageSender::send_package() {
    this->buffer = std::nullopt;
    //przesyłanie dalej
}
