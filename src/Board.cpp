//
// Created by alexandre on 07/11/23.
//

#include <iostream>
#include "Board.hpp"

namespace Gomoku {

    Board::Board(int size) : size(size), cells(size, std::vector<Cell>(size)) {}

    bool Board::isValidMove(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size && cells[x][y].get_state() == CellState::Empty;
    }

    void Board::makeMove(int x, int y, CellState type) {
        if (isValidMove(x, y)) {
            cells[x][y].set_state(type);
        }
    }

    void Board::undoMove(int x, int y) {
        if (isValidMove(x, y)) {
            cells[x][y].set_state(CellState::Empty);
        }
    }

    void Board::clear() {
        for (auto& row : cells) {
            for (auto& cell : row) {
                cell.set_state(CellState::Empty);
            }
        }
    }

    std::vector<Move> Board::getLegalMoves() const
    {
        std::vector<Move> legalMoves;
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                if (cells[x][y].get_state() == CellState::Empty) {
                    legalMoves.emplace_back(Move{x, y});
                }
            }
        }
        return legalMoves;
    }

    bool Board::isGameOver() const
    {
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                if (cells[x][y].get_state() != CellState::Empty) {
                    CellState player = cells[x][y].get_state();
                    if (checkDirection(x, y, 1, 0, player) ||
                        checkDirection(x, y, 0, 1, player) ||
                        checkDirection(x, y, 1, 1, player) ||
                        checkDirection(x, y, 1, -1, player)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Board::checkDirection(int x, int y, int dx, int dy, CellState type) const
    {
        for (int i = 0; i < 5; ++i) {
            if (x < 0 || x >= size || y < 0 || y >= size || cells[x][y].get_state() != type) {
                return false;
            }
            x += dx;
            y += dy;
        }
        return true;
    }

    bool Board::isGameOverFor(CellState type) const
    {
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                if (cells[x][y].get_state() == type) {
                    if (checkDirection(x, y, 1, 0, type) ||
                        checkDirection(x, y, 0, 1, type) ||
                        checkDirection(x, y, 1, 1, type) ||
                        checkDirection(x, y, 1, -1, type)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    int Board::minimax(int depth, bool isMaximizingPlayer, int alpha, int beta)
    {
        if (depth == 0 || isGameOver()) {
            return evaluate();
        }

        if (isMaximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& move : getLegalMoves()) {
                makeMove(move.x, move.y, CellState::Me);
                int eval = minimax(depth - 1, false, alpha, beta);
                undoMove(move.x, move.y);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& move : getLegalMoves()) {
                makeMove(move.x, move.y, CellState::Opponent);
                int eval = minimax(depth - 1, true, alpha, beta);
                undoMove(move.x, move.y);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return minEval;
        }
    }

    int Board::evaluate() const
    {
        int score = 0;
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                CellState player = cells[x][y].get_state();
                if (player != CellState::Empty) {
                    score += evaluateCell(x, y, player);
                }
            }
        }
        return score;
    }

    int Board::evaluateCell(int x, int y, CellState type) const
    {
        int score = 0;

        score += evaluateDirection(x, y, 1, 0, type);
        score += evaluateDirection(x, y, 0, 1, type);
        score += evaluateDirection(x, y, 1, 1, type);
        score += evaluateDirection(x, y, -1, 1, type);

        return score;
    }

    int Board::evaluateDirection(int x, int y, int dx, int dy, CellState type) const
    {
        int inLine = 0;
        int openEnds = 0;

        if (x < 0 || x >= size || y < 0 || y >= size) {
            return 0;
        }

        int i = 0;
        while (i < size && isValidCoordinate(x + i*dx, y + i*dy) && cells[x + i*dx][y + i*dy].get_state() == type) {
            ++inLine;
            ++i;
        }
        if (isValidCoordinate(x + i*dx, y + i*dy) && cells[x + i*dx][y + i*dy].get_state() == CellState::Empty) {
            ++openEnds;
        }

        i = 1;
        while (i < size && isValidCoordinate(x - i*dx, y - i*dy) && cells[x - i*dx][y - i*dy].get_state() == type) {
            ++inLine;
            ++i;
        }
        if (isValidCoordinate(x - i*dx, y - i*dy) && cells[x - i*dx][y - i*dy].get_state() == CellState::Empty) {
            ++openEnds;
        }

        if (inLine >= 5)
            return 10000;
        else if (inLine == 4 && openEnds == 2)
            return 1000;
        else if (inLine == 4 && openEnds == 1)
            return 500;

        return 0;
    }

    bool Board::isValidCoordinate(int x, int y) const
    {
        return x >= 0 && x < size && y >= 0 && y < size;
    }




}
