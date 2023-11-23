//
// Created by alexandre on 13/11/23.
//

#include <functional>
#include "LineConfig.hpp"

struct LineConfigHash {

    std::size_t operator()(const LineConfig& lc) const;
};
