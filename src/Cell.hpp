//
// Created by alexandre on 07/11/23.
//

#pragma once

namespace Gomoku {

    enum class CellState {
        Empty,
        Player1,
        Player2
    };

    class Cell {
    public:
        Cell();

        void set_state(CellState new_state);

        CellState get_state() const;

    private:
        CellState state;
    };

}
