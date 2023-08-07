#include "guiding_window.h"
#include <gtkmm/application.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "hicibas.guiding");

	GuidingWindow window;
    /*
	if (fork()==0)
	{
		if (system("ds9")!=0)
		{
			fprintf(stderr,"[Warning] ds9 not installed!\n");
			return 0;
		}	
	}
     */
  //Shows the window and returns when it is closed.
  return app->run(window);
}
