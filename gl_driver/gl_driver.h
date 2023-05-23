/**
 * @brief 
 * 
 * @param[in] width 
 * @param[in] height 
 */
void gl_graph_init(unsigned width, unsigned height);

/**
 * @brief 
 * 
 * @return int 
 */
int gl_window_is_open();

/**
 * @brief Get the time milliseconds object
 * 
 * @return int 
 */
long long gl_get_msecs();

/**
 * @brief 
 * 
 * @param[in] x 
 * @param[in] y 
 * @param[in] rgba 
 */
void gl_set_pixel(unsigned x, unsigned y, unsigned rgba);

/**
 * @brief 
 * 
 */
void gl_flush();

int gl_rand();
void gl_clear();