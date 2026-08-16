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

.PHONY: all clean bench

# Separate sequential and parallel sources/targets
SEQ_SRCS = sieve_sequential.c
SEQ_OBJS = $(SEQ_SRCS:.c=.o)

PAR_SRCS = sieve_parallel.c
PAR_OBJS = $(PAR_SRCS:.c=.o)

SEQ_TARGET = sieve_sequential$(EXEEXT)
PAR_TARGET = sieve_parallel$(EXEEXT)

all: $(SEQ_TARGET) $(PAR_TARGET)

$(SEQ_TARGET): $(SEQ_OBJS)
	$(CC) $(SEQ_OBJS) -o $(SEQ_TARGET) $(LDLIBS)
	-$(RM) $(SEQ_OBJS)

$(PAR_TARGET): $(PAR_OBJS)
	$(CC) $(PAR_OBJS) -o $(PAR_TARGET) $(LDLIBS)
	-$(RM) $(PAR_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) $(SEQ_OBJS) $(PAR_OBJS) $(SEQ_TARGET) $(PAR_TARGET)
