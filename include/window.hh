#ifndef __SRC_WINDOW_HH__
#define __SRC_WINDOW_HH__

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>

using std::vector;

class WindowManager
{
  private:
    uint m_width, m_height;
    bool m_quit;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;

  public:
    WindowManager(int width, int height);
    ~WindowManager();
    void event_loop();
    void update(vector<uint32_t> pixels);
};

#endif // __SRC_WINDOW_HH__
