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
    if (preferences_.empty()) {
        preferences_.insert({r, 1});
        return;
    }

    const double p = pg_();
    const double p_sum = 1 + p;

    for (auto item : preferences_) {
        preferences_[item.first] /= p_sum;
    }

    preferences_.insert({r, p / p_sum});
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    if (preferences_.empty()) return;

    const double p_sum = 1 - preferences_[r];
    preferences_.erase(r);

    for (auto item : preferences_) {
        preferences_[item.first] *= p_sum;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double p = pg_();
    double sum = 0;

    for (auto item : preferences_) {
        sum += item.second;
        if (sum > p) return item.first;
    }

    return nullptr;
}

ReceiverPreferences::preferences_t& ReceiverPreferences::get_preferences() {
    return preferences_;
}

ElementID Worker::get_id() const {
    return id_;
}

ElementID Storehouse::get_id() const {
    return id_;
}