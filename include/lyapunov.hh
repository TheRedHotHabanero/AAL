#ifndef __INCLUDE_LYAPUNOV_HH__
#define __INCLUDE_LYAPUNOV_HH__

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <ctime>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_quit.h>
#include "../include/window.hh"

constexpr int X0            = 0.5;
constexpr int NUM_OF_ITER   = 700;

using std::cin;
using std::cout;
using std::endl;
using std::array;
using std::thread;
using std::string;
using std::vector;
using std::ofstream;
using std::to_string;
using std::domain_error;

class Lyapunov: WindowManager
{
  private:
    vector<uint32_t> m_pixels;
    string m_sequence;
    SDL_Rect m_size;
    float m_a_start{0};
    float m_b_start{0};
    float m_a_end{4};
    float m_b_end{4};
    void generate_sequence();

  public:
    Lyapunov( uint window_width, uint window_height, 
              uint lyap_width, uint lyap_height);
    void generate(float a_start = 0, float b_start = 0, float a_end = 4, float b_end = 4);
    void generate_part(uint x_start, uint y_start, uint x_end, uint y_end);
    array<float, 2> get_coord(int x, int y);
    void on_resized(uint new_width, uint new_height) override;
    void set_pixel_RGB(uint index, uint r, uint g, uint b);
    void set_pixel_HSV(uint index, float h, float s, float v);
    void update_pixels();
    void on_mouse_click(uint x, uint y) override;
    void on_mouse_move(uint x, uint y) override;
    void start_loop();
};

#endif // __INCLUDE_LYAPUNOV_HH__
