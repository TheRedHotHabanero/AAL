#include "../include/window.hh"

WindowManager::WindowManager(uint w, uint h): m_width(w), m_height(h), m_quit(false), 
                                            m_window(nullptr), m_renderer(nullptr), m_texture(nullptr)
{
  SDL_Init(SDL_INIT_VIDEO);
  m_window = SDL_CreateWindow("Lyapunov Fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, 0);
  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, m_width, m_height);
}

void WindowManager::update(vector<uint32_t>& pixels)
{
  SDL_UpdateTexture(m_texture, nullptr, pixels.data(), m_width * sizeof(uint32_t));
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
  SDL_RenderPresent(m_renderer);
}

void WindowManager::event_loop()
{
  SDL_Event event;
  while(!m_quit)
  {
    SDL_WaitEvent(&event);
    switch(event.type)
    {
      case SDL_QUIT:{ m_quit = true;
                      break; }
    }
  }
}

WindowManager::~WindowManager()
{
  SDL_DestroyTexture(m_texture);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}
