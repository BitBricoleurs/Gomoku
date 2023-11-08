//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Cell.hpp"

namespace Gomoku {

    class Board {
    public:
        explicit Board(int size);

        [[nodiscard]] bool isValidMove(int x, int y) const;

        void makeMove(int x, int y, CellState player);

        void undoMove(int x, int y);

        [[nodiscard]] int evaluate() const;

    private:
        int size;
        std::vector<std::vector<Cell>> cells;
    };
}
