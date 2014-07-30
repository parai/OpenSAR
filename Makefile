#! Makefile for MicroSAR

target = MicroSAR

#Base Directory
root = .

src-dir = $(root)/source/c
obj-dir = $(root)/objs
bin-dir = $(root)/binaries

#common compilers
AS  = gcc
CC  = gcc
LD  = gcc
AR  = ar
RM  = rm

#common flags
cflags-y   = -c -g -std=gnu99 -O0 -Wall
ldflags-y  =    -g -std=gnu99 -O0 -Wall
ldflags-y += -lm
ldflags-y += -Wl,-Map,$(obj-dir)/$(TARGET).map	

def-y += -DPC_DEBUG

dir-y += $(src-dir)/include
dir-y += $(src-dir)/lib
dir-y += $(src-dir)/lib/include
dir-y += $(src-dir)/source/system
dir-y += $(src-dir)/source/system/os/osek
dir-y += $(src-dir)/source/system/os

ifeq ($(OS),)
$(warning OS not defined,default "make all OS=NON_BCC1")
dir-y += $(src-dir)/source/system/os/osek/bcc1/non_preempt
dir-y += $(src-dir)/source/system/os/osek/bcc1/non_preempt/example
else	
dir-y += $(src-dir)/source/system/os/$(OS)
dir-y += $(src-dir)/source/system/os/$(OS)/example
endif

VPATH += $(dir-y) 

inc-y += $(foreach x,$(dir-y),$(addprefix -I,$(x)))	 
	
obj-y += $(patsubst %.c,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.c)))))	


#common rules
$(obj-dir)/%.o:%.c
	@echo
	@echo "  >> CC $(notdir $<)"
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -MM -MF $(patsubst %.o,%.d,$@) -MT $@ $<
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -o $@ $<	
	
.PHONY:all clean

$(obj-dir):
	@mkdir -p $@
$(bin-dir):
	@mkdir -p $@

include $(wildcard $(obj-dir)/*.d)

all:$(obj-dir) $(bin-dir) $(obj-y)
	@echo ">> LD $(target)"
	@$(LD) $(obj-y) $(ldflags-y) -o $(bin-dir)/$(target)	

clean:
	@rm -fv $(obj-dir)/*
	
run:
	@start $(bin-dir)/$(target).exe
	