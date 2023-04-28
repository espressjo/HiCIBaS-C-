#include "HiCIBaS_window.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "hicibas.gui.example");

  HiCIBaSWindow window;

  //Shows the window and returns when it is closed.
  return app->run(window);
}
