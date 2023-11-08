//
// Created by alexandre on 07/11/23.
//

#pragma once

namespace Gomoku {

    enum class CellState {
        Empty,
        Me,
        Opponent
    };

    class Cell {
    public:
        Cell();

        void set_state(CellState new_state);

        [[nodiscard]] CellState get_state() const;

    private:
        CellState state;
    };

}
