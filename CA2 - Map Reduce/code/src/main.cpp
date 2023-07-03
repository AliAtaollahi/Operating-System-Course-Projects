#include "../includes/system.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    string assetsDirectory = argv[1];
    System sys = System(assetsDirectory);
    sys.start_program();
    return 0;
}