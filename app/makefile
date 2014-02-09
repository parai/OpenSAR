
obj-y += $(subst .c,.o,$(wildcard $(addprefix $(ROOTDIR)/app/src/*,.c)))
obj-y += $(subst .c,.o,$(wildcard $(addprefix $(ROOTDIR)/app/src/Dia/*,.c)))


inc-y += $(ROOTDIR)/app/config
inc-y += $(ROOTDIR)/app/config/GEN
VPATH +=  $(ROOTDIR)/app/config
VPATH +=  $(ROOTDIR)/app/config/GEN

inc-y += $(ROOTDIR)/app/inc
inc-y += $(ROOTDIR)/diagnostic/Dcm

def-y += cfgCPU_ENDIAN=COM_LITTLE_ENDIAN
ldcmdfile-y=

build-exe-y = OpenSAR




