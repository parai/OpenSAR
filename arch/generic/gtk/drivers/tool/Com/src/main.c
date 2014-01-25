#include "GtkCom.h"

int main (int argc, char *argv[])
{
	g_type_init ();
	gtk_init (&argc, &argv);

	gtk_com_init();

	GtkWidget *window;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	gtk_com_gui_init(GTK_WINDOW(window));

	gtk_widget_show_all (window);

	gtk_main ();
	return 0;

}
