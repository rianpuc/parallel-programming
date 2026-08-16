CC = gcc
CFLAGS = -std=c11 -Wall -Iinclude -fopenmp
LDLIBS = -fopenmp -lm

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXEEXT = .exe
else
    RM = rm -f
    EXEEXT =
endif

TARGET = main$(EXEEXT)

SRCS = main.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDLIBS)
	-$(RM) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) $(OBJS) $(TARGET)