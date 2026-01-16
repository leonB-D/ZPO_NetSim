//
// Created by Kacper on 13.01.2026.
//
#include "raport.hpp"


void generate_structure_report(const Factory& f,std::ostream& os) {
  	std::set<ElementID> workers;
    std::set<ElementID> storehouses;

    // RAMPY
	os << "\n== LOADING RAMPS ==\n\n";
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it) {
    	os << "LOADING RAMP #" << std::to_string(it->get_id()) << "\n  Delivery interval: "
    	<< std::to_string(it->get_delivery_interval()) << '\n  Recievers:\n';
    	for (auto i = it->reciever_preferences_.cbegin(); i != it->reciever_preferences_.cend(); ++i) {
			if (i->first->get_reciever_type() == RecieverType::WORKER) {
    			workers.insert(i->first->get_id());
   			}
    		else if (i->first->get_reciever_type() == RecieverType::STOREHOUSE) {
    			storehouses.insert(i->first->get_id());
   			}
		}
    	for (auto it: storehouses) {
      		os << "    storehouse #" << std::to_string(it) << '\n';
    	}
    	for (auto it: workers) {
      		os << "    worker #" << std::to_string(it) << '\n';
    	}
    	os << "\n";
	}
    workers.clear();
    storehouses.clear();
}

