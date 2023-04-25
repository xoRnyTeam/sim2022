#include "SEngine.h"

#ifdef ASM_INLINE_GRAPH
void sEngine_init(unsigned width, unsigned height) {
    asm(".byte 0x73, 0x00, 0x00, 0x01"
    : /*without output*/
    :"r" (width), "r" (height)); // input
}

void put_pixel(unsigned x, unsigned y, unsigned rgba) {
    asm(".byte 0x73, 0x00, 0x00, 0x02"
    : /*without output*/
    :"r" (x), "r" (y), "r" (rgba));
}

int sEngine_windowIsOpen() {
    int res;
    asm(".byte 0x73, 0x00, 0x00, 0x03"
    :"=r" (res)); // result operand
    return res;
}

int get_time_milliseconds() {
    int res;
    asm(".byte 0x73, 0x00, 0x00, 0x04"
    :"=r" (res));
    return res;
}

void flush() {
    asm(".byte 0x73, 0x00, 0x00, 0x05");
}

int sEngine_rand() {
    int res;
    asm(".byte 0x73, 0x00, 0x00, 0x06"
    :"=r" (res));
    return res;
}

void sEngine_clear() {
    asm(".byte 0x73, 0x00, 0x00, 0x07");
}
#else
void sEngine_init(unsigned width, unsigned height) {
    int a = width + height;
}

void put_pixel(unsigned x, unsigned y, unsigned rgba) {
    int a = x + y;
}

int sEngine_windowIsOpen() {
    int res = 0xab;
    return res;
}

int get_time_milliseconds() {
    int res = 0xab;
    return res;
}

void flush() {
    int a = 0xab;
}

int sEngine_rand() {
    int res = 0xab;
    return res;
}

void sEngine_clear() {
    int a = 0xab;
}
#endif