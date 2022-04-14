#include <iostream>
#include "../include/window.hh"

WindowManager::WindowManager(int w, int h): width(w), height(h), quit(false), 
                                            pixels(nullptr), window(nullptr), 
                                            renderer(nullptr), texture(nullptr)
{
  SDL_Init(SDL_INIT_VIDEO);
  pixels = new Uint32[width * height];
  window = SDL_CreateWindow("Lyapunov fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
  memset(pixels, 255, width * height * sizeof(Uint32));
}

void WindowManager::update()
{
  SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(Uint32));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void WindowManager::event_loop()
{
  SDL_Event event;
  while(!quit)
  {
    SDL_WaitEvent(&event);
    SDL_RenderPresent(renderer);
    switch(event.type)
    {
      case SDL_QUIT:{ quit = true;
                      break; }
    }
  }
}

void WindowManager::set_pixels(uint32_t* curr_pixs)
{
  for(int i = 0, size = width * height; i < size; ++i)
  { pixels[i] = curr_pixs[i]; }
}

WindowManager::~WindowManager()
{
  delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
