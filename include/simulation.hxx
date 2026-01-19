
#ifndef SIMULATION_HXX
#define SIMULATION_HXX


#include "storage_types.hxx"
#include <functional>
#include <set>
#include "types.hxx"

class Factory;

void simulate(Factory& f, TimeOffset duration, std::function<void(Factory&, Time)> report_function);

class IntervalReportNotifier {
public:
    IntervalReportNotifier(TimeOffset to) : to_(to) {}


    bool should_generate_report(Time t) const;

private:
    TimeOffset to_;
};


class SpecificTurnsReportNotifier {
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {}


    bool should_generate_report(Time t) const;

private:
    std::set<Time> turns_;
};
#endif //SIMULATION_HXX
