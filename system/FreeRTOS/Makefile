
# Makefile for AutoSAR VFB running on Gtk
# >>>>>>>> Config <<<<<<<<<<<<<<<<<
TARGET = FreeRTOS

#Base Directory
BD = ../..

OUT = $(BD)/binaries/FreeRTOS

#common compilers
AS  = gcc
CC  = gcc
LD  = gcc
AR  = ar
RM  = rm

#common flags
cflags-y   = -c -g -std=gnu99 -O2 -Wall
ldflags-y  = -g -std=gnu99 -O2 -Wall
#cflags-y  += `pkg-config --cflags gtk+-3.0`
#ldflags-y += `pkg-config --cflags gtk+-3.0` \
#			 `pkg-config --libs gtk+-3.0 glib-2.0 gthread-2.0` 	
ldflags-y += -lm -lwinmm	

inc-y += -I$(BD)/include
inc-y += -I$(BD)/include/generic
inc-y += -I$(BD)/system/FreeRTOS/Source/include
inc-y += -I$(BD)/system/FreeRTOS/Source/portable/MSVC-MingW
inc-y += -I$(BD)/system/FreeRTOS/Config
inc-y += -I$(BD)/system/kernel/include
inc-y += -I$(BD)/arch/generic/gtk/kernel
inc-y += -I$(BD)/app/config/GEN
inc-y += -I$(BD)/app/config

def-y += -DWIN32
#-DDEBUG_ENABLED

#objects
dir-y = $(BD)/system/FreeRTOS/Source						\
		$(BD)/system/FreeRTOS/Config						\
		$(BD)/system/FreeRTOS/Source/portable/MSVC-MingW
		
VPATH += $(dir-y)	\
		$(BD)/system/FreeRTOS/Source/portable/MemMang	\
		$(BD)/app/config/GEN
		
obj-dir = $(OUT)/objs

ldflags-y += -Wl,-Map,$(obj-dir)/$(TARGET).map	
	
obj-y += $(patsubst %.c,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.c)))))	
#obj-y += $(patsubst %.cpp,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.cpp)))))	

# special objs
obj-y += $(obj-dir)/heap_3.o	\
		 $(obj-dir)/Os_Cfg.o

#common rules
.SUFFIXES: .c .cpp

$(obj-dir)/%.o:%.cpp
	@echo
	@echo "  >> CC $(notdir $<)"
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -MM -MF $(patsubst %.o,%.d,$@) -MT $@ $<
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -o $@ $<
	
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

GEN:

POST:

all:$(obj-dir) $(obj-y) $(OUT) POST
	@echo ">> LD $(TARGET)"
	@$(LD) $(obj-y) $(ldflags-y) -o $(OUT)/$(TARGET) 
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET) DONE   <<<<<<<<<<<<<<<<<<<<<<"	

clean:
	@rm -fv $(obj-dir)/*
