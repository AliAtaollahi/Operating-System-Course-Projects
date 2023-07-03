#pragma once


#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

class System
{
public:
    System(std::string assetsDirectory_);
    void start_program();
    void categorize_and_print(const std::vector<std::string>& genersList, int numberOfParts, std::string genersFileData);
    void reduces(const std::vector<std::string>& genersList, int numberOfParts);   
    void maps(int numberOfParts, std::string genersFileData);
private:
    std::string assetsDirectory;
};
