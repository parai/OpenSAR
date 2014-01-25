#include "GtkCan.h"
#include "GtkFL.h"
#include "GtkTp.h"

int main (int argc, char *argv[])
{
	g_type_init ();


	CanTp_Init();
	FL_Init();



	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	return 0;

}
