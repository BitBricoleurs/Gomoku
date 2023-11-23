//
// Created by alexandre on 07/11/23.
//

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
                    legalMoves.emplace_back(x, y);
                }
            }
        }
        if (legalMoves.empty()) {
            int centerX = size / 2;
            int centerY = size / 2;
            if (cells[centerX][centerY].get_state() == CellState::Empty) {
                legalMoves.emplace_back(centerX, centerY);
            } else {
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (isValidCoordinate(centerX + dx, centerY + dy) &&
                            cells[centerX + dx][centerY + dy].get_state() == CellState::Empty) {
                            legalMoves.emplace_back(centerX + dx, centerY + dy);
                        }
                    }
                }
            }
        }
        return legalMoves;
    }


    bool Board::isNearbyOccupied(int x, int y) const {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int newX = x + dx, newY = y + dy;
                if (isValidCoordinate(newX, newY) && cells[newX][newY].get_state() != CellState::Empty) {
                    return true;
                }
            }
        }
        return false;
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

    void Board::printBoard(int lastMoveX = -1, int lastMoveY = -1) const
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

                if (x == lastMoveX && y == lastMoveY) {
                    std::cout << "[" << cellChar << "]";
                } else {
                    std::cout << " " << cellChar << " ";
                }
            }
            std::cout << std::endl;
        }
    }


    int Board::getSize() const
    {
        return size;
    }

    CellState Board::getCellState(int x, int y) const
    {
        if (!isValidCoordinate(x, y)) {
            return CellState::Error;
        }
        return cells[x][y].get_state();
    }
}
