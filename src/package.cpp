#include "package.hxx"
#include <set>

std::set<ElementID> assigned_ids = {};
std::set<ElementID> freed_ids = {};

Package::Package() {
  if(assigned_ids.empty() && freed_ids.empty()) { id_ = 1 }
  else if(not freed_ids.empty()) {
    id_ = *freed_ids.end();
    freed_ids.erase(*freed_ids.end());
  }
  else{
      id_ = *assigned_ids.end() + 1;
  }
  assigned_ids.insert(id_);
}
Package::~Package() {
    assigned_ids.erase(id_);
    freed_ids.insert(id_);
}

Package::Package& operator=(Package$$ package) noexcept {


}