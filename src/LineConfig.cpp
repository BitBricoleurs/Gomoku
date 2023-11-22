//
// Created by alexandre on 13/11/23.
//

#include "LineConfig.hpp"

bool LineConfig::operator==(const LineConfig& other) const {
    return count == other.count && openEnds == other.openEnds && blockedEnds == other.blockedEnds;
}
