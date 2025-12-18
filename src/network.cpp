#include "network.h"
#include "config.h"
#include "protocol.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

int get_motion_socket() {
    static int sockfd = -1;
    if (sockfd < 0) {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Socket Error");
            return -1;
        }
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; 
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }
    return sockfd;
}

void send_udp_packet(uint32_t code, int32_t value, const char* msg_desc) {
    int sockfd = get_motion_socket();
    if (sockfd < 0) return;
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MOTION_PORT);
    servaddr.sin_addr.s_addr = inet_addr(ROBOT_MOTION_IP);
    
    CommandHead cmd;
    cmd.code = code;
    cmd.parameters_size = (uint32_t)value; 
    cmd.type = 0; 
    sendto(sockfd, &cmd, sizeof(cmd), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
}
