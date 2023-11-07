//
// Created by alexandre on 07/11/23.
//

#include "Core.hpp"

namespace Gomoku {

    Core::Core() {
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

    void Core::run() {
        std::string line = std::string();

        while (std::getline(std::cin, line)) {
            if (!line.empty()) {
                auto tokens = splitString(line, ' ');
                if (!tokens.empty()) {
                    std::string command = tokens.front();
                    tokens.erase(tokens.begin());
                    if (!parseCommand(command, tokens)) {
                        myBot.respond("UNKNOWN");
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
                myBot.handleStart(args);
                return true;
            case CommandType::TURN:
                myBot.handleTurn(args);
                return true;

            case CommandType::BEGIN:
                myBot.handleBegin();
                return true;

            case CommandType::BOARD:
                myBot.handleBoard(args);
                return true;

            case CommandType::END:
                myBot.handleEnd();
                return true;

            case CommandType::INFO:
                myBot.handleInfo(args);
                return true;

            case CommandType::ABOUT:
                myBot.handleAbout();
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
