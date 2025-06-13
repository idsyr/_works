PROJECT = tasker-asm-tests
BUILD_DIR = build

SHARED_DIR = .
CFILES = my-project.c
DEVICE=stm32f103?8*

PATH_TO_TEMPLATE = ../libopencm3-template/

#===============================================
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../$(PATH_TO_TEMPLATE)/libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../$(PATH_TO_TEMPLATE)/rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
