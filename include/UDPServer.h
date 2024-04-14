#ifndef __COAP_SERVER_UDP_SERVER_H__
#define __COAP_SERVER_UDP_SERVER_H__

#include <list>
#include <fstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define HEADSIZE     5
#define MAXEPOLLSIZE 20
#define PORT         8888
#define _PKG_INIT    0
#define _PKG_RECVING 1

class UDPServer{
public:
    UDPServer(uint16_t port);
    ~UDPServer() {close(udp_fd);}

    void run();
    bool sendData(const char* data, ssize_t size, sockaddr_in client_addr);
protected:
    virtual void receive_handler(const char* data, ssize_t size, sockaddr_in clinet_addr);
private:
    int udp_fd, epoll_udp_fd;
    socklen_t len;
    struct sockaddr_in udp_server_addr;
    struct epoll_event event;
    struct epoll_event events[MAXEPOLLSIZE];
};

#endif
