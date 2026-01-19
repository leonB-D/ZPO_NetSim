#include "package.hxx"
#include <set>

std::set<ElementID> Package::assigned_ids = {};
std::set<ElementID> Package::freed_ids = {};

Package::Package() {
  if(assigned_ids.empty() && freed_ids.empty()) { id_ = 1; }
  else if(not freed_ids.empty()) {
    id_ = *freed_ids.begin();
    freed_ids.erase(freed_ids.begin());
  }
  else{
      id_ = *assigned_ids.rbegin() + 1;
  }
  assigned_ids.insert(id_);
}
Package::~Package() {
    if (id_ != 0) {
        assigned_ids.erase(id_);
        freed_ids.insert(id_);
    }
}

Package& Package::operator=(Package&& package) noexcept {
    if (id_ != 0) {
        assigned_ids.erase(this->id_);
        freed_ids.insert(this->id_);
    }
    this->id_ = package.id_;
    package.id_ = 0;

    return *this;
}