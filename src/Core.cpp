//
// Created by alexandre on 07/11/23.
//

#include "Core.hpp"

namespace Gomoku {

    Core::Core(bool isPrintGame, bool valgrindEnable, const std::string& nameBot): myBot(std::make_unique<GameBot>(isPrintGame, valgrindEnable, nameBot))
    {
        initializeCommandMap();
    }

    void Core::initializeCommandMap() {
        commandMap["START"] = CommandType::START;
        commandMap["TURN"] = CommandType::TURN;
        commandMap["BEGIN"] = CommandType::BEGIN;
        commandMap["BOARD"] = CommandType::BOARD;
        commandMap["END"] = CommandType::END;
        commandMap["INFO"] = CommandType::INFO;
        commandMap["ABOUT"] = CommandType::ABOUT;
    }

    std::string Core::trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n");
        if (first == std::string::npos)
            return "";
        size_t last = str.find_last_not_of(" \t\n");
        return str.substr(first, (last - first + 1));
    }

    void Core::run() {
        std::string line;
        bool isBoardCommand = false;
        std::vector<std::string> boardLines;

        while (!myBot->isEndBot() && std::getline(std::cin, line)) {

            line = trim(line);

            if (line.empty()) continue;

            if (line == "BOARD") {
                isBoardCommand = true;
                boardLines.clear();
                continue;
            }

            if (isBoardCommand) {
                if (line == "DONE") {
                    boardLines.push_back(line);
                    isBoardCommand = false;
                    if (!parseCommand("BOARD", boardLines)) {
                        Gomoku::GameBot::respond("UNKNOWN");
                    }
                } else {
                    boardLines.push_back(line);
                }
            } else {
                auto tokens = splitString(line, ' ');
                if (!tokens.empty()) {
                    std::string command = tokens.front();
                    tokens.erase(tokens.begin());
                    if (!parseCommand(command, tokens)) {
                        Gomoku::GameBot::respond("UNKNOWN");
                    }
                }
            }
        }
    }


    bool Gomoku::Core::parseCommand(const std::string& command, const std::vector<std::string>& args) {
        if (commandMap.empty()) {
            initializeCommandMap();
        }

        auto cmdIt = commandMap.find(command);
        CommandType cmdType = (cmdIt != commandMap.end()) ? cmdIt->second : CommandType::UNKNOWN;

        switch (cmdType) {
            case CommandType::START:
                myBot->handleStart(args);
                return true;
            case CommandType::TURN:
                myBot->handleTurn(args);
                return true;

            case CommandType::BEGIN:
                myBot->handleBegin();
                return true;

            case CommandType::BOARD:
                myBot->handleBoard(args);
                return true;

            case CommandType::END:
                myBot->handleEnd();
                return true;

            case CommandType::INFO:
                myBot->handleInfo(args);
                return true;

            case CommandType::ABOUT:
                myBot->handleAbout();
                return true;

            default:
                return false;
        }
    }

    std::vector<std::string> Gomoku::Core::splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);

        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
}
