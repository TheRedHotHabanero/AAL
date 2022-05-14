#include "../include/window.hh"

ostream& operator << (ostream& flow, SDL_Rect rect)
{
  flow << "x = " << rect.x << ", y = " << rect.y << ", w = " << rect.w << ", h = " << rect.h << " ";
  return flow;
}

WindowManager::WindowManager(): m_quit{false}, 
                                m_window{nullptr}, m_renderer{nullptr},
                                m_texture{nullptr}, m_texture_position{},
                                m_pitch{}, m_mouse_position{}
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw runtime_error(SDL_GetError());
  m_window = SDL_CreateWindow("Lyapunov Fractals", SDL_WINDOWPOS_UNDEFINED, 
                              SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
  if (m_window == nullptr)
    throw runtime_error(SDL_GetError());
}

void WindowManager::init_render(SDL_Rect size)
{
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(m_renderer == nullptr)
    throw runtime_error(SDL_GetError());
  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, size.w, size.h);
  if(m_texture == nullptr)
    throw runtime_error(SDL_GetError());
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  m_pitch = size.h;
}

void WindowManager::draw_rect(int x, int y, int w, int h)
{
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderDrawRect(m_renderer, &rect);
}

void WindowManager::update_texture(vector<Uint32>& pixels) const
{ SDL_UpdateTexture(m_texture, nullptr, pixels.data(), (int)(m_pitch * sizeof(Uint32))); }

void WindowManager::blit_texture() const
{ SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_texture_position); }

void WindowManager::update_screen() const
{ SDL_RenderPresent(m_renderer); }

void WindowManager::event_loop()
{
  SDL_Event event;
  while(!m_quit)
  {
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_MOUSEBUTTONUP:
          on_mouse_click(event.button.x, event.button.y, event.button.button);
          break;
        case SDL_MOUSEWHEEL:
          on_mouse_wheel(event.wheel.y);
          break;
        case SDL_MOUSEMOTION:
          m_mouse_position.x = event.motion.x;
          m_mouse_position.y = event.motion.y;
          on_mouse_move();
          break;
        case SDL_KEYDOWN:
          on_keyboard_down(event.key.keysym.sym);
		      break;
        case SDL_WINDOWEVENT:
          switch(event.window.event)
          {
            case SDL_WINDOWEVENT_RESIZED:
              on_resized(event.window.data1, event.window.data2);
              break;
            case SDL_WINDOWEVENT_MOVED:
              blit_texture();
              update_screen();
              break;
          }
          break;
        case SDL_QUIT:
          m_quit = true;
          break;
      }
    }
    SDL_Delay(10);
  }
}

const SDL_Rect& WindowManager::get_texture_position() const
{ return m_texture_position; }

void WindowManager::set_texture_position(SDL_Rect texture_position)
{ m_texture_position = texture_position; }

const SDL_Rect& WindowManager::get_mouse_position() const
{ return m_mouse_position; }

int WindowManager::get_max_size() const
{
  SDL_DisplayMode display;
  SDL_GetCurrentDisplayMode(0, &display);
  return display.w < display.h ? display.w : display.h;
}

void WindowManager::screen_shot() const
{
  blit_texture();
  long current_time = get_current_time();
  string file_name = to_string(current_time);
  file_name = "screenshot/" + file_name + ".bmp";
  SDL_Surface* screen_shot = SDL_CreateRGBSurface(0, m_texture_position.w, m_texture_position.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(m_renderer, &m_texture_position, screen_shot->format->format, screen_shot->pixels, screen_shot->pitch);
  SDL_SaveBMP(screen_shot, file_name.data());
  SDL_FreeSurface(screen_shot);
}

WindowManager::~WindowManager()
{
  SDL_DestroyTexture(m_texture);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}