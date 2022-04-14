#ifndef __SRC_WINDOW_HH__
#define __SRC_WINDOW_HH__

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>

using std::vector;
using std::runtime_error;

class WindowManager
{
  private:
    SDL_Rect m_window_position;
    SDL_Rect m_texture_position;
    bool m_quit;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;

  public:
    WindowManager(uint w, uint h);
    ~WindowManager();
    virtual void event_loop();
    virtual void on_resized(uint new_width, uint new_height);
    void update(vector<uint32_t>& pixels);
    void init_render(SDL_Rect texture_position);
    void set_texture_position(SDL_Rect texture_position);
    const SDL_Rect& get_window_position() const;
    SDL_Rect& get_texture_position();
};

#endif // __SRC_WINDOW_HH__
