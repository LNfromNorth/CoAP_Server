#ifndef __COAP_SERVER_COAP_SERVER__
#define __COAP_SERVER_COAP_SERVER__

#include <functional>

#include "UDPServer.h"
#include "CoAPMessage.h"
#include "Threadpool.h"

class COAPServer : public UDPServer {
using TaskType = std::function<void(void*)>;
public:
    COAPServer(int max_thread, uint16_t port);
    ~COAPServer() {}

    bool sendACK(COAPMessage msg_in, sockaddr_in client_addr);
    bool sendMessage();
protected:
    virtual void receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) override;
    virtual void data_handler(void* arg);
private:
    ThreadPool* tp;
};

#endif
