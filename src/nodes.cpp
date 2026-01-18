#include <utility>
#include <iostream>
#include "nodes.hxx"

void PackageSender::push_package(Package&& p) {
    this->buffer = std::move(p);
}

void PackageSender::send_package() {
    if (this->buffer.has_value()) {
        receiver_preferences_.choose_receiver()->receive_package(std::move(*this->buffer));
        this->buffer = std::nullopt;
    }
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
        preferences_[item.first] /= p_sum;
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

void Ramp::deliver_goods(Time t) {
    if (!buffer.has_value()) start_time_ = t;

    if (buffer.has_value() && t >= start_time_ + time_offset_) {
        this->send_package();
    }
}

void Worker::do_work(Time t) {
    if (!buffer.has_value()) {
        start_time_ = t;
        if (queue_) push_package(queue_->pop());
    }

    if (buffer.has_value() && t >= start_time_ + time_offset_) {
        this->send_package();
        if (queue_) push_package(queue_->pop());
    }
}
