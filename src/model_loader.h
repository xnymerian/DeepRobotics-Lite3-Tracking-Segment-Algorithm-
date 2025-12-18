#pragma once
#include <dlfcn.h>

bool init_rga();
unsigned char* load_model(const char* filename, int* model_size);
