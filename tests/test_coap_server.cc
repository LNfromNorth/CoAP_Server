#include "CoAPServer.h"
#include "coap.h"

class MyServer: public COAPServer {
public:
    MyServer(): COAPServer(8888) {}
    virtual void data_handler(COAPMessage msg, sockaddr_in client_addr) override {
        logger.log(DEBUG, "receive new message");
        char out[100];
        if(msg.isCon()) {
            sprintf(out, "message type is CON");
        } else {
            sprintf(out, "message type is NON");
        }
        logger.log(DEBUG, out);
        sprintf(out, "message id is %d", msg.get_msgid());
        logger.log(DEBUG, out);
        if(msg.isCon()) {
            sendACK(msg, client_addr);
        }
        return;
    }
};

int main() {
    MyServer ms = MyServer();
    ms.run();
    return 0;
}