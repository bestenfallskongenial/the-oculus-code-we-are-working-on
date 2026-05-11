#
# Makefile
#

CIRCLEHOME = ../..

OBJS  = main.o \
        kernel.o \
        kernel_initialize.o \
        kernel_run.o \
        CLogger.o \
        code_datamanagement.o \
        code_hardware.o \
        code_logging.o \
        code_util.o \
        code_wrappers.o \
        code_debug.o

# libraries
LIBS =  $(CIRCLEHOME)/lib/usb/libusb.a \
        $(CIRCLEHOME)/lib/input/libinput.a \
        $(CIRCLEHOME)/addon/SDCard/libsdcard.a \
        $(CIRCLEHOME)/lib/fs/fat/libfatfs.a \
        $(CIRCLEHOME)/lib/fs/libfs.a \
        $(CIRCLEHOME)/lib/libcircle.a

include ../Rules.mk

-include $(DEPS)