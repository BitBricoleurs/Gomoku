//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <mutex>
#include <cmath>

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
        explicit GameBot(bool isPrintGame);
        GameBot() = default;

        void handleStart(const std::vector<std::string>& args);
        void handleTurn(const std::vector<std::string>& args);
        void handleBegin();
        void handleBoard(const std::vector<std::string>& args);
        void handleEnd();
        void handleInfo(const std::vector<std::string>& args);
        void handleAbout();

        [[nodiscard]] bool isEndBot() const;
        static void respond(const std::string& response);
        std::map<std::string, std::string> infoMap;

    private:
        static size_t getMemoryUsage();
        static bool isValidBoardSize(int size);
        static bool areValidCoordinates(const std::string &xStr, const std::string &yStr);

        void enforceTimeLimit(const std::chrono::time_point<std::chrono::steady_clock> &startTime,
                                     const std::chrono::time_point<std::chrono::steady_clock> &endTime);
        Move calculateBestMove();
        static void enforceMemoryLimit();
        void enforceMatchTimeLimit();

        std::unique_ptr<Board> board;
        bool endBot = false;
        int DEPTH = 1;

        std::chrono::steady_clock::time_point matchStartTime;
        int timeoutMatch = 0;
        int maxMemoryMB = MAX_MEMORY_MB;
        bool isPrintGame = false;
    };
}
