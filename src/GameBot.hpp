//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
#include <psapi.h>
#elif __APPLE__
#include <mach/mach.h>
#elif __linux__
#include <sys/resource.h>
#endif

    constexpr int MAX_MEMORY_MB = 70;
    constexpr std::chrono::seconds MAX_TIME_PER_MOVE(5);

namespace Gomoku {

    class GameBot {
    public:
        GameBot() = default;

        void calculateNextMove();
        void respond(const std::string& response);

    private:
        size_t getMemoryUsage();

    };
}
