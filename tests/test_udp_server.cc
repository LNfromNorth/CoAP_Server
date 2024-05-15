#include "coap.h"
#include <iostream>

class MyUDPServer : public UDPServer {
public:
    MyUDPServer() 
        :UDPServer(8888) {}
protected:
    virtual void receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) {
        logger.log(DEBUG, "    ");
        std::cout << "Received data from " << inet_ntoa(client_addr.sin_addr)
                << ": " << data;
        std::string response = "hello, client";
        logger.log(DEBUG, "send 'hello client' to client");
        sendData(response.c_str(), response.length(), client_addr);
    }
};

int main() {
    MyUDPServer* mus = new MyUDPServer();
    mus->run(); 
    return 0;
}
