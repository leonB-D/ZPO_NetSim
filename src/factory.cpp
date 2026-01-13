#include "factory.hxx"
#include "nodes.hxx"
#include <stdexcept>


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
            return true;
        } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
        {
            PackageSender *sendptr = dynamic_cast(dynamic_cast(receiver.first));
            if (sendptr == sender) {
                continue;
            }

            if (node_colors[sendptr] == NodeColor::UNVISITED && has_reachable_storehouse(sendptr, node_colors))
            {
                return true;
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;
    throw std::logic_error("Error");
}
bool Factory::is_consistent()
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
    for (auto& ramp : ramps)
    {
        ramp.deliver_goods(t);
    }
}


void Factory::do_package_passing() {
    for (auto& ramp : ramps) {
        ramp.send_package();
    }
    for (auto& worker : workers) {
        worker.send_package();
    }
}


void Factory::do_work(Time t)
{
    for (auto& worker : workers)
    {
        worker.do_work(t);
    }
}
void Factory::remove_worker(ElementID id)
{
    remove_receiver(workers, id);
}

void Factory::remove_storehouse(ElementID id)
{
    remove_receiver(storehouses, id);
}

