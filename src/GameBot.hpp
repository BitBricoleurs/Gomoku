//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>

#ifdef _WIN32
    #include <windows.h>
#include <psapi.h>
#elif __APPLE__
#include <mach/mach.h>
#elif __linux__
#include <sys/resource.h>
#endif

#include "Board.hpp"

    constexpr int MAX_MEMORY_MB = 70;
    constexpr std::chrono::seconds MAX_TIME_PER_MOVE(5);

namespace Gomoku {

    class GameBot {
    public:
        GameBot() = default;

        void handleStart(const std::vector<std::string>& args);
        void handleTurn(const std::vector<std::string>& args);
        void handleBegin();
        void handleBoard(const std::vector<std::string>& args);
        void handleEnd();
        void handleInfo(const std::vector<std::string>& args);
        void handleAbout();

    private:
        static size_t getMemoryUsage();
        static bool isValidBoardSize(int size);
        static bool areValidCoordinates(const std::string &xStr, const std::string &yStr);
        void calculateNextMove();
        static void respond(const std::string& response);


        std::map<std::string, std::string> infoMap;
        std::unique_ptr<Board> board;
    };
}
