#include "gl/gl.hpp"

#include <SFML/Graphics.hpp>

#include <chrono>
#include <memory>
#include <vector>

namespace graph_impl {
//! NOTE: imagine here SfmlImpl class realziation
static sf::RenderWindow window;
static sf::Image buffer;
static sf::Texture texture;
static sf::Sprite sprite;
//
} // namespace graph_impl

std::chrono::time_point<std::chrono::high_resolution_clock> g_timer =
    std::chrono::high_resolution_clock::now();

uint8_t unpack_4ui8_r(uint32_t rgba) { return (rgba & 0xFF000000) >> 24; }
uint8_t unpack_4ui8_g(uint32_t rgba) { return (rgba & 0x00FF0000) >> 16; }
uint8_t unpack_4ui8_b(uint32_t rgba) { return (rgba & 0x0000FF00) >> 8; }
uint8_t unpack_4ui8_a(uint32_t rgba) { return (rgba & 0x000000FF); }

extern "C" void gl_graph_init(uint32_t width, uint32_t heigth) {
  uint32_t h = AppParams::APP_HEIGHT, w = AppParams::APP_WIDTH;
  //
  graph_impl::window.create(sf::VideoMode(w, h), "Game of Life");
  graph_impl::buffer.create(w, h, sf::Color(BLACK));
}

extern "C" int gl_window_is_open() {
  sf::Event event;
  while (graph_impl::window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      graph_impl::window.close();
  }
  return graph_impl::window.isOpen();
}

extern "C" void gl_set_pixel(uint32_t x, uint32_t y, uint32_t rgba) {
  sf::Color color{unpack_4ui8_r(rgba), unpack_4ui8_g(rgba), unpack_4ui8_b(rgba),
                  unpack_4ui8_a(rgba)};
  graph_impl::buffer.setPixel(x, y, color);
}

extern "C" void gl_flush() {
  sf::Texture texture;
  texture.loadFromImage(graph_impl::buffer);

  sf::Sprite sprite(texture);

  graph_impl::window.clear();
  graph_impl::window.draw(sprite);
  graph_impl::window.display();
}

extern "C" long long gl_get_msecs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() - g_timer)
      .count();
}

// DLC
extern "C" int gl_rand() { return rand(); }

extern "C" void gl_clear() { graph_impl::window.clear(sf::Color::Black); }