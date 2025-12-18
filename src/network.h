#pragma once
#include <cstdint>

int get_motion_socket();
void send_udp_packet(uint32_t code, int32_t value, const char* msg_desc);
