
#include "GtkCom.h"
#include "GtkFL.h"
#include "GtkTp.h"
#include <gtk/gtk.h>
// ============================= LOCAL TYPEs      ======================

// ============================= LOCAL FUNCTIONs  ======================
static void on_menu_activate  (GtkMenuItem* item,gpointer data);
//static void on_toolbar_clicked(GtkButton *button,gpointer data);
// ============================= LOCAL  DATAs     ======================
GtkTextBuffer *pFL_TextBuffer = NULL;
// ============================= LOCAL  FUNCTIONs ======================
static void on_menu_activate  (GtkMenuItem* item,gpointer data)
{
	g_print("You clicked on:  %s !\n",(gchar*)data);
}
#if 0
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
#endif
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
#if 0
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
#endif
static void on_fl_button_clicked(GtkButton *button,gpointer data)
{
	if(0==strcmp(data,"Start"))
	{
		FL_Start();
	}
	else if(0==strcmp(data,"Stop"))
	{
		FL_Stop();
	}
	else
	{

	}
}
void FL_PrintLog(gchar* Log,...)
{
	va_list args;
	unsigned long length;
	static char log_buf[1024];
	va_start(args, Log);
	length = vsprintf(log_buf,Log,args);
	va_end(args);

	GtkTextIter Iter;
	gtk_text_buffer_get_end_iter(pFL_TextBuffer,&Iter);
	gtk_text_buffer_insert(pFL_TextBuffer,&Iter,log_buf,length);
}
static GtkWidget* CreateNotebook_FL(void)
{
	GtkWidget* pBox;
	GtkWidget* pButton;

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	pButton = gtk_button_new_with_label("Start");
	gtk_box_pack_start(GTK_BOX(pBox),pButton,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT (pButton), "clicked", G_CALLBACK(on_fl_button_clicked) , "Start");

	pButton = gtk_button_new_with_label("Stop");
	gtk_box_pack_start(GTK_BOX(pBox),pButton,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT (pButton), "clicked", G_CALLBACK(on_fl_button_clicked) , "Stop");

	{
		GtkWidget *swindow;
		GtkWidget *textview;
		GtkTextIter Iter;
		swindow = gtk_scrolled_window_new (NULL, NULL);
		gtk_widget_set_size_request(swindow,800,600);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swindow),
									  GTK_POLICY_AUTOMATIC,
									  GTK_POLICY_AUTOMATIC);
		gtk_box_pack_start (GTK_BOX (pBox), swindow, TRUE, TRUE, 0);
		textview = gtk_text_view_new ();
		//gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
		gtk_text_view_set_editable(GTK_TEXT_VIEW (textview),FALSE);
		gtk_container_add (GTK_CONTAINER (swindow), textview);
		pFL_TextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
		gtk_text_buffer_get_end_iter(pFL_TextBuffer,&Iter);
		gtk_text_buffer_insert(pFL_TextBuffer,&Iter,"FL Log:\n",8);
	}
	return pBox;
}
static GtkWidget* CreateNotebook(void)
{
	GtkWidget* pNotebook;
	pNotebook = gtk_notebook_new ();
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),CreateNotebook_FL(),gtk_label_new("Flash Loader"));
	return pNotebook;
}
// ============================= EXTERN FUNCTIONs ======================
void gtk_com_init(void)
{
	CanTp_Init();
	FL_Init();
}

void gtk_com_gui_init(GtkWindow *pWindow)
{
	GtkWidget* pBox;
	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous(GTK_BOX(pBox),FALSE);

	gtk_container_add(GTK_CONTAINER (pWindow), pBox);

	// Initialize
	gtk_window_set_title(pWindow,(const gchar*)"GtkCom (Diagnostic and Communication)\n");
	gtk_container_set_border_width (GTK_CONTAINER (pWindow), 0);

	gtk_box_pack_start(GTK_BOX(pBox),CreateMenubar(),FALSE,FALSE,0);
//	gtk_box_pack_start(GTK_BOX(pBox),CreateToolbar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),CreateNotebook(),FALSE,FALSE,0);

}
