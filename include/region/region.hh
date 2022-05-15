#ifndef __INCLUDE_REGION_REGION_HH__
#define __INCLUDE_REGION_REGION_HH__

#include <ostream>
#include <iostream>

using std::ostream;

class Region
{
  private:

    double from_x;
    double to_x;
    double from_y;
    double to_y;

  public:

    Region(double from_x, double to_x, double from_y, double to_y);
    double get_from_x() const;
    double get_to_x() const;
    double get_from_y() const;
    double get_to_y() const;
};

ostream& operator<<(ostream& os, const Region& region);

#endif // __INCLUDE_REGION_REGION_HH__
