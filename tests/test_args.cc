#include "Args.h"
#include <iostream>

int main(int argc, char** argv) {
    Args* args = new Args(argc, argv);
    printf("init flag %d\n", args->get_init_flag());
    printf("thread pool num %d\n", args->get_tp_num());
    return 0;
}