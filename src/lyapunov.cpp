#include "../include/lyapunov.hh"

// color is yellow is exp() < 0
// color is blue is exp() > 0
// сalculation of the Lyapunov exponent through https://www.youtube.com/watch?v=8xZyA09zRXY

Lyapunov::Lyapunov(uint lyap_width, uint lyap_height)
                   : WindowManager(), m_exponents(lyap_width * lyap_height), m_size(), m_last_position{}
{
  update_settings();
  generate_sequence();
  m_size.w = (int)lyap_width;
  m_size.h = (int)lyap_height;
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
  int precise;
  while (i < 4 && file >> str >> r >> g >> b)
  {
    m_color_lyap[i] = (r << 16u) + (g << 8u) + b;
    cout << r << " " << g << " " << b << endl;
    i++;
  }
  set_color_scale(0, m_color_lyap[1], m_color_lyap[0]); //Neg
  set_color_scale(3, m_color_lyap[3], m_color_lyap[2]); //Pos

  file >> str >> precise;
  m_precision = precise;
  cout << m_precision << endl;

  string seq;
  file >> str >> seq;
  m_sequence = seq;
  cout << m_sequence << endl;
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

void Lyapunov::set_color_scale(int tab, uint32_t max, uint32_t min)
{
  int curr_max = max;
  int curr_min = min;
  for (int i = 2; i > 0; i--)
  {
    curr_max = max % 256;
    curr_min = min % 256;
    color_scale[tab + i] = curr_max - curr_min;
    color_scale[tab + 6 + i] = curr_min;
    max = (max - curr_max) / 256;
    min = (min - curr_min) / 256;
  }
}

void Lyapunov::update_pixels()
{
  vector<uint32_t> pixels(m_size.w * m_size.h);
  int red;
  int green;
  int blue;
  int choix_tab;
  double divider;
  for (int i = 0, size = m_size.w * m_size.h; i < size; ++i)
  {
    double exponent = m_exponents[i];
    if (exponent > 0)
    {
      choix_tab = 3;
      divider = max_expo;
    }
    else
    {
      choix_tab = 0;
      divider = min_expo;
    }
    exponent = exponent / divider;
    red   = (int)(color_scale[choix_tab]      * exponent) + color_scale[choix_tab + 6];
    green = (int)(color_scale[choix_tab + 1]  * exponent) + color_scale[choix_tab + 7];
    blue  = (int)(color_scale[choix_tab + 2]  * exponent) + color_scale[choix_tab + 8];
    set_pixel_RGB(pixels, i, red, green, blue);
  }
  update_texture(pixels);
}

void Lyapunov::generate_sequence()
{
  string sequence;
  bool error = false;
  cout << m_sequence.length() << endl;
  for (char i : m_sequence)
  {
    switch (i)
    {
      case 'A':
      case 'B':
        continue;
      default:
        error = true;
    }
    break;
  }
  if (m_sequence.empty() || error)
  {
    cout  << "An error in the construction of the sequence has been detected. \
              Sequence must contains only A and B. Default Sequence : AB" 
          << endl;
    m_sequence = "AB";
  }
  sequence = m_sequence;
  while((int)m_sequence.length() < m_precision)
    m_sequence += sequence;
}

void Lyapunov::generate(Region region)
{
  if (region.get_from_x() < 0 || region.get_to_x() > 4 || region.get_from_y() < 0 || region.get_to_y() > 4)
  {
    double fx = region.get_from_x();
    double tx = region.get_to_x();
    double fy = region.get_from_y();
    double ty = region.get_to_y();
    double length =  tx - fx;
    if (fx < 0)
    {
      fx = 0;
      tx = 0 + length;
    }
    else if(tx > 4)
    {
      tx = 4;
      fx = 4 - length;
    }
    if (fy < 0)
    {
      fy = 0;
      ty = 0 + length;
    }
    else if (ty > 4)
    {
      ty = 4;
      fy = 4 - length;
    }
    m_current_region = Region(fx, tx, fy, ty);
  }
  else
    m_current_region = Region{region};

  if (m_sequence.empty())
    generate_sequence();

  uint nb_thread = thread::hardware_concurrency();
  vector<thread> threads(nb_thread);
  for(uint i = 0; i < nb_thread; i++)
  {
    threads[i] = thread(&Lyapunov::generate_part, this, 0,
                 i * m_size.w / nb_thread, m_size.w,
                 (i + 1) * m_size.h / nb_thread);
  }
  for (auto& th: threads)
    th.join();
  update_pixels();
  SDL_Rect mouse_pos = get_mouse_position();
  draw_zoom();
}

void Lyapunov::generate_part(uint x_start, uint y_start, uint x_end, uint y_end)
{
  uint width  = m_size.w;
  uint height = m_size.h;
  uint number_of_products;

  uint x, y, y_pos, index;
  int i, j;
  double a, b, exp_lyap, xn, rn;

  min_expo = 0;
  max_expo = 0;

  double a_start = m_current_region.get_from_x();
  double b_start = m_current_region.get_from_y();
  double scale_a = ((m_current_region.get_to_x() - a_start) / (double)width);
  double scale_b = ((m_current_region.get_to_y() - b_start) / (double)height);
  for (y = y_start; y < y_end; ++y)
  {
    y_pos = y * width;
    for (x = x_start; x < x_end; ++x)
    {
      index = y_pos + x;
      a = a_start + x * scale_a;
      b = b_start + y * scale_b;
      exp_lyap = 0;
      xn = X0;
      number_of_products = m_precision / 10;
      vector<double> product(number_of_products);
      for (i = 0; i < number_of_products; ++i)
      {
        product[i] = 1;
        for (j = i * number_of_products; j < (i + 1) * number_of_products; ++j)
        {
          rn = m_sequence[j] == 'A' ? a : b;
          xn = rn * xn * (1 - xn);
          product[i] *= (fabs(rn * (1 - 2 * xn)));
        }
        product[i] = log2(product[i]);
        exp_lyap += product[i];
      }
      exp_lyap = exp_lyap / m_precision;
      if (exp_lyap < - 30)
        exp_lyap = min_expo;
      else if (exp_lyap > 30)
        exp_lyap = max_expo;
      m_exponents[index] = exp_lyap;
      min_expo = (exp_lyap < min_expo) ? exp_lyap : min_expo;
      max_expo = (exp_lyap > max_expo) ? exp_lyap : max_expo;
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

void Lyapunov::on_mouse_move(int x, int y)
{
  long time = get_current_time();
  if (time - m_last_move < 16)
    return;
  m_last_move = time;
  draw_zoom();
}

void Lyapunov::start_loop()
{ event_loop(); }

void Lyapunov::on_mouse_wheel(int amount)
{
  int max = (int) (get_texture_position().w * (3 / 4.0));
  m_zoom_precision += amount * 10;
  if (m_zoom_precision < 5)
    m_zoom_precision = 10;
  else if (m_zoom_precision > max)
    m_zoom_precision = max;
  draw_zoom();
}

void Lyapunov::on_keyboard_up(int c) {}

void Lyapunov::on_keyboard_down(int c)
{
  switch(c)
  {
    case SDLK_RIGHT:
    {
      double distance = (m_current_region.get_to_x() - m_current_region.get_from_x()) / 2;
      m_current_region = {m_current_region.get_from_x() + distance, m_current_region.get_to_x() + distance,
                          m_current_region.get_from_y(), m_current_region.get_to_y()};
      generate(m_current_region);
    }
      break;
    case SDLK_LEFT:
    {
      double distance = (m_current_region.get_to_x() - m_current_region.get_from_x()) / ( - 2 );
      m_current_region = {m_current_region.get_from_x() + distance, m_current_region.get_to_x() + distance,
                          m_current_region.get_from_y(), m_current_region.get_to_y()};
      generate(m_current_region);
    }
      break;
    case SDLK_DOWN:
    {
      double distance= (m_current_region.get_to_y() - m_current_region.get_from_y()) / 2;
      m_current_region = {m_current_region.get_from_x(), m_current_region.get_to_x(),
                          m_current_region.get_from_y() + distance, m_current_region.get_to_y() + distance};
      generate(m_current_region);
    }
      break;
    case SDLK_UP:
    {
      double distance = (m_current_region.get_to_y() - m_current_region.get_from_y()) / ( - 2);
      m_current_region = {m_current_region.get_from_x(), m_current_region.get_to_x(),
                          m_current_region.get_from_y() + distance, m_current_region.get_to_y() + distance};
      generate(m_current_region);
    }
      break;
    case SDLK_RETURN:
      screen_shot();
      break;
    case SDLK_ESCAPE:
    {
      string seq = m_sequence;
      int precise = m_precision;
      Menu k = Menu();
      k.set_color_button();
      Gtk::Main::run(k);
      update_settings();

      if (m_sequence.length() == 0)
        m_sequence = seq;
      generate_sequence();

      if (seq.compare(m_sequence) != 0 || precise != m_precision)
        generate();

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

void Lyapunov::draw_zoom()
{
  SDL_Rect mouse = get_mouse_position();
  int shift = m_zoom_precision >> 1;
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

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  Gtk::Main app(argc, argv);
  Menu m = Menu();
  Gtk::Main::run(m);
  m.write_file();
  Lyapunov lyapunov(1000, 1000);
  lyapunov.generate();
  lyapunov.start_loop();
  return 0;
}
