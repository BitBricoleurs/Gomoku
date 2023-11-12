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

        int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);

        void printBoard() const;

        [[nodiscard]] std::vector<Move> getStrategicLegalMoves() const;


    private:
        int size;
        std::vector<std::vector<Cell>> cells;
        [[nodiscard]] bool isValidCoordinate(int x, int y) const;

        [[nodiscard]] int evaluate() const;

        [[nodiscard]] int evaluateLine(int x, int y, int dx, int dy, CellState type) const;

        [[nodiscard]] bool isGameOver() const;

        [[nodiscard]] bool isNearbyOccupied(int x, int y) const;

        [[nodiscard]] bool isGameOverFor(CellState type) const;

        [[nodiscard]] bool checkDirection(int x, int y, int dx, int dy, CellState type) const;

        [[nodiscard]] int evaluateCell(int x, int y, CellState player) const;
    };
}
