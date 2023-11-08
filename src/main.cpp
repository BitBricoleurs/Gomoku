#include <iostream>
#include "Core.hpp"
#include "Error.hpp"

int main() {
    try {

        Gomoku::Core core;

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
