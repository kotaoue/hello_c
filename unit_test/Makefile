# ===========================================================================
# Makefile - Unity / CMock ユニットテストビルド
# ===========================================================================
CC      = gcc
CFLAGS  = -Wall -Wextra -g

# ソースディレクトリ
SRC_DIR    = src
TEST_DIR   = test
MOCK_DIR   = mocks
BUILD_DIR  = build

UNITY_DIR  = vendor/unity/src
CMOCK_DIR  = vendor/cmock/src

# インクルードパス
INCLUDES = \
	-I$(SRC_DIR) \
	-I$(UNITY_DIR) \
	-I$(CMOCK_DIR) \
	-I$(MOCK_DIR)

# ===========================================================================
# テスト 1: Unity を使った単体テスト（モックなし）
# ===========================================================================
TEST_GREET_SRCS = \
	$(UNITY_DIR)/unity.c \
	$(SRC_DIR)/greet.c \
	$(SRC_DIR)/clock.c \
	$(TEST_DIR)/test_greet.c

TEST_GREET_BIN = $(BUILD_DIR)/test_greet

# ===========================================================================
# テスト 2: CMock を使ったモックテスト（get_current_hour をモック）
# ===========================================================================
TEST_MOCK_SRCS = \
	$(UNITY_DIR)/unity.c \
	$(CMOCK_DIR)/cmock.c \
	$(MOCK_DIR)/Mockclock.c \
	$(SRC_DIR)/greet.c \
	$(TEST_DIR)/test_clock_mock.c

TEST_MOCK_BIN = $(BUILD_DIR)/test_clock_mock

# ===========================================================================
# ターゲット
# ===========================================================================
.PHONY: all test clean mocks

all: $(BUILD_DIR) $(TEST_GREET_BIN) $(TEST_MOCK_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_GREET_BIN): $(TEST_GREET_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(TEST_MOCK_BIN): $(TEST_MOCK_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# モックファイルを再生成する
mocks:
	ruby scripts/generate_mocks.rb

# テストを実行する
test: all
	@echo ""
	@echo "=========================================="
	@echo " テスト 1: Unity 単体テスト"
	@echo "=========================================="
	./$(TEST_GREET_BIN)
	@echo ""
	@echo "=========================================="
	@echo " テスト 2: CMock モックテスト"
	@echo "=========================================="
	./$(TEST_MOCK_BIN)
	@echo ""

clean:
	rm -rf $(BUILD_DIR)
