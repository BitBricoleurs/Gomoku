//
// Created by alexandre on 07/11/23.
//

#include "GameBot.hpp"

void Gomoku::GameBot::calculateNextMove()
{
    auto startTime = std::chrono::steady_clock::now();

    std::cout << "PLACE_MOVE 1 1" << std::endl;
    auto endTime = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime) > MAX_TIME_PER_MOVE) {
        std::cerr << "Time limit exceeded" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (getMemoryUsage() > MAX_MEMORY_MB * 1024 * 1024) {
        std::cerr << "Memory limit exceeded" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

size_t Gomoku::GameBot::getMemoryUsage()
{
    #ifdef _WIN32
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            return pmc.WorkingSetSize;
    #elif __APPLE__
            mach_task_basic_info info;
            mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
            if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS)
                return 0;
            return info.resident_size;
    #elif __linux__
            struct rusage usage;
            if (getrusage(RUSAGE_SELF, &usage) == -1)
                return 0;
            return usage.ru_maxrss * 1024;
    #endif
}

void Gomoku::GameBot::respond(const std::string& response) {
    std::cout << response << std::endl;
    std::fflush(stdout); // Ensure immediate output
}
