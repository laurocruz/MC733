# ####################################################
# Hello World
# ####################################################

#changed as this option was given in path
CC = mips-newlib-elf-gcc
CFLAGS = -msoft-float -specs=archc -D_PER
LDFLAGS = -lm

TARGET = hello_world.mips
INC_DIR := -I.

SRCS := demo_serial.c
OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET) $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ $(TARGET)
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
