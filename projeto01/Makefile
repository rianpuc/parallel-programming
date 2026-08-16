CC = gcc
CFLAGS = -Wall -Iinclude -fopenmp
LDLIBS = -lm

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXEEXT = .exe
else
    RM = rm -f
    EXEEXT =
endif

TARGET = main$(EXEEXT)

SRCS = src/main.c src/reader_posix.c src/reader_win.c src/reader.c
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