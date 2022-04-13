#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>


const int     X0            = -0.5;
const int     WIDTH         = 600;
const int     HEIGHT        = 600;
const int     NUM_OF_ITER   = 700;
const double  INF_A         = 0.0;
const double  SUP_A         = 4.0;
const double  INF_B         = 0.0;
const double  SUP_B         = 4.0;

using std::sin;
using std::cout;
using std::endl;
using std::string;

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
double **exposant(string seq)
{
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
    }
  }
  return exp_arr;
}

int main()
{
  string seq = "AAB";
  seq = ab(seq);
  std::ofstream data("data.ppm");
  data << "P3";;
  data << endl;
  data << "600 600";
  data << endl;
  data << "255";
  data << endl;

  double ** lyap_arr = exposant(seq);

  for(int i = 0; i < WIDTH; ++i)
  {
    for(int j = 0; j < HEIGHT; ++j)
    {
      if(lyap_arr[i][j] < -2)
        data << " 184 134 11 ";
      else if(lyap_arr[i][j] >= -2 && lyap_arr[i][j] < -1)
        data << " 255 215 0 ";
      else if(lyap_arr[i][j] >= -1 && lyap_arr[i][j] < -0.5)
        data << " 255 228 54 ";
      else if(lyap_arr[i][j] >=0.5 && lyap_arr[i][j] < 0)
        data << " 255 255 0";
      else
        data << " 0 0 255";
    }

    data << endl;
  }

  free(lyap_arr);
  data.close();
  return 0;
}