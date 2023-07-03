#pragma once

#include "utils.hpp"

#include <algorithm>
#include <string.h>

class MapProc {
public:
    MapProc(std::string dataLine_);
    void startMap();

private:
    std::string dataLine;
};