//
// Created by alexandre on 07/11/23.
//

#include "GameBot.hpp"

void Gomoku::GameBot::enforceTimeLimit(const std::chrono::time_point<std::chrono::steady_clock>& startTime,
                               const std::chrono::time_point<std::chrono::steady_clock>& endTime) {
    if (std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime) > MAX_TIME_PER_MOVE) {
        std::cerr << "Time limit exceeded" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Gomoku::GameBot::enforceMemoryLimit() {
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
            struct rusage usage{};
            if (getrusage(RUSAGE_SELF, &usage) == -1)
                return 0;
            return usage.ru_maxrss * 1024;
    #endif
}

void Gomoku::GameBot::respond(const std::string& response)
{
    std::cout << response << std::endl;
    std::fflush(stdout);
}

bool Gomoku::GameBot::isValidBoardSize(int size)
{
    return size > 0;
}

bool Gomoku::GameBot::areValidCoordinates(const std::string& xStr, const std::string& yStr)
{
    int x = std::stoi(xStr);
    int y = std::stoi(yStr);
    return x >= 0 && y >= 0;
}

void Gomoku::GameBot::handleStart(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        int size = std::stoi(args[0]);
        if (isValidBoardSize(size)) {
            board = std::make_unique<Board>(size);
            respond("OK");
        } else {
            respond("ERROR unsupported size");
        }
    } else {
        respond("ERROR not enough arguments");
    }
}

void Gomoku::GameBot::handleTurn(const std::vector<std::string>& args)
{
    auto startTime = std::chrono::steady_clock::now();
    Move opponentMove(args[0], args[1]);
    if (args.size() == 2 && areValidCoordinates(args[0], args[1])) {
        board->makeMove(opponentMove.x, opponentMove.y, CellState::Opponent);

        //Move bestMove = calculateBestMove();
        Move bestMove(0, 0);
        respond(std::to_string(bestMove.x) + "," + std::to_string(bestMove.y));

        auto endTime = std::chrono::steady_clock::now();
        enforceTimeLimit(startTime, endTime);
        enforceMemoryLimit();
    } else {
        respond("ERROR invalid coordinates");
    }
}

void Gomoku::GameBot::handleBegin()
{
    auto startTime = std::chrono::steady_clock::now();

    //Move bestMove = calculateBestMove();
    Move bestMove(0, 0);
    respond(std::to_string(bestMove.x) + "," + std::to_string(bestMove.y));

    auto endTime = std::chrono::steady_clock::now();
    enforceTimeLimit(startTime, endTime);
    enforceMemoryLimit();
}

void Gomoku::GameBot::handleBoard(const std::vector<std::string> &args)
{
    // Votre logique pour gérer l'état du plateau de jeu.
    respond("BOARD message - board state processed");
}

void Gomoku::GameBot::handleEnd()
{
    endBot = true;
}

void Gomoku::GameBot::handleInfo(const std::vector<std::string> &args) {
    for (const auto& arg : args) {
        std::istringstream iss(arg);
        std::string key;
        std::string value;
        if (std::getline(iss, key, ' ') && std::getline(iss, value)) {
            infoMap[key] = value;
        } else {
            return;
        }
    }

    respond("OK - Information updated");
}

void Gomoku::GameBot::handleAbout()
{
    respond(R"(name="GameBot", author="Alexandre", version="1.0", country="France")");
}

bool Gomoku::GameBot::isEndBot() const
{
    return endBot;
}