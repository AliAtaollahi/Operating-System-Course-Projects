#include "../includes/reduce_proc.hpp"

using namespace std;

ReduceProc::ReduceProc(string dataLine_) {
    dataLine = dataLine_;
}

void ReduceProc::startReduce() {

    int sum = 0, fd;
    string dir, pipeNamed;
    string procGener = Utils::parse_line(dataLine,'#');
    int partCount = stoi(Utils::parse_line(dataLine,'#'));
    char buff[MAX_SIZE_STR];
    
    for (int i = 0; i < partCount; i++) {
        dir = Utils::make_pipe_dir(procGener, i + 1);
        fd = open(&dir[0], O_RDONLY);
        buff[MAX_SIZE_STR] = {0};
        read(fd, buff, MAX_SIZE_STR);
        sum += stoi(buff);
        unlink(dir.c_str());
        close(fd);
    }

    cout << "Total number of " << procGener << " : " << sum << endl;
}

