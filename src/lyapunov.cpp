#include "../include/lyapunov.hh"

// color is yellow is exp() < 0
// color is blue is exp() > 0
// сalculation of the Lyapunov exponent through https://www.youtube.com/watch?v=8xZyA09zRXY

void Lyapunov::set_pixel(int index, unsigned int r, unsigned int g, unsigned int b)
{ m_pixels[index] = (r << 16) + (g << 8) + b; }

void Lyapunov::update_pixels()
{ m_window_manager.update(m_pixels); }

Lyapunov::Lyapunov() : m_window_manager(HEIGHT, WIDTH), m_pixels(){}

void Lyapunov::event_loop()
{ m_window_manager.event_loop(); }

void Lyapunov::generate_sequence()
{
  cout << "ВВедите последовательностьA-B\n";
  string sequence;
  sequence = "BA";
  while(m_sequence.length() < NUM_OF_ITER)
  { m_sequence += sequence; }
}

// creation of a 2D array in order to retrieve an array containing the exponents
void Lyapunov::generate()
{
  if(m_sequence.empty())
    generate_sequence();
  vector<uint32_t> pixels(WIDTH * HEIGHT);

  int green_layer = 0;
  int red_layer = 0;
  int blue_layer = 0;

  uint i, x, y, yPos, index;
  double a, b, exp_arr, exp_lyap, xn, rn;

  double scale_a = ((SUP_A - INF_A) / WIDTH);
  double scale_b = ((SUP_B - INF_B) / HEIGHT);
  for(y = 0; y < HEIGHT; ++y)
  {
    cout << (float)y / WIDTH * 100 << "%" << endl;
    yPos = y * WIDTH;
    for(x = 0; x < WIDTH; ++x)
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
      exp_arr = exp_lyap /= NUM_OF_ITER;

      //LAMBDA NEGATIF
      green_layer = ((int) (210 + exp_lyap * 50) >= 0) ? (int) (210 + exp_lyap * 50) : 0;
      red_layer = ((int) (255 + exp_lyap * 52) >= 100) ? (int) (255 + exp_lyap * 52) : 100;
      blue_layer = ((int) (255 - exp_lyap * 200) >= 0) ? (int) (255 - exp_lyap * 200) : 0;

      if(exp_arr < -6)
        set_pixel(index, 0, 0, 0);
      else if(exp_arr <= 0)
        set_pixel(index, 0, green_layer, 0);
      else if(exp_arr > 0)
        set_pixel(index, 0, 0, blue_layer);
      else if(exp_arr >= 1)
        set_pixel(index, 0, 0, 0);
    }
  }
  update_pixels();
  cout << "==========     Генерация завершена     ==========\n";
}

int main()
{
  Lyapunov lyapunov;
  lyapunov.generate();
  lyapunov.event_loop();

  return 0;
}
