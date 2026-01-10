#include "package.hxx"

Package::~Package() {
    assigned_ids.erase(id_);
    freed_ids.insert(id_);
}
