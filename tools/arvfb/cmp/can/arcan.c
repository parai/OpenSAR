
#include "arvfb.h"
// ========================== [ LOCAL MACROS    ] ===============================
#define mDefaultBus  (const gchar*)"8000"


// ========================== [ LOCAL VARIANTS  ] ===============================
static GtkTextBuffer *pTextBuffer = NULL;


// ========================== [ LOCAL FUNCTIONS ] ===============================
static void Trace(const gchar* format,...)
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
}

static void
text_changed_cb (GtkEntry   *entry,
                 GParamSpec *pspec,
                 GtkWidget  *button)
{
  gboolean has_text;

  has_text = gtk_entry_get_text_length (entry) > 0;

  if(has_text)
  {
	  Trace( gtk_entry_get_text(entry) );
  }
}

GtkWidget* ArCan(void)
{
	GtkWidget* pBox;

	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	// Bus Slection
	{
		GtkWidget* pBox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
		gtk_box_pack_start (GTK_BOX (pBox2), gtk_label_new("Bus:"), TRUE, TRUE, 0);
		GtkWidget* pBus = gtk_entry_new();
		g_signal_connect (pBus, "notify::text",
		                  G_CALLBACK (text_changed_cb), NULL);
		gtk_entry_set_text(GTK_ENTRY(pBus),mDefaultBus);
		gtk_box_pack_start (GTK_BOX (pBox2), pBus, TRUE, TRUE, 0);

		gtk_box_pack_start (GTK_BOX (pBox), pBox2, TRUE, TRUE, 0);
	}

	// Trace
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
		pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
		gtk_text_buffer_get_end_iter(pTextBuffer,&Iter);
	}
	return pBox;
}
