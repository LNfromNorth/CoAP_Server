#include "CoAPServer.h"
#include "coap.h"

class MyServer: public COAPServer {
public:
    MyServer(): COAPServer(8888) {}
    virtual void data_handler(void* arg) override {
        COAPMessage* msg = (COAPMessage*)arg;
        logger.log(DEBUG, "receive new message");
        printf("message id is %d\n", msg->get_msgid());
        return;
    }
};

int main() {
    MyServer ms = MyServer();
    ms.run();
    return 0;
}