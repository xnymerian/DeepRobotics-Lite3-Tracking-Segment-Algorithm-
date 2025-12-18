#include "model_loader.h"
#include <cstdio>
#include <cstdlib>

typedef int (*rga_init_func)();
static void* rga_handle = nullptr;

bool init_rga() {
    if (rga_handle) return true;
    rga_handle = dlopen("./librga.so", RTLD_LAZY);
    if (!rga_handle) rga_handle = dlopen("librga.so", RTLD_LAZY);
    if (rga_handle) {
        auto init_ptr = (rga_init_func)dlsym(rga_handle, "c_RkRgaInit");
        if (!init_ptr) init_ptr = (rga_init_func)dlsym(rga_handle, "RkRgaInit");
        if (init_ptr && init_ptr() == 0) return true;
    }
    return false;
}

unsigned char* load_model(const char* filename, int* model_size) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return nullptr;
    fseek(fp, 0, SEEK_END); 
    *model_size = ftell(fp); 
    fseek(fp, 0, SEEK_SET);
    unsigned char* data = (unsigned char*)malloc(*model_size);
    if(fread(data, 1, *model_size, fp) != (size_t)*model_size) { 
        free(data); 
        fclose(fp); 
        return nullptr; 
    }
    fclose(fp);
    return data;
}
