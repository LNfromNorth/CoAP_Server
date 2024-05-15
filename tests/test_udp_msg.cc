#include "UDPServer.h"
#include "CoAPMessage.h"
#include "Log.h"

#include <vector>
#include <cstring>
#include <stdlib.h>

// static Logger& logger = Logger::get_instance();

class MyUDPServer : public UDPServer{
public:
    MyUDPServer() : UDPServer(8888) {}
protected:
    virtual void receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) {
        std::cout << "Received data from: " << inet_ntoa(client_addr.sin_addr)
                << ": " << data << std::endl;
        std::string response = "hello, client";

        for(int i = 0; i < size; i++) {
            printf("0x%02x  ", data[i]);
        }

        std::vector<uint8_t>data_v;
        data_v.insert(data_v.end(), data, data + strlen(data));

        COAPMessage coap_message;
        if(coap_message.parse(data_v)) {
            coap_message.print();
        } else {
            logger.log(DEBUG, "parse error");
        }
        sendData(response.c_str(), response.length(), client_addr);
    }
};

int main() {
    MyUDPServer mus = MyUDPServer();
    mus.run();
    return 0;
}