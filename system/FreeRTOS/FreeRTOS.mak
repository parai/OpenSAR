
ldflags-y += -lwinmm

obj-y += port2.o croutine.o event_groups.o kos.o list.o queue.o tasks.o timers.o
obj-y += heap_3.o Os_Cfg.o
		 		
VPATH += $(ROOTDIR)/system/FreeRTOS/Source	\
		 $(ROOTDIR)/system/FreeRTOS/Source/portable/MSVC-MingW	\
		 $(ROOTDIR)/system/FreeRTOS/Source/portable/MemMang

VPATH += $(ROOTDIR)/arch/$(ARCH_FAM)/$(ARCH)/kernel
VPATH += $(ROOTDIR)/arch/$(ARCH_FAM)/$(ARCH)/drivers

inc-y += $(ROOTDIR)/include/generic
inc-y += $(ROOTDIR)/system/FreeRTOS/Source/include
inc-y += $(ROOTDIR)/system/FreeRTOS/Source/portable/MSVC-MingW
inc-y += $(ROOTDIR)/system/FreeRTOS/Config
inc-y += $(ROOTDIR)/system/kernel/include

inc-y += $(ROOTDIR)/arch/$(ARCH_FAM)/$(ARCH)/kernel
inc-y += $(ROOTDIR)/arch/$(ARCH_FAM)/$(ARCH)/drivers
inc-y += $(ROOTDIR)/arch/$(ARCH_FAM)/$(ARCH)/config
inc-y += $(ROOTDIR)/include/$(ARCH_FAM)