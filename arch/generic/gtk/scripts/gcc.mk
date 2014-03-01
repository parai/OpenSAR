# Makefile for AutoSAR simulator running on Gtk
# Hope better quality of true-time 
# >>>>>>>> Config <<<<<<<<<<<<<<<<<
# System: Linux Windows
SYSTEM=Windows

ifeq ($(SYSTEM),Linux)

cflags-y += `pkg-config --cflags gtk+-3.0`
ldflags-y += `pkg-config --cflags gtk+-3.0` \
			 `pkg-config --libs gtk+-3.0 glib-2.0 gthread-2.0` \
			 -lpthread
	
	 
else
ifeq ($(SYSTEM),Windows)
Gtk-InstallDir = D:/usr/opt/gtk
cflags-y += `$(Gtk-InstallDir)/bin/pkg-config --cflags gtk+-3.0`
ldflags-y += `$(Gtk-InstallDir)/bin/pkg-config --cflags gtk+-3.0` \
		  `$(Gtk-InstallDir)/bin/pkg-config --libs gtk+-3.0 glib-2.0 gthread-2.0` \
	      -lpthread
else
error "Error Config"
endif
endif		 


#common flags

inc-y = 	-I$(ROOTDIR)/arch/generic/gtk/drivers/tool/inc	\
		 	-I$(ROOTDIR)/arch/generic/gtk/drivers/tool/Com/inc