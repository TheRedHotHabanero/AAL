#include "../include/menu.hh"

Menu::Menu() : menu_write()
{
  Menu::set_position(Gtk::WIN_POS_CENTER);
  Menu::resize(450, 300);

  grid.set_row_homogeneous(true);
  grid.set_column_homogeneous(true);
  grid.set_column_spacing(0);
  grid.set_row_spacing(0);

  m_color[0].set_rgba(1.0, 1.0, 0);
  m_color[1].set_rgba(0.196, 0.118, 0);
  m_color[2].set_rgba(0, 0.753, 1.0);
  m_color[3].set_rgba(0, 0.11, 0.337);

  Menu::set_title("Параметры генерации фракталов Ляпунова");

  m_color_button_neg_s.set_rgba(m_color[0]);
  m_color_button_neg_e.set_rgba(m_color[1]);
  m_color_button_pos_s.set_rgba(m_color[2]);
  m_color_button_pos_e.set_rgba(m_color[3]);

  // min neg
  label_expo_neg_e = Gtk::AccelLabel("Минимальный отрицательный терминал");
  label_expo_neg_e.set_xalign(0.4);
  m_color_button_neg_e.set_can_focus(false);
  m_color_button_neg_e.set_alignment(0.1, 0.5);
  m_color_button_neg_e.set_title("Цветовая экспонента Ляпунова отрицательная далеко от 0");
  grid.attach(m_color_button_neg_e, 4, 0, 6, 1);
  grid.attach(label_expo_neg_e, 0, 0, 4, 1);

  // max neg
  label_expo_neg_s = Gtk::AccelLabel("Максимальный отрицательный терминал");
  label_expo_neg_s.set_xalign(0.409);
  m_color_button_neg_s.set_can_focus(false);
  m_color_button_neg_s.set_alignment(0.1, 0.5);
  m_color_button_neg_s.set_title("Цветовая экспонента Ляпунова отрицательная близка к 0");
  grid.attach(m_color_button_neg_s, 4, 1, 6, 1);
  grid.attach(label_expo_neg_s, 0, 1, 4, 1);

  // min pos
  label_expo_pos_s = Gtk::AccelLabel("Минимальный положительный терминал");
  label_expo_pos_s.set_xalign(0.395);
  m_color_button_pos_s.set_can_focus(false);
  m_color_button_pos_s.set_alignment(0.1, 0.5);
  m_color_button_pos_s.set_title("Цветовая экспонента Ляпунова положительная близка к 0");
  grid.attach(m_color_button_pos_s, 4, 2, 6, 1);
  grid.attach(label_expo_pos_s, 0, 2, 4, 1);

  // max pos
  label_expo_pos_e = Gtk::AccelLabel("Максимальный положительный терминал");
  label_expo_pos_e.set_xalign(0.4);
  m_color_button_pos_e.set_can_focus(false);
  m_color_button_pos_e.set_alignment(0.1, 0.5);
  m_color_button_pos_e.set_title("Цветовая экспонента Ляпунова положительная далека от 0:");
  grid.attach(m_color_button_pos_e, 4, 3, 6, 1);
  grid.attach(label_expo_pos_e, 0, 3, 4, 1);

  m_color_button_neg_e.signal_color_set().connect(sigc::mem_fun(*this, &Menu::set_color_min_neg_expo));
  m_color_button_neg_s.signal_color_set().connect(sigc::mem_fun(*this, &Menu::set_color_max_neg_expo));
  m_color_button_pos_s.signal_color_set().connect(sigc::mem_fun(*this, &Menu::set_color_min_pos_expo));
  m_color_button_pos_e.signal_color_set().connect(sigc::mem_fun(*this, &Menu::set_color_max_pos_expo));

  Glib::RefPtr<Gtk::Adjustment> limits = Gtk::Adjustment::create(100, MINSPIN, MAXSPIN, 1);
  label_precision = Gtk::AccelLabel("Точность");
  m_select_precision.set_adjustment(limits);
  m_select_precision.set_numeric();
  m_select_precision.signal_value_changed().connect(sigc::mem_fun(*this, &Menu::new_precision));
  label_precision.set_xalign(0.1);
  grid.attach(m_select_precision, 4, 4, 6, 1);
  grid.attach(label_precision, 0, 4, 4, 1);

  precision = m_select_precision.get_value_as_int();
  sequence.set_activates_default(false);
  label_sequence = Gtk::AccelLabel("Последовательность А-В");
  label_sequence.set_xalign(0.195);
  grid.attach(label_sequence, 0, 5, 4, 1);
  menu_write.signal_clicked().connect(sigc::mem_fun(*this, &Menu::write_file));
  menu_write.set_label("Подтверждение");
  grid.attach(menu_write, 8, 6, 2, 1);
  Menu::text_area();
  grid.show_all();
  add(grid);
}

void Menu::set_color_button()
{
  ifstream file("config.txt");
  string str;
  int r;
  int g;
  int b;
  int i = 0;
  while(i < 4 && file >> str >> r >> g >> b)
  {
    m_color[i].set_rgba(r / 255.0, g / 255.0, b / 255.0);
    i++;
  }
  m_color_button_neg_s.set_rgba(m_color[0]);
  m_color_button_neg_e.set_rgba(m_color[1]);
  m_color_button_pos_s.set_rgba(m_color[2]);
  m_color_button_pos_e.set_rgba(m_color[3]);
}

void Menu::set_color_min_neg_expo()
{ m_color[0] = m_color_button_neg_e.get_rgba(); }

void Menu::set_color_max_neg_expo()
{ m_color[1] = m_color_button_neg_s.get_rgba(); }

void Menu::set_color_min_pos_expo()
{ m_color[2] = m_color_button_pos_s.get_rgba(); }

void Menu::set_color_max_pos_expo()
{ m_color[3] = m_color_button_pos_e.get_rgba(); }

void Menu::text_area()
{
  sequence.set_activates_default(false);
  sequence.set_placeholder_text("Введите желаемую последовательность A и B");
  sequence.set_max_length(precision);
  sequence.set_alignment(Gtk::ALIGN_CENTER);
  grid.attach(sequence, 4, 5, 6, 1);
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

void Menu::write_file()
{
  ofstream file("config.txt");
  if (!file.is_open())
  {
    cout << "Error : Cannot open file config " << endl;
    return;
  }
  get_color(file);
  get_precision(file);
  get_sequence(file);
  return;
}