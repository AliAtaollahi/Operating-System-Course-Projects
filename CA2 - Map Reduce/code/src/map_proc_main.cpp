#include "../includes/map_proc.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    string dataLine = argv[0];
    MapProc mp = MapProc(dataLine);
    mp.startMap();
    return 0;

}