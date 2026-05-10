#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= main.o kernel.o kernel_initialize.o kernel_run.o datamanagement.o hardware.o logging.o CLogger.o debug.o

# libraries
LIBS =  $(CIRCLEHOME)/lib/usb/libusb.a \
        $(CIRCLEHOME)/lib/input/libinput.a \
        $(CIRCLEHOME)/addon/SDCard/libsdcard.a \
        $(CIRCLEHOME)/lib/fs/fat/libfatfs.a \
        $(CIRCLEHOME)/lib/fs/libfs.a \
        $(CIRCLEHOME)/lib/libcircle.a

include ../Rules.mk

-include $(DEPS)