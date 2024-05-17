#include <stdlib.h>
#include <iostream>

#include "CoAPMessage.h"

int main() {
    std::vector<uint8_t> buffer = {0x40, 0x01, 0x12, 0x34, 0x45, 0x63, 0x81, 0x65, 0x6c, 0x65, 0xff, 0x6d, 0x6f, 0x6e, 0x21, 0x77, 0x6f, 0x72, 0x6c, 0x64};
    COAPMessage coap_message;
    printf("--------- message in --------------\n");
    for(int i = 0; i < (int)buffer.size(); i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    printf("--------- test parse --------------\n");
    if(coap_message.parse(buffer)) {
        coap_message.print();
    } else {
        std::cout << "error" << std::endl;
    }
    printf("--------- test func --------------\n");
    printf("is ack : %d\n", coap_message.isACK());
    printf("is con : %d\n", coap_message.isCon());
    printf("--------- test format ------------\n");
    char* format = coap_message.format();
    for(int i = 0; i < coap_message.get_size(); i++)
        printf("%02x ", (uint8_t)format[i]);
    printf("\n");
    printf("--------- test match -------------\n");
    for(int i = 0; i < (int)sizeof(format); i++) {
        if((uint8_t)buffer[i] != (uint8_t)format[i])  {
            printf("not match!\n");
            return 0;
        }
    }
    printf("match!\n");
    return 0;
}