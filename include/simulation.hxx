

#ifndef SIMULATION_HXX
#define SIMULATION_HXX
#include "factory.hxx"
#include "storage_types.hxx"
#include <functional>
#include <set>


void simulate(Factory& f, TimeOffset duration, std::function<void(Factory&, Time)> report_function);


class IntervalReportNotifier {
public:
    IntervalReportNotifier(TimeOffset to) : to_(to) {}


    bool should_generate_report(Time t);

private:
    TimeOffset to_;
};


class SpecificTurnsReportNotifier {
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(turns) {}


    bool should_generate_report(Time t);

private:
    std::set<Time> turns_;
};
#endif //SIMULATION_HXX
