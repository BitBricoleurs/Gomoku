#include <iostream>
#include <string>
#include "Core.hpp"
#include "Error.hpp"


int main(int ac, char **av) {
    try {
        bool printMode = false;

        for (int i = 1; i < ac; ++i) {
            std::string arg = av[i];
            if (arg == "-h") {
                std::cout << "Usage: " << av[0] << " [-h] [-p]\n"
                          << "Options:\n"
                          << "  -h   Afficher l'aide.\n"
                          << "  -p   Activer le mode d'impression.\n";
                return 0;
            } else if (arg == "-p") {
                printMode = true;
            }
        }

        Gomoku::Core core(printMode);
        core.run();
        return 0;
    } catch (const Error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return 1;
    }
}
