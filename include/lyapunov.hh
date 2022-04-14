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

const int     X0            = 0.5;
const int     WIDTH         = 700;
const int     HEIGHT        = 700;
const int     NUM_OF_ITER   = 700;
const double  INF_A         = 0.0;
const double  SUP_A         = 4.0;
const double  INF_B         = 0.0;
const double  SUP_B         = 4.0;

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::to_string;

class Lyapunov
{
  private:
    WindowManager window_manager;
    vector<uint32_t> pixels;

  public:
    Lyapunov();
    void set_pixel(int index, unsigned int r, unsigned int g, unsigned int b);
    void set_many_pixels(vector<uint32_t> pixels);
    void event_loop();
};

#endif // __INCLUDE_LYAPUNOV_HH__
