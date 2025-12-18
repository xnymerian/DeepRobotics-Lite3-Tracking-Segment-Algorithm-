#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct CommandHead {
    uint32_t code;           
    uint32_t parameters_size; 
    uint32_t type;           
};
#pragma pack(pop)
