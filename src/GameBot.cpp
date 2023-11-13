//
// Created by alexandre on 07/11/23.
//

#include "GameBot.hpp"
#include "Core.hpp"

Gomoku::GameBot::GameBot(bool isPrintGame) : isPrintGame(isPrintGame)
{
}

void Gomoku::GameBot::enforceTimeLimit(const std::chrono::time_point<std::chrono::steady_clock>& startTime,
                                       const std::chrono::time_point<std::chrono::steady_clock>& endTime) {
    auto it = infoMap.find("timeout_turn");
    if (it != infoMap.end()) {
        try {
            int timeout_turn = std::stoi(it->second);

            if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() > timeout_turn) {
                std::cerr << "Time limit for move exceeded" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid value for timeout_turn: " << it->second << std::endl;
            return;
        } catch (const std::out_of_range& e) {
            std::cerr << "Value for timeout_turn out of range: " << it->second << std::endl;
            return;
        }
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
    //std::fflush(stdout);
}

bool Gomoku::GameBot::isValidBoardSize(int size)
{
    return size > 0;
}

bool Gomoku::GameBot::areValidCoordinates(const std::string& xStr, const std::string& yStr)
{
    try {
        int x = std::stoi(xStr);
        int y = std::stoi(yStr);

        bool isWithinBounds = x >= 0 && x < board->getSize() && y >= 0 && y <  board->getSize();
        bool isCellFree = isWithinBounds && board->getCellState(x, y) == CellState::Empty;

        return isWithinBounds && isCellFree;
    } catch (const std::invalid_argument& e) {
        return false;
    } catch (const std::out_of_range& e) {
        return false;
    }
}


void Gomoku::GameBot::handleStart(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        matchStartTime = std::chrono::steady_clock::now();

        auto it = infoMap.find("timeout_match");
        if (it != infoMap.end()) {
            timeoutMatch = std::stoi(it->second);
        }
        int size = std::stoi(args[0]);
        if (isValidBoardSize(size)) {
            //std::cout << "Size" << size << std::endl;
            board = std::make_unique<Board>(size);
            respond("OK");
            if (isPrintGame)
                board->printBoard();
        } else {
            respond("ERROR unsupported size");
        }
    } else {
        respond("ERROR not enough arguments");
    }
}

void Gomoku::GameBot::handleTurn(const std::vector<std::string>& args)
{
    enforceMatchTimeLimit();
    auto startTime = std::chrono::steady_clock::now();

    std::vector<std::string> tokens = Gomoku::Core::splitString(args[0], ',');
    Move opponentMove(tokens[0], tokens[1]);
    if (tokens.size() == 2 && areValidCoordinates(tokens[0], tokens[1]) && board->isBoardInit()) {
        board->makeMove(opponentMove.x, opponentMove.y, CellState::Opponent);

        Move bestMove = calculateBestMove();
        board->makeMove(bestMove.x, bestMove.y, CellState::Me);
        respond(std::to_string(bestMove.x) + "," + std::to_string(bestMove.y));

        auto endTime = std::chrono::steady_clock::now();
        enforceTimeLimit(startTime, endTime);
        enforceMemoryLimit();
        if (isPrintGame)
            board->printBoard();
    } else {
        respond("ERROR invalid coordinates");
    }
}

void Gomoku::GameBot::handleBegin()
{
    enforceMatchTimeLimit();
    auto startTime = std::chrono::steady_clock::now();

    if (board->isBoardInit()) {

        Move bestMove = calculateBestMove();
        board->makeMove(bestMove.x, bestMove.y, CellState::Me);
        respond(std::to_string(bestMove.x) + "," + std::to_string(bestMove.y));

        auto endTime = std::chrono::steady_clock::now();
        enforceTimeLimit(startTime, endTime);
        enforceMemoryLimit();
        if (isPrintGame)
            board->printBoard();
    } else {
        respond("ERROR board not initialized");
    }

}

void Gomoku::GameBot::handleBoard(const std::vector<std::string> &args) {
    board->clear();

    for (const auto &line : args) {
        if (line == "DONE") {
            Move nextMove = calculateBestMove();
            respond(std::to_string(nextMove.x) + "," + std::to_string(nextMove.y));
            return;
        }

        std::istringstream iss(line);
        std::string part;
        std::vector<int> moveDetails;

        try {
            while (std::getline(iss, part, ',')) {
                moveDetails.push_back(std::stoi(part));
            }

            if (moveDetails.size() == 3) {
                int x = moveDetails[0];
                int y = moveDetails[1];
                int stateValue = moveDetails[2];

                if (x >= 0 && x < board->getSize() && y >= 0 && y < board->getSize() &&
                    stateValue > static_cast<int>(CellState::Empty) &&
                    stateValue < static_cast<int>(CellState::Error)) {

                    auto state = static_cast<CellState>(stateValue);
                    board->makeMove(x, y, state);
                } else {
                    respond("ERROR invalid board input");
                    return;
                }
            } else {
                respond("ERROR invalid board input");
                return;
            }
        } catch (const std::invalid_argument& e) {
            respond("ERROR invalid board input");
            return;
        } catch (const std::out_of_range& e) {
            respond("ERROR invalid board input");
            return;
        }
    }
    respond("ERROR board data incomplete");
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
            if (key == "max_memory") {
                try {
                    maxMemoryMB = std::stoi(value) / (1024 * 1024);
                } catch (const std::exception& e) {
                }
            }
        }
    }
}

void Gomoku::GameBot::handleAbout()
{
    respond(R"(name="GameBot", author="Alexandre", version="1.0", country="France")");
}

bool Gomoku::GameBot::isEndBot() const
{
    return endBot;
}

/*
Gomoku::Move Gomoku::GameBot::calculateBestMove()
{
    std::vector<Move> legalMoves = board->getStrategicLegalMoves();
    int bestScore = std::numeric_limits<int>::min();
    Move bestMove{-1, -1};

    unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads);

    std::mutex mutex;

    int movesPerThread = std::ceil(legalMoves.size() / static_cast<double>(numThreads));
    std::cout << "Moves per thread: " << movesPerThread << std::endl;
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread([&, i]() {
            int start = i * movesPerThread;
            int end = std::min(start + movesPerThread, static_cast<int>(legalMoves.size()));

            for (int j = start; j < end; ++j) {
                Move move = legalMoves[j];
                board->makeMove(move.x, move.y, CellState::Me);
                int score = board->minimax(DEPTH - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                board->undoMove(move.x, move.y);

                std::lock_guard<std::mutex> lock(mutex);
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
        });
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    return bestMove;
}*/

Gomoku::Move Gomoku::GameBot::calculateBestMove()
{
    std::vector<Move> legalMoves = board->getLegalMoves();
    int bestScore = std::numeric_limits<int>::min();
    Move bestMove{-1, -1};

    for (const auto& move : legalMoves) {
        board->makeMove(move.x, move.y, CellState::Me);
        int score = board->minimax(DEPTH - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        board->undoMove(move.x, move.y);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}


void Gomoku::GameBot::enforceMatchTimeLimit()
{
    if (timeoutMatch > 0) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - matchStartTime).count();
        if (elapsedTime > timeoutMatch) {
            std::cerr << "Match time limit exceeded" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}
