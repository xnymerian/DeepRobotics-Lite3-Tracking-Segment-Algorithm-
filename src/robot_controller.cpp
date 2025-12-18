#include "robot_controller.h"
#include "network.h"
#include "config.h"
#include <thread>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

std::atomic<bool> show_segmentation(false);
std::atomic<bool> program_running(true);

void heartbeat_loop() {
    printf(">>> HEARTBEAT ACTIVE (100ms)\n");
    while (program_running) {
        send_udp_packet(0x21010200, 30005, NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void command_listener() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) return;
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(CMD_PORT);
    
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) return;
    
    socklen_t len = sizeof(cliaddr);
    while (program_running) {
        int n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        
        if (buffer[0] == '1') {
            show_segmentation = true;
            printf("\n>>> TRACKING MODE STARTING...\n");
            send_udp_packet(0x21010C03, 0, "Navigation Mode");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            send_udp_packet(0x21010D05, 0, "Pose Mode"); 
            printf(">>> ROBOT READY (NAV+POSE)\n");
            
        } else if (buffer[0] == '0') {
            show_segmentation = false;
            send_udp_packet(0x21010135, 0, "Yaw Zero"); 
            send_udp_packet(0x21010130, 0, "Pitch Zero"); 
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            send_udp_packet(0x21010D06, 0, "Move Mode");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            send_udp_packet(0x21010C02, 0, "Manual Mode");
            printf(">>> ROBOT STOPPED\n");
        }
    }
}

