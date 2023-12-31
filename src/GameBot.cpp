//
// Created by alexandre on 07/11/23.
//

#include "GameBot.hpp"

#include <utility>
#include <iostream>
#include <sstream>
#include "Core.hpp"

Gomoku::GameBot::GameBot(bool isPrintGame, int valgrindEnable, std::string  nameBot) : isPrintGame(isPrintGame), valgrindEnabled(valgrindEnable), botName(std::move(nameBot))
{
}

void Gomoku::GameBot::enforceMemoryLimit() const {
    if (valgrindEnabled) {
        return;
    }
    if (getMemoryUsage() > maxMemoryMB * 1024 * 1024) {
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
            board = std::make_unique<Board>(size);
            isBoardInit = true;
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
    auto startTime = std::chrono::steady_clock::now();
    auto winnerType = CellState::Empty;

    std::vector<std::string> tokens = Gomoku::Core::splitString(args[0], ',');
    if (isBoardInit && tokens.size() == 2 && areValidCoordinates(tokens[0], tokens[1])) {
        Move opponentMove(tokens[0], tokens[1]);
        board->makeMove(opponentMove.x, opponentMove.y, CellState::Opponent);

        if (isPrintGame) {
            board->printBoard();
            if (isGameOver(winnerType))
                std::cout << "Winner is " << (winnerType == CellState::Me ? "Me" : "Opponent") << std::endl;
        }

        Move bestMove = calculateBestMove();
        board->makeMove(bestMove.x, bestMove.y, CellState::Me);
        respond(std::to_string(bestMove.x) + "," + std::to_string(bestMove.y));

        auto endTime = std::chrono::steady_clock::now();
        enforceMemoryLimit();
        if (isPrintGame) {
            board->printBoard();
            if (isGameOver(winnerType))
                std::cout << "Winner is " << (winnerType == CellState::Me ? "Me" : "Opponent") << std::endl;
        }
    } else {
        respond("ERROR invalid coordinates");
    }
}

void Gomoku::GameBot::handleBegin()
{
    auto startTime = std::chrono::steady_clock::now();

    if (isBoardInit) {

        Move bestMove = calculateBestMove();
        board->makeMove(bestMove.x, bestMove.y, CellState::Me);
        respond(std::to_string(bestMove.x) + "," + std::to_string(bestMove.y));

        auto endTime = std::chrono::steady_clock::now();
        enforceMemoryLimit();
        if (isPrintGame)
            board->printBoard();
    } else {
        respond("ERROR board not initialized");
    }

}

void Gomoku::GameBot::handleBoard(const std::vector<std::string> &args) {
    board->clear();
    CellState winnerType = CellState::Empty;

    for (const auto &line : args) {
        if (line == "DONE") {
            Move nextMove = calculateBestMove();
            board->makeMove(nextMove.x, nextMove.y, CellState::Me);
            respond(std::to_string(nextMove.x) + "," + std::to_string(nextMove.y));
            if (isPrintGame)
                board->printBoard();
            return;
        }

        std::vector<int> moveDetails = parseLineToMoveDetails(line);

        if (isValidMove(moveDetails)) {
            int x = moveDetails[0];
            int y = moveDetails[1];
            auto state = static_cast<CellState>(moveDetails[2]);
            board->makeMove(x, y, state);
            if (isPrintGame)
                board->printBoard();
        } else {
            respond("ERROR invalid board input");
            return;
        }
    }
    if (isPrintGame)
        if (isGameOver(winnerType))
            std::cout << "Winner is " << (winnerType == CellState::Me ? "Me" : "Opponent") << std::endl;


    respond("ERROR board data incomplete");
}

std::vector<int> Gomoku::GameBot::parseLineToMoveDetails(const std::string &line) {
    std::istringstream iss(line);
    std::string part;
    std::vector<int> moveDetails;

    while (std::getline(iss, part, ',')) {
        try {
            moveDetails.push_back(std::stoi(part));
        } catch (const std::invalid_argument& e) {
            return {};
        } catch (const std::out_of_range& e) {
            return {};
        }
    }

    return moveDetails;
}

bool Gomoku::GameBot::isValidMove(const std::vector<int> &moveDetails) {
    if (moveDetails.size() != 3) {
        return false;
    }

    int x = moveDetails[0];
    int y = moveDetails[1];
    int stateValue = moveDetails[2];

    return x >= 0 && x < board->getSize() &&
           y >= 0 && y < board->getSize() &&
           stateValue > static_cast<int>(CellState::Empty) &&
           stateValue < static_cast<int>(CellState::Error);
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
    std::ostringstream responseStream;
    responseStream << R"(name=")" << botName << R"(", author="Alexandre", version="1.0", country="France")";
    respond(responseStream.str());
}

bool Gomoku::GameBot::isEndBot() const
{
    return endBot;
}

bool Gomoku::GameBot::isGameOver() {
    for (int x = 0; x < board->getSize(); ++x) {
        for (int y = 0; y < board->getSize(); ++y) {
            if (board->getCellState(x,y) != CellState::Empty) {
                CellState player = board->getCellState(x,y);
                if (checkDirection(x, y, 1, 0, player) ||
                    checkDirection(x, y, 0, 1, player) ||
                    checkDirection(x, y, 1, 1, player) ||
                    checkDirection(x, y, 1, -1, player)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Gomoku::GameBot::isGameOver(CellState& winnerType) {
    for (int x = 0; x < board->getSize(); ++x) {
        for (int y = 0; y < board->getSize(); ++y) {
            if (board->getCellState(x,y) != CellState::Empty) {
                CellState player = board->getCellState(x,y);
                if (checkDirection(x, y, 1, 0, player) ||
                    checkDirection(x, y, 0, 1, player) ||
                    checkDirection(x, y, 1, 1, player) ||
                    checkDirection(x, y, 1, -1, player)) {
                    if (winnerType == CellState::Empty) {
                        winnerType = player;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool  Gomoku::GameBot::checkDirection(int x, int y, int dx, int dy, CellState type) const {
    for (int i = 0; i < 5; ++i) {
        if (x < 0 || x >= board->getSize() || y < 0 || y >= board->getSize() || board->getCellState(x, y) != type) {
            return false;
        }
        x += dx;
        y += dy;
    }
    return true;
}

/*
Gomoku::Move Gomoku::GameBot::calculateBestMove() {
    std::vector<Move> legalMoves = board->getStrategicLegalMoves();
    std::vector<std::future<std::pair<Move, int>>> futures;

    for (const auto& move : legalMoves) {
        futures.push_back(std::async(std::launch::async, [this, move]() {
            Board localBoard = *board;
            localBoard.makeMove(move.x, move.y, CellState::Me);
            int score = minimax(localBoard, DEPTH - 1, false, (std::numeric_limits<int>::min()), (std::numeric_limits<int>::max()));
            std::cout << "Move " << move.x << " " << move.y << " " << "Score:  " << score << std::endl;
            std::cout << "Score Move " << move.x << " " << move.y << " " << "Score:  " << score << std::endl;
            localBoard.undoMove(move.x, move.y);
            return std::make_pair(move, score);
        }));
    }

    Move bestMove{-1, -1};
    int bestScore = std::numeric_limits<int>::min();

    for (auto& future : futures) {
        auto [move, score] = future.get();
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}


int Gomoku::GameBot::minimax(Board boardCpy, int depth, bool isMaximizingPlayer, int alpha, int beta)
{
    if (depth == 0 || isGameOver()) {
        return evaluate();
    }

    if (isMaximizingPlayer) {
        int maxEval = (std::numeric_limits<int>::min());
        for (const auto& move : boardCpy.getStrategicLegalMoves()) {
            boardCpy.makeMove(move.x, move.y, CellState::Me);
            if (isPrintGame) {
                boardCpy.printBoard();
            }
            int eval = minimax(boardCpy, depth - 1, false, alpha, beta);
            boardCpy.undoMove(move.x, move.y);
            maxEval = (std::max(maxEval, eval));
            alpha = (std::max(alpha, eval));
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = (std::numeric_limits<int>::max());
        for (const auto& move : boardCpy.getStrategicLegalMoves()) {
            boardCpy.makeMove(move.x, move.y, CellState::Opponent);
            if (isPrintGame) {
                boardCpy.printBoard();
            }
            int eval = minimax(boardCpy, depth - 1, true, alpha, beta);
            boardCpy.undoMove(move.x, move.y);
            minEval = (std::min(minEval, eval));
            beta = (std::min(beta, eval));
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}*/


Gomoku::Move Gomoku::GameBot::calculateBestMove()
{
    std::vector<Move> legalMoves = board->getStrategicLegalMoves();
    int bestScore = (std::numeric_limits<int>::min());
    Move bestMove{-1, -1};

    int timeout_turn = std::chrono::duration_cast<std::chrono::milliseconds>(MAX_TIME_PER_MOVE).count();

    auto it = infoMap.find("timeout_turn");
    if (it != infoMap.end()) {
        try {
            timeout_turn = std::stoi(it->second);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid value for timeout_turn, using default: " << it->second << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Value for timeout_turn out of range, using default: " << it->second << std::endl;
        }
    }

    timeIsUp = false;

    searchComplete = false;
    std::thread timeThread(&GameBot::timeWatcher, this, timeout_turn - 100);

    for (const auto& move : legalMoves) {
        if (timeIsUp.load()) {
            break;
        }
        board->makeMove(move.x, move.y, CellState::Me);
        int score = minimax(DEPTH - 1, false, (std::numeric_limits<int>::min()), (std::numeric_limits<int>::max()));
        board->undoMove(move.x, move.y);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    searchComplete = true;

    if (timeThread.joinable()) {
        timeThread.join();
    }
    if (bestMove.x == -1 && bestMove.y == -1) {
        return legalMoves.front();
    }
    return bestMove;
}

int Gomoku::GameBot::minimax(int depth, bool isMaximizingPlayer, int alpha, int beta)
{
    if (timeIsUp.load()) {
        return (std::numeric_limits<int>::min());
    }
    if (depth == 0 || isGameOver()) {
        return evaluate();
    }

    if (isMaximizingPlayer) {
        int maxEval = (std::numeric_limits<int>::min());
        for (const auto& move : board->getStrategicLegalMoves()) {
            board->makeMove(move.x, move.y, CellState::Me);
            if (isPrintGame) {
                board->printBoard();
            }
            int eval = minimax(depth - 1, false, alpha, beta);
            board->undoMove(move.x, move.y);
            maxEval = (std::max(maxEval, eval));
            alpha = (std::max(alpha, eval));
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = (std::numeric_limits<int>::max());
        for (const auto& move : board->getStrategicLegalMoves()) {
            board->makeMove(move.x, move.y, CellState::Opponent);
            if (isPrintGame) {
                board->printBoard();
            }
            int eval = minimax(depth - 1, true, alpha, beta);
            board->undoMove(move.x, move.y);
            minEval = (std::min(minEval, eval));
            beta = (std::min(beta, eval));
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

int Gomoku::GameBot::evaluate()
{
    int score = 0;

    for (int x = 0; x < board->getSize(); ++x) {
        for (int y = 0; y <  board->getSize(); ++y) {
            CellState player = board->getCellState(x,y);
            if (player != CellState::Empty) {
                score += evaluateCell(x, y, player);
            }
        }
    }
    return score;
}

int Gomoku::GameBot::evaluateCell(int x, int y, CellState type) {
    int score = 0;

    score += evaluateLine(x, y, 1, 0, type);
    score += evaluateLine(x, y, 0, 1, type);
    score += evaluateLine(x, y, 1, 1, type);
    score += evaluateLine(x, y, -1, 1, type);

    return score;
}

int Gomoku::GameBot::countConsecutiveStones(int x, int y, int dx, int dy, CellState type)
{
    int count = 0;
    int i = 1;

    while (board->isValidCoordinate(x + i*dx, y + i*dy) && board->getCellState(x + i*dx, y + i*dy) == type) {
        if (board->getCellState(x + i*dx, y + i*dy) == type) {
            count++;
        }
        i++;
    }

    return count;
}


void Gomoku::GameBot::checkEnds(int x, int y, int dx, int dy, CellState type, int& openEnds, int& blockedEnds)
{
    if (!board->isValidCoordinate(x, y)) {
        blockedEnds++;
    } else if (board->getCellState(x, y) == CellState::Empty) {
        openEnds++;
    } else if (board->getCellState(x, y) != type) {
        blockedEnds++;
    }
}

int Gomoku::GameBot::countEmptySpaces(int x, int y, int dx, int dy)
{
    int count = 0;

    while (board->isValidCoordinate(x, y) && board->getCellState(x, y) == CellState::Empty) {
        count++;
        x += dx;
        y += dy;
    }

    return count;
}

int Gomoku::GameBot::evaluateLine(int x, int y, int dx, int dy, CellState type) {
    int countForward = countConsecutiveStones(x, y, dx, dy, type);
    int countBackward = countConsecutiveStones(x, y, -dx, -dy, type);
    int count = countForward + countBackward;

    int openEnds = 0;
    int blockedEnds = 0;
    int extraSpaces = 0;

    checkEnds(x + (countForward + 1) * dx, y + (countForward + 1) * dy, dx, dy, type, openEnds, blockedEnds);
    checkEnds(x - (countBackward + 1) * dx, y - (countBackward + 1) * dy, dx, dy, type, openEnds, blockedEnds);


    extraSpaces = countEmptySpaces(x + (count + 1) * dx, y + (count + 1) * dy, dx, dy) +
                  countEmptySpaces(x - (count + 1) * dx, y - (count + 1) * dy, -dx, -dy);

    if (count + extraSpaces < 5)
        return 0;
    LineConfig config = {count, openEnds, blockedEnds};
    auto it = scoreMap.find(config);
    if (it != scoreMap.cend()) {
        return (type == CellState::Me) ? it->second : -(it->second) * 1.2;
    }
    return 0;
}

void Gomoku::GameBot::timeWatcher(int timeLimitMs)
{
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        if (elapsedTime >= timeLimitMs || searchComplete.load()) {
            timeIsUp = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
