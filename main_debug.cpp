//test
#include "input_output.hxx"
#include "raport.hpp"
#include <fstream>


int main()
{
    std::ifstream input_file("struct-input.txt");

    Factory factory = load_factory_structure(input_file);

    for (Time t = 1; t <= 10; t++)
    {
        factory.do_work(t);
        factory.do_deliveries(t);
        factory.do_package_passing();
        generate_simulation_turn_report(factory, std::cout, t);
    }

    input_file.close();

    return 0;
}