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
#include <future>
#include <vector>
#include <mutex>
#include <atomic>

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#include <psapi.h>
#elif __APPLE__
#include <mach/mach.h>
#elif __linux__
#include <sys/resource.h>
#endif

#include "Board.hpp"
#include "LineConfigHash.hpp"

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


    private:

        std::atomic<bool> timeIsUp;

        std::atomic<bool> searchComplete;

        void timeWatcher(int timeLimitMs);

        std::map<std::string, std::string> infoMap;

        int MAX_MEMORY_MB = 70;

        std::chrono::seconds MAX_TIME_PER_MOVE = std::chrono::seconds(5);

        std::unordered_map<LineConfig, int, LineConfigHash> scoreMap = {

                // Critical

                {{3, 2, 0}, 6000}, // block 3 in a row 2 open End
                {{4, 2, 0}, 10000},
                {{4, 1, 1}, 8000},
                {{4, 0, 2}, 5000},
                {{4, 0, 1}, 500},

                // Attack
                {{3, 1, 1}, 2000},
                {{2, 1, 1}, 20},
                {{2, 2, 0}, 20},
                //{{1, 1, 1}, 10},
                {{1, 2, 0}, 10}

        };

        static size_t getMemoryUsage();

        static bool isValidBoardSize(int size);

        bool areValidCoordinates(const std::string &xStr, const std::string &yStr);


        Move calculateBestMove();

        void enforceMemoryLimit() const;

        std::unique_ptr<Board> board;

        std::chrono::steady_clock::time_point matchStartTime;

        int timeoutMatch = 0;

        int DEPTH = 3;

        int maxMemoryMB = MAX_MEMORY_MB;

        bool isPrintGame = false;

        bool endBot = false;

        bool valgrindEnabled = false;

        bool isBoardInit = false;

        const std::string botName = "GomokuBot";

        int evaluateLine(int x, int y, int dx, int dy, CellState type) ;

        int evaluateCell(int x, int y, CellState type) ;

        int evaluate();

        void checkEnds(int x, int y, int dx, int dy, CellState type, int &openEnds, int &blockedEnds);

        bool isValidMove(const std::vector<int> &moveDetails);

        static std::vector<int> parseLineToMoveDetails(const std::string &line);

        int countConsecutiveStones(int x, int y, int dx, int dy, CellState type);

        bool checkDirection(int x, int y, int dx, int dy, CellState type) const;

        bool isGameOver();

        int countEmptySpaces(int x, int y, int dx, int dy);

        bool isGameOver(CellState &winnerType);

        int minimax(Board boardCpy, int depth, bool isMaximizingPlayer, int alpha, int beta);

        int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);
    };
}
