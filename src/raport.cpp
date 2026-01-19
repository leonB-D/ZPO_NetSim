//
// Created by Kacper on 13.01.2026.
//
#include "raport.hpp"


void generate_structure_report(const Factory& factory,std::ostream& os) {
  	std::set<ElementID> workers;
    std::set<ElementID> storehouses;

    // RAMPY
	os << "\n== LOADING RAMPS ==\n\n";
    for (auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
    	os << "LOADING RAMP #" << std::to_string(ramp->get_id()) << "\n  Delivery interval: "
    	<< std::to_string(ramp->get_delivery_interval()) << "\n  Receivers:\n";
    	for (auto reciever = ramp->receiver_preferences_.cbegin(); reciever != ramp->receiver_preferences_.cend(); ++reciever) {
			if (reciever->first->get_receiver_type() == ReceiverType::WORKER) {
    			workers.insert(reciever->first->get_id());
   			}
    		else if (reciever->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
    			storehouses.insert(reciever->first->get_id());
   			}
		}
    	for (auto storehouse: storehouses) {
      		os << "    storehouse #" << std::to_string(storehouse) << "\n";
    	}
    	for (auto worker: workers) {
      		os << "    worker #" << std::to_string(worker) << "\n";
    	}
    	os << "\n";
	}
    workers.clear();
    storehouses.clear();

    // PRACOWNICY
    os << "\n== WORKERS ==\n\n";
    std::string queue_type;
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
    	switch (worker->get_queue()->get_QueueType()) {
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
        os << "WORKER #" << std::to_string(worker->get_id()) << "\n  Processing time: "
        << std::to_string(worker->get_processing_duration()) << "\n  Queue type: " << queue_type << "\n  Receivers:\n";
        for (auto reciever_preference = worker->receiver_preferences_.cbegin();
		reciever_preference != worker->receiver_preferences_.cend(); reciever_preference++) {
       		if (reciever_preference->first->get_receiver_type() == ReceiverType::WORKER) {
            	workers.insert(reciever_preference->first->get_id());
       		}
            else if (reciever_preference->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            	storehouses.insert(reciever_preference->first->get_id());
            }
    	}
        for (auto storehouse: storehouses) {
        	os << "    storehouse #" << std::to_string(storehouse) << "\n";
        }
        for (auto worker: workers) {
        	os << "    worker #" << std::to_string(worker) << "\n";
        }
        os << "\n";
    }
    // MAGAZYNY
    os << "\n== STOREHOUSES ==\n\n";
    for (auto storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++) {
    	os << "STOREHOUSE #" << std::to_string(storehouse->get_id()) << "\n\n";
    }
}

void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time t) {
	std::set<ElementID> workers;
    std::set<ElementID> storehouses;
	os << "=== [ Turn: " << std::to_string(t) << " ] ===\n\n";

    os << "== WORKERS ==\n";
    if (factory.worker_cend() != factory.worker_cbegin()) {
    	os << "\n";
    }
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
    	workers.insert(worker->get_id());
    }
    for (auto worker_id: workers) {
    	auto worker = factory.find_worker_by_id(worker_id);
        os << "WORKER #" << std::to_string(worker_id) << "\n";
        if (worker->get_processing_buffer().has_value()) {
        	os << "  PBuffer: #" << worker->get_processing_buffer()->get_id() << " (pt = "
            << std::to_string(t - worker->get_package_processing_start_time() + 1) << ")\n";
        }
        else{
        	os << "  PBuffer: (empty)\n";
        }
        if (worker->get_queue()->empty()) {
        	os << "  Queue: (empty)\n";
        }
        else{
        	os <<"  Queue:";
            for (auto worker_queue = worker->get_queue()->cbegin(); worker_queue
           	!= worker->get_queue()->cend(); worker_queue++) {
            	if (worker_queue == worker->get_queue()->cbegin()) {
              	os << " #" << worker_queue->get_id();
            	}
            	else {
            		os <<", #" << worker_queue->get_id();
            	}
        	}
        	os << "\n";
        }
        if (worker->get_sending_buffer().has_value()) {
        	os << "  SBuffer: #" << worker->get_sending_buffer()->get_id() << "\n";
        }
        else{
        	os << "  SBuffer: (empty)\n";
        }
    }

    os << "\n\n== STOREHOUSES ==\n\n";
    for (auto storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++) {
    	storehouses.insert(storehouse->get_id());
    }
    for (auto storehouse_id: storehouses) {
    	auto storehouse = factory.find_storehouse_by_id(storehouse_id);
        os << "STOREHOUSE #" << std::to_string(storehouse_id) << "\n";

        if (storehouse->get_queue()->empty())
        {
	        os << "  Stock: (empty)\n\n";
        }
        else
        {
	        os << "  Stock:";
        	for (auto storehouse_queue = storehouse->get_queue()->cbegin();
			storehouse_queue != storehouse->get_queue()->cend(); storehouse_queue++) {
        		if (storehouse_queue == storehouse->get_queue()->cbegin()) {
        			os << " #" << storehouse_queue->get_id();
        		}
        		else {
        			os <<", #" << storehouse_queue->get_id();
        		}
			}
        	os <<"\n\n";
        }
    }
}
