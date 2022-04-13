#ifndef __SRC_WINDOW_HH__
#define __SRC_WINDOW_HH__

#include <SDL2/SDL.h>
#include <exception>

class WindowManager
{
  private:
    int width, height;
    bool quit;
    uint32_t* pixels;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

  public:
    WindowManager(int width, int height);
    ~WindowManager();
    void event_loop();
    void set_pixels(Uint32* pixels);
    void update();
};

#endif // __SRC_WINDOW_HH__
