
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
inc-y += -I$(BD)/tools/arvfb/source/parser
inc-y += -I$(BD)/include

def-y += -D__GTK__

#objects
dir-y = $(BD)/tools/arvfb/source	\
		$(BD)/tools/arvfb/source/parser
		
VPATH += $(dir-y)
		
obj-dir = $(BD)/tools/arvfb/objs
		
obj-y += $(patsubst %.c,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.c)))))	

bison-y = $(patsubst %.y,%.yc, $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.y))))

flex-y  = $(patsubst %.f,%.yc, $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.f))))

#common rules
.SUFFIXES: .c .y .f

$(obj-dir)/%.o:%.c
	@echo
	@echo "  >> CC $(notdir $<)"
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -MM -MF $(patsubst %.o,%.d,$@) -MT $@ $<
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -o $@ $<
	
%.yc:%.y
	@echo
	@echo "  >> bison $(notdir $<)"
	@bison $< -o $@
	@rm -v $(patsubst %.yc,%.output,$@)
	
%.yc:%.f
	@echo
	@echo "  >> flex $(notdir $<)"
	@flex $<
	@mv -v lex.yy.c $@
	
.PHONY:all clean

$(obj-dir):
	@mkdir -p $(obj-dir) $(flex-y)

$(OUT):
	@mkdir -p $(OUT)

include $(wildcard $(obj-dir)/*.d)

cly:
	@rm -fv $(bison-y) $(flex-y)

all:$(obj-dir) cly $(bison-y) $(flex-y) $(obj-y) $(OUT)
	@echo $(obj-y)
	@echo ">> LD $(TARGET)"
	@$(LD) $(obj-y) $(ldflags-y) -o $(OUT)/$(TARGET) 
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET) DONE   <<<<<<<<<<<<<<<<<<<<<<"	

clean:cly
	@rm -fv $(obj-dir)/*
