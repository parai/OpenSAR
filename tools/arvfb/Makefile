
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
cflags-y   = -c -g --std=gnu99 -O2 -Wall
ldflags-y  = -g --std=gnu99 -O2 -Wall
cflags-y  += `pkg-config --cflags gtk+-3.0`
ldflags-y += `pkg-config --cflags gtk+-3.0` \
			 `pkg-config --libs gtk+-3.0 glib-2.0 gthread-2.0` \
			 -lpthread -lm		

inc-y += -I$(BD)/tools/arvfb/include
inc-y += -I$(BD)/tools/arvfb/source/parser
inc-y += -I$(BD)/tools/arvfb/source/lua
inc-y += -I$(BD)/include

def-y += -DUSE_GTK
#-DDEBUG_ENABLED

#objects
dir-y = $(BD)/tools/arvfb/source			\
		$(BD)/tools/arvfb/source/parser		\
		$(BD)/tools/arvfb/source/lua
		
VPATH += $(dir-y)
		
obj-dir = $(BD)/tools/arvfb/objs

ldflags-y += -Wl,-Map,$(obj-dir)/arvfb.map	
	
obj-y += $(patsubst %.c,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.c)))))	
obj-y += $(patsubst %.cpp,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.cpp)))))	


#common rules
.SUFFIXES: .c .cpp .y .f

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
	@mkdir -p $(obj-dir) $(flex-y)

$(OUT):
	@mkdir -p $(OUT)

include $(wildcard $(obj-dir)/*.d)

GEN:
	@echo "  >> flex grammar.f"
	@flex $(BD)/tools/arvfb/source/parser/grammar.f
	@mv -fv lex.yy.c $(BD)/tools/arvfb/source/parser/grammar.fc
	@echo "  >> bison rule.y"
	@bison  $(BD)/tools/arvfb/source/parser/rule.y -o $(BD)/tools/arvfb/source/parser/rule.yc
	@rm -fv $(BD)/tools/arvfb/source/parser/rule.output
	
	@echo "  >> flex lex.f"
	@flex $(BD)/tools/arvfb/source/smalvl/lex.f
	@mv -fv lex.yy.c $(BD)/tools/arvfb/source/smalvl/lex.yy.cpp
	@echo "  >> bison bison.y"
	@bison  -v -t -d $(BD)/tools/arvfb/source/smalvl/bison.y
	@mv -v bison.tab.c  $(BD)/tools/arvfb/source/smalvl/bison.tab.cpp
	@mv -v bison.tab.h	$(BD)/tools/arvfb/source/smalvl/
	@rm -fv bison.output

POST:
	@cp -rv $(BD)/tools/arvfb/example   $(OUT)
	@mv -fv  $(OUT)/example/libc.ext    $(OUT)
	@mv -fv  $(OUT)/example/lib.inc    $(OUT)
	@echo Hello World! > $(OUT)/source_file.txt
	@echo >> $(OUT)/source_file.txt
	@echo >> $(OUT)/source_file.txt
	@echo orange >> $(OUT)/source_file.txt
	@echo pear >> $(OUT)/source_file.txt
	@echo apple >> $(OUT)/source_file.txt
	@echo banana >> $(OUT)/source_file.txt

all:$(obj-dir) $(obj-y) $(OUT)
	@echo $(obj-y)
	@echo "  >> LD $(TARGET)"
	@$(LD) $(obj-y) $(ldflags-y) -o $(OUT)/$(TARGET) 
	@echo "  >>>>>>>>>>>>>>>>>  BUILD $(TARGET) DONE   <<<<<<<<<<<<<<<<<<<<<<"	

clean:
	@rm -fv $(obj-dir)/*
