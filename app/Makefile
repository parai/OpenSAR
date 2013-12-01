# Makefile for the OpenRTOS part of OpenOSEK

#output EXE
TARGET = OpenSAR

#Base Directory
BD = ..

OUT = $(BD)/out

#common compilers
AS = gcc
CC = gcc
LD = gcc
AR = ar
RM = rm

#common flags
ASFLGS = 
INCDIR = 	-I$(BD)/include 		\
			-I$(BD)/include/generic \
			-I$(BD)/include/sys		\
			-I$(BD)/system/kernel/include \
			-I$(BD)/app/config	\
			-I$(BD)/app/inc	\
			-I$(BD)/arch/generic/mingw/kernel	\
			-I$(BD)/system/EcuM	

MACROS = -DWIN32 -DUSE_DEBUG_PRINTF
CCFLGS = -c -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR)
LDFLGS = -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR) -lwsock32
ARFLGS = rv
RMFLGS = -f	

#common objectives		
dir-y =	$(BD)/app/config	\
		$(BD)/arch/generic/mingw/kernel	\
		$(BD)/arch/generic/mingw/drivers	\
		$(BD)/app/src		\
		$(BD)/communication/CanIf
		
src-y = $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.c)))
obj-y = $(subst .c,.o,$(src-y))	
# kernel files
obj-y += 	$(BD)/system/kernel/alarm.o	\
			$(BD)/system/kernel/application.o	\
			$(BD)/system/kernel/counter.o	\
			$(BD)/system/kernel/event.o	\
			$(BD)/system/kernel/isr.o	\
			$(BD)/system/kernel/resource.o	\
			$(BD)/system/kernel/sched_table.o	\
			$(BD)/system/kernel/task.o

# common library needed for OpenSAR
lib-y = 

#common rules
%.o:%.c
	@echo compile $<  to $@
	@$(CC) $(CCFLGS) -o $@ $<

.PHONY:all clean gen

gen:
			
all:$(obj-y)
	@$(LD) $(BD)/system/kernel/init.c -o $(OUT)/$(TARGET) $(obj-y) $(LDFLGS)
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET).EXE DONE   <<<<<<<<<<<<<<<<<<<<<<"

run:
	@(start python $(OUT)/CanBusServer.py --server 8000)
	@(start $(OUT)/$(TARGET).exe)	
clean:
	@$(RM) -f $(OUT)/$(TARGET).exe $(obj-y)
	@echo ">>>>>>>>>>>>>>>>>>>> CLEAN $(TARGET) Objs Done <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"		