#include "udp_server.h"
#include <iostream>

class MyUDPServer : public udp_server {
protected:
    virtual void receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) {
        std::cout << "Received data from" << inet_ntoa(client_addr.sin_addr)
                << ": " << data << std::endl;
        std::string response = "hello, client";
        sendData(response.c_str(), response.length(), client_addr);
    }
};

int main() {
    MyUDPServer mus = MyUDPServer();
    mus.run(); 
    return 0;
}