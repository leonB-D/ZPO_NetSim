//
// Created by Kacper on 13.01.2026.
//
#ifndef RAPORT_HPP
#define RAPORT_HPP

#include "factory.hxx"
#include <iostream>


void generate_structure_report(const Factory& f,std::ostream& os);
void generate_simulation_turn_report(const Factory& f,std::ostream& os,Time t);

#endif //RAPORT_HPP

