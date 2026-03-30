# 外部 API 接続サンプル

libcurl を使って外部 REST API を叩く最小限のサンプルです。  
C における外部 HTTP リクエストのスタンダードな書き方を示します。

## 学べること

| テーマ | 内容 |
|---|---|
| HTTP GET | `curl_easy_setopt()` で URL を設定して `curl_easy_perform()` で送信 |
| HTTP POST | `CURLOPT_POSTFIELDS` でボディを渡す |
| レスポンス受信 | `CURLOPT_WRITEFUNCTION` コールバックで動的バッファに蓄積 |
| ステータスコード | `curl_easy_getinfo(CURLINFO_RESPONSE_CODE)` で取得 |
| エラー処理 | `CURLcode` と `curl_easy_strerror()` で原因を表示 |
| リソース管理 | `curl_global_init()` / `curl_global_cleanup()` のスコープ管理 |

## ファイル構成

```
external_api/
├── http_client.h   — HttpResponse 構造体と公開 API の宣言
├── http_client.c   — libcurl を使った GET / POST の実装
├── main.c          — httpbin.org を叩くデモプログラム
└── Makefile        — ビルドと実行
```

## 依存ライブラリ

| ライブラリ | 用途 |
|---|---|
| [libcurl](https://curl.se/libcurl/) | HTTP リクエストの送受信 |

### インストール方法

```bash
# macOS
brew install curl

# Ubuntu / Debian
sudo apt install libcurl4-openssl-dev

# Fedora / RHEL
sudo dnf install libcurl-devel
```

## ビルドと実行

```bash
cd external_api
make run
```

`make clean` でビルド成果物を削除できます。

## 実行結果の例

```
--- GET リクエスト ---
URL: https://httpbin.org/get
ステータスコード: 200
レスポンスボディ:
{
  "args": {},
  "headers": {
    "Accept": "*/*",
    "Host": "httpbin.org",
    ...
  },
  "url": "https://httpbin.org/get"
}

--- POST リクエスト ---
URL:              https://httpbin.org/post
リクエストボディ: {"name": "hello_c", "version": 1}
ステータスコード: 200
レスポンスボディ:
{
  "data": "{\"name\": \"hello_c\", \"version\": 1}",
  "json": {
    "name": "hello_c",
    "version": 1
  },
  ...
}
```

## 仕組みの解説

### libcurl の基本的な使い方

```c
/* 1. グローバル初期化（プログラムに一度だけ） */
curl_global_init(CURL_GLOBAL_DEFAULT);

/* 2. ハンドルを作成 */
CURL *curl = curl_easy_init();

/* 3. オプションを設定 */
curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

/* 4. リクエストを実行 */
CURLcode res = curl_easy_perform(curl);

/* 5. エラー確認 */
if (res != CURLE_OK) {
    fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
}

/* 6. ハンドルを解放 */
curl_easy_cleanup(curl);

/* 7. グローバルリソースを解放 */
curl_global_cleanup();
```

### レスポンスボディの受け取り方

libcurl はデータをチャンク単位で届けます。  
`CURLOPT_WRITEFUNCTION` にコールバック関数を登録し、受け取るたびに動的バッファへ追記します。

```c
typedef struct { char *data; size_t len; } Buffer;

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t  total = size * nmemb;
    Buffer *buf   = (Buffer *)userdata;

    char *tmp = realloc(buf->data, buf->len + total + 1);
    if (tmp == NULL) return 0;          /* エラー → CURLE_WRITE_ERROR */

    buf->data = tmp;
    memcpy(buf->data + buf->len, ptr, total);
    buf->len += total;
    buf->data[buf->len] = '\0';         /* NULL 終端 */
    return total;
}

/* ハンドルにコールバックを登録 */
Buffer buf = {NULL, 0};
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &buf);
```

### HTTP ステータスコードの取得

```c
long status_code;
curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
printf("Status: %ld\n", status_code); /* 200, 404, 500 など */
```

### POST リクエストとカスタムヘッダ

```c
/* リクエストボディを設定（自動的に POST になる） */
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"key\": \"value\"}");

/* Content-Type ヘッダを追加 */
struct curl_slist *headers = NULL;
headers = curl_slist_append(headers, "Content-Type: application/json");
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

/* 使い終わったら解放 */
curl_slist_free_all(headers);
```

## 他言語との比較

| 言語 | 標準的な HTTP クライアント |
|---|---|
| C | libcurl |
| Go | `net/http` (標準ライブラリ) |
| Python | `requests` / `httpx` |
| JavaScript | `fetch` (標準) / `axios` |
| Ruby | `Net::HTTP` (標準) / `faraday` |
