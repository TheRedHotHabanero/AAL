#ifndef __INCLUDE_WINDOW_WINDOWMANAGER_HH__
#define __INCLUDE_WINDOW_WINDOWMANAGER_HH__

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include "time/time.hh"

using std::ostream;
using std::runtime_error;
using std::string;
using std::to_string;
using std::vector;

class WindowManager
{
  private:

    bool m_quit;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    SDL_Rect m_texture_position;
    int m_pitch;
    SDL_Rect m_mouse_position;

  public:

    WindowManager();
    void init_render(SDL_Rect size);
    void draw_rect(int x, int y, int w, int h);
    void update_texture(vector<Uint32>& pixels) const;
    void blit_texture() const;
    void update_screen() const;
    const SDL_Rect& get_mouse_position() const;
    int get_max_size() const;
    virtual void on_resized(uint new_width, uint new_height) = 0;
    virtual void on_mouse_click(int x, int y, int button) = 0;
    virtual void on_mouse_move() = 0;
    virtual void on_mouse_wheel(int amount) = 0;
    virtual void on_keyboard_down(int c) = 0;
    void event_loop();
    const SDL_Rect& get_texture_position() const;
    void set_texture_position(SDL_Rect texture_position);
    ~WindowManager();
};

ostream& operator<<(ostream& flux, SDL_Rect rect);

#endif // __INCLUDE_WINDOW_WINDOWMANAGER_HH__
