#include "../include/region.hh"

Region::Region(double from_x, double to_x, double from_y, double to_y)
              : from_x(from_x), to_x(to_x), from_y(from_y), to_y(to_y) {}

double Region::get_from_x() const
{ return from_x; }

double Region::get_to_x() const
{ return to_x; }

double Region::get_from_y() const
{ return from_x; }

double Region::get_to_y() const
{ return to_y;}

ostream& operator<<(ostream& os, const Region& region)
{
  os << "from_x: "  << region.from_x
     << " to_x: "   << region.to_x
     << " from_y: " << region.from_y
     << " to_y: "   << region.to_y;
  return os;
}

void Region::rotate(int degrees, double length)
{
  cout << length << endl;
  if (length != 4)
    return;
  if (length < 0)
    length = -length;
  double old_from_x;
  double old_to_x;
  cout << *this << endl;
  switch(degrees / 90)
  {
    case 1:
      old_from_x = get_from_x();
      old_to_x = get_to_x();
      from_x = length - to_y;
      to_x = length - from_y;
      from_y = old_from_x;
      to_y = old_to_x;
    case 2:
      old_from_x = get_from_x();
      old_to_x = get_to_x();
      from_x = length - to_y;
      to_x = length - from_y;
      from_y = old_from_x;
      to_y = old_to_x;
    case 3:
      old_from_x = get_from_x();
      old_to_x = get_to_x();
      from_x = length - to_y;
      to_x = length - from_y;
      from_y = old_from_x;
      to_y = old_to_x;
      cout << *this << endl;
      break;
  }
}