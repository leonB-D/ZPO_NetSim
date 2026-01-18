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

void generate_simulation_turn_report(const Factory& f, std::ostream os, Time t) {
	std::set<ElementID> workers;
    std::set<ElementID> storehouses;
	os << "=== [ Turn: " << ost::to_string(t) << " ]===\n\n";

    os << "== WORKERS ==\n";
    if (f.worker_cend() != f.worker_cbegin()) {
    	os << "\n";
    }
    for (auto i = f.worker_cebgin(); i != f.worker_cend(); i++) {
    	workers.insert(i->get_id());
    }
    for (auto i: workers) {
    	auto it = f.find_worker_by_id(i);
        os << "WORKER #" << std::to_string(i) << "\n";
        if (it->get_processing_buffer().has_value()) {
        	os << "  PBuffer: #" << it->get_processing_buffer()->get_id() << " (pt = "
            << std::to_string(t - it->get_package_processing_start_time() + 1) << ")\n";
        }
        else{
        	os << "  PBuffer: (empty)\n";
        }
        if (it->get_queue()->empty()) {
        	os << "  Queue: (empty)\n";
        }
        else{
        	os <<"  Queue:";
            for (auto iter = it->get_queue().cbegin(); i
           	!= it->get_queue().cend(); i++) {
            	if (iter == it->get_queue().cbegin()) {
              	os << " #" << iter->get_id();
            	}
            	else {
            		os <<", #" << iter->get_id();
            	}
        	}
        	os << "\n";
        }
        if (it->get_sendinge_buffer().has_value()) {
        	os << "  SBuffer: #" << it->get_sending_buffer()->get_id() << "\n";
        }
        else{
        	os << "  SBuffer: (empty)\n";
        }
    }

    os << "\n\n== STOREHOUSES ==\n\n";
    for (auto i = f.storehouse_cbegin(); i != f.storehouse_cend(); i++) {
    	storehouses.insert(i->get_id());
    }
    for (auto i: storehouses) {
    	auto it = f.find_storehouse_by_id(i);
        os << "STOREHOUSE #" << std::to_string(i) << "\n";

        if (it->get_queue().has_value()) {
        	os << "  Stock:";
            for (auto iter = it->get_queue().cbegin();
            i != it->get_queue().cend(); i++) {
            	if (iter == it->get_queue().cbegin()) {
                	os << " #" << iter->get_id();
            	}
                else {
                	os <<", #" << iter->get_id();
                }
            }
            os <<"\n\n";
        }
        else{
        	os << "  Stock: (empty)\n\n";
        }
    }
}
