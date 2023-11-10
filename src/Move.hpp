//
// Created by alexandre on 07/11/23.
//

namespace Gomoku {

    struct Move {
        int x;
        int y;

        Move(int x, int y);
        Move(const std::string& x, const std::string& y);

    };

}
