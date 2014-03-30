#include "arvfb.h"


// ======================== [ LOCAL VARIANTS  ] ==============================================
static boolean isPaused = FALSE;

// ======================== [ LOCAL FUNCTIONS ] ==============================================
static void on_menu_activate  (GtkMenuItem* item,gpointer data)
{
	g_print("You clicked on:  %s !\n",(gchar*)data);
}

static void on_start_stop_clicked(GtkButton *button,gpointer data)
{
	const gchar * stock_id = gtk_tool_button_get_stock_id(GTK_TOOL_BUTTON(button));

	if(0 == strcmp(stock_id,GTK_STOCK_MEDIA_PAUSE))
	{
		gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(button),GTK_STOCK_MEDIA_PLAY);
		isPaused = TRUE;
	}
	else
	{
		gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(button),GTK_STOCK_MEDIA_PAUSE);
		isPaused = FALSE;
	}
}
static GtkWidget* CreateMenubar(void)
{
	GtkWidget* pMenubar;
	GtkWidget* pMenu;
	GtkWidget* pMain;
	GtkWidget* pSub;

	pMenubar = gtk_menu_bar_new();

//{  FILE START
	pMenu = gtk_menu_new();
	pMain = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenubar), pMain);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMain), pMenu);
	//{
	pSub = gtk_menu_item_new_with_label("Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pSub);
	g_signal_connect(G_OBJECT(pSub), "activate",G_CALLBACK(on_menu_activate), "Open");

	pSub = gtk_menu_item_new_with_label("Save");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pSub);
	g_signal_connect(G_OBJECT(pSub), "activate",G_CALLBACK(on_menu_activate), "Save");

	pSub = gtk_menu_item_new_with_label("-----");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pSub);

	pSub = gtk_menu_item_new_with_label("Quit");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pSub);
	g_signal_connect(G_OBJECT(pSub), "activate",G_CALLBACK(gtk_main_quit), NULL);
	//}
//} FILE END

	gtk_widget_show(pMenubar);
	return pMenubar;
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

static GtkWidget* CreateNotebook(void)
{
	GtkWidget* pNotebook;
	pNotebook = gtk_notebook_new ();
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),
			ArCan(),
			gtk_label_new("arcan"));
	return pNotebook;
}

int main (int argc, char *argv[])
{
	g_type_init ();
	gtk_init (&argc, &argv);

	GtkWidget *window;
	GtkWidget* pBox;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_title(window,(const gchar*)"arvfb (parai@foxmail.com)\n");


	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous(GTK_BOX(pBox),FALSE);

	gtk_container_add(GTK_CONTAINER (window), pBox);

	// Initialize
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);

	gtk_box_pack_start(GTK_BOX(pBox),CreateMenubar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),CreateToolbar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),CreateNotebook(),FALSE,FALSE,0);

	gtk_widget_show_all (window);

	gtk_main ();
	return 0;

}
