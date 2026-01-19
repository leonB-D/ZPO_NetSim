//test
#include "simulation.hxx"
#include <fstream>


int main()
{
    std::ifstream input_file("struct-input.txt");

    Factory factory = load_factory_structure(input_file);

    input_file.close();

    // Testowanie z użyciem "wydmuszki" funkcji raportującej.
    simulate(factory, 3, [](Factory&, TimeOffset) {});

    // Testowanie z użyciem konkretnego obiektu klasy raportującej.
    SpecificTurnsReportNotifier spec_notifier(std::set<Time>{1});
    simulate(factory, 3, [&spec_notifier](Factory& f, TimeOffset t_offset) {
        if (spec_notifier.should_generate_report(t_offset)) {
            generate_structure_report(f, std::cout);
        }
    });

    return 0;
}