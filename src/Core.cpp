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
                if (args.size() == 1) {
                    int boardSize = std::stoi(args[0]);
                    std::cout << "OK" << std::endl;
                    return true;
                }
                break;
            case CommandType::TURN:
                return true;

            case CommandType::BEGIN:
                return true;

            case CommandType::BOARD:
                return true;

            case CommandType::END:
                return true;

            default:
                return false;
        }
        return false;
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
