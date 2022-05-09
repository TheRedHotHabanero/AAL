#include "../include/lyapunov.hh"

// color is yellow is exp() < 0
// color is blue is exp() > 0
// сalculation of the Lyapunov exponent through https://www.youtube.com/watch?v=8xZyA09zRXY

Lyapunov::Lyapunov(uint window_width, uint window_height,
                   uint lyap_width, uint lyap_height)
                   : WindowManager(window_width, window_height), m_exponents(lyap_width * lyap_height), m_size(), m_last_position{}
{
  m_size.w = (int)lyap_width;
  m_size.h = (int)lyap_height;
  SDL_Rect texture_position;
  texture_position.w = (int)(window_width < lyap_width ? window_width : lyap_width);
  texture_position.h = (int)(window_height < lyap_height ? window_height : lyap_height);
  texture_position.w = texture_position.h = texture_position.w < texture_position.h ? texture_position.w : texture_position.h;
  texture_position.x = (int) ((window_width >> 1u) - ((uint)texture_position.w >> 1u));
  texture_position.y = (int) ((window_height >> 1u) - ((uint)texture_position.h >> 1u));
  init_render(m_size, texture_position);
}

Region Lyapunov::get_region(int from_x, int to_x, int from_y, int to_y)
{
  SDL_Rect texture_position = get_texture_position();
  Region region
  {
    (double) (from_x  < texture_position.x ? 0 : from_x > texture_position.x + texture_position.w ? texture_position.w : from_x - texture_position.x) / (double) texture_position.w * (m_current_region.get_to_x() - m_current_region.get_from_x()) + m_current_region.get_from_x(),
    (double) (to_x    < texture_position.x ? 0 : to_x   > texture_position.x + texture_position.w ? texture_position.w : to_x   - texture_position.x) / (double) texture_position.w * (m_current_region.get_to_x() - m_current_region.get_from_x()) + m_current_region.get_from_x(),
    (double) (from_y  < texture_position.y ? 0 : from_y > texture_position.y + texture_position.h ? texture_position.h : from_y - texture_position.y) / (double) texture_position.h * (m_current_region.get_to_y() - m_current_region.get_from_y()) + m_current_region.get_from_y(),
    (double) (to_y    < texture_position.y ? 0 : to_y   > texture_position.y + texture_position.h ? texture_position.h : to_y   - texture_position.y) / (double) texture_position.h * (m_current_region.get_to_y() - m_current_region.get_from_y()) + m_current_region.get_from_y()
  };
  return region;
}

void Lyapunov::set_pixel_RGB(vector<uint32_t>& pixels, uint index, uint r, uint g, uint b)
{ pixels[index] = (r << 16u) + (g << 8u) + b; }

// https://ru.wikipedia.org/wiki/HSV_(%D1%86%D0%B2%D0%B5%D1%82%D0%BE%D0%B2%D0%B0%D1%8F_%D0%BC%D0%BE%D0%B4%D0%B5%D0%BB%D1%8C)
void Lyapunov::set_pixel_HSV(vector<uint32_t>& pixels, uint index, int h, double s, double v)
{
  h %= 360;
  int hi = (int) (h / 60) % 6;
  switch(hi)
  {
    case 0:
    {
      set_pixel_RGB(pixels, index, (int) (v * 255), //v
                    (int) (v * (1 - (1 - (h / 60.0 - (double)hi)) * s) * 255), //n
                    (int) (v * (1 - s) * 255)); //l
      break;
    }
    case 1:
    {
      set_pixel_RGB(pixels, index, (int) (v * (1 - (h / 60.0 - (double)hi) * s) * 255), //m
                    (int) (v * 255), //v
                    (int) (v * (1 - s) * 255)); //l
      break;
    }
    case 2:
    {
      set_pixel_RGB(pixels, index, (int) (v * (1 - s) * 255), //l
                    (int) (v * 255), //v
                    (int) (v * (1 - (1 - (h / 60.0 - (double)hi)) * s) * 255)); //n
      break;
    }
    case 3:
    {
      set_pixel_RGB(pixels, index, (int) (v * (1 - s) * 255), //l
                    (int) (v * (1 - (h / 60.0 - (double)hi) * s) * 255), //m
                    (int) (v * 255)); //v
      break;
    }
    case 4:
    {
      set_pixel_RGB(pixels, index, (int) (v * (1 - (1 - (h / 60.0 - (double)hi)) * s) * 255), //n
                    (int) (v * (1 - s) * 255), //l
                    (int) (v * 255)); //v
      break;
    }
    case 5:
    {
      set_pixel_RGB(pixels, index, (int) (v * 255), //v
                    (int) (v * (1 - s) * 255), //l
                    (int) (v * (1 - (h / 60.0 - (double)hi) * s) * 255)); //m
    break;
    }
  }
}

