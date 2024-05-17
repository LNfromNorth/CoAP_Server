#include "coap.h"
#include <cstring>

static Logger& logger = Logger::get_instance();

int main() {

    logger.log(DEBUG, "=========== init socket ============");
    COAPServer cs = COAPServer(9999);
    struct sockaddr_in si;
    memset(&si, 0, sizeof(struct sockaddr_in));
    si.sin_family = AF_INET;
    si.sin_port = htons(8888);
    si.sin_addr.s_addr = inet_addr("127.0.0.1");

    COAPMessage msg1 = COAPMessage();
    char* payload = (char*)"hello";
    msg1.make(COAPMessage::Type::NON, NULL, 0, 
            COAPMessage::Code::POST, cs.get_msgid(),
            (uint8_t*)payload, 5);
//     char* msg1_format = (char*)msg1.format();

    char out_id[100];

    logger.log(DEBUG, "=========== send NON ===============");
    sprintf(out_id, "send message id is %d", msg1.get_msgid());
    logger.log(DEBUG, out_id);
    cs.sendMessage(msg1, si);

    COAPMessage msg = COAPMessage();
    msg.make(COAPMessage::Type::CON, NULL, 0, 
            COAPMessage::Code::POST, cs.get_msgid(),
            (uint8_t*)payload, 5);
//     char* msg_format = (char*)msg.format();

    logger.log(DEBUG, "=========== send CON ===============");
    sprintf(out_id, "send message id is %d", msg.get_msgid());
    logger.log(DEBUG, out_id);
    cs.sendMessage_with_timeout(msg, si);
    cs.run();

    return 0;
}