//
// Created by alexandre on 13/11/23.
//

#include "LineConfigHash.hpp"

std::size_t LineConfigHash::operator()(const LineConfig& lc) const {
    return std::hash<int>()(lc.count) ^ std::hash<int>()(lc.openEnds) ^ std::hash<int>()(lc.blockedEnds);
}
