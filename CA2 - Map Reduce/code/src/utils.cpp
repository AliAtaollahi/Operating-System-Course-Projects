#include "../includes/utils.hpp"

using namespace std;

Utils::Utils(/* args */) {}

string Utils::parse_line(string &str, char del) {
    string res = "";
    istringstream sstream(str);
    getline(sstream, res, del);
    str = str.substr(res.size() + 1);
    return res;
}

vector<string> Utils::make_list_from_line(string line, char divider) {
    vector<string> dividedLine;
    string argument;
    istringstream arguments(line);
    while (getline(arguments, argument, divider)) {
        if (argument == "")
            continue;
        dividedLine.push_back(argument);
    }
    return dividedLine;
}

string Utils::read_from_file(string fileName) {
    ifstream file(fileName);
    string res = "", temp;
    while(getline(file, temp)) 
        res += temp + '\n';
    return res;
}

ostream &operator<<(ostream &out, const vector<string> &str) {
    for (auto substring : str) 
        out << substring << '\n';
    return out;
}

int Utils::get_number_of_parts() {
    return NUM_OF_PARTS;
}

int Utils::get_index(string K,const vector<string>& v)
{
    auto it = find(v.begin(), v.end(), K);
    return (it != v.end()) ? it - v.begin() : -1;
}

string Utils::delete_last_char(string str) {
    str.pop_back();
    return str;
}

string Utils::make_pipe_dir(string dir, int partNum) {
    return  PIPES_DIR + "/" + dir + "_part" + to_string(partNum);
}

string Utils::make_message(const vector<string>& args)
{
    string message = "";
    for (unsigned int i = 0; i < args.size(); i++)
        message += (args[i] + "#");
    return message;
}

string Utils::make_part_dir(string assetsDirectory, int num) {
    string partDir = assetsDirectory + "/part" + to_string(num) + ".csv";
    return partDir;
}

void Utils::wait(int num) {
    sleep(num);
}

void Utils::error(const char *msg) {
    perror(msg);
    exit(0);
}