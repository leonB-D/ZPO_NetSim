#ifndef NETSIM_TYPES_HXX
#define NETSIM_TYPES_HXX

#include <functional>

using ElementID = unsigned;
using TimeOffset = int;
using Time = int;

using TimeOffset = Time;

using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HXX