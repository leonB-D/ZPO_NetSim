

#include "simulation.hxx"
#include <stdexcept>

void simulate(Factory& f, TimeOffset duration, std::function<void(Factory&, Time)> report_function){

    if (!f.is_consistent()) {
        //throw std::logic_error("Network is inconsistent");
    }


    for (Time t = 1; t <= duration; ++t) {


        f.do_deliveries(t);
        f.do_package_passing();
        f.do_work(t);


        report_function(f, t);
    }
}


bool IntervalReportNotifier::should_generate_report(Time t) const {

    if (to_ == 0) return false;
    return (t - 1) % to_ == 0;
}


bool SpecificTurnsReportNotifier::should_generate_report(Time t) const
{

    return turns_.find(t) != turns_.end();
}