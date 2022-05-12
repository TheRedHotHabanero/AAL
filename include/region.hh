#ifndef __SRC_REGION_HH__
#define __SRC_REGION_HH__

#include <ostream>
#include <iostream>

using std::ostream;
using std::cout;
using std::endl;

class Region
{
  private:
    double from_x;
    double to_x;
    double from_y;
    double to_y;

  public:
    friend ostream& operator<<(ostream& os, const Region& region);
    Region(double from_x, double to_x, double from_y, double to_y);

    double get_from_x() const;
    double get_to_x() const;
    double get_from_y() const;
    double get_to_y() const;
    void rotate(int degree, double length);
};


#endif // __SRC_REGION_HH__