void Lyapunov::update_pixels()
{
  vector<uint32_t> pixels(m_size.w * m_size.h);
  for (int i = 0, size = m_size.w * m_size.h; i < size; ++i)
  {
    double exponent = m_exponents[i];
    int green   = ((int) (210 + exponent * 50) >= 0) ? (int) (210 + exponent * 50) : 0;
    int red     = ((int) (255 + exponent * 52) >= 100) ? (int) (255 + exponent * 52) : 100;
    int blue    = ((int) (255 - exponent * 200) >= 0) ? (int) (255 - exponent * 200) : 0;
    if (exponent < -6)
      set_pixel_RGB(pixels, i, 0, 0, 0);
    else if (exponent <= 0)
      set_pixel_RGB(pixels, i, 0, green, 0);
    else if (exponent > 0)
      set_pixel_RGB(pixels, i, 0, 0, blue);
    else if (exponent >= 1)
      set_pixel_RGB(pixels, i, 0, 0, 0);
  }
  update_texture(pixels);
}

void Lyapunov::generate_sequence()
{
  string sequence;
  sequence = "BBABBABBABBABAABBBBA";
  while(m_sequence.length() < m_precision)
  { m_sequence += sequence; }
}

void Lyapunov::generate(Region region)
{
  m_current_region = Region{region};
  if (region.get_from_x() < 0 || region.get_to_x() > 4 || region.get_from_y() < 0 || region.get_to_y() > 4)
    throw domain_error("Invalid domain to generate Lyapunov");

  if (m_sequence.empty())
    generate_sequence();

  uint nb_thread = thread::hardware_concurrency();
  vector<thread> threads(nb_thread);
  for (uint i = 0; i < nb_thread; ++i)
    threads[i] = thread(&Lyapunov::generate_part, this, 0, (i * m_size.w / nb_thread), 
                        m_size.w, (i + 1) * m_size.h / nb_thread);
  for (auto& th: threads)
    th.join();

  update_pixels();
  blit_texture();
  SDL_Rect mouse_pos = get_mouse_position();
  draw_rect((int)mouse_pos.x - 200, (int)mouse_pos.y - 200, 400, 400);
  update_screen();
}

void Lyapunov::generate_part(uint x_start, uint y_start, uint x_end, uint y_end)
{
  uint width = m_size.w;
  uint height = m_size.h;

  uint i, x, y, yPos, index;
  double a, b, exp_lyap, xn, rn;

  double a_start = m_current_region.get_from_x();
  double b_start = m_current_region.get_from_y();
  double scale_a = ((m_current_region.get_to_x() - a_start) / (double)width);
  double scale_b = ((m_current_region.get_to_y() - b_start) / (double)height);
  for ( y = y_start; y < y_end; ++y)
  {
    cout << y * 100 / width << "%" << endl;
    yPos = y * width;
    for ( x = x_start; x < x_end; ++x)
    {
      index = yPos + x;
      a = a_start + x * scale_a;
      b = b_start + y * scale_b;
      exp_lyap = 0;
      xn = X0;

      for (int i = 0; i < m_precision; ++i)
      {
        rn = m_sequence[i] == 'A' ? a : b;
        xn = rn * xn * (1 - xn);
        exp_lyap += log2(fabs(rn * (1 - 2 * xn)));
      }
      m_exponents[index] = exp_lyap / m_precision;
    }
  }
}

void Lyapunov::on_resized(uint new_width, uint new_height)
{
  SDL_Rect new_pos;
  new_pos.w = new_pos.h = (int)(new_width < new_height ? new_width : new_height);
  new_pos.x = (int)((new_width >> 1u) - ((uint)new_pos.w >> 1));
  new_pos.y = (int)((new_height >> 1u) - ((uint)new_pos.h >> 1));
  set_texture_position(new_pos);
  blit_texture();
  update_screen();
}

void Lyapunov::on_mouse_click(uint x, uint y, uint button)
{
  switch(button)
  {
    case SDL_BUTTON_LEFT:
    {
      m_last_position.emplace(m_current_region);
      Region new_region = get_region((int)x - 200, (int)x + 200, (int)y - 200, (int)y + 200);
      generate(new_region);
    }
      break;
    case SDL_BUTTON_RIGHT:
    {
      if (m_last_position.empty())
        return;
      Region dezoom = m_last_position.top();
      m_last_position.pop();
      generate(dezoom);
    }
      break;
    default:
      break;
  }
}

void Lyapunov::on_mouse_move(uint x, uint y)
{
  long time = get_current_time();
  if (time - m_last_move < 16)
    return;
  blit_texture();
  draw_rect((int)x - 200, (int)y - 200, 400, 400);
  update_screen();
}

void Lyapunov::start_loop()
{ event_loop(); }

void Lyapunov::on_mouse_wheel() {}

void Lyapunov::on_tick()
{
  /*
  if (!m_stop_color)
  {
    m_current_color = (360 + (m_current_color - 5 % 360)) % 360;
    cout << m_current_color << endl;
    update_pixels();
    blit_texture();
    update_screen();
  }
  */
}

 void Lyapunov::on_keyboard(int c)
{
  switch (c)
  {
    case SDLK_SPACE:
      m_stop_color = !m_stop_color;
      break;
  }
}

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  Lyapunov lyapunov(1400, 1000, 1000, 1000);
  lyapunov.generate();
  lyapunov.start_loop();

  return 0;
}
