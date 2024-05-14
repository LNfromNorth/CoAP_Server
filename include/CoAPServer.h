#ifndef __COAP_SERVER_COAP_SERVER__
#define __COAP_SERVER_COAP_SERVER__

#include <functional>
#include <vector>

#include "UDPServer.h"
#include "CoAPMessage.h"
#include "FManager.h"

class COAPServer : public UDPServer {
public:
    COAPServer(uint16_t port);
    ~COAPServer() {}

    bool sendACK(COAPMessage msg_in, sockaddr_in client_addr);
<<<<<<< HEAD
    bool sendMessage(COAPMessage msg_in, sockaddr_in client_addr);
    bool sendMessage_with_timeout(COAPMessage msg_in, sockaddr_in client_addr);

    uint16_t get_msgid() { return msg_id++;}
=======
    bool sendError(sockaddr_in clinet_addr);
    bool sendMessage();
>>>>>>> 85b9951d9fa79e3efbe27b5ff3291211c857b71b
protected:
    virtual void receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) override;
    virtual void data_handler(void* arg);
private:
    void ack_received(COAPMessage msg_in);
    uint16_t msg_id;
    FManager* fm;
};

#endif
