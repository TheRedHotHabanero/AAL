#include "lyapunov.hh"

Lyapunov::Lyapunov(): WindowManager(),
                      m_exponents(get_max_size() * get_max_size()), m_size(), m_last_position{}
{
  update_settings();
  generate_sequence();
  m_size.h = m_size.w = get_max_size();
  init_render(m_size);
}

void Lyapunov::update_settings()
{
  ifstream file("config.txt");
  string str;
  int r;
  int g;
  int b;
  int i = 0;
  int precision;
  while(i < 4 && file >> str >> r >> g >> b)
  { m_color_lyap[i++] = ((uint) r << 16u) + ((uint) g << 8u) + b; }
  set_color_scale(0, m_color_lyap[1], m_color_lyap[0]);
  set_color_scale(3, m_color_lyap[3], m_color_lyap[2]);
  file >> str >> precision;
  m_precision = precision;
  string seq;
  file >> str >> seq;
  m_sequence = seq;
}

Region Lyapunov::get_region(int from_x, int to_x, int from_y, int to_y) const
{
  SDL_Rect texture_position = get_texture_position();
  Region region
  {
    (double)(from_x < texture_position.x ? 0 : from_x > texture_position.x + texture_position.w ?
            texture_position.w : from_x - texture_position.x) / (double) texture_position.w *
            (m_current_region.get_to_x() - m_current_region.get_from_x()) + m_current_region.get_from_x(),
    (double)(to_x < texture_position.x ? 0 : to_x > texture_position.x + texture_position.w ?
            texture_position.w : to_x - texture_position.x) / (double) texture_position.w *
            (m_current_region.get_to_x() - m_current_region.get_from_x()) + m_current_region.get_from_x(),
    (double)(from_y < texture_position.y ? 0 : from_y > texture_position.y + texture_position.h ?
            texture_position.h : from_y - texture_position.y) / (double) texture_position.h *
            (m_current_region.get_to_y() - m_current_region.get_from_y()) + m_current_region.get_from_y(),
    (double)(to_y < texture_position.y ? 0 : to_y > texture_position.y + texture_position.h ?
            texture_position.h : to_y - texture_position.y) / (double) texture_position.h *
            (m_current_region.get_to_y() - m_current_region.get_from_y()) + m_current_region.get_from_y()
  };
  return region;
}

void Lyapunov::set_color_scale(int tab, int max, int min)
{
  int curr_max = 0;
  int curr_min = 0;
  for (int i = 2; i >= 0; --i)
  {
    curr_max = max % 256;
    curr_min = min % 256;
    m_color_scale[tab + i] = curr_max - curr_min;
    m_color_scale[tab + 6 + i] = curr_min;
    max = (max - curr_max) / 256;
    min = (min - curr_min) / 256;
  }
}

void Lyapunov::update_pixels() const
{
  vector<Uint32> pixels(m_size.w * m_size.h);
  uint choice;
  double divider;
  double exponent;
  for (int i = 0, size = m_size.w * m_size.h; i < size; ++i)
  {
    exponent = m_exponents[i];
    if (exponent > 0)
    {
      choice = 3;
      divider = m_max_expo;
    }
    else
    {
      choice = 0;
      divider = m_min_expo;
    }
    exponent /= divider;
    pixels[i] = (((uint) (m_color_scale[choice]     * exponent) + m_color_scale[choice + 6]) << 16u) + //red
                (((uint) (m_color_scale[choice + 1] * exponent) + m_color_scale[choice + 7]) << 8u) +  //green
                (m_color_scale[choice + 2]          * exponent) + m_color_scale[choice + 8];           //blue
  }
  update_texture(pixels);
}

