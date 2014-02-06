#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "Os.h"
#include "internal.h"
#include "sys.h"
#include "irq_types.h"

// ====================================== MACROs ====================================

// ==================================== EXTENDs ===================================
extern void start_main(void);
extern OsTaskVarType Os_TaskVarList[OS_TASK_CNT];

extern GtkWidget* Lcd(void);
extern GtkWidget* Dio(void);
extern GtkWidget* Com(void);

// ====================================== TYPEs ====================================
static GTimer* pSysTimer;
static gboolean isPaused = FALSE;

// ====================================== DATAs ====================================


// ====================================== LOCAL FUNCTIONs ====================================
static void arch_task_runnable(void)
{
	// In this arch, no support of ECC
	if((NULL!=Os_Sys.currTaskPtr) && (ST_RUNNING == Os_Sys.currTaskPtr->state))
	{	// TODO: support of WAITING is not supported
		Os_Sys.currTaskPtr->constPtr->entry();
	}
}
static void arch_system_timer(void)
{
	gdouble elapsed;
	gulong  elapsed_microseconds;

	elapsed = g_timer_elapsed(pSysTimer,NULL); // unit in
	elapsed_microseconds = (elapsed*1000);
	while(elapsed_microseconds>0)
	{
		elapsed_microseconds--;
		OsTick();
		if(0==elapsed_microseconds)
		{
			g_timer_start(pSysTimer);
		}
	}
}

static gboolean arch_daemon(gpointer data)
{
	if(FALSE == isPaused)
	{
		arch_system_timer();
		arch_task_runnable();
	}
	return TRUE;
}
static void arch_init_daemon(void)
{
	g_type_init ();	// for glib socket
	start_main();


	//
	pSysTimer = g_timer_new();
	g_idle_add(arch_daemon,NULL);

}
// --------------- Gui Action ----------------
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
// --------------- Gui Target ----------------
static GtkWidget* Menubar(void)
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

static GtkWidget* Toolbar(void)
{
	GtkWidget* pToolbar;
	GtkToolItem* pToolItem;
	pToolbar=gtk_toolbar_new();

	//-- New Pause Start
	pToolItem=gtk_tool_button_new (NULL,NULL);
	gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(pToolItem),GTK_STOCK_MEDIA_PAUSE);
	gtk_toolbar_insert(GTK_TOOLBAR(pToolbar),pToolItem,0);
	g_signal_connect(G_OBJECT (pToolItem), "clicked", G_CALLBACK(on_start_stop_clicked) , NULL);

	// ----------
	gtk_toolbar_set_style(GTK_TOOLBAR(pToolbar),GTK_TOOLBAR_ICONS);
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(pToolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);

	return pToolbar;
}
static GtkWidget*  Notebook(void)
{
	GtkWidget* pNotebook;
	pNotebook = gtk_notebook_new ();
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),Lcd(),gtk_label_new("Lcd"));
#ifdef USE_DIO
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),Dio(),gtk_label_new("Dio"));
#endif
#ifdef USE_COM
	gtk_notebook_append_page (GTK_NOTEBOOK(pNotebook),Com(),gtk_label_new("Com"));
#endif
	return pNotebook;
}

// ====================================== FUNCTIONs ====================================
void OsIdle(void)
{
}
boolean arch_is_paused(void)
{
	return isPaused;
}
imask_t arch_save_int(void)
{
	return 0;
}
void arch_restore_int(imask_t flags)
{
}
void arch_disable_int(void)
{

}
void arch_enable_int(void)
{

}

void arch_generate_irqn(IrqType IRQn)
{
	Os_Isr(NULL,IRQn);
}

void Os_ArchInit(void) {
}
void Os_ArchFirstCall( void ) {
	// TODO: make switch here... for now just call func.
	Irq_Enable();
	Os_Sys.currTaskPtr->constPtr->entry();
}
unsigned int Os_ArchGetScSize( void ) {
	return 0u;
}
void Os_ArchSetTaskEntry(OsTaskVarType *pcbPtr ) {
}

void Os_ArchSetupContext( OsTaskVarType *pcb ) {

}
void Os_ArchSetSpAndCall(void *sp, void (*f)(void) ) {

}
void Os_ArchSwapContext(OsTaskVarType *old,OsTaskVarType *new) {

}

void Os_ArchSwapContextTo(OsTaskVarType *old,OsTaskVarType *new){

}
void *Os_ArchGetStackPtr( void ) {

	return NULL; // Simualte on Win32
}
int main( int argc, char *argv[] )
{

	GtkWidget* pWindow;
	GtkWidget* pBox;
	gtk_init (&argc, &argv);
	pWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WIDGET(pWindow),"https://github.com/parai/OpenSAR.git\n");
	gtk_window_resize(GTK_WIDGET(pWindow),800,40);

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous(GTK_BOX(pBox),FALSE);
	gtk_container_add(GTK_CONTAINER (pWindow), pBox);
	gtk_box_pack_start(GTK_BOX(pBox),Menubar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),Toolbar(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(pBox),Notebook(),FALSE,FALSE,0);

	gtk_widget_show_all (pWindow);
	g_signal_connect (pWindow, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	arch_init_daemon();
	gtk_main ();

	return ( 0 ) ;
}
