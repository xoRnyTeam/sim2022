#include "gl_driver.h"

// #ifdef ASM_INLINE_GRAPH
// void gl_graph_init(unsigned width, unsigned height) {
//   asm(".byte 0x73, 0x00, 0x00, 0x01" : : "r"(width), "r"(height));
// }
// 
// void gl_set_pixel(unsigned x, unsigned y, unsigned rgba) {
//   asm(".byte 0x73, 0x00, 0x00, 0x02" : : "r"(x), "r"(y), "r"(rgba));
// }
// 
// int gl_window_is_open() {
//   int res;
//   asm(".byte 0x73, 0x00, 0x00, 0x03" : "=r"(res)); // result operand
//   return res;
// }
// 
// int gl_get_msecs() {
//   int res;
//   asm(".byte 0x73, 0x00, 0x00, 0x04" : "=r"(res));
//   return res;
// }
// 
// void flush() { asm(".byte 0x73, 0x00, 0x00, 0x05"); }
// 
// int gl_rand() {
//   int res;
//   asm(".byte 0x73, 0x00, 0x00, 0x06" : "=r"(res));
//   return res;
// }
// 
// void gl_clear() { asm(".byte 0x73, 0x00, 0x00, 0x07"); }
// #else

void gl_graph_init(unsigned width, unsigned height) { int a = width + height; }

void gl_set_pixel(unsigned x, unsigned y, unsigned rgba) { int a = x + y; }

int gl_window_is_open() {
  int res = 0xab;
  return res;
}

long long gl_get_msecs() {
  int res = 0xab;
  return res;
}

void gl_flush() { int a = 0xab; }

int gl_rand() {
  int res = 0xab;
  return res;
}

void gl_clear() { int a = 0xab; }
// #endif