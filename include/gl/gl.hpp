#include <stdint.h>

#define EPS 0.1

#define GREEN 0, 255, 0
#define BLACK 0, 0, 0

#define RECT_SIZE 1.0, 1.0

enum AppParams { APP_WIDTH = 800, APP_HEIGHT = 800 };

/**
 * 
 * @brief create window
 *
 * @param width - width of the window in pixels
 * @param height - height of the window in pixels
 */
extern "C" void gl_graph_init(unsigned width, unsigned height);

/**
 * @brief Checks the window events and closes it if necessary
 *
 * @return true - window is open
 * @return false - window was closed
 *
 * @note without a call sEngine_init the behavior is undefined
 */
extern "C" int gl_window_is_open();

/**
 * @brief returns the time since the start of the program
 *
 */
extern "C" long long gl_get_msecs();

extern "C" void gl_set_pixel(unsigned x, unsigned y, unsigned rgba);

extern "C" void gl_flush();

// DLC
extern "C" int gl_rand();
extern "C" void gl_clear();