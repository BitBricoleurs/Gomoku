//
// Created by alexandre on 13/11/23.
//

#pragma once

struct LineConfig {
    int count;
    int openEnds;
    int blockedEnds;

    bool operator==(const LineConfig& other) const;
};
