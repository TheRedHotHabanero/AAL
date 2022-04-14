#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>

#include "../include/window.hh"

const int     X0            = 0.5;
const int     WIDTH         = 700;
const int     HEIGHT        = 700;
const int     NUM_OF_ITER   = 700;
const double  INF_A         = 0.0;
const double  SUP_A         = 4.0;
const double  INF_B         = 0.0;
const double  SUP_B         = 4.0;

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::to_string;

// color is yellow is exp() < 0
// color is blue is exp() > 0
// сalculation of the Lyapunov exponent through https://www.youtube.com/watch?v=8xZyA09zRXY

// scale spacing between each a/b for x/y
double scaleA()
{ return ((SUP_A - INF_A) / WIDTH);}

double scaleB()
{ return ((SUP_B - INF_B) / HEIGHT); }

// fill WINDOW size string
string ab(string seq)
{
  string res;
  while (res.length() < NUM_OF_ITER)
  { res += seq; }

  return res;
}

// calculate the position of X/Y in A/B
// j stands for A or B
double decide_a_or_b(double scale, double j)
{ return j * scale; }

// calculation of Xn+1
double nextXN(double xn, double j)
{ return j * xn * (1 - xn); }

// choice between A or B depending on the current Char
double rN(char current_char, double a, double b)
{ return ((current_char == 'A') ? a : b); }

uint32_t color_pixel(int red, int green, int blue)
{
  uint32_t pixel =+ (255 << 24) + (red << 16) + (green << 8) + (blue << 0);
  return pixel;
}

// creation of a 2D array in order to retrieve an array containing the exponents
uint32_t* exposant(string seq)
{
  uint32_t* pixels = new uint32_t[WIDTH * HEIGHT];
  ofstream data("data.ppm");
  data << "P3";
  data << endl;
  data << to_string(WIDTH);
  data << " ";
  data << to_string(HEIGHT);
  data << endl;
  data << "255";
  data << endl;

  int green_layer = 0;
  int red_layer   = 0;
  int blue_layer  = 0;

  double **exp_arr = new double *[WIDTH];
  for (int i = 0; i < WIDTH; ++i)
    exp_arr[i] = new double[HEIGHT];

  double a;
  double b;
  double exp_lyap;
  static double scale_a = scaleA();
  static double scale_b = scaleB();
  cout << scale_a << endl;
  cout << scale_b << endl;
  for(int x = 0; x < WIDTH; ++x)
  {
    cout << (float)x / WIDTH * 100 << "%" << endl;
    for(int y = 0; y < HEIGHT; ++y)
    {
      int index = y * WIDTH + x;
      a = decide_a_or_b(scale_a, x);
      b = decide_a_or_b(scale_b, y);
      exp_lyap = 0;

      double xn = X0;
      double rn = 0;

      for(unsigned long int i = 0; i < NUM_OF_ITER; ++i)
      {
	      xn = nextXN(xn,rn);
	      rn = rN(seq[i], a, b);
	      exp_lyap += log2(fabs(rn * (1 - 2 * xn))) ;
      }
      exp_lyap = exp_lyap / NUM_OF_ITER;
      exp_arr[x][y] = exp_lyap;

      //LAMBDA NEGATIF
      green_layer = ( (int)210 + exp_lyap * 50 >= 0 ) ? (int) 210 + exp_lyap * 50 : 0 ;
      red_layer = ( (int)255 + exp_lyap * 52 >= 100 ) ? (int) 255 + exp_lyap * 52 : 100 ;
      blue_layer = ( (int)255 - exp_lyap * 200 >= 0) ? (int) 255 - exp_lyap * 200 : 0 ;
      if(exp_arr[x][y] < -6)
        pixels[index] = color_pixel(0, 0, 0);
      else if (exp_arr[x][y] <= 0)
        pixels[index] = color_pixel(0, green_layer, 0);
      else if (exp_arr[x][y] > 0)
        pixels[index] = color_pixel(0, 0, blue_layer);
      else if (exp_arr[x][y] >= 1)
        pixels[index] = color_pixel(0, 0, 0);
    }
    data << endl;
  }
  data.close();
  return pixels;
}

int main()
{
  cout << "==========     Введите последовательность A-B     ==========\n";
  string seq;
  cin >> seq;
  seq = "BA";
  seq = ab(seq);
  WindowManager manager(WIDTH, HEIGHT);
  uint32_t* exp_arr = exposant(seq);
  cout << "==========     Генерация завершена     ==========\n";
  manager.set_pixels(exp_arr);
  manager.update();
  manager.event_loop();
  return 0;
}
