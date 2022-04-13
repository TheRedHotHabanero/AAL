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

  double **exp_arr = new double *[WIDTH];
  for (int i = 0; i < WIDTH; ++i)
    exp_arr[i] = new double[HEIGHT];

  double a, b;
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

      if(exp_arr[x][y] < -4)
        //data << " 100 45 0 ";
        pixels[y * WIDTH + x] = (100 << 16) + (45 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -4 && exp_arr[x][y] < -3.7)
        //data << " 112 62 0 ";
        pixels[y * WIDTH + x] = (112 << 16) + (62 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -3.7 && exp_arr[x][y] < -3.4)
        //data << " 124 79 0 ";
        pixels[y * WIDTH + x] = (124 << 16) + (79 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -3.4 && exp_arr[x][y] < -3.1)
        //data << "  136 96 0 ";
        pixels[y * WIDTH + x] = (136 << 16) + (96 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -3.1 && exp_arr[x][y] < -2.9)
        //data << " 148 113 0 ";
        pixels[y * WIDTH + x] = (148 << 16) + (113 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -2.9 && exp_arr[x][y] < -2.6)
        //data << " 160 130 0 ";
        pixels[y * WIDTH + x] = (160 << 16) + (130 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -2.6 && exp_arr[x][y] < -2.3)
        //data << " 172 147 0 ";
        pixels[y * WIDTH + x] = (172 << 16) + (147 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -2.3 && exp_arr[x][y] < -2)
        //data << " 184 164 0 ";
        pixels[y * WIDTH + x] = (184 << 16) + (164 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -2 && exp_arr[x][y] < -1.7)
        //data << " 196 181 0 ";
        pixels[y * WIDTH + x] = (196 << 16) + (181 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -1.7 && exp_arr[x][y] < -1.4)
        //data << " 208 198 0 ";
        pixels[y * WIDTH + x] = (208 << 16) + (198 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -1.4 && exp_arr[x][y] < -1.1)
        //data << " 220 215 0 ";
        pixels[y * WIDTH + x] = (220 << 16) + (215 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -1.1 && exp_arr[x][y] < -0.8)
        //data << " 232 232 0 ";
        pixels[y * WIDTH + x] = (232 << 16) + (232 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -0.5 && exp_arr[x][y] < -0.2)
        //data << " 244 249 0 ";
        pixels[y * WIDTH + x] = (244 << 16) + (249 << 8) + (0 << 0) + (255 << 24);
      else if(exp_arr[x][y] >= -0.2 && exp_arr[x][y] < 0)
        //data << " 255 255 0 ";
        pixels[y * WIDTH + x] = (255 << 16) + (255 << 8) + (0 << 0) + (255 << 24);

      // LAMBDA POSITIF
      else
        //data << " 0 0 139 ";
        pixels[y * WIDTH + x] = (0 << 16) + (0 << 8) + (139 << 0) + (255 << 24);
      data << endl;
    }
  }
  data.close();
  return pixels;
}

int main()
{
  cout << "==========     Введите последовательность A-B     ==========\n";
  string seq;
  cin >> seq;
  seq = ab(seq);
  WindowManager manager(WIDTH, HEIGHT);
  uint32_t* exp_arr = exposant(seq);
  cout << "==========     Генерация завершена     ==========\n";
  manager.set_pixels(exp_arr);
  manager.update();
  manager.event_loop();
  return 0;
}
