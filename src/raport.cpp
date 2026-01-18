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
    	<< std::to_string(it->get_delivery_interval()) << "\n  Recievers:\n";
    	for (auto i = it->reciever_preferences_.cbegin(); i != it->reciever_preferences_.cend(); ++i) {
			if (i->first->get_reciever_type() == RecieverType::WORKER) {
    			workers.insert(i->first->get_id());
   			}
    		else if (i->first->get_reciever_type() == RecieverType::STOREHOUSE) {
    			storehouses.insert(i->first->get_id());
   			}
		}
    	for (auto i: storehouses) {
      		os << "    storehouse #" << std::to_string(i) << "\n";
    	}
    	for (auto i: workers) {
      		os << "    worker #" << std::to_string(i) << "\n";
    	}
    	os << "\n";
	}
    workers.clear();
    storehouses.clear();

    // PRACOWNICY
    os << "\n== WORKERS ==\n\n";
    std::string queue_type;
    for (auto i = f.worker_cbegin(); i != f.worker_cend(); i++) {
    	switch (i->get_queue()->get_queue_type()) {
        	case PackageQueueType::LIFO: {
            	queue_type = "LIFO";
                break;
    		}
            case PackageQueueType::FIFO: {
              queue_type = "FIFO";
              break;
            }
            default: break;
    	}
        os << "WORKER #" << std::to_string(i->get_id()) << "\n  Processing time: "
        << std::to_string(i->get_processing_duration()) << "\n  Queue Type: " << queue_type << "\n  Recievers:\n";
        for (auto it = i->reciever_preferences_.cbegin();
		it != i->reciever_preferences_.cend(); it++) {
       		if (it->first->get_reciever_type() == RecieverType::WORKER) {
            	workers.insert(it->first->get_id())
       		}
            else if (it->first->get_reciever_type() == RecieverType::STOREHOUSE) {
            	storehouses.insert(it->first->get_id())
            }
    	}
        for (auto it: storehouses) {
        	os << "    storehouse #" << std::to_string(it) << "\n";
        }
        for (auto it: workers) {
        	os << "    worker #" << std::to_string(it) << "\n";
        }
        os << "\n";
    }
    // MAGAZYNY
    os << "\n== STOREHOUSES ==\n\n";
    for (auto i = f.storehouse_cbegin(); i != f.storehouse_cend(); i++) {
    	os << "STOREHOUSE #" << std::to_string(i->get_id()) << "\n\n";
    }
}

