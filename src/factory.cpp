#include "factory.hxx"
#include "nodes.hxx"



bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if (node_colors[sender] == NodeColor::VERIFIED)
    {
        return true;
    }

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty())
    {
        throw std::logic_error();
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
    throw std::logic_error();
}

void Factory::remove_worker(ElementID id)
{
    Worker *node = &(*cont_w.find_by_id(id));

    std::for_each(cont_w.begin(), cont_w.end(), [node](Worker& worker)
    {
        worker.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(cont_r.begin(), cont_r.end(), [node](Ramp& ramp)
    {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    cont_w.remove_by_id(id);
}

