/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
#include "arvfb.h"

// ======================== [ IMPORTS        ] ==============================================
extern void ArScriptLoad(const char* file);
extern void ArScriptRun(void);

// ======================== [ LOCAL VARIANTS  ] ==============================================
static boolean isPaused = TRUE;

static GtkTextBuffer *pTextBuffer = NULL;

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
	g_signal_connect(G_OBJECT(pSub), "activate",G_CALLBACK(on_menu_activate), (gpointer)"Open");

	pSub = gtk_menu_item_new_with_label("Save");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pSub);
	g_signal_connect(G_OBJECT(pSub), "activate",G_CALLBACK(on_menu_activate), (gpointer)"Save");

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

static void on_button_clicked(GtkButton *button,gpointer data)
{
	if(0==strcmp((const char*)data,"Run"))
	{
		ArScriptRun();
	}
	else
	{

	}
}

static void on_file_set(GtkFileChooser *button)
{
	const char* file = (const char*)gtk_file_chooser_get_filename(button);
	ArScriptLoad(file);
}

static GtkWidget* Console(void)
{
	GtkWidget* pBox;

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	GtkWidget* pButton;

	pButton = gtk_file_chooser_button_new("Load Script",GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_box_pack_start(GTK_BOX(pBox),pButton,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT (pButton), "file_set", G_CALLBACK(on_file_set) , (gpointer)NULL);
	GtkFileFilter* filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.svl");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pButton),filter);

	pButton = gtk_button_new_with_label("Run");
	gtk_box_pack_start(GTK_BOX(pBox),pButton,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT (pButton), "clicked", G_CALLBACK(on_button_clicked) , (gpointer)"Run");

	{
		GtkWidget *swindow;
		GtkWidget *textview;
		GtkTextIter Iter;
		swindow = gtk_scrolled_window_new (NULL, NULL);
		gtk_widget_set_size_request(swindow,800,500);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swindow),
									  GTK_POLICY_AUTOMATIC,
									  GTK_POLICY_AUTOMATIC);
		gtk_box_pack_start (GTK_BOX (pBox), swindow, TRUE, TRUE, 0);
		textview = gtk_text_view_new ();
		//gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
		gtk_text_view_set_editable(GTK_TEXT_VIEW (textview),FALSE);
		gtk_container_add (GTK_CONTAINER (swindow), textview);
		pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
		gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
		Arch_Trace("Arvfb Console:\n");
	}
	return pBox;
}

static GtkWidget* CreateNotebook(void)
{
	GtkWidget* pNotebook;
	pNotebook = gtk_notebook_new ();
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),
							ArCan(),
							gtk_label_new("Can"));
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),
							ArFlashLoader(),
							gtk_label_new("FlashLoader"));
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),
							ArCom(),
							gtk_label_new("Com"));
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),
								Console(),
								gtk_label_new("Console"));
	return pNotebook;
}

static void Initialize(void)
{
	ArCan_Init();
	ArTp_Init();
	ArFl_Init();
	ArCom_Init();
}

static gboolean Schedule(gpointer data)
{
	if(!isPaused)
	{
		ArCan_Schedule();
		ArTp_Schedule();
		ArFl_Schedule();
		ArCom_Schedule();
	}

	return TRUE;
}


void Arch_Trace(const char* format,...)
{
	va_list args;
	unsigned long length;
	static char log_buf[1024];
	va_start(args, format);
	length = vsprintf(log_buf,format,args);
	va_end(args);

	GtkTextIter Iter;
	gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
	gtk_text_buffer_insert(pTextBuffer,&Iter,log_buf,length);

	gtk_text_iter_forward_visible_cursor_position(&Iter);
}

int main (int argc, char *argv[])
{
	g_type_init ();
	g_thread_init(NULL);
	gdk_threads_init();

	gtk_init (&argc, &argv);

	GtkWidget *window;
	GtkWidget* pBox;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_title(GTK_WINDOW(window),(const gchar*)"arvfb (parai@foxmail.com)\n");


	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous(GTK_BOX(pBox),FALSE);

	gtk_container_add(GTK_CONTAINER (window), pBox);
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);

	// Initialize
	Initialize();

	ArParser(argc,argv);

	printf("Initialize Done.\n");



	gtk_box_pack_start(GTK_BOX(pBox),CreateMenubar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),CreateToolbar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),CreateNotebook(),FALSE,FALSE,0);

	gtk_widget_show_all (window);


	g_idle_add(Schedule,NULL);
	g_timeout_add(1,Schedule,NULL);


	gtk_main ();
	return 0;

}
