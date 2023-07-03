#include "../includes/dashboard.h"
#include "time.h"
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    srand(time(NULL));
    dashboard(argv);
    return 0;
}