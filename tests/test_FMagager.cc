#include "FManager.h"
#include <iostream>

int main() {
    FManager* fm = new FManager();
    int index = fm->alloc_f(0x0011);
    std::cout << index << std::endl;
    std::cout << fm->check_f(index) << std::endl;
    fm->set_f(0x0011);
    std::cout << fm->check_f(index) << std::endl;
    fm->delete_f(index);
    return 0;
}