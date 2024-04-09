#include <stdlib.h>
#include <iostream>

#include "CoAPMessage.h"

int main() {
    std::vector<uint8_t> buffer = {0x40, 0x01, 0x12, 0x34, 0x45, 0x63, 0x81, 0x65, 0x6c, 0x65, 0xff, 0x6d, 0x6f, 0x6e, 0x21, 0x77, 0x6f, 0x72, 0x6c, 0x64};
    COAPMessage coap_message;
    if(coap_message.parse(buffer)) {
        coap_message.print();
    } else {
        std::cout << "error" << std::endl;
    }
    return 0;
}