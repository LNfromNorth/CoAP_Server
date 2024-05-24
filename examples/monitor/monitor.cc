#include "coap.h"
#include "monitor.h"
#include <stdio.h>
#include <string>

static Logger& logger = Logger::get_instance();

class MyServer: public COAPServer {
public:
    MyServer():COAPServer(8888) {
        printf("init myserver\n");
        rs = ResourceManager();
    }

    virtual void data_handler(COAPMessage msg, sockaddr_in client_addr) {
        // msg_in
        COAPMessage::Code request = msg.get_code();
        std::string path = msg.get_uri_path();
        char* payload = msg.get_data();
        ssize_t payload_size = msg.get_data_size();
        std::string data_str(payload, payload_size);
        std::string ret_msg;
        int ret;

        // msg.print();
        // msg_out
        COAPMessage msg_out = COAPMessage();
        std::string request_str;
        if(request == COAPMessage::Code::GET) {
            ret_msg = rs.getResource(path);
            request_str = "GET";
            msg_out.make(COAPMessage::Type::ACK, msg.get_token(), msg.get_tokenl(),
                        COAPMessage::Code::Valid, msg.get_msgid(), (uint8_t*)ret_msg.c_str(), 
                        ret_msg.length());
        } else if(request == COAPMessage::Code::POST) {
            rs.postResource(path, data_str);
            request_str = "POST";
            msg_out.make(COAPMessage::Type::ACK, msg.get_token(), msg.get_tokenl(),
                        COAPMessage::Code::Created, msg.get_msgid(), NULL, 0);
        } else if(request == COAPMessage::Code::PUT) {
            ret = rs.putResource(path, data_str);
            request_str = "PUT";
            COAPMessage::Code ret_code = ret == 1 ? COAPMessage::Code::Changed : COAPMessage::Code::Bad_Request;
            msg_out.make(COAPMessage::Type::ACK, msg.get_token(), msg.get_tokenl(),
                        ret_code, msg.get_msgid(), NULL, 0);
        } else if(request == COAPMessage::Code::DELETE) {
            ret = rs.deleteResource(path);
            request_str = "DELETE";
            COAPMessage::Code ret_code = ret == 1 ? COAPMessage::Code::Delete : COAPMessage::Code::Bad_Request;
            msg_out.make(COAPMessage::Type::ACK, msg.get_token(), msg.get_tokenl(),
                        ret_code, msg.get_msgid(), NULL, 0);
        }
        char out[512];
        sprintf(out, "[Receive from %s:%d][Request: %s]", 
        inet_ntoa(client_addr.sin_addr), client_addr.sin_port, request_str.c_str());
        logger.log(INFO, out);
        sendMessage(msg_out, client_addr);
    }
private:
    ResourceManager rs;
};

int main(int argc, char** argv) {
    MyServer* ms = new MyServer();
    ms->run();
    return 0;
}
