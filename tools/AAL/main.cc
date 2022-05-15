#include "lyapunov/lyapunov.hh"

int main(int argc, char* argv[])
{
  srand(time(nullptr));
  (void) argc, (void) argv;
  Gtk::Main app(argc, argv);
  Menu m = Menu();
  Gtk::Main::run(m);
  Lyapunov lyapunov;
  lyapunov.generate();
  lyapunov.start_loop();
  return 0;
}
