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
#include <stack>
#include "window.hh"
#include "time.hh"
#include "region.hh"
#include "menu.hh"

constexpr int X0            = 0.5;
constexpr int NUM_OF_ITER   = 700;

using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::array;
using std::thread;
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::to_string;
using std::domain_error;

class Lyapunov: WindowManager
{
  private:
    vector<double> m_exponents;
    string m_sequence;
    SDL_Rect m_size;
    Region m_current_region{0, 4, 0, 4};
    int m_precision{100};
    double max_expo;
    double min_expo;
    long m_last_move{get_current_time()};
    int m_current_color{0};
    bool m_stop_color{false};
    stack<Region> m_last_position;
    uint32_t m_color_lyap[4];
    int color_scale[6];
    void generate_sequence();

  public:
    Lyapunov( uint window_width, uint window_height, 
              uint lyap_width, uint lyap_height);
    void generate(Region region = {0, 4, 0, 4});
    void generate_part(uint x_start, uint y_start, uint x_end, uint y_end);
    Region get_region(int from_x, int to_x, int from_y, int to_y);
    void on_resized(uint new_width, uint new_height) override;
    void set_pixel_RGB(vector<uint32_t>& pixels, uint index, uint r, uint g, uint b);
    void set_pixel_HSV(vector<uint32_t>& pixels, uint index, int h, double s, double v);
    void set_color_scale(int tab, uint32_t max, uint32_t min);
    void update_pixels();
    void on_mouse_click(uint x, uint y, uint button) override;
    void on_mouse_move(uint x, uint y) override;
    void on_mouse_wheel() override;
    void on_keyboard_up(int c) override;
    void on_keyboard_down(int c) override;
    void on_tick() override;
    void start_loop();
};

#endif // __INCLUDE_LYAPUNOV_HH__
