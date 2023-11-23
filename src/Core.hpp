//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "GameBot.hpp"

namespace Gomoku {

    enum class CommandType {
        START,
        TURN,
        BEGIN,
        BOARD,
        END,
        INFO,
        ABOUT,
        UNKNOWN
    };

    class Core {
        public :
            Core(bool isPrintGame, bool valgrindEnable, const std::string& nameBot);
            ~Core() = default;

            void run();
            static std::vector<std::string> splitString(const std::string& str, char delimiter);

        private:
            std::unique_ptr<GameBot> myBot;

            bool parseCommand(const std::string& command, const std::vector<std::string>& args);

            std::unordered_map<std::string, CommandType> commandMap;

            void initializeCommandMap();

        static std::string trim(const std::string &str);
    };

}
