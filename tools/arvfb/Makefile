
# Makefile for AutoSAR VFB running on Gtk
# >>>>>>>> Config <<<<<<<<<<<<<<<<<
TARGET = arvfb

#Base Directory
BD = ../..

OUT = $(BD)/binaries/gtk

#common compilers
AS  = gcc
CC  = gcc
LD  = gcc
AR  = ar
RM  = rm

#common flags
cflags-y   = -c -g -std=gnu99 -O2 -Wall
ldflags-y  = -g -std=gnu99 -O2 -Wall
cflags-y  += `pkg-config --cflags gtk+-3.0`
ldflags-y += `pkg-config --cflags gtk+-3.0` \
			 `pkg-config --libs gtk+-3.0 glib-2.0 gthread-2.0` \
			 -lpthread -lm

inc-y += -I$(BD)/tools/arvfb/include
inc-y += -I$(BD)/include

#objects
dir-y = $(BD)/tools/arvfb/cmp			\
		$(BD)/tools/arvfb/cmp/can		\
		$(BD)/tools/arvfb/cmp/lin
		
src-y = $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.c)))
obj-y = $(subst .c,.o,$(src-y))	

#common rules
.SUFFIXES: .c
%.o:%.c
	@echo
	@echo "  >> CC $(notdir $<)"
	@$(CC) $(cflags-y) $(inc-y) -o $@ $<

.PHONY:all clean

all:$(obj-y) $(OUT)
	@$(LD) $(obj-y) $(ldflags-y) -o $(OUT)/$(TARGET) 
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET) DONE   <<<<<<<<<<<<<<<<<<<<<<"	

clean:
	@rm -fv $(obj-y)

