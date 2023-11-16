//
// Created by alexandre on 13/11/23.
//

#include "LineConfigHash.hpp"

size_t LineConfigHash::operator()(const LineConfig& config) const {
    return std::hash<int>()(config.count) ^
           std::hash<int>()(config.openEnds) ^
           std::hash<int>()(config.blockedEnds) ^
           std::hash<bool>()(config.holes);
}
