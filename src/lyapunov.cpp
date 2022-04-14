#include "../include/lyapunov.hh"

// color is yellow is exp() < 0
// color is blue is exp() > 0
// сalculation of the Lyapunov exponent through https://www.youtube.com/watch?v=8xZyA09zRXY

Lyapunov::Lyapunov(uint width, uint height,
                   uint lyap_width, uint lyap_height)
                   : WindowManager(width, height), m_pixels(lyap_width * lyap_height)
{
  SDL_Rect texture_position;
  texture_position.x = (int)((width >> 1) - (lyap_width >> 1));
  texture_position.y = (int)((height >> 1) - (lyap_height >> 1));
  texture_position.w = (int) lyap_width;
  texture_position.h = (int) lyap_height;
  init_render(texture_position);
}

void Lyapunov::set_pixel_RGB(uint index, uint r, uint g, uint b)
{ m_pixels[index] = (r << 16) + (g << 8) + b; }

// https://ru.wikipedia.org/wiki/HSV_(%D1%86%D0%B2%D0%B5%D1%82%D0%BE%D0%B2%D0%B0%D1%8F_%D0%BC%D0%BE%D0%B4%D0%B5%D0%BB%D1%8C)
void Lyapunov::set_pixel_HSV(uint index, float h, float s, float v)
{
  int hi = (int) (h / 60) % 6;
  switch(hi)
  {
    case 0:
    {
      set_pixel_RGB(index,(int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60 - hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255)); //l
      break;
    }
    case 1:
    {
      set_pixel_RGB(index,(int) (v * (1 - (h / 60 - hi) * s) * 255), //m
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255)); //l
      break;
    }
    case 2:
    {
      set_pixel_RGB(index, (int) (v * (1 - s) * 255), //l
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60 - hi)) * s) * 255)); //n
      break;
    }
    case 3:
    {
      set_pixel_RGB(index, (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60 - hi) * s) * 255), //m
                        (int) (v * 255)); //v
      break;
    }
    case 4:
    {
      set_pixel_RGB(index, (int) (v * (1 - (1 - (h / 60 - hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255)); //v
      break;
    }
    case 5:
    {
      set_pixel_RGB(index,(int) (v * 255), //v
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60 - hi) * s) * 255)); //m
    break;
    }
  }
}

void Lyapunov::update_pixels()
{ update(m_pixels); }

void Lyapunov::generate_sequence()
{
  cout << "ВВедите последовательностьA-B\n";
  string sequence;
  sequence = "BA";
  while(m_sequence.length() < NUM_OF_ITER)
  { m_sequence += sequence; }
}

void Lyapunov::generate()
{
  if(m_sequence.empty())
    generate_sequence();
  SDL_Rect position = get_texture_position();
  vector<uint32_t> pixels(position.w * position.h);

  int green_layer = 0;
  int red_layer = 0;
  int blue_layer = 0;

  uint i, x, y, yPos, index;
  double a, b, exp_lyap, xn, rn;

  double scale_a = ((SUP_A - INF_A) / position.w);
  double scale_b = ((SUP_B - INF_B) / position.h);
  for ( y = 0; y < position.h; ++y)
  {
    cout << (float)y / position.w * 100 << "%" << endl;
    yPos = y * position.w;
    for ( x = 0; x < position.w; ++x)
    {
      index = yPos + x;
      a = x * scale_a;
      b = y * scale_b;
      exp_lyap = 0;
      xn = X0;

      for(i = 0; i < NUM_OF_ITER; ++i)
      {
        rn = m_sequence[i] == 'A' ? a : b;
        xn = rn * xn * (1 - xn);
        exp_lyap += log2(fabs(rn * (1 - 2 * xn)));
      }
      exp_lyap /= NUM_OF_ITER;

      //LAMBDA NEGATIF
      green_layer = ((int) (210 + exp_lyap * 50) >= 0) ? (int) (210 + exp_lyap * 50) : 0;
      red_layer = ((int) (255 + exp_lyap * 52) >= 100) ? (int) (255 + exp_lyap * 52) : 100;
      blue_layer = ((int) (255 - exp_lyap * 200) >= 0) ? (int) (255 - exp_lyap * 200) : 0;

      if(exp_lyap < -6)
        set_pixel_RGB(index, 0, 0, 0);
      else if(exp_lyap <= 0)
        set_pixel_RGB(index, 0, green_layer, 0);
      else if(exp_lyap > 0)
        set_pixel_RGB(index, 0, 0, blue_layer);
      else if(exp_lyap >= 1)
        set_pixel_RGB(index, 0, 0, 0);
    }
  }
  update_pixels();
  cout << "==========     Генерация завершена     ==========\n";
}

void Lyapunov::on_resized(uint new_width, uint new_height)
{
  SDL_Rect new_pos{ get_texture_position() };
  new_pos.x = (int)((new_width >> 1) - (new_pos.w >> 1));
  new_pos.y = (int)((new_height >> 1) - (new_pos.h >> 1));
  set_texture_position(new_pos);
}

void Lyapunov::event_loop()
{ WindowManager::event_loop();}


int main()
{
  Lyapunov lyapunov(1280, 720, 720, 720);
  lyapunov.generate();
  lyapunov.event_loop();

  return 0;
}
