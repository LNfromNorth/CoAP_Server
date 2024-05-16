#include "coap.h"
#include <cstring>

int main() {
    COAPServer cs = COAPServer(9999);

    struct sockaddr_in si;
    memset(&si, 0, sizeof(struct sockaddr_in));
    si.sin_family = AF_INET;
    si.sin_port = htons(8888);
    si.sin_addr.s_addr = inet_addr("127.0.0.1");

    int udp_fd = socket(PF_INET, SOCK_DGRAM, 0);
    bind(udp_fd, (struct sockaddr*)&si, sizeof(struct sockaddr));

    COAPMessage msg = COAPMessage();
    char* payload = "hello";
    msg.make(COAPMessage::Type::CON, NULL, 0, 
            COAPMessage::Code::POST, cs.get_msgid(),
            (uint8_t*)payload, 5);
    char* msg_format = msg.format();

    printf("send message id is %d\n", msg.get_msgid());
    cs.sendMessage_with_timeout(msg, si);
    cs.run();

    return 0;
}