#include <stdlib.h>
#include <sys/socket.h>
#include <cstring>
#include <random>
#include <iostream>

#include "coap.h"
#include "monitor.h"

static int times = 0;

int main() {
    // init sockaddr
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    // init sensor data
    sensor_t* data = (sensor_t*)malloc(sizeof(sensor_t));
    // init message

    COAPServer* cs = new COAPServer(1, 9999);
    while(true) {
        data->temperature = rand() & 0xffffffff;
        data->humidity = rand() & 0xffffffff;
        COAPMessage* msg = new COAPMessage();
        msg->make(COAPMessage::Type::CON, NULL, 0, COAPMessage::Code::POST, (uint16_t)6666, (uint8_t*)data, sizeof(sensor_t));
        msg->add_option(10, 4, (uint8_t*)"NULL");
        char* send_data = msg->format();
        printf("%d, %d\n", data->temperature, data->humidity);
        cs->sendData(send_data, msg->get_size(), server);
        printf("times = %d\n", times++);
        usleep(100);
    }
    return 0;
}