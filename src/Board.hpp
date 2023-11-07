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
        Board(int size) : size(size), cells(size, std::vector<Cell>(size)) {}

        bool isValidMove(int x, int y) const;

        void makeMove(int x, int y, CellState player);

        void undoMove(int x, int y);

        int evaluate() const;

    private:
        int size;
        std::vector<std::vector<Cell>> cells;
    };
}
