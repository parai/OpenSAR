
# ARCH defines
ARCH=gtk
ARCH_FAM=generic
ARCH_MCU=

#
# CFG (y/n) macros
# 

CFG=GTK
# Add our board  
CFG+=BRD_GTK

# What buildable modules does this board have, 
# default or private

# MCAL
MOD_AVAIL+=CAN DIO MCU FLS PORT EEP 
# System + Communication + Diagnostic
MOD_AVAIL+=CANIF CANTP J1939TP COM DCM DEM DET ECUM IOHWAB KERNEL PDUR WDGM RTE SCHM
# Network management
MOD_AVAIL+=COMM NM CANNM CANSM
# Additional
MOD_AVAIL+=RAMLOG TCF LWIP SLEEP RTE
# CRC
MOD_AVAIL+=CRC32 CRC16
# Memory
MOD_AVAIL+= FEE EA NVM
# Required modules
MOD_USE += MCU KERNEL ECUM CAN CANIF CANTP PDUR DCM COM DET SCHM  COMM NM CANNM CANSM CRC32 CRC16 \
		   FEE EA EEP DIO PORT FLS NVM

#
# Extra defines 
#

def-y += __GTK__ _WIN32 WIN32


# Default cross compiler
DEFAULT_CROSS_COMPILE = 
