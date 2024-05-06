#include <vector>
#include <functional>

#include "CoAPServer.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();

COAPServer::COAPServer(uint16_t port) 
    : UDPServer(port) {
}

void COAPServer::receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) {
    COAPMessage msg_in = COAPMessage();
    std::vector<uint8_t> data_v;
    data_v.insert(data_v.end(), data, data + size);
    bool ret = msg_in.parse(data_v);
    if(!ret) {
        logger.log(ERROR, "parse failed");
        // return;
    }
    if(msg_in.isCon()) {
        sendACK(msg_in, client_addr);
    }
    msg_in.print();
    // tp->AddTask(std::bind(&COAPServer::data_handler, this, &msg_in), NULL);
    // tp->AddTask(std::bind(&COAPServer::data_handler, this, &msg_in), &msg_in);
    data_handler(&msg_in);
}

bool COAPServer::sendACK(COAPMessage msg_in, sockaddr_in client_addr) {
    COAPMessage msg_out = COAPMessage();
    msg_out.make(COAPMessage::Type::ACK, msg_in.get_token(), msg_in.get_tokenl(),
            COAPMessage::Code::EMPTY, msg_in.get_msgid(), NULL, 0);
    char* ret_data = msg_out.format();
    sendData(ret_data, msg_out.get_size(), client_addr);
    return true;
}

void COAPServer::data_handler(void* arg) {
    logger.log(DEBUG, "data handler");
}