void Lyapunov::generate_sequence()
{
  bool error = false;
  for (char i : m_sequence)
  {
    switch(i)
    {
      case 'A' :
      case 'B' :
        continue;
      default :
        error = true;
    }
    break;
  }
  if (m_sequence.empty() || error)
  {
    m_sequence = "";
    for (int i = 0, size = rand() % m_precision; i < size; ++i)
    { m_sequence += (rand() % 2 == 0 ? "A" : "B"); }
    cout << "Случайная последовательность: " << m_sequence << endl;
  }
  string sequence = m_sequence;
  while((int) m_sequence.length() < m_precision)
  { m_sequence += sequence; }
}

void Lyapunov::generate(Region region)
{
  if (region.get_from_x() < 0 || 
      region.get_to_x() > 4 || 
      region.get_from_y() < 0 || 
      region.get_to_y() > 4)
  {
    double x1 = region.get_from_x();
    double x2 = region.get_to_x();
    double y1 = region.get_from_y();
    double y2 = region.get_to_y();
    double length = x2 - x1;
    if (x1 < 0)
    {
      x1 = 0;
      x2 = length;
    }
    else if (x2 > 4)
    {
      x2 = 4;
      x1 = 4 - length;
    }
    if (y1 < 0)
    {
      y1 = 0;
      y2 = length;
    }
    else if(y2 > 4)
    {
      y2 = 4;
      y1 = 4 - length;
    }
    m_current_region = Region(x1, x2, y1, y2);
  }
  else
    m_current_region = region;
  if (m_sequence.empty())
    generate_sequence();
  m_min_expo = 0;
  m_max_expo = 0;
  uint nb_thread = thread::hardware_concurrency();
  vector<thread> threads(nb_thread);
  for (uint i = 0; i < nb_thread; i++)
  {
    threads[i] = thread(&Lyapunov::generate_part, this, 0,
                        i * m_size.w / nb_thread, m_size.w,
                        (i + 1) * m_size.h / nb_thread);
  }
  for (auto& th : threads)
  { th.join(); }
  update_pixels();
  draw_zoom();
}

void Lyapunov::generate_part(int x_start, int y_start, int x_end, int y_end)
{
  int width = m_size.w;
  int height = m_size.h;
  int x, y, y_pos, index, i;
  double a, b, expo_lyap, xn, rn;
  double a_start = m_current_region.get_from_x();
  double b_start = m_current_region.get_from_y();
  double scale_of_a = ((m_current_region.get_to_x() - a_start) / (double) width);
  double scale_of_b = ((m_current_region.get_to_y() - b_start) / (double) height);
  double expo = 1e100;
  double max = 1e100;
  for (y = y_start; y < y_end; ++y)
  {
    y_pos = y * width;
    for(x = x_start; x < x_end; ++x)
    {
      i = 0;
      index = y_pos + x;
      a = a_start + x * scale_of_a;
      b = b_start + y * scale_of_b;
      expo_lyap = 0;
      xn = X0;
      while (i < m_precision)
      {
        expo = 1;
        while (expo < max)
        {
          rn = m_sequence[i] == 'A' ? a : b;
          xn = rn * xn * (1 - xn);
          expo *= (fabs(rn * (1 - 2 * xn)));
          if (++i >= m_precision)
            break;
        }
        expo_lyap += log2(expo);
      }
      expo_lyap /= m_precision;
      if (expo_lyap < -10)
          expo_lyap = m_min_expo;
      else if (expo_lyap > 10)
        expo_lyap = m_max_expo;
      m_exponents[index] = expo_lyap;
      if (expo_lyap < m_min_expo)
        m_min_expo = expo_lyap;
      if (expo_lyap > m_max_expo)
        m_max_expo = expo_lyap;
    }
  }
}

void Lyapunov::on_resized(uint new_width, uint new_height)
{
  SDL_Rect new_pos;
  new_pos.w = new_pos.h = (int) (new_width < new_height ? new_width : new_height);
  new_pos.x = (int) ((new_width >> 1u) - ((uint) new_pos.w >> 1u));
  new_pos.y = (int) ((new_height >> 1u) - ((uint) new_pos.h >> 1u));
  set_texture_position(new_pos);
}

