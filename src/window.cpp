#include "../include/window.hh"

ostream& operator << (ostream& flow, SDL_Rect rect)
{
  flow << "x = " << rect.x << ", y = " << rect.y << ", w = " << rect.w << ", h = " << rect.h << " ";
  return flow;
}


WindowManager::WindowManager(uint w, uint h): m_window_position{}, m_quit{false}, 
                                              m_window{nullptr}, m_renderer{nullptr}, m_draw{nullptr},
                                              m_texture{nullptr}, m_texture_position{}, m_texture_original_size{}
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
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  if(m_renderer == nullptr)
    throw runtime_error(SDL_GetError());
  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, size.w, size.h);
  if(m_texture == nullptr)
    throw runtime_error(SDL_GetError());
  m_texture_position = texture_position;
  m_texture_original_size = size;
}

void WindowManager::draw_rect(int x, int y, int w, int h)
{
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderDrawRect(m_renderer, &rect);
}

void WindowManager::update_texture(vector<uint32_t>& pixels) const
{ SDL_UpdateTexture(m_texture, nullptr, pixels.data(), (int)(m_texture_original_size.h * sizeof(uint32_t))); }

void WindowManager::blit_texture() const
{
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
  SDL_RenderClear(m_renderer);
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_texture_position);
}

void WindowManager::update_screen() const
{ SDL_RenderPresent(m_renderer); }

void WindowManager::render(SDL_Rect* clip,double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  SDL_Rect render_quad = {clip->x, clip->y, clip->w, clip->h};
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
  SDL_RenderClear(m_renderer);
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

  if (clip!= NULL)
  {
    render_quad.w = clip->w;
    render_quad.h = clip->h;
  }
  SDL_RenderCopyEx(m_renderer, m_texture, &m_texture_original_size, &render_quad, angle, center, flip);
}

void WindowManager::event_loop()
{
  SDL_Event event;
  double degree = 0;
  bool pause = true;
  SDL_Rect test_rect = m_texture_original_size;
  SDL_RendererFlip flip_type = SDL_FLIP_NONE;
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
          on_mouse_wheel();
          break;
        case SDL_MOUSEMOTION:
          m_mouse_position.x = event.motion.x;
          m_mouse_position.y = event.motion.y;
          on_mouse_move(event.motion.x, event.motion.y);
          break;
        case SDL_KEYUP:
          on_keyboard(event.key.keysym.sym);
          break;
        case SDL_KEYDOWN:
		      switch(event.key.keysym.sym)
		      {
		        case SDLK_a:
		          degree += 1;
		          break;
		        case SDLK_e:
		          degree -= 1;
		          break;
		        case SDLK_d:
		          flip_type = SDL_FLIP_HORIZONTAL;
		          break;
		        case SDLK_s:
		          flip_type = SDL_FLIP_NONE;
		          break;
		        case SDLK_z:
		          flip_type = SDL_FLIP_VERTICAL;
		          break;
		        case SDLK_q:
		          degree += 180;
		          break;
		        case SDLK_p:
		          pause = !pause;
		          break;
            case SDLK_RIGHT:
              test_rect.x +=10;
              break;
            case SDLK_LEFT:
              test_rect.x -=10;
              break;
            case SDLK_UP:
              test_rect.y -=10;
              break;
            case SDLK_DOWN:
              test_rect.y +=10;
              break;
		      }
          render(&test_rect, degree, NULL, flip_type);
          update_screen();
          break;
        case SDL_WINDOWEVENT:
          switch(event.window.event)
          {
            case SDL_WINDOWEVENT_RESIZED:
              on_resized(event.window.data1, event.window.data2);
              break;
            case SDL_WINDOWEVENT_MOVED:
              render(&m_texture_position, 0, NULL, SDL_FLIP_NONE);
              update_screen();
              break;
          }
          break;
        case SDL_QUIT:
          m_quit = true;
          break;
      }
    }
    on_tick();
    SDL_Delay(10);
  }
}

const SDL_Rect& WindowManager::get_texture_position() const
{ return m_texture_position; }

void WindowManager::set_texture_position(SDL_Rect texture_position)
{ m_texture_position = texture_position; }

WindowManager::~WindowManager()
{
  SDL_DestroyTexture(m_texture);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

const SDL_Rect& WindowManager::get_mouse_position() const
{ return m_mouse_position; }
