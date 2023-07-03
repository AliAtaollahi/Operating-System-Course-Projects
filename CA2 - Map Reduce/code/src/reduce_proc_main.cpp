#include "../includes/reduce_proc.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    string dataLine = argv[0];
    ReduceProc rp = ReduceProc(dataLine);
    rp.startReduce();
    return 0;
}