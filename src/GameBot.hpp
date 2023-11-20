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
#include "LineConfigHash.hpp"

    constexpr int MAX_MEMORY_MB = 70;
    constexpr std::chrono::seconds MAX_TIME_PER_MOVE(5);

namespace Gomoku {

    class GameBot {
    public:
        GameBot(bool isPrintGame, int valgrindEnable, std::string nameBot);

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

        std::unordered_map<LineConfig, int, LineConfigHash> scoreMap = {

                {{5, 2, 0}, 100000},
                {{4, 2, 0}, 50000},
                {{4, 1, 0}, 10000},
                {{3, 2, 0}, 5000},
                {{3, 1, 0}, 1000},
                {{4, 2, 1}, 20000},
                {{4, 1, 1}, 15000},
                {{3, 2, 1}, 7500},
                {{3, 2, 1}, 4000},
                {{3, 1, 1}, 3000},
                {{3, 2, 0}, 3000},
                {{3, 2, 1}, 3000},
                {{3, 1, 0}, 1500},
                {{2, 2, 1}, 1000},
                {{2, 2, 0}, 500},
                {{2, 1, 0}, 200},

        };

        static size_t getMemoryUsage();

        static bool isValidBoardSize(int size);

        bool areValidCoordinates(const std::string &xStr, const std::string &yStr);

        void enforceTimeLimit(const std::chrono::time_point<std::chrono::steady_clock> &startTime,
                                     const std::chrono::time_point<std::chrono::steady_clock> &endTime);

        Move calculateBestMove();

        void enforceMemoryLimit() const;

        void enforceMatchTimeLimit();

        std::unique_ptr<Board> board;

        std::chrono::steady_clock::time_point matchStartTime;

        int timeoutMatch = 0;

        int DEPTH = 2;

        int maxMemoryMB = MAX_MEMORY_MB;

        bool isPrintGame = false;

        bool endBot = false;

        bool valgrindEnabled = false;

        bool isBoardInit = false;

        const std::string botName = "GomokuBot";

        int evaluateLine(int x, int y, int dx, int dy, CellState type) ;

        int evaluateCell(int x, int y, CellState type) ;

        int evaluate();

        int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);

        void checkEnds(int x, int y, int dx, int dy, CellState type, int &openEnds, int &blockedEnds);

        bool isValidMove(const std::vector<int> &moveDetails);

        static std::vector<int> parseLineToMoveDetails(const std::string &line);

        int countConsecutiveStones(int x, int y, int dx, int dy, CellState type);

        bool checkDirection(int x, int y, int dx, int dy, CellState type) const;

        bool isGameOver() const;
    };
}
