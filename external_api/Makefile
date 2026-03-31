CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = external_api
SRCS    = http_client.c main.c

# curl-config が存在する場合はそこからフラグを取得する
CURL_CONFIG := $(shell which curl-config 2>/dev/null)
ifneq ($(CURL_CONFIG),)
    CURL_CFLAGS := $(shell $(CURL_CONFIG) --cflags)
    CURL_LIBS   := $(shell $(CURL_CONFIG) --libs)
else
    # フォールバック: miniconda の libcurl を使用する
    # CURL_PREFIX 環境変数で上書き可能 (例: make CURL_PREFIX=/usr/local)
    CURL_PREFIX  ?= /usr/share/miniconda
    CURL_CFLAGS  = -I$(CURL_PREFIX)/include
    CURL_LIBS    = -L$(CURL_PREFIX)/lib -lcurl -Wl,-rpath,$(CURL_PREFIX)/lib
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(CURL_CFLAGS) -o $(TARGET) $(SRCS) $(CURL_LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
