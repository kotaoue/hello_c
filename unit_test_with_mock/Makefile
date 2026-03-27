# ===========================================================================
# Makefile - CMock モックテストビルド
# ===========================================================================
CC      = gcc
CFLAGS  = -Wall -Wextra -g

SRC_DIR   = src
TEST_DIR  = test
MOCK_DIR  = mocks
BUILD_DIR = build

UNITY_DIR = vendor/unity/src
CMOCK_DIR = vendor/cmock/src

INCLUDES = \
	-I$(SRC_DIR) \
	-I$(UNITY_DIR) \
	-I$(CMOCK_DIR) \
	-I$(MOCK_DIR)

# ===========================================================================
# CMock を使ったモックテスト（get_current_hour をモック）
# ===========================================================================
TEST_SRCS = \
	$(UNITY_DIR)/unity.c \
	$(CMOCK_DIR)/cmock.c \
	$(MOCK_DIR)/Mockclock.c \
	$(SRC_DIR)/greet.c \
	$(TEST_DIR)/test_clock_mock.c

TEST_BIN = $(BUILD_DIR)/test_clock_mock

.PHONY: all test clean mocks

all: $(BUILD_DIR) $(TEST_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BIN): $(TEST_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# モックファイルを再生成する（clock.h を変更したとき）
mocks:
	ruby scripts/generate_mocks.rb

# テストを実行する
test: all
	@echo ""
	@echo "=========================================="
	@echo " CMock モックテスト"
	@echo "=========================================="
	./$(TEST_BIN)
	@echo ""

clean:
	rm -rf $(BUILD_DIR)
