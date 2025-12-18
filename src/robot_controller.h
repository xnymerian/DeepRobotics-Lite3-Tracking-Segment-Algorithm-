#pragma once
#include <atomic>

extern std::atomic<bool> show_segmentation;
extern std::atomic<bool> program_running;

void heartbeat_loop();
void command_listener();

