#include <vector>
#include <functional>

#include "CoAPServer.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();

COAPServer::COAPServer(int max_thread) {
    tp = new ThreadPool(1, max_thread, 5);
}

void COAPServer::receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) {
    COAPMessage msg_in = COAPMessage();
    std::vector<uint8_t> data_v;
    data_v.insert(data_v.end(), data, data + size);
    bool ret = msg_in.parse(data_v);
    if(!ret) {
        logger.log(ERROR, "parse failed");
    }
    if(msg_in.isCon()) {
        sendACK(msg_in, client_addr);
    }
    tp->AddTask(std::bind(&COAPServer::data_handler, this, static_cast<void*>(&msg_in)), NULL);
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