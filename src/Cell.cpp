//
// Created by alexandre on 07/11/23.
//

#include "Cell.hpp"

namespace Gomoku {

    Cell::Cell() : state(CellState::Empty) {}

    void Cell::set_state(Gomoku::CellState new_state)
    {
        state = new_state;
    }

    CellState Cell::get_state() const
    {
        return state;
    }

}
