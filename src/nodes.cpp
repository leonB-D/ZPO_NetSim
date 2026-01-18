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
        this->preferences_.insert({r, 1});
        return;
    }

    const double p = pg_();
    const double p_sum = 1 + p;

    for (auto item : this->preferences_) {
        this->preferences_[item.first] /= p_sum;
    }

    this->preferences_.insert({r, p / p_sum});
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    if (this->preferences_.size() == 0) return;

    const double p_sum = 1 - this->preferences_[r];
    this->preferences_.erase(r);

    for (auto item : this->preferences_) {
        this->preferences_[item.first] *= p_sum;
    }
}