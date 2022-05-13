#ifndef __SRC_WINDOW_HH__
#define __SRC_WINDOW_HH__

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include "time.hh"

using std::vector;
using std::string;
using std::to_string;
using std::runtime_error;
using std::ostream;
using std::cout;
using std::endl;

class WindowManager
{
  private:
    SDL_Rect m_window_position;
    bool m_quit;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_draw;
    SDL_Texture* m_texture;
    SDL_Rect m_texture_position;
    int m_pitch;
    SDL_Rect m_mouse_position;

  public:
    WindowManager();
    ~WindowManager();
    void init_render(SDL_Rect size);
    void draw_rect(int x, int y, int w, int h);
    void update_texture(vector<uint32_t>& pixels) const;
    void blit_texture() const;
    void update_screen() const;
    void screen_shot() const;
    const SDL_Rect& get_mouse_position() const;
    virtual void on_resized(uint new_width, uint new_height) = 0;
    virtual void on_mouse_click(int x, int y, int button) = 0;
    virtual void on_mouse_move(int x, int y) = 0;
    virtual void on_mouse_wheel(int amount) = 0;
    virtual void on_keyboard_up(int c) = 0;
    virtual void on_keyboard_down(int c) = 0;
    void event_loop();
    const SDL_Rect& get_texture_position() const;
    void set_texture_position(SDL_Rect texture_position);
};

ostream& operator << (ostream& flow, SDL_Rect rect);

#endif // __SRC_WINDOW_HH__
