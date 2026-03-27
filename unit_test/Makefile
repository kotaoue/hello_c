CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = test_math_functions
SRCS    = math_functions.c test_math_functions.c

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
