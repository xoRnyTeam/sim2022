#include "SEngine.h"

// llvm-project/build/bin/clang -S -emit-llvm ../start.c --target=sim
// llvm-project/build/bin/llc --filetype=obj -march=sim start.ll

int main() {

    const int width = 524;
    const int heigth = 524;
    sEngine_init(width, heigth);

    int x = 0;
    int y = 0;
    while(sEngine_windowIsOpen())
    {
        put_pixel(x, y, 0x8000ffff);
        flush();
        x++;
        if(x == width) {
            x = 0;
            y++;
            if(y == heigth) {
                y = 0;
            }
        }
    }

    return 0;
}