
# Makefile for AutoSAR simulator running on Qt
# Hope better quality of true-time 

#output EXE
TARGET = OpenSAR

#Base Directory
BD = ../../..

OUT = $(BD)/out

#common compilers
AS  = g++
CC  = g++
LD  = g++
AR  = ar
RM  = rm

# Flags For Qt
Qt-InstallDir    = d:/usr/opt/Qt4.8.1
Qt-INC    = -I$(Qt-InstallDir)/include/QtCore 	\
			-I$(Qt-InstallDir)/include/QtGui 	\
			-I$(Qt-InstallDir)/include 			\
			-I$(Qt-InstallDir)/include/ActiveQt \
			-I$(Qt-InstallDir)/mkspecs/default
Qt-Macros = -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB -DQT_HAVE_MMX -DQT_HAVE_3DNOW -DQT_HAVE_SSE -DQT_HAVE_MMXEXT -DQT_HAVE_SSE2 -DQT_THREAD_SUPPORT -DQT_NEEDS_QMAIN
Qt-CCFLGS = -frtti -fexceptions -mthreads
Qt-LDFLGS = -mthreads -Wl,-subsystem,windows \
			-L$(Qt-InstallDir)/lib -lqtmaind -lQtGuid4 -lQtCored4 


#common flags
ASFLGS = 
INCDIR = 	$(Qt-INC)				\
			-I$(BD)/include 		\
			-I$(BD)/include/generic \
			-I$(BD)/include/sys		\
			-I$(BD)/system/kernel/include \
			-I$(BD)/app/config	\
			-I$(BD)/app/config/GEN	\
			-I$(BD)/app/inc			\
			-I$(BD)/arch/generic/mingw/kernel	\
			-I$(BD)/system/EcuM			\
			-I$(BD)/system/SchM			\
			-I$(BD)/diagnostic/Dcm		\
			-I$(BD)/communication/Com

MACROS = 	$(Qt-Macros)		\
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
			-DDEBUG_FEE		\
			-DUSE_NVM	\
			-DUSE_MEMIF	

CCFLGS = -c -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR)
LDFLGS = -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR) -lmingw32 $(Qt-LDFLGS) 
ARFLGS = rv
RMFLGS = -f	

#common objectives		
dir-y =	$(BD)/app/config	\
		$(BD)/app/config/GEN			\
		$(BD)/arch/generic/Qt/kernel	\
		$(BD)/arch/generic/Qt/drivers	\
		$(BD)/app/src					\
		$(BD)/app/src/Dia				\
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

# step 1: get C		
src-C = $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.c)))
obj-C = $(subst .c,.o,$(src-C))	
# step 2: get CPP
src-CPP = $(foreach x,$(dir-y),$(wildcard $(addprefix $(x)/*,.cpp)))
obj-CPP = $(subst .cpp,.o,$(src-CPP))

obj-y = $(obj-C) $(obj-CPP) 
# kernel files
obj-y +=

# common library needed for OpenSAR
lib-y = 

#common rules
.SUFFIXES: .cpp .c
%.o:%.c
	@echo compile $<  to $@
	@$(CC) $(CCFLGS) -o $@ $<

%.o:%.cpp
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

