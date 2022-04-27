#ifndef __INCLUDE_LYAPUNOV_HH__
#define __INCLUDE_LYAPUNOV_HH__

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_quit.h>
#include "../include/window.hh"

constexpr int     X0            = 0.5;
constexpr int     NUM_OF_ITER   = 700;

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::to_string;

class Lyapunov: WindowManager
{
  private:
    vector<uint32_t> m_pixels;
    string m_sequence;
    SDL_Rect m_size;
    void generate_sequence();

  public:
    Lyapunov( uint window_width, uint window_height, 
              uint lyap_width, uint lyap_height);
    void update_pixels();
    void generate(float a_start, float a_end, float b_start, float b_end);
    void start_loop();
    void on_resized(uint new_width, uint new_height) override;
    void set_pixel_RGB(uint index, uint r, uint g, uint b);
    void set_pixel_HSV(uint index, float h, float s, float v);
};

#endif // __INCLUDE_LYAPUNOV_HH__
