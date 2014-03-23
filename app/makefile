
obj-y += $(subst .c,.o,$(notdir $(wildcard $(addprefix $(ROOTDIR)/app/src/*,.c))))
obj-y += $(subst .c,.o,$(notdir $(wildcard $(addprefix $(ROOTDIR)/app/src/Dia/*,.c))))

inc-y += $(ROOTDIR)/app/config
inc-y += $(ROOTDIR)/app/config/GEN
VPATH +=  $(ROOTDIR)/app/config
VPATH +=  $(ROOTDIR)/app/config/GEN
VPATH +=  $(ROOTDIR)/app/src
VPATH +=  $(ROOTDIR)/app/src/Dia

# some special files for GTK
obj-y += Stmo.o Stmo_Cfg.o
def-y += USE_GUI
obj-y += Gui.o SgDraw.o Gui_Cfg.o Lcd.o lcd_font10x14.o
inc-y += $(ROOTDIR)/system/Gui/include
VPATH += $(ROOTDIR)/system/Gui


inc-y += $(ROOTDIR)/app/inc
inc-y += $(ROOTDIR)/diagnostic/Dcm

ldcmdfile-y=

build-exe-y = $(ROOTDIR)/out/OpenSAR.exe