void Lyapunov::on_mouse_click(int mouse_x, int mouse_y, int button)
{
  switch(button)
  {
    case SDL_BUTTON_LEFT:
    {
      m_last_position.emplace(m_current_region);
      int shift = (int) ((uint) m_zoom_precision >> 1u);
      int x = mouse_x - shift;
      int y = mouse_y - shift;
      int w = m_zoom_precision;
      int h = w;
      validate_region(x, y, w, h);
      m_current_region = get_region(x, x + w, y, y + h);
      generate(m_current_region);
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

void Lyapunov::on_mouse_move()
{
  long time = get_current_time();
  if (time - m_last_move < 16)
    return;
  m_last_move = time;
  draw_zoom();
}

void Lyapunov::on_mouse_wheel(int amount)
{
  int max = (int) (get_texture_position().w * (3 / 4.0));
  m_zoom_precision += amount * 10;
  if (m_zoom_precision < 5)
    m_zoom_precision = 5;
  else if (m_zoom_precision > max)
    m_zoom_precision = max;
  draw_zoom();
}

void Lyapunov::on_keyboard_down(int c)
{
  switch(c)
  {
    case SDLK_RIGHT:
    {
      double distance = (m_current_region.get_to_x() - m_current_region.get_from_x()) / 2;
      m_current_region = {m_current_region.get_from_x() + distance,
                         m_current_region.get_to_x() + distance,
                         m_current_region.get_from_y(),
                         m_current_region.get_to_y()};
      generate(m_current_region);
    }
      break;
    case SDLK_LEFT:
    {
      double distance  = (m_current_region.get_to_x() - m_current_region.get_from_x()) / 2;
      m_current_region = {m_current_region.get_from_x() - distance,
                         m_current_region.get_to_x() - distance,
                         m_current_region.get_from_y(),
                         m_current_region.get_to_y()};
      generate(m_current_region);
    }
      break;
    case SDLK_DOWN:
    {
      double distance = (m_current_region.get_to_y() - m_current_region.get_from_y()) / 2;
      m_current_region = {m_current_region.get_from_x(),
                         m_current_region.get_to_x(),
                         m_current_region.get_from_y() + distance,
                         m_current_region.get_to_y() + distance};
      generate(m_current_region);
    }
      break;
    case SDLK_UP:
    {
      double distance = (m_current_region.get_to_y() - m_current_region.get_from_y()) / 2;
      m_current_region = {m_current_region.get_from_x(),
                         m_current_region.get_to_x(),
                         m_current_region.get_from_y() - distance,
                         m_current_region.get_to_y() - distance};
      generate(m_current_region);
    }
      break;
    case SDLK_ESCAPE:
    {
      string seq = m_sequence;
      int precis = m_precision;
      Menu k = Menu(m_precision);
      k.set_color_button();
      Gtk::Main::run(k);
      update_settings();
      if (m_sequence.length() == 0)
        m_sequence = seq;
      generate_sequence();
      if (seq != m_sequence || precis != m_precision)
          generate(m_current_region);
      update_pixels();
      blit_texture();
      update_screen();
    }
      break;
    default:
      break;
  }
  blit_texture();
  update_screen();
}

void Lyapunov::start_loop()
{ event_loop(); }

void Lyapunov::draw_zoom()
{
  SDL_Rect mouse = get_mouse_position();
  int shift = (int) ((uint) m_zoom_precision >> 1u);
  int x = mouse.x - shift;
  int y = mouse.y - shift;
  int w = m_zoom_precision;
  int h = w;
  validate_region(x, y, w, h);
  blit_texture();
  draw_rect(x, y, w, h);
  update_screen();
}

void Lyapunov::validate_region(int& x, int& y, int& w, int& h)
{
  SDL_Rect texture = get_texture_position();
  if (x < texture.x)
    x = texture.x;
  if (x + w > texture.x + texture.w)
    x = texture.x + texture.w - w;
  if (y < texture.y)
    y = texture.y;
  if (y + h > texture.y + texture.h)
    y = texture.y + texture.h - h;
}
