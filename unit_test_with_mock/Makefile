CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = test_clock_mock
SRCS    = vendor/unity/src/unity.c \
          vendor/cmock/src/cmock.c \
          mocks/Mockclock.c \
          src/greet.c \
          test/test_clock_mock.c
INCLUDES = -Isrc -Ivendor/unity/src -Ivendor/cmock/src -Imocks

.PHONY: all test clean mocks

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS)

# テストを実行する
test: $(TARGET)
	./$(TARGET)

# モックファイルを再生成する（clock.h を変更したとき）
mocks:
	ruby scripts/generate_mocks.rb

clean:
	rm -f $(TARGET)
