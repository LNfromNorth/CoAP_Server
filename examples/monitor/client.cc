#include <stdlib.h>
#include <sys/socket.h>
#include <cstring>
#include <random>
#include <iostream>
#include <thread>

#include "coap.h"
#include "monitor.h"

static int times = 0;

void task(uint16_t port, int no) {
     // init sockaddr
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    // init sensor data
    sensor_t* data = (sensor_t*)malloc(sizeof(sensor_t));
    data->device = no;
    // init message

    COAPServer* cs = new COAPServer(port);
    while(true) {
        data->temperature = 35 + (rand() & 50) / 10;
        data->humidity = 80 + (rand() & 20) / 10;
        COAPMessage* msg = new COAPMessage();
        msg->make(COAPMessage::Type::CON, NULL, 0, COAPMessage::Code::POST, (uint16_t)6666, (uint8_t*)data, sizeof(sensor_t));
        msg->add_option(10, 4, (uint8_t*)"NULL");
        char* send_data = msg->format();
        cs->sendData(send_data, msg->get_size(), server);
        std::this_thread::sleep_for(std::chrono::seconds(6));
    }
}

int main() {
    std::vector<std::thread> threads;
    for(int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        threads.emplace_back(task, 9999 + i, i);
    }

    for(auto& thread: threads)
        thread.join();

    return 0;
}