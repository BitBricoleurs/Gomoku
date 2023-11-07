//
// Created by alexandre on 07/11/23.
//

#include "Board.hpp"

namespace Gomoku {

    bool Board::isValidMove(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size && cells[x][y].get_state() == CellState::Empty;
    }

    void Board::makeMove(int x, int y, CellState player) {
        if (isValidMove(x, y)) {
            cells[x][y].set_state(player);
        }
    }

    void Board::undoMove(int x, int y) {
        if (isValidMove(x, y)) {
            cells[x][y].set_state(CellState::Empty);
        }
    }

    int Board::evaluate() const {
        return 0;
    }

}
