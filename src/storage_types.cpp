#include "storage_types.hxx"

Package PackageQueue::pop()
{
    Package pack;
    if (this->package_QueueType_ == PackageQueueType::LIFO)
    {
        pack = std::move(this->package_list_.back());
        this->package_list_.pop_back();
    }
    else
    {
        pack = std::move(this->package_list_.front());
        this->package_list_.pop_front();
    }
    return pack;
}