#include "../include/menu.hh"

Menu::Menu() : m_color_button_neg_s((Gtk::Stock::SELECT_COLOR)), 
               m_color_button_neg_e((Gtk::Stock::SELECT_COLOR)), 
               m_color_button_pos_s((Gtk::Stock::SELECT_COLOR)),
               m_color_button_pos_e(Gtk::Stock::SELECT_COLOR)
{
  Menu::set_position(Gtk::WIN_POS_CENTER);
  Menu::resize(450, 300);

  grid.set_row_homogeneous(true);
  grid.set_column_homogeneous(true);
  grid.set_column_spacing(0);
  grid.set_row_spacing(0);

  Menu::set_title("Параметры генерации фракталов Ляпунова");
   m_color_button_neg_s.set_can_focus(false);
   m_color_button_neg_s.set_alignment(0.1,0.5);
   m_color_button_neg_s.set_label("Цветовая экспонента Ляпунова отрицательная далеко от 0: ████");
   m_color_button_neg_s.signal_clicked().connect(sigc::mem_fun(*this, &Menu::set_color_max_neg_expo));
   grid.attach(m_color_button_neg_s,0,0,10,1);

   m_color_button_neg_e.set_can_focus(false);
   m_color_button_neg_e.set_alignment(0.1,0.5);
   m_color_button_neg_e.set_label("Цветовая экспонента Ляпунова отрицательная близка к 0: ████");
   m_color_button_neg_e.signal_clicked().connect(sigc::mem_fun(*this, &Menu::set_color_min_neg_expo));
   grid.attach(m_color_button_neg_e,0,1,10,1);

   m_color_button_pos_s.set_can_focus(false);
   m_color_button_pos_s.set_alignment(0.1,0.5);
   m_color_button_pos_s.set_label("Цветовая экспонента Ляпунова положительная близка к 0: ████");
   m_color_button_pos_s.signal_clicked().connect(sigc::mem_fun(*this, &Menu::set_color_min_pos_expo));
   grid.attach(m_color_button_pos_s,0,2,10,1);

   m_color_button_pos_e.set_can_focus(false);
   m_color_button_pos_e.set_alignment(0.1,0.5);
   m_color_button_pos_e.set_label("Цветовая экспонента Ляпунова положительная далека от 0: ████");
   m_color_button_pos_e.signal_clicked().connect(sigc::mem_fun(*this, &Menu::set_color_max_pos_expo));
   grid.attach(m_color_button_pos_e,0,3,10,1);

   Glib::RefPtr<Gtk::Adjustment> limits = Gtk::Adjustment::create(100, MINSPIN, MAXSPIN, 1);
   m_select_precision.set_adjustment(limits);
   m_select_precision.set_numeric();
   m_select_precision.signal_value_changed().connect(sigc::mem_fun(*this, &Menu::new_precision));
   grid.attach(m_select_precision,0,4,10,1);

   precision = m_select_precision.get_value_as_int();
   sequence.set_activates_default(false);

   Menu::text_area();
   grid.show_all();
   add(grid);

}
void Menu::set_color_max_neg_expo()
{
  Gtk::ColorSelectionDialog color("Цвет отрицательного значения наименьшего рассчитанного показателя Ляпунова ");
  Gtk::ColorSelection* select_color = color.get_color_selection();
  select_color->set_current_rgba(m_color[0]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK)
  {
    m_color[0] = select_color->get_current_rgba();
    m_color_button_neg_s.override_color(m_color[0], Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::set_color_min_neg_expo()
{
  Gtk::ColorSelectionDialog color("Цвет отрицательного значения рассчитанного наибольшего показателя Ляпунова ");
  Gtk::ColorSelection* select_color = color.get_color_selection();
  select_color->set_current_rgba(m_color[1]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK)
  {
    m_color[1] = select_color->get_current_rgba();
    m_color_button_neg_e.override_color(m_color[1], Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::set_color_min_pos_expo()
{
  Gtk::ColorSelectionDialog color("Цвет положительного значения рассчитанного наибольшего показателя Ляпунова");
  Gtk::ColorSelection* select_color = color.get_color_selection();
  select_color->set_current_rgba(m_color[2]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK)
  {
    m_color[2] = select_color->get_current_rgba();
    m_color_button_pos_s.override_color(m_color[2], Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::set_color_max_pos_expo()
{
  Gtk::ColorSelectionDialog color("Цвет отрицательного значения рассчитанного наибольшего показателя Ляпунова ");
  Gtk::ColorSelection* select_color = color.get_color_selection();
  select_color->set_current_rgba(m_color[3]);
  int end = color.run();
  if(end == Gtk::RESPONSE_OK)
  {
    m_color[3] = select_color->get_current_rgba();
    m_color_button_pos_e.override_color(m_color[3], Gtk::StateFlags::STATE_FLAG_NORMAL);
  }
}

void Menu::text_area()
{
  sequence.set_activates_default(false);
  sequence.set_placeholder_text("Введите желаемую последовательность A и B");
  sequence.set_max_length(precision);
  sequence.set_alignment(Gtk::ALIGN_CENTER);
  grid.attach(sequence,0,5,10,1);
}

void Menu::get_sequence(ofstream& file)
{
  string seq = sequence.get_text();
  file << "sequence= " + seq;
}

void Menu::new_precision()
{ m_select_precision.set_text(m_select_precision.get_text()); }

string Menu::color_to_string(Gdk::RGBA color)
{
  stringstream convertRed;
  stringstream convertGreen;
  stringstream convertBlue;
  convertRed    << (int)(color.get_red()    * 255);
  convertGreen  << (int)(color.get_green()  * 255);
  convertBlue   << (int)(color.get_blue()   * 255);
  string str = convertRed.str() + " " + convertGreen.str() + " " + convertBlue.str();
  return str;
}

void Menu::get_color(ofstream& file)
{
  file << "color_neg_+= "+ color_to_string(m_color[0]) << endl;
  file << "color_neg_-= "+ color_to_string(m_color[1]) << endl;
  file << "color_pos_-= "+ color_to_string(m_color[2]) << endl;
  file << "color_pos_+= "+ color_to_string(m_color[3]) << endl;
}

void Menu::get_precision(ofstream& file)
{
  stringstream precise;
  precise << m_select_precision.get_value();
  file << "precision= "+ precise.str()<< endl;
}

int Menu::write_file()
{
  ofstream file("config.txt");
  if (!file.is_open())
  {
    cout << "Error : Cannot open file config " << endl;
    return -1;
  }
  get_color(file);
  get_precision(file);
  get_sequence(file);
  return 0;
}