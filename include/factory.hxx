#ifndef ZPO_NETSIM_FACTORY_HXX
#define ZPO_NETSIM_FACTORY_HXX



#include <list>
#include <algorithm>
#include <utility>
#include "storage_types.hxx"
#include "nodes.hxx"

class Factory
{
    public:
        void add_ramp(Ramp&& ramp) {
            ramps.add(std::move(ramp));
        }
        void remove_ramp(ElementID id) {
            ramps.remove_by_id(id);
        }
        NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {
            return ramps.find_by_id(id);
        }

        NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {
            return ramps.find_by_id(id);
        }
        NodeCollection<Ramp>::iterator ramp_begin() { return ramps.begin(); }
        NodeCollection<Ramp>::iterator ramp_end() { return ramps.end(); }
        NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps.cbegin(); }
        NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps.cend();}

        void add_worker(Worker&& worker) {
            workers.add(std::move(worker));
        }

        void remove_worker(ElementID id) {
            workers.remove_by_id(id);
        }
        NodeCollection<Worker>::iterator find_worker_by_id(ElementID id)
        {
            return workers.find_by_id(id);
        }

        NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const
        {

            return workers.find_by_id(id);
        }

        NodeCollection<Worker>::iterator worker_begin() { return workers.begin(); }
        NodeCollection<Worker>::iterator worker_end() { return workers.end(); }
        NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers.cbegin(); }
        NodeCollection<Worker>::const_iterator worker_cend() const { return workers.cend(); }

        void add_storehouse(Storehouse&& storehouse)
        {
            storehouses.add(std::move(storehouse));
        }

        void remove_storehouse(ElementID id)
        {
            storehouses.remove_by_id(id);
        }
        NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {
            return storehouses.find_by_id(id);
        }

        NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const
        {
            return storehouses.find_by_id(id);
        }


        NodeCollection<Storehouse>::iterator storehouse_begin() { return storehouses.begin(); }
        NodeCollection<Storehouse>::iterator storehouse_end() { return storehouses.end(); }
        NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses.cbegin(); }
        NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses.cend(); }

        bool is_consistent();
        void do_deliveries(Time t);
        void do_work(Time t);
        void do_package_passing();
    protected:
    NodeCollection<Ramp> ramps;
    NodeCollection<Worker> workers;
    NodeCollection<Storehouse> storehouses;


    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
};

#endif //ZPO_NETSIM_FACTORY_HXX
