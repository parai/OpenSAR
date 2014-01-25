
#include "GtkCom.h"
#include "GtkFL.h"
#include "GtkTp.h"
#include <gtk/gtk.h>
// ============================= LOCAL FUNCTIONs  ======================
static void on_menu_activate  (GtkMenuItem* item,gpointer data);
static void on_toolbar_clicked(GtkButton *button,gpointer data);
// ============================= LOCAL  DATAs     ======================

// ============================= LOCAL  FUNCTIONs ======================
void on_menu_activate  (GtkMenuItem* item,gpointer data)
{

}

static void on_toolbar_clicked(GtkButton *button,gpointer data)
{
	g_print("You clicked on:  %s !\n",(gchar*)data);
}
static void on_start_stop_clicked(GtkButton *button,gpointer data)
{
	const gchar * stock_id = gtk_tool_button_get_stock_id(GTK_TOOL_BUTTON(button));

	if(0 == strcmp(stock_id,GTK_STOCK_MEDIA_PAUSE))
	{
		gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(button),GTK_STOCK_MEDIA_PLAY);
		FL_Start();
	}
	else
	{
		gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(button),GTK_STOCK_MEDIA_PAUSE);
		FL_Stop();
	}
}
static GtkWidget* CreateToolbar(void)
{
	GtkWidget* pToolbar;
	GtkToolItem* pToolItem;
	pToolbar=gtk_toolbar_new();

	//-- New Pause Start
	pToolItem=gtk_tool_button_new (NULL,NULL);
	gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(pToolItem),GTK_STOCK_MEDIA_PLAY);
	gtk_toolbar_insert(GTK_TOOLBAR(pToolbar),pToolItem,0);
	g_signal_connect(G_OBJECT (pToolItem), "clicked", G_CALLBACK(on_start_stop_clicked) , NULL);
	gtk_widget_show(GTK_WIDGET(pToolItem));
	// ----------
	gtk_toolbar_set_style(GTK_TOOLBAR(pToolbar),GTK_TOOLBAR_ICONS);
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(pToolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(pToolbar);
	return pToolbar;
}
// ============================= EXTERN FUNCTIONs ======================
void gtk_com_init(void)
{
	CanTp_Init();
	FL_Init();
}

void gtk_com_gui_init(GtkWindow *pWindow)
{

	// Initialize
	gtk_window_set_title(pWindow,(const gchar*)"GtkCom (Diagnostic and Communication)\n");
	gtk_container_set_border_width (GTK_CONTAINER (pWindow), 10);
	gtk_window_resize(pWindow,800,20);

	gtk_container_add(GTK_CONTAINER(pWindow),CreateToolbar());
}
