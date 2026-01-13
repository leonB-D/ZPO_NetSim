#include <utility>
#include <iostream>
#include "nodes.hxx"

void PackageSender::push_package(Package&& p) {
    //this->buffer = p;
}

void PackageSender::send_package() {
    this->buffer = std::nullopt;
    //przesyłanie dalej
}

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    if (this->preferences_.size() == 0) {
        std::cout<<1<<std::endl;
        this->preferences_.insert({r, 1});
        return;
    }

    double p = pg_();
    double sum = 1 + p;

    for (auto item : this->preferences_) {
        item.second /= sum;
        std::cout<<item.second<<std::endl;
    }

    std::cout<<p/sum<<std::endl;
    this->preferences_.insert({r, p / sum});
}
