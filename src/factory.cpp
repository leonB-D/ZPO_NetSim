#include "factory.hxx"
#include "nodes.hxx"
#include <stdexcept>
#include <sstream>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if (node_colors[sender] == NodeColor::VERIFIED)
    {
        return true;
    }

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty())
    {
        throw std::logic_error("Error");
    }

    for (const auto& receiver : sender->receiver_preferences_.get_preferences())
    {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
        {
            node_colors[sender] = NodeColor::VERIFIED;
            return true;
        }
        if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
        {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender)
            {
                continue;
            }
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED)
            {
                if (has_reachable_storehouse(sendrecv_ptr, node_colors))
                {
                    node_colors[sender] = NodeColor::VERIFIED;
                    return true;
                }
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;
    throw std::logic_error("Error");
}
bool Factory::is_consistent() const
{
    std::map<const PackageSender*, NodeColor> node_colors;
    for (const auto& ramp : ramps) {
        node_colors[&ramp] = NodeColor::UNVISITED;
    }
    for (const auto& worker : workers) {
        node_colors[&worker] = NodeColor::UNVISITED;
    }
    for (const auto& ramp : ramps) {
        try {
            has_reachable_storehouse(&ramp, node_colors);
        } catch (const std::logic_error& e) {

            return false;
        }
    }

    return true;
}
void Factory::do_deliveries(Time t) {
    for (auto &ramp : ramps)
        ramp.deliver_goods(t);
}

void Factory::do_package_passing() {
    for (auto &ramp : ramps)
        ramp.send_package();

    for (auto &worker : workers)
        worker.send_package();
}

void Factory::do_work(Time t)
{
    for (auto& worker : workers)
    {
        worker.do_work(t);
    }
}
std::vector<std::string> character_split(const std::string& splittable_str, char delimiter) {
    std::stringstream parameter_stream(splittable_str);
    std::string part;
    std::vector<std::string> result;

    while(std::getline(parameter_stream, part, delimiter)) {
        result.push_back(part);
    }

    return result;
}



