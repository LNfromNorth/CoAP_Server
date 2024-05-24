#include "coap.h"
#include "stdio.h"
#include "thread"

static Logger& logger = Logger::get_instance();



class MyServer: public COAPServer {
public:
    MyServer(): COAPServer(8888) {};
    virtual void data_handler(COAPMessage msg, sockaddr_in client_addr) override {
        char out[1024];
        sprintf(out, "[Receive] [From -> %s:%d] { Message ID: %d Type: %s }", 
        inet_ntoa(client_addr.sin_addr), client_addr.sin_port,
        msg.get_msgid(), msg.get_code2string());
        logger.log(INFO, out);



        if(msg.isCon()) {
            sprintf(out, "[Send   ] [To   -> %s:%d] { Message ID: %d Type: ACK }", 
            inet_ntoa(client_addr.sin_addr), client_addr.sin_port,
            msg.get_msgid());
            logger.log(INFO, out);
            sendACK(msg, client_addr);
        }
    }

    
private:

};

int main() {
    MyServer ms = MyServer();
    ms.run();
    return 0;
}