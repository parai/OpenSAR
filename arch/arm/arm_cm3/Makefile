# >>>>>>>> Config <<<<<<<<<<<<<<<<<

TARGET = OpenSAR

#Base Directory
BD = ../../..

OUT = $(BD)/out

GCC_EXEC_PREFIX=/c/uLoong/uT-Studio/v200/gcc/libexec/gcc/
export GCC_EXEC_PREFIX

#common compilers
CM-DIR =/c/uLoong/uT-Studio/v200/gcc
AS  = $(CM-DIR)/arm-none-eabi/bin/gcc -B $(CM-DIR)/lib/gcc/ -B $(CM-DIR)/arm-none-eabi/
CC  = $(CM-DIR)/arm-none-eabi/bin/gcc -B $(CM-DIR)/lib/gcc/ -B $(CM-DIR)/arm-none-eabi/
LD  = $(CM-DIR)/arm-none-eabi/bin/gcc -B $(CM-DIR)/lib/gcc/ -B $(CM-DIR)/arm-none-eabi/
AR  = $(CM-DIR)/arm-none-eabi/bin/ar
RM  = rm 


#common flags
		 	
INCDIR = 	-I$(BD)/include 		\
			-I$(BD)/include/arm     \
			-I$(BD)/include/arm/arm_cm3     \
			-I$(BD)/include/sys		\
			-I$(BD)/system/kernel/include \
			-I$(BD)/arch/arm/arm_cm3/kernel	\
			-I$(BD)/system/EcuM			\
			-I$(BD)/system/SchM			\
			-I$(BD)/arch/arm/arm_cm3/drivers/STM32F10x_StdPeriph_Driver/inc	\
			-I$(BD)/boards/stm32_stm3210c/examples/os_simple/config
			
MACROS = 	-DSTM32F10X_CL	\
			-DCFG_ARM_CM3	\
			-DUSE_ECUM		\
			-DUSE_MCU

ASFLGS = -c -g -mcpu=cortex-m3 -mthumb -mfpu=vfp -mthumb-interwork
CCFLGS = -c -g $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR) \
		-mthumb -mcpu=cortex-m3 -mfix-cortex-m3-ldrd 	\
		-mfpu=vfp -mthumb-interwork -mstructure-size-boundary=8 -ffreestanding
LDFLGS = $(MACROS) -std=gnu99 -O2 -Wall $(INCDIR)	\
		-mthumb -mcpu=cortex-m3 -mfix-cortex-m3-ldrd		\
		-mfpu=vfp -mthumb-interwork -static -nostdlib	    \
		-lgcc -lc		\
		-T $(BD)/arch/arm/arm_cm3/scripts/linkscript_gcc_lm.ldf 
ARFLGS = rv
RMFLGS = -f	

#common objectives		
dir-y =	$(BD)/arch/arm/arm_cm3/kernel	\
		$(BD)/system/EcuM			\
		$(BD)/arch/arm/arm_cm3/drivers/STM32F10x_StdPeriph_Driver/src	\
		$(BD)/boards/stm32_stm3210c/examples/os_simple/config
		
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
			$(BD)/common/cirq_buffer.o			\
			$(BD)/arch/arm/arm_cm3/drivers/Mcu.o	\
			$(BD)/arch/arm/arm_cm3/kernel/arch_krn.o	\
			$(BD)/arch/arm/arm_cm3/kernel/startup_stm32f10x.o
			

# common library needed for OpenSAR
lib-y = 

#common rules
.SUFFIXES: .c .s .sx
%.o:%.sx
	@echo compile $<  to $@
	@$(AS) $(ASFLGS) -o $@ $<
	
%.o:%.s
	@echo compile $<  to $@
	@$(AS) $(ASFLGS) -o $@ $<

%.o:%.c
	@echo compile $<  to $@
	$(CC) $(CCFLGS) -o $@ $<

.PHONY:all clean gen

gen:
	# Gen asm_offset.h
	@$(CC) -S $(BD)/system/kernel/asm_offset.c -o $(BD)/arch/arm/arm_cm3/kernel/asm_offset.hh $(INCDIR)
	@sed -n '/#define/p' $(BD)/arch/arm/arm_cm3/kernel/asm_offset.hh > $(BD)/arch/arm/arm_cm3/kernel/asm_offset.h
	@rm $(BD)/arch/arm/arm_cm3/kernel/asm_offset.hh
	# Gen arch_offset.h
	@$(CC) -S $(BD)/arch/arm/arm_cm3/kernel/arch_offset.c -o $(BD)/arch/arm/arm_cm3/kernel/arch_offset.hh $(INCDIR)
	@sed -n '/#define/p' $(BD)/arch/arm/arm_cm3/kernel/arch_offset.hh > $(BD)/arch/arm/arm_cm3/kernel/arch_offset.h
	@rm $(BD)/arch/arm/arm_cm3/kernel/arch_offset.hh
	
all:$(obj-y)
	@$(LD) -o $(OUT)/$(TARGET) $(obj-y) $(LDFLGS)
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(TARGET) DONE   <<<<<<<<<<<<<<<<<<<<<<"

clean:
	@$(RM) -f $(OUT)/$(TARGET) $(obj-y)
	@echo ">>>>>>>>>>>>>>>>>>>> CLEAN $(TARGET) Objs Done <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"		

