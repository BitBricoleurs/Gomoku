//
// Created by alexandre on 07/11/23.
//

#include <string>
#include "Move.hpp"

namespace Gomoku {

    Move::Move(int x, int y) : x(x), y(y) {}
    Move::Move(const std::string& x, const std::string& y) : x(std::stoi(x)), y(std::stoi(y)) {}

}
