#ifndef __INCLUDE_LYAPUNOV_LYAPUNOV_HH__
#define __INCLUDE_LYAPUNOV_LYAPUNOV_HH__

#include <SDL2/SDL_quit.h>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <stack>
#include "window/window.hh"
#include "time/time.hh"
#include "region/region.hh"
#include "menu/menu.hh"

const double X0 = 0.5;

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::thread;
using std::stack;
using std::ifstream;

class Lyapunov : WindowManager
{
  private:
    vector<double> m_exponents;
    string m_sequence;
    SDL_Rect m_size;
    Region m_current_region{0, 4, 0, 4};
    int m_precision{100};
    double m_max_expo{};
    double m_min_expo{};
    long m_last_move{get_current_time()};
    stack<Region> m_last_position;
    Uint32 m_color_lyap[4]{};
    int m_color_scale[12]{};
    int m_zoom_precision = 400;
    void generate_sequence();

  public:
    Lyapunov();
    void generate(Region region = {0, 4, 0, 4});
    void generate_part(int x_start, int y_start, int x_end, int y_end);
    Region get_region(int from_x, int to_x, int from_y, int to_y) const;
    void set_color_scale(int tab, int max, int min);
    void draw_zoom();
    void validate_region(int& x, int& y, int& w, int& h);
    void update_pixels() const ;
    void update_settings();
    void on_resized(uint new_width, uint new_height) override;
    void on_mouse_click(int mouse_x, int mouse_y, int button) override;
    void on_mouse_move() override;
    void on_mouse_wheel(int amount) override;
    void on_keyboard_down(int c) override;
    void start_loop();
};

#endif // __INCLUDE_LYAPUNOV_LYAPUNOV_HH__
