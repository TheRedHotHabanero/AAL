#ifndef __SRC_WINDOW_HH__
#define __SRC_WINDOW_HH__

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>

using std::vector;
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
    SDL_Texture* m_texture;
    SDL_Rect m_texture_position;
    SDL_Rect m_texture_original_size;


  public:
    WindowManager(uint w, uint h);
    ~WindowManager();
    const SDL_Rect& get_window_position() const;
    void event_loop();
    virtual void on_resized(uint new_width, uint new_height) = 0;
    void update(vector<uint32_t>& pixels) const;
    void init_render(SDL_Rect size, SDL_Rect position);
    void set_texture_position(SDL_Rect texture_position);
    SDL_Rect& get_texture_position();
    SDL_Rect& get_original_size();
    void show_texture() const;

};

ostream& operator << (ostream& flow, SDL_Rect rect);

#endif // __SRC_WINDOW_HH__
