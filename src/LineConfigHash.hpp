//
// Created by alexandre on 13/11/23.
//

#include "LineConfig.hpp"
#include <functional>

struct LineConfigHash {

    std::size_t operator()(const LineConfig& lc) const;
};
