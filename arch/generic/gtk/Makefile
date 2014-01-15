
# Makefile for AutoSAR simulator running on Gtk
# Hope better quality of true-time 

#output EXE
TARGET = OpenSAR

#Base Directory
BD = ../../..

OUT = $(BD)/out

#common compilers
AS  = gcc
CC  = gcc
LD  = gcc
AR  = ar
RM  = rm

# Flags For Gtk
Gtk-InstallDir    = D:/usr/opt/gtk
Gtk-INC    = 
Gtk-Macros = -D__GTK__
Gtk-CCFLGS = `$(Gtk-InstallDir)/bin/pkg-config --cflags gtk+-3.0`
Gtk-LDFLGS = `$(Gtk-InstallDir)/bin/pkg-config --cflags gtk+-3.0` \
			 `$(Gtk-InstallDir)/bin/pkg-config --libs gtk+-3.0 glib-2.0 gthread-2.0` \
			 -lpthread


#common flags
ASFLGS = 
INCDIR = 	$(Gtk-INC)				\
			-I$(BD)/include 		\
			-I$(BD)/include/generic \
			-I$(BD)/include/sys		\
			-I$(BD)/system/kernel/include \
			-I$(BD)/app/config	\
			-I$(BD)/app/config/GEN	\
			-I$(BD)/app/inc			\
			-I$(BD)/arch/generic/gtk/kernel	\
			-I$(BD)/system/EcuM			\
			-I$(BD)/system/SchM			\
			-I$(BD)/diagnostic/Dcm		\
			-I$(BD)/communication/Com

MACROS = 	$(Gtk-Macros)		\
			-DWIN32 			\
			-DAUTOSAR_SIMULATOR	\
			-DUSE_DEBUG_PRINTF	\
			-DUSE_ECUM		\
			-DUSE_CAN		\
			-DUSE_CANIF	\
			-DUSE_CANTP	\
			-DUSE_PDUR	\
			-DUSE_COM	\
			-DUSE_DCM	\
			-DUSE_COMM	\
			-DUSE_CANNM	\
			-DUSE_DET	\
			-DUSE_CANNM	\
			-DUSE_NM	\
			-DUSE_CANSM	\
			-DUSE_MCU	\
			-DUSE_PORT	\
			-DUSE_DIO	\
			-DUSE_FLS	\
			-DUSE_FEE	\
			-DcfgCPU_ENDIAN=COM_LITTLE_ENDIAN	\
			-DUSE_NVM	\
			-DUSE_MEMIF	

CCFLGS = -c -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR) $(Gtk-CCFLGS)
LDFLGS = -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR) $(Gtk-LDFLGS) 
ARFLGS = rv
RMFLGS = -f	

#common objectives		
dir-y =	$(BD)/app/config	\
		$(BD)/app/config/GEN			\
		$(BD)/arch/generic/gtk/kernel	\
		$(BD)/arch/generic/gtk/drivers	\
		$(BD)/app/src		\
		$(BD)/app/src/Dia		\
		$(BD)/communication/CanIf	\
		$(BD)/communication/CanTp	\
		$(BD)/communication/PduR	\
		$(BD)/communication/CanNm	\
		$(BD)/communication/Nm		\
		$(BD)/communication/CanSM	\
		$(BD)/communication/Com	\
		$(BD)/communication/ComM	\
		$(BD)/diagnostic/Dcm		\
		$(BD)/diagnostic/Det		\
		$(BD)/system/EcuM			\
		$(BD)/system/SchM			\
		$(BD)/memory/Fee			\
		$(BD)/memory/NvM			\
		$(BD)/memory/MemIf			\
		$(BD)/system/Crc
		
src-y = $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.c)))
obj-y = $(subst .c,.o,$(src-y))	

# kernel files
obj-y +=	$(BD)/system/kernel/alarm.o	\
			$(BD)/system/kernel/application.o	\
			$(BD)/system/kernel/counter.o	\
			$(BD)/system/kernel/event.o	\
			$(BD)/system/kernel/isr.o	\
			$(BD)/system/kernel/resource.o	\
			$(BD)/system/kernel/sched_table.o	\
			$(BD)/system/kernel/task.o			\
			$(BD)/system/kernel/init.o			\
			$(BD)/common/cirq_buffer.o
			

# common library needed for OpenSAR
lib-y = 

#common rules
.SUFFIXES: .c
%.o:%.c
	@echo compile $<  to $@
	@$(CC) $(CCFLGS) -o $@ $<

.PHONY:all clean gen

gen:
			
all:$(obj-y)
	@$(LD) -o $(OUT)/$(TARGET) $(obj-y) $(LDFLGS)
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET).EXE DONE   <<<<<<<<<<<<<<<<<<<<<<"

Flash.img:
	dd if=/dev/zero of=Flash.img bs=1M count=1
run:Flash.img
	@(start $(OUT)/$(TARGET).exe)	

clean:
	@$(RM) -f $(OUT)/$(TARGET).exe $(obj-y)
	@rm Flash.img
	@echo ">>>>>>>>>>>>>>>>>>>> CLEAN $(TARGET) Objs Done <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"		

