#include "CoAPMessage.h"

#include <iostream>

int main() {
    COAPMessage cm = COAPMessage();
    char* data = "hello world";
    cm.make(COAPMessage::Type::ACK, NULL, 0, COAPMessage::Code::EMPTY, 0x1111, (uint8_t*)data, 11);
    cm.print();
    char* ret = cm.format();
    int size = cm.get_size();
    for(int i = 0; i < size; i++) {
        printf("%c", ret[i]);
    }
    printf("\n");
    for(int i = 0; i < size; i++) {
        printf("%02x ", ret[i]);
    }
    printf("\n");
    return 0; 
}