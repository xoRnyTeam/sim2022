#include <stdint.h>

/**
 * @brief create window
 * 
 * @param width - width of the window in pixels
 * @param height - height of the window in pixels
 */
extern "C" void sEngine_init(unsigned width, unsigned height);

/**
 * @brief Checks the window events and closes it if necessary
 * 
 * @return true - window is open
 * @return false - window was closed
 * 
 * @note without a call sEngine_init the behavior is undefined
 */
extern "C" int sEngine_windowIsOpen();

/**
 * @brief returns the time since the start of the program
 * 
 */
extern "C" long long get_time_milliseconds();


extern "C" void put_pixel(unsigned x, unsigned y, unsigned rgba);

extern "C" void flush();

// DLC
extern "C" int sEngine_rand();
extern "C" void sEngine_clear();