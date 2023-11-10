//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <string>
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
            explicit Core(bool isPrintGame);
            ~Core() = default;

            void run();
            static std::vector<std::string> splitString(const std::string& str, char delimiter);

        private:
            GameBot myBot;
            bool parseCommand(const std::string& command, const std::vector<std::string>& args);
                std::unordered_map<std::string, CommandType> commandMap;
                void initializeCommandMap();
        bool isPrintGame;
    };

}
