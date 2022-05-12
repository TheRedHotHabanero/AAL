#ifndef __SRC_MENU_HH__
#define __SRC_MENU_HH__

#include <gtkmm.h>
#include <gdk/gdk.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::stringstream;

const int MINSPIN = 10;
const int MAXSPIN = 500;

class Menu: public Gtk::Window
{
  private:
    Gtk::Entry sequence;
    Gdk::RGBA m_color[4];
    Gtk::Button menu_write;
    Gtk::AccelLabel label_expo_neg_e;
    Gtk::AccelLabel label_expo_neg_s;
    Gtk::AccelLabel label_expo_pos_s;
    Gtk::AccelLabel label_expo_pos_e;
    Gtk::AccelLabel label_precision;
    Gtk::AccelLabel label_sequence;
    Gtk::ColorButton m_color_button_neg_e;
    Gtk::ColorButton m_color_button_neg_s;
    Gtk::ColorButton m_color_button_pos_s;
    Gtk::ColorButton m_color_button_pos_e;
    Gtk::SpinButton m_select_precision;
    Gtk::Grid grid;
    uint precision = 100;

  public:
    Menu();
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
    void close_button();
    void set_color_button();
};

#endif // __SRC_MENU_HH__
