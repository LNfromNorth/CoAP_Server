#include <vector>
#include <functional>
#include <chrono>

#include "CoAPServer.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();

COAPServer::COAPServer(uint16_t port) 
    : UDPServer(port) {
    fm = new FManager();
    msg_id = 0x1111;
}

void COAPServer::receive_handler(const char* data, ssize_t size, sockaddr_in client_addr) {
    COAPMessage msg_in = COAPMessage();
    std::vector<uint8_t> data_v;
    data_v.insert(data_v.end(), data, data + size);
    bool ret = msg_in.parse(data_v);
    if(!ret) {
        logger.log(ERROR, "parse failed");
<<<<<<< HEAD
        return;
    }
    if(msg_in.isACK()) {
        // logger.log(DEBUG, "receive new ack");
        ack_received(msg_in);
    } else {
        data_handler(msg_in, client_addr);
=======
        //set in new thread sendError(client_addr);
        return;
    }
    if(msg_in.isACK()) {
        logger.log(DEBUG, "is ack");
        ack_received(msg_in);
    } else {
        if(msg_in.isCon()) {
            logger.log(DEBUG, "is CON");
            sendACK(msg_in, client_addr);
        }
        // msg_in.print();
        data_handler(&msg_in);
>>>>>>> cb1eb1c268cddcedb00aba60888b603433849bbe
    }
}

bool COAPServer::sendACK(COAPMessage msg_in, sockaddr_in client_addr) {
    // logger.log(DEBUG, "send ack");
    COAPMessage msg_out = COAPMessage();
    msg_out.make(COAPMessage::Type::ACK, msg_in.get_token(), msg_in.get_tokenl(),
            COAPMessage::Code::EMPTY, msg_in.get_msgid(), NULL, 0);
<<<<<<< HEAD
=======
    logger.log(DEBUG, "============ ack ==============");
    msg_out.print();
>>>>>>> cb1eb1c268cddcedb00aba60888b603433849bbe
    char* ret_data = msg_out.format();
    sendData(ret_data, msg_out.get_size(), client_addr);
    return true;
}
<<<<<<< HEAD
=======

>>>>>>> cb1eb1c268cddcedb00aba60888b603433849bbe
bool COAPServer::sendMessage(COAPMessage msg_out, sockaddr_in client_addr) {
    char* ret_data = msg_out.format();
    sendData(ret_data, msg_out.get_size(), client_addr);
    return true;
}

void COAPServer::ack_received(COAPMessage msg_in) {
    // logger.log(DEBUG, "ack received handler");
    char out[100];
    sprintf(out, "release timeout msgid is %d", msg_in.get_msgid());
    // logger.log(DEBUG, out);
    fm->set_f(msg_in.get_msgid());
}


bool COAPServer::sendMessage_with_timeout(COAPMessage msg_out, sockaddr_in client_addr) {
    int timeout_ms = 1000;
    int index = fm->alloc_f(msg_out.get_msgid());
    // logger.log(DEBUG, "add time ack");
    // sendMessage(msg_out, client_addr);
    enqueue([this, index, msg_out, client_addr, timeout_ms]() {
        for(int i = 0; i < 5; i++) {
            sendMessage(msg_out, client_addr);
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
<<<<<<< HEAD
            // check
=======
            logger.log(DEBUG, "time out");
>>>>>>> cb1eb1c268cddcedb00aba60888b603433849bbe
            if(fm->check_f(index) == false) {
                // logger.log(DEBUG, "finish time ack");
                fm->delete_f(index);
                return 0;
            }
            logger.log(ERROR, "time out");
        }
        return 1;
    });
    return 0;
}


<<<<<<< HEAD
void COAPServer::data_handler(COAPMessage msg, sockaddr_in client_addr) {
    // logger.log(DEBUG, "data handler");
}
=======
void COAPServer::data_handler(void* arg) {
    logger.log(DEBUG, "data handler");
}
>>>>>>> cb1eb1c268cddcedb00aba60888b603433849bbe
