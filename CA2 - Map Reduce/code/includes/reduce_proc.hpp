#pragma once

#include "utils.hpp"

class ReduceProc {
public:
    ReduceProc(std::string dataLine_);
    void startReduce();

private:
    std::string dataLine;
};