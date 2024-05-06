#include <errno.h>
#include <string.h>
#include <iostream>

#include "UDPServer.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();


UDPServer::UDPServer(uint16_t port) : ThreadPool(4) {
    if((udp_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        logger.log(ERROR, "socket init error");
        exit(1);
    }
    udp_server_addr.sin_family = PF_INET;
    udp_server_addr.sin_port = htons(port);
    udp_server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(udp_fd, (struct sockaddr*)&udp_server_addr, sizeof(struct sockaddr)) == -1) {
        logger.log(ERROR, "socket bind error");
        exit(1);
    }
    epoll_udp_fd = epoll_create1(0);
    event.events = EPOLLIN;
    event.data.fd = udp_fd;
    if(epoll_ctl(epoll_udp_fd, EPOLL_CTL_ADD, udp_fd, &event) < 0) {
        logger.log(ERROR, "epoll set insertion error");
        exit(1);
    }
}

void UDPServer::receive_handler(const char* data, ssize_t size, sockaddr_in clinet_addr) {
    logger.log(DEBUG, "virtual function");
}

void UDPServer::run() {
    while(1) {
        int nfds = epoll_wait(epoll_udp_fd, events, MAXEPOLLSIZE, -1);
        if(nfds == -1) {
            logger.log(ERROR, "epoll wait error");
            break;
        }
        for(int n = 0; n < nfds; ++n) {
            int fd = events[n].data.fd;
            enqueue([this, fd]() {
                logger.log(DEBUG, "set in new thread");
                receiveData(fd);
            });
        }
    }
}

bool UDPServer::receiveData(int fd) {
    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    ssize_t bytes_received = recvfrom(fd, buffer, sizeof(buffer), 0,
                    (struct sockaddr*)&client_addr, &client_addr_len);
    if(bytes_received == -1) {
        logger.log(ERROR, "Failed to recvive data");
        return 0;
    } 
    receive_handler(buffer, bytes_received, client_addr);
    return 1;
}

bool UDPServer::sendData(const char* data, ssize_t size, sockaddr_in client_addr) {
    ssize_t bytes_send = sendto(udp_fd, data, size, 0,
                        (struct sockaddr*)&client_addr, sizeof(client_addr));
    if(bytes_send == -1) {
        logger.log(ERROR, "Failed to send data");
        return false;
    }
    return true;
}