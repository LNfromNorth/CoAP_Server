#include "CoAPMessage.h"

#include <iostream>

int main() {
    COAPMessage cm = COAPMessage();
    char* data = (char*)"hello world";
    char* data2 = (char*)"hello nomes";
    char* data3 = (char*)"hello nomes hello hello hello hello hello";
    cm.make(COAPMessage::Type::ACK, NULL, 0, COAPMessage::Code::EMPTY, 0x1111, (uint8_t*)data, 11);
    cm.add_option(16, 11, (uint8_t*)data);
    cm.add_option(1, 11, (uint8_t*)data2);
    cm.add_option(301, 41, (uint8_t*)data3);
    cm.print();
    char* ret = cm.format();
    int size = cm.get_size();
    printf("size is %d\n", size);
    for(int i = 0; i < size; i++) {
        printf("%02hx ", (uint8_t)ret[i]);
    }
    printf("\n");
    return 0; 
}