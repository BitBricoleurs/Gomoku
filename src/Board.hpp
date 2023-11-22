//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
#include "Cell.hpp"
#include "Move.hpp"

namespace Gomoku {

    class Board {
    public:
        explicit Board(int size);

        [[nodiscard]] bool isValidMove(int x, int y) const;

        void makeMove(int x, int y, CellState type);

        void undoMove(int x, int y);

        void clear();

        [[nodiscard]] std::vector<Move> getLegalMoves() const;

        void printBoard() const;

        [[nodiscard]] std::vector<Move> getStrategicLegalMoves() const;

        [[nodiscard]] int getSize() const;

        [[nodiscard]] CellState getCellState(int x, int y) const;

        [[nodiscard]] bool isValidCoordinate(int x, int y) const;

        void printBoard(int lastMoveX, int lastMoveY) const;

    private:
        int size;

        std::vector<std::vector<Cell>> cells;

        [[nodiscard]] bool isNearbyOccupied(int x, int y) const;

    };
}
