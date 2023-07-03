#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include<stdarg.h>

#include "consts.hpp"

class Utils
{
public:
    Utils(/* args */);
    static std::string parse_line(std::string &str, char del);
    static std::vector<std::string> make_list_from_line(std::string line, char divider);
    static std::string read_from_file(std::string fileName);
    static int get_number_of_parts();
    static int get_index(std::string K,const std::vector<std::string>& v);
    static std::string delete_last_char(std::string str);
    static std::string make_pipe_dir(std::string dir, int partNum);
    static std::string make_message(const std::vector<std::string>& args);
    static std::string make_part_dir(std::string assetsDirectory, int num);
    static void error(const char *msg);
    static void wait(int num);
};

std::ostream &operator<<(std::ostream &out, const std::vector<std::string> &str);
