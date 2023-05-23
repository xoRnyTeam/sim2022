#include "gl_driver.h"

int main() {

  const int width = 600;
  const int heigth = 800;
  gl_graph_init(width, heigth);

  int x = 0;
  int y = 0;
  //
  while (gl_window_is_open()) {
    gl_set_pixel(x, y, 0xFF0000FF);
    gl_flush();
    y++;
    if (y == heigth) {
      y = 0;
      x++;
      if (x == width) {
        x = 0;
      }
    }
  }

  return 0;
}