
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

def-y += 

#objects
dir-y = $(BD)/tools/arvfb/cmp			\
		$(BD)/tools/arvfb/cmp/vfb		\
		$(BD)/tools/arvfb/cmp/can		\
		$(BD)/tools/arvfb/cmp/lin		\
		$(BD)/tools/arvfb/cmp/dl		\
		$(BD)/tools/arvfb/cmp/tp		\
		$(BD)/tools/arvfb/cmp/fl
		
VPATH += $(dir-y)
		
obj-dir = $(BD)/tools/arvfb/objs
		
obj-y = $(patsubst %.c,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.c)))))	

#common rules
.SUFFIXES: .c

$(obj-dir)/%.o:%.c
	@echo
	@echo "  >> CC $(notdir $<)"
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -MM -MF $(patsubst %.o,%.d,$@) -MT $@ $<
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -o $@ $<

.PHONY:all clean

$(obj-dir):
	@mkdir -p $(obj-dir)

$(OUT):
	@mkdir -p $(OUT)

include $(wildcard $(obj-dir)/*.d)

all:$(obj-dir) $(obj-y) $(OUT)
	@$(LD) $(obj-y) $(ldflags-y) -o $(OUT)/$(TARGET) 
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET) DONE   <<<<<<<<<<<<<<<<<<<<<<"	

clean:
	@rm -fv $(obj-dir)/*

