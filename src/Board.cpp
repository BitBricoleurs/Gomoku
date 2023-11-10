//
// Created by alexandre on 07/11/23.
//

#include <iostream>
#include "Board.hpp"

namespace Gomoku {

    Board::Board(int size) : size(size), cells(size, std::vector<Cell>(size)) {}

    bool Board::isValidMove(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    void Board::makeMove(int x, int y, CellState type) {
        if (isValidMove(x, y) && cells[x][y].get_state() == CellState::Empty) {
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

    std::vector<Move> Board::getStrategicLegalMoves() const {
        std::vector<Move> legalMoves;
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                if (cells[x][y].get_state() == CellState::Empty && isNearbyOccupied(x, y)) {
                    legalMoves.emplace_back(Move{x, y});
                }
            }
        }
        return legalMoves;
    }

    bool Board::isNearbyOccupied(int x, int y) const {
        for (int dx = -2; dx <= 2; ++dx) {
            for (int dy = -2; dy <= 2; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (isValidCoordinate(nx, ny) && cells[nx][ny].get_state() != CellState::Empty) {
                    return true;
                }
            }
        }
        return false;
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

    int Board::evaluate() const {
        int score = 0;
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                CellState player = cells[x][y].get_state();
                if (player != CellState::Empty) {

                    if (player == CellState::Me) {
                        score += evaluateCell(x, y, player);
                    } else {
                        score -= evaluateCell(x, y, player);
                    }
                }
            }
        }
        return score;
    }

    int Board::evaluateCell(int x, int y, CellState type) const {
        int score = 0;

        score += evaluateLine(x, y, 1, 0, type);
        score += evaluateLine(x, y, 0, 1, type);
        score += evaluateLine(x, y, 1, 1, type);
        score += evaluateLine(x, y, -1, 1, type);

        return score;
    }

    int Board::evaluateLine(int x, int y, int dx, int dy, CellState type) const {
        int count = 0;
        int openEnds = 0;
        int blockedEnds = 0;

        int i = 1;
        while (isValidCoordinate(x + i*dx, y + i*dy)) {
            if (cells[x + i*dx][y + i*dy].get_state() == type) {
                count++;
                i++;
            } else if (cells[x + i*dx][y + i*dy].get_state() == CellState::Empty) {
                openEnds++;
                break;
            } else {
                blockedEnds++;
                break;
            }
        }

        i = 1;
        while (isValidCoordinate(x - i*dx, y - i*dy)) {
            if (cells[x - i*dx][y - i*dy].get_state() == type) {
                count++;
                i++;
            } else if (cells[x - i*dx][y - i*dy].get_state() == CellState::Empty) {
                openEnds++;
                break;
            } else {
                blockedEnds++;
                break;
            }
        }

        //std::cout << "Cell" << x << "," << y << " count: " << count << " openEnds: " << openEnds << " blockedEnds: " << blockedEnds <<  "type: " <<  (type == CellState::Me ? "Me" : "Opponent") << std::endl << "dir" << dx << "," << dy << std::endl;
        if (type == CellState::Opponent) {
            if (count == 4 && openEnds == 1) {
                std::cout << "here" << std::endl;
                return -100000;
            }
        }
        if (count >= 5) {
            return 1000000;
        } else if (count == 4) {
            if (openEnds == 2) {
                return 100000;
            } else if (openEnds == 1) {
                return 10000;
            } else if (blockedEnds == 1) {
                return 1000;
            }
        } else if (count == 3) {
            if (openEnds == 2) {
                return 1000;
            } else if (openEnds == 1) {
                return 100;
            } else if (blockedEnds == 1) {
                return 10;
            }
        } else if (count == 2) {
            if (openEnds == 2) {
                return 10;
            } else if (openEnds == 1) {
                return 1;
            }
        }
        return 0;
    }

    bool Board::isValidCoordinate(int x, int y) const
    {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    void Board::printBoard() const
    {
        std::cout << "  ";
        for (int x = 0; x < size; ++x) {
            std::cout << x % 10 << " ";
        }
        std::cout << std::endl;

        for (int y = 0; y < size; ++y) {
            std::cout << y % 10 << " ";
            for (int x = 0; x < size; ++x) {
                char cellChar = '.';
                if (cells[x][y].get_state() == CellState::Me) {
                    cellChar = 'X';
                } else if (cells[x][y].get_state() == CellState::Opponent) {
                    cellChar = 'O';
                }
                std::cout << cellChar << " ";
            }
            std::cout << std::endl;
        }
    }




}
