#ifndef __INCLUDE_MENU_MENU_HH__
#define __INCLUDE_MENU_MENU_HH__

#include <gtkmm.h>
#include <gdk/gdk.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const int MINSPIN = 10;
const int MAXSPIN = 10000;

using std::string;
using std::ofstream;
using std::cout;
using std::stringstream;
using std::ifstream;
using std::endl;

class Menu : public Gtk::Window
{
  private:

    Gtk::Entry sequence;
    int precision;
    Gdk::RGBA m_color[4];
    Gtk::Button menu_write;
    Gtk::AccelLabel label_expoNegE;
    Gtk::AccelLabel label_expoNegS;
    Gtk::AccelLabel label_expoPosS;
    Gtk::AccelLabel label_expoPosE;
    Gtk::AccelLabel label_precision;
    Gtk::AccelLabel label_sequence;
    Gtk::ColorButton m_color_button_neg_e;
    Gtk::ColorButton m_color_button_neg_s;
    Gtk::ColorButton m_color_button_pos_s;
    Gtk::ColorButton m_color_button_pos_e;
    Gtk::SpinButton m_select_precision;
    Gtk::Grid grid;

  public :

    Menu(int precision = 100);
    void set_color_max_neg_expo();
    void set_color_min_neg_expo();
    void set_color_min_pos_expo();
    void set_color_max_pos_expo();
    string color_to_string(Gdk::RGBA color);
    void get_sequence(ofstream& file);
    void get_color(ofstream& file);
    void get_precision(ofstream& file);
    void text_area();
    void new_precision();
    void write_file();
    void set_color_button();
    void validate();
};

#endif // __INCLUDE_MENU_MENU_HH__
