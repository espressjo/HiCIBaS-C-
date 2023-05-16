#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <gtkmm.h>
#include <gtkmm/checkbutton.h>

class ledWidget : public Gtk::CheckButton
{
	
public:
		ledWidget();
		virtual ~ledWidget();
		void activate();
		void activate_red();
		void deactivate();
		bool isActive();
private:
	Glib::RefPtr<Gtk::CssProvider> css_provider;
	bool active_state;
	
};




#endif