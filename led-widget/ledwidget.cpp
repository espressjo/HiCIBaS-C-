#include "ledwidget.h"

ledWidget::ledWidget(){
	
	css_provider = Gtk::CssProvider::create();
	deactivate();
	set_sensitive(false);
	active_state = false;

}
bool ledWidget::isActive()
{
	return active_state;
}
void ledWidget::activate()
{
	css_provider->load_from_data(
		"check {background-image: radial-gradient(ellipse at center, white 0%, #33c435 40%);}\
		");
	get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	active_state = true;
}
void ledWidget::activate_red()
{
	css_provider->load_from_data(
		"check {background-image: radial-gradient(ellipse at center, white 0%, #b03131 40%);}\
		");
	get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	active_state = true;
}
void ledWidget::deactivate()
{
	css_provider->load_from_data(
		"check {background-image: radial-gradient(ellipse at center,white 0%, #919991 40%);}\
		");
	get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	active_state = false;
}
ledWidget::~ledWidget(){
	
	

}