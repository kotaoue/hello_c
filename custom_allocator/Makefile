# カスタムメモリアロケータ — Makefile

CC      = gcc
# GCC 13 では C23 を -std=c2x で指定する（GCC 14 以降は -std=c23 でも可）
CFLAGS  = -std=c2x -Wall -Wextra -Wpedantic -g

TARGET  = allocator_demo
SRCS    = allocator.c main.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
