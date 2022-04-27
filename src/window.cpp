#include "../include/window.hh"

ostream& operator << (ostream& flow, SDL_Rect rect)
{
  flow << "x = " << rect.x << ", y = " << rect.y << ", w = " << rect.w << ", h = " << rect.h << " ";
  return flow;
}


WindowManager::WindowManager(uint w, uint h): m_window_position(), m_quit(false), 
                                              m_window(nullptr), m_renderer(nullptr), 
                                              m_texture(nullptr), m_texture_position(), m_texture_original_size{}
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw runtime_error(SDL_GetError());
  m_window = SDL_CreateWindow("Lyapunov Fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)w, (int)h, SDL_WINDOW_RESIZABLE);

  if (m_window == nullptr)
    throw runtime_error(SDL_GetError());
  m_window_position.w = (int)w;
  m_window_position.h = (int)h;
}

void WindowManager::init_render(SDL_Rect size, SDL_Rect texture_position)
{
  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  if(m_renderer == nullptr)
    throw runtime_error(SDL_GetError());
  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, size.w, size.h);
  if(m_texture == nullptr)
    throw runtime_error(SDL_GetError());
  m_texture_position = texture_position;
  m_texture_original_size = size;
}

SDL_Rect& WindowManager::get_texture_position()
{ return m_texture_position; }

SDL_Rect& WindowManager::get_original_size()
{ return m_texture_original_size; }

const SDL_Rect& WindowManager::get_window_position() const
{
  return m_window_position;
}

void WindowManager::update(vector<uint32_t>& pixels) const
{
  SDL_UpdateTexture(m_texture, nullptr, pixels.data(), (int)(m_texture_original_size.h * sizeof(uint32_t)));
  show_texture();
}

void WindowManager::show_texture() const
{
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_texture_position);
  SDL_RenderPresent(m_renderer);
}

void WindowManager::set_texture_position(SDL_Rect texture_position)
{ m_texture_position = texture_position; }

void WindowManager::event_loop()
{
  SDL_Event event;
  while(!m_quit)
  {
    SDL_WaitEvent(&event);
    switch(event.type)
    {
      case SDL_WINDOWEVENT: switch(event.window.event)
                            {
                              case SDL_WINDOWEVENT_RESIZED:
                              on_resized(event.window.data1, event.window.data2);
                              break;
                            }
      break;
      case SDL_QUIT:  m_quit = true;
                      break;
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